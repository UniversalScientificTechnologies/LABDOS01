String githash = "379276a";
String FWversion = "R2"; // 16 MHz crystal
#define ZERO 256  // 5th channel is channel 1 (column 10 from 0, ussually DCoffset or DCoffset+1)

/*
  SPACEDOS with Resetitko for RT
 
ISP
---
PD0     RX
PD1     TX
RESET#  through 50M capacitor to RST#

SDcard
------
DAT3   SS   4 B4
CMD    MOSI 5 B5
DAT0   MISO 6 B6
CLK    SCK  7 B7

ANALOG
------
+      A0  PA0
-      A1  PA1
RESET  0   PB0

LED
---
LED_yellow  23  PC7         // LED for Dasa


                     Mighty 1284p    
                      +---\/---+
           (D 0) PB0 1|        |40 PA0 (AI 0 / D24)
           (D 1) PB1 2|        |39 PA1 (AI 1 / D25)
      INT2 (D 2) PB2 3|        |38 PA2 (AI 2 / D26)
       PWM (D 3) PB3 4|        |37 PA3 (AI 3 / D27)
    PWM/SS (D 4) PB4 5|        |36 PA4 (AI 4 / D28)
      MOSI (D 5) PB5 6|        |35 PA5 (AI 5 / D29)
  PWM/MISO (D 6) PB6 7|        |34 PA6 (AI 6 / D30)
   PWM/SCK (D 7) PB7 8|        |33 PA7 (AI 7 / D31)
                 RST 9|        |32 AREF
                VCC 10|        |31 GND
                GND 11|        |30 AVCC
              XTAL2 12|        |29 PC7 (D 23)
              XTAL1 13|        |28 PC6 (D 22)
      RX0 (D 8) PD0 14|        |27 PC5 (D 21) TDI
      TX0 (D 9) PD1 15|        |26 PC4 (D 20) TDO
RX1/INT0 (D 10) PD2 16|        |25 PC3 (D 19) TMS
TX1/INT1 (D 11) PD3 17|        |24 PC2 (D 18) TCK
     PWM (D 12) PD4 18|        |23 PC1 (D 17) SDA
     PWM (D 13) PD5 19|        |22 PC0 (D 16) SCL
     PWM (D 14) PD6 20|        |21 PD7 (D 15) PWM
                      +--------+
*/

/*
// Compiled with: Arduino 1.8.9
// MightyCore 2.0.2 https://mcudude.github.io/MightyCore/package_MCUdude_MightyCore_index.json
Fix old bug in Mighty SD library
~/.arduino15/packages/MightyCore/hardware/avr/2.0.2/libraries/SD/src/SD.cpp:
boolean SDClass::begin(uint32_t clock, uint8_t csPin) {
  if(root.isOpen()) root.close();
*/

#include "wiring_private.h"
#include <Wire.h>           
#include "src/RTCx/RTCx.h"  // Modified version included

//#define LED_yellow  23   // PC7
#define RESET       0    // PB0
#define SDpower1    1    // PB1
#define SDpower2    2    // PB2
#define SDpower3    3    // PB3
#define SS          4    // PB4
#define MOSI        5    // PB5
#define MISO        6    // PB6
#define SCK         7    // PB7
#define INT         20   // PC4
#define RELE_ON     19   // PC3
#define RELE_OFF    23   // PC7

#define CHANNELS 512 // number of channels in buffer for histogram, including negative numbers

uint16_t count = 0;
uint32_t serialhash = 0;
uint16_t offset;
uint16_t base_offset = ZERO - 1;
uint8_t lo, hi;
uint16_t u_sensor, maximum;
struct RTCx::tm tm;

// Read Analog Differential without gain (read datashet of ATMega1280 and ATMega2560 for refference)
// Use analogReadDiff(NUM)
//   NUM  | POS PIN             | NEG PIN           |   GAIN
//  0 | A0      | A1      | 1x
//  1 | A1      | A1      | 1x
//  2 | A2      | A1      | 1x
//  3 | A3      | A1      | 1x
//  4 | A4      | A1      | 1x
//  5 | A5      | A1      | 1x
//  6 | A6      | A1      | 1x
//  7 | A7      | A1      | 1x
//  8 | A8      | A9      | 1x
//  9 | A9      | A9      | 1x
//  10  | A10     | A9      | 1x
//  11  | A11     | A9      | 1x
//  12  | A12     | A9      | 1x
//  13  | A13     | A9      | 1x
//  14  | A14     | A9      | 1x
//  15  | A15     | A9      | 1x
#define PIN 0
uint8_t analog_reference = INTERNAL2V56; // DEFAULT, INTERNAL, INTERNAL1V1, INTERNAL2V56, or EXTERNAL

void setup()
{

  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.println("#Cvak...");
  
  ADMUX = (analog_reference << 6) | ((PIN | 0x10) & 0x1F);
  
  ADCSRB = 0;               // Switching ADC to Free Running mode
  sbi(ADCSRA, ADATE);       // ADC autotrigger enable (mandatory for free running mode)
  sbi(ADCSRA, ADSC);        // ADC start the first conversions
  sbi(ADCSRA, 2);           // 0x111 = clock divided by 128, 125 kHz, 104 us for 13 cycles of one AD conversion, 12 us for 1.5 cycle for sample-hold
  sbi(ADCSRA, 1);        
  sbi(ADCSRA, 0);        

  pinMode(RESET, OUTPUT);   // reset for peak detetor

  //pinMode(SDpower1, OUTPUT);  // SDcard interface
  //pinMode(SDpower2, OUTPUT);     
  //pinMode(SDpower3, OUTPUT);     
  //pinMode(SS, OUTPUT);     
  //pinMode(MOSI, INPUT);     
  //pinMode(MISO, INPUT);     
  //pinMode(SCK, OUTPUT);  

  DDRB = 0b10011110;
  PORTB = 0b00000000;  // SDcard Power OFF

  DDRA = 0b11111100;
  PORTA = 0b00000000;  // SDcard Power OFF
  DDRC = 0b11101100;
  PORTC = 0b00000000;  // SDcard Power OFF
  DDRD = 0b11111100;
  PORTD = 0b00000000;  // SDcard Power OFF

  pinMode(RELE_ON, OUTPUT);
  pinMode(RELE_OFF, OUTPUT);
  digitalWrite(RELE_ON, LOW);  
  digitalWrite(RELE_OFF, HIGH); 
  delay(100); 
  digitalWrite(RELE_OFF, LOW);  
  digitalWrite(RESET, LOW);  
  
  Wire.setClock(100000);

  Serial.println("#Hmmm...");

  // make a string for device identification output
  String dataString = "$AIRDOS," + FWversion + "," + String(ZERO) + "," + githash + ","; // FW version and Git hash
  
  Wire.beginTransmission(0x58);                   // request SN from EEPROM
  Wire.write((int)0x08); // MSB
  Wire.write((int)0x00); // LSB
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)0x58, (uint8_t)16);    
  for (int8_t reg=0; reg<16; reg++)
  { 
    uint8_t serialbyte = Wire.read(); // receive a byte
    if (serialbyte<0x10) dataString += "0";
    dataString += String(serialbyte,HEX);    
    serialhash += serialbyte;
  }

  {
    Serial.println(dataString);  // print SN to terminal 
  }    

  // measurement of ADC offset
  ADMUX = (analog_reference << 6) | 0b10001; // Select +A1,-A1 for offset correction
  delay(200);
  ADCSRB = 0;               // Switching ADC to Free Running mode
  sbi(ADCSRA, ADATE);       // ADC autotrigger enable (mandatory for free running mode)
  sbi(ADCSRA, ADSC);        // ADC start the first conversions
  sbi(ADCSRA, 2);           // 0x111 = clock divided by 128
  sbi(ADCSRA, 1);
  sbi(ADCSRA, 0);
  sbi(ADCSRA, ADIF);                  // reset interrupt flag from ADC
  while (bit_is_clear(ADCSRA, ADIF)); // wait for the first conversion
  sbi(ADCSRA, ADIF);                  // reset interrupt flag from ADC
  lo = ADCL;
  hi = ADCH;
  ADMUX = (analog_reference << 6) | 0b10000; // Select +A0,-A1 for measurement
  ADCSRB = 0;               // Switching ADC to Free Running mode
  sbi(ADCSRA, ADATE);       // ADC autotrigger enable (mandatory for free running mode)
  sbi(ADCSRA, ADSC);        // ADC start the first conversions
  sbi(ADCSRA, 2);           // 0x111 = clock divided by 128
  sbi(ADCSRA, 1);
  sbi(ADCSRA, 0);
  // combine the two bytes
  u_sensor = (hi << 7) | (lo >> 1);
  // manage negative values
  if (u_sensor <= (CHANNELS / 2) - 1 ) {
    u_sensor += (CHANNELS / 2);
  } else {
    u_sensor -= (CHANNELS / 2);
  }

  // Initiates RTC
  rtc.autoprobe();
  rtc.resetClock();
}

bool rele_on = false; 
bool rele_off = false; 

void loop()
{
  uint16_t histogram[CHANNELS];
 
  if (rele_on)
  {
      digitalWrite(RELE_OFF, LOW);  // switch on rele     
      digitalWrite(RELE_ON, HIGH);  
      delay(100); 
      digitalWrite(RELE_ON, LOW);
      rele_on = false;
      rele_off = true;  
  }
  
  for(int n=0; n<CHANNELS; n++)
  {
    histogram[n]=0;
  }

  // measurement of ADC offset
  ADMUX = (analog_reference << 6) | 0b10001; // Select +A1,-A1 for offset correction
  delay(50);
  ADCSRB = 0;               // Switching ADC to Free Running mode
  sbi(ADCSRA, ADATE);       // ADC autotrigger enable (mandatory for free running mode)
  sbi(ADCSRA, ADSC);        // ADC start the first conversions
  sbi(ADCSRA, 2);           // 0x111 = clock divided by 128
  sbi(ADCSRA, 1);        
  sbi(ADCSRA, 0);        
  sbi(ADCSRA, ADIF);                  // reset interrupt flag from ADC
  while (bit_is_clear(ADCSRA, ADIF)); // wait for the first conversion 
  sbi(ADCSRA, ADIF);                  // reset interrupt flag from ADC
  lo = ADCL;
  hi = ADCH;
  ADMUX = (analog_reference << 6) | 0b10000; // Select +A0,-A1 for measurement
  ADCSRB = 0;               // Switching ADC to Free Running mode
  sbi(ADCSRA, ADATE);       // ADC autotrigger enable (mandatory for free running mode)
  sbi(ADCSRA, ADSC);        // ADC start the first conversions
  sbi(ADCSRA, 2);           // 0x111 = clock divided by 128
  sbi(ADCSRA, 1);        
  sbi(ADCSRA, 0);        
  // combine the two bytes
  u_sensor = (hi << 7) | (lo >> 1);
  // manage negative values
  if (u_sensor <= (CHANNELS/2)-1 ) {u_sensor += (CHANNELS/2);} else {u_sensor -= (CHANNELS/2);}
  offset = u_sensor;
  
  PORTB = 1;                          // Set reset output for peak detector to H
  sbi(ADCSRA, ADIF);                  // reset interrupt flag from ADC
  while (bit_is_clear(ADCSRA, ADIF)); // wait for the first dummy conversion 
  DDRB = 0b10011111;                  // Reset peak detector
  delayMicroseconds(100);             // guaranteed reset
  DDRB = 0b10011110;

  sbi(ADCSRA, ADIF);        // reset interrupt flag from ADC

  uint16_t suppress = 0;      
    
  while (bit_is_clear(ADCSRA, ADIF)); // wait for dummy conversion 
  DDRB = 0b10011111;                  // Reset peak detector
  asm("NOP");                         // cca 6 us for 2k2 resistor and 1k capacitor in peak detector
  asm("NOP");                         
  asm("NOP");                         
  asm("NOP");                         
  asm("NOP");                         
  DDRB = 0b10011110;
  sbi(ADCSRA, ADIF);                  // reset interrupt flag from ADC
  
  // dosimeter integration
  for (uint32_t i=0; i<(2 * 46000); i++)    // cca 10 s
  {
    while (bit_is_clear(ADCSRA, ADIF)); // wait for end of conversion 
    delayMicroseconds(24);            // 24 us wait for 1.5 cycle of 62.5 kHz ADC clock for sample/hold for next conversion
    //asm("NOP");                         // cca 8 us after loop
    //asm("NOP");                         
    //asm("NOP");                         
    //asm("NOP");                         
    //asm("NOP");                         
    //asm("NOP");                         
    
    DDRB = 0b10011111;                  // Reset peak detector
    delayMicroseconds(7);
    //asm("NOP");                         // cca 7 us for 2k2 resistor and 100n capacitor in peak detector
    //asm("NOP");                         
    //asm("NOP");                         
    //asm("NOP");                         
    //asm("NOP");                         
    DDRB = 0b10011110;
    sbi(ADCSRA, ADIF);                  // reset interrupt flag from ADC

    // we have to read ADCL first; doing so locks both ADCL
    // and ADCH until ADCH is read.  reading ADCL second would
    // cause the results of each conversion to be discarded,
    // as ADCL and ADCH would be locked when it completed.
    lo = ADCL;
    hi = ADCH;

    // combine the two bytes
    u_sensor = (hi << 7) | (lo >> 1);

    // manage negative values
    if (u_sensor <= (CHANNELS/2)-1 ) {u_sensor += (CHANNELS/2);} else {u_sensor -= (CHANNELS/2);}
              
    if (u_sensor > maximum) // filter double detection for pulses between two samples
    {
      maximum = u_sensor;
      suppress++;
    }
    else
    {
      histogram[maximum]++;
      maximum = 0;
    }
  }  
  
  // Data out
  {
    rtc.readClock(tm);
    RTCx::time_t t = RTCx::mktime(&tm);

    uint16_t noise = base_offset+4;
    uint32_t dose=0;
    #define RANGE 252

    for(int n=noise; n<(base_offset+RANGE); n++)  
    {
      dose += histogram[n]; 
    }

    // make a string for assembling the data to log:
    String dataString = "";
    
    // make a string for assembling the data to log:
    dataString += "$CANDY,";
    dataString += String(count); 
    dataString += ",";  
    dataString += String(t-946684800); 
    dataString += ",";
    dataString += String(suppress);
    dataString += ",";
    dataString += String(dose);
    dataString += ",";
    dataString += String(offset);
    dataString += ",";
    if (rele_off) {dataString += '1';} else {dataString += "0";} 
    
    for(int n=base_offset; n<(base_offset+RANGE); n++)  
    {
      dataString += ",";
      dataString += String(histogram[n]); 
    }
    
    count++;

    while (Serial.available()) 
    {
      char cvak = Serial.read();
      if (cvak=='r') 
      {
        rele_on = true;
        rele_off = false;
      }
    }
  
    if (rele_off)
    {
      digitalWrite(RELE_ON, LOW);     // switch off rele  
      digitalWrite(RELE_OFF, HIGH);  
      delay(100); 
      digitalWrite(RELE_OFF, LOW);
      rele_off = false;  
    }

    {
      Serial.println(dataString);  // print to terminal (additional 700 ms in DEBUG mode)
    }          
  }    
}
