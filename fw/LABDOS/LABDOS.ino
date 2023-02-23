#define VERSION "07"
#define MAJOR 7
#define MINOR 4 // parser version ??
#ifndef RELEASE
  #define RELEASE 0
#endif
#ifndef BUILD
  #define BUILD 0 // 0 - means manual 
#endif



#ifndef CHANNELS
  #define CHANNELS 1024 // number of channels in buffer for histogram, including negative numbers (512 or 1024)
#endif
#define ZERO CHANNELS/2 // 3th channel is channel 1 (ussually DCoffset or DCoffset+1, for version with noise reduction transistor)
#define RANGE ZERO-12

#if CHANNELS==1024
  String FWversion = MAJOR"."MINOR"."RELEASE"-"BUILD".L500_"; // 500 effective channels for 1024 ADC channels
#else
  String FWversion = MAJOR"."MINOR"."RELEASE"-"BUILD".L244_"; // 244 effective channels for 512 ADC channels
#endif

#define MAXFILESIZE MAX_MEASUREMENTS * BYTES_MEASUREMENT // in bytes, 4 MB per day, 28 MB per week, 122 MB per month
#define MAX_MEASUREMENTS 11000ul // in measurement cycles, 5 500 per day
#define BYTES_MEASUREMENT 531ul // number of bytes per one measurement
#define MAXFILES 200 // maximal number of files on SD card

/*
  LABDOS
 
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

// Compiled with: Arduino 1.8.9
// MightyCore 2.0.2 https://mcudude.github.io/MightyCore/package_MCUdude_MightyCore_index.json

#include "wiring_private.h"
#include <Wire.h>           
#include <SD.h>             
#include <SPI.h>
#include "githash.h"

#define RESET       0    // PB0
#define SDpower1    1    // PB1
#define SDpower2    2    // PB2
#define SDpower3    3    // PB3
#define SS          4    // PB4
#define MOSI        5    // PB5
#define MISO        6    // PB6
#define SCK         7    // PB7
#define INT         20   // PC4
#define ANALOG_ON   15   // PD7
#define LED1        21 // PC5
#define LED2        22 // PC6
#define LED3        23 // PC7

String filename = "";
uint16_t fn;
uint16_t count = 0;
uint32_t serialhash = 0;
uint16_t base_offset = ZERO;
uint8_t lo, hi;
uint16_t u_sensor;
boolean SDinserted = true;

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


uint8_t bcdToDec(uint8_t b)
{
  return ( ((b >> 4)*10) + (b%16) );
}

uint32_t tm;
uint8_t tm_s100;

void readRTC()
{
  Wire.beginTransmission(0x51);
  Wire.write(0);
  Wire.endTransmission();
  
  Wire.requestFrom(0x51, 6);
  tm_s100 = bcdToDec(Wire.read());
  uint8_t tm_sec = bcdToDec(Wire.read() & 0x7f);
  uint8_t tm_min = bcdToDec(Wire.read() & 0x7f);
  tm = bcdToDec(Wire.read());
  tm += bcdToDec(Wire.read()) * 100;
  tm += bcdToDec(Wire.read()) * 10000;
  tm = tm * 60 * 60 + tm_min * 60 + tm_sec;
}

#define SD_ON     1
#define SD_OFF    2

void set_power(uint8_t state)
{
  switch(state)
  {
    case SD_ON:
      pinMode(MISO, OUTPUT);     
      pinMode(MOSI, OUTPUT);     
      PORTB |= 0b00001110;
      digitalWrite(SS, LOW);  
      break;
    case SD_OFF:
      digitalWrite(SS, HIGH);  
      PORTB &= 0b11110001;
      break;
    default:
      delay(100);
  }
}


void setup()
{
  pinMode(LED1, OUTPUT); 
  digitalWrite(LED1, HIGH); 
  delay(100);  

  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  Serial.println("#Cvak...");
  
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
  PORTD = 0b10000000;  // SDcard Power OFF

  Wire.setClock(100000);

  pinMode(LED1, OUTPUT); 
  digitalWrite(LED1, HIGH); 
  Serial.println("#Hmmm...");

  {
    uint16_t DCoffset;
    ADMUX = (analog_reference << 6) | 0b10001; // Select +A1,-A1 for offset correction
    delay(50);
    ADCSRB = 0;               // Switching ADC to Free Running mode
    sbi(ADCSRA, ADATE);       // ADC autotrigger enable (mandatory for free running mode)
    sbi(ADCSRA, ADSC);        // ADC start the first conversions
#if F_CPU==8000000L
    sbi(ADCSRA, 2);           // 0x110 = clock divided by 64
    sbi(ADCSRA, 1);        
    cbi(ADCSRA, 0);        
#else
    sbi(ADCSRA, 2);           // 0x111 = clock divided by 128
    sbi(ADCSRA, 1);        
    sbi(ADCSRA, 0);        
#endif  
    sbi(ADCSRA, ADIF);                  // reset interrupt flag from ADC
    while (bit_is_clear(ADCSRA, ADIF)); // wait for the first conversion 
    sbi(ADCSRA, ADIF);                  // reset interrupt flag from ADC
    for (uint8_t n=0; n<8; n++) 
    { 
      // measurement of ADC offset
      while (bit_is_clear(ADCSRA, ADIF)); // wait for conversion 
      sbi(ADCSRA, ADIF);                  // reset interrupt flag from ADC
      lo = ADCL;    
      hi = ADCH;

      // combine the two bytes
      #if CHANNELS==1024
        u_sensor = (hi << 8) | (lo);
      #else
        u_sensor = (hi << 7) | (lo >> 1);    
      #endif
      
      // manage negative values
      if (u_sensor <= (CHANNELS/2)-1 ) {u_sensor += (CHANNELS/2);} else {u_sensor -= (CHANNELS/2);}
      DCoffset += u_sensor;
    }
    base_offset = DCoffset >> 3; // Calculate mean of 8 measurements
  }

  // Initiation of RTC
  Wire.beginTransmission(0x51); // init clock
  Wire.write((uint8_t)0x23); // Start register
  Wire.write((uint8_t)0x00); // 0x23
  Wire.write((uint8_t)0x00); // 0x24 Two's complement offset value
  Wire.write((uint8_t)0b00000101); // 0x25 Normal offset correction, disable low-jitter mode, set load caps to 6 pF
  Wire.write((uint8_t)0x00); // 0x26 Battery switch reg, same as after a reset
  Wire.write((uint8_t)0x00); // 0x27 Enable CLK pin, using bits set in reg 0x28
  Wire.write((uint8_t)0x97); // 0x28 stop watch mode, no periodic interrupts, CLK pin off
  Wire.write((uint8_t)0x00); // 0x29
  Wire.write((uint8_t)0x00); // 0x2a
  Wire.endTransmission();
  Wire.beginTransmission(0x51); // reset clock
  Wire.write(0x2f); 
  Wire.write(0x2c);
  Wire.endTransmission();
  Wire.beginTransmission(0x51); // start stop-watch
  Wire.write(0x28); 
  Wire.write(0x97);
  Wire.endTransmission();
  Wire.beginTransmission(0x51); // reset stop-watch
  Wire.write((uint8_t)0x00); // Start register
  Wire.write((uint8_t)0x00); // 0x00
  Wire.write((uint8_t)0x00); // 0x01 
  Wire.write((uint8_t)0x00); // 0x02 
  Wire.write((uint8_t)0x00); // 0x03
  Wire.write((uint8_t)0x00); // 0x04
  Wire.write((uint8_t)0x00); // 0x05
  Wire.endTransmission();
  
  // make a string for device identification output
  String dataString = "$DOS,LABDOS01A," + FWversion + VERSION + "," + String(base_offset) + "," + githash + ","; // FW version and Git hash
  
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
  pinMode(LED2, OUTPUT); 
  digitalWrite(LED2, HIGH); 

  {
    set_power(SD_ON);
     
    // make sure that the default chip select pin is set to output
    // see if the card is present and can be initialized:
    if (!SD.begin(SS)) 
    {
      Serial.println("#SD init false");
      SDinserted = false;
    }
    for (fn = 1; fn<MAXFILES; fn++) // find last file
    {
       filename = String(fn) + ".txt";
       if (SD.exists(filename) == 0) break;
    }
    fn--;
    filename = String(fn) + ".txt";
    
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open(filename, FILE_WRITE);

    uint32_t filesize = dataFile.size();
    Serial.print("#Filesize,");
    Serial.println(filesize); 
    if (filesize > MAXFILESIZE)
    {
      dataFile.close();
      fn++;
      filename = String(fn) + ".txt";      
      dataFile = SD.open(filename, FILE_WRITE);
    }
    Serial.print("#Filename,");
    Serial.println(filename); 
  
    // if the file is available, write to it:
    if (dataFile) 
    {
      pinMode(LED3, OUTPUT); 
      digitalWrite(LED3, HIGH); 
      dataFile.println(dataString);  // write to SDcard (800 ms)     
      dataFile.close();  
    }  
    // if the file isn't open, pop up an error:
    else 
    {
      Serial.println("#SD false");
      SDinserted = false;
    }
    Serial.println(dataString);  // print SN to terminal 
    set_power(SD_OFF);
  }    
  
  pinMode(LED1, OUTPUT); 
  digitalWrite(LED1, LOW); 
  delay(100);  
  pinMode(LED2, OUTPUT); 
  digitalWrite(LED2, LOW); 
  delay(100);  
  pinMode(LED3, OUTPUT); 
  digitalWrite(LED3, LOW); 
  delay(100);  
}


void loop()
{
  uint16_t histogram[CHANNELS];
  for(int n=0; n<CHANNELS; n++)
  {
    histogram[n]=0;
  }

  // dummy conversion
  ADMUX = (analog_reference << 6) | 0b10000; // Select +A0,-A1 for measurement
  ADCSRB = 0;               // Switching ADC to Free Running mode
  sbi(ADCSRA, ADATE);       // ADC autotrigger enable (mandatory for free running mode)
  sbi(ADCSRA, ADSC);        // ADC start the first conversions
#if F_CPU==8000000L
  sbi(ADCSRA, 2);           // 0x110 = clock divided by 64
  sbi(ADCSRA, 1);        
  cbi(ADCSRA, 0);        
#else
  sbi(ADCSRA, 2);           // 0x111 = clock divided by 128
  sbi(ADCSRA, 1);        
  sbi(ADCSRA, 0);        
#endif  
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
  delayMicroseconds(7);               // cca 7 us for 2k2 resistor and 100n capacitor in peak detector
  DDRB = 0b10011110;
  sbi(ADCSRA, ADIF);                  // reset interrupt flag from ADC
  
  uint8_t previous_sample = 1; // ignore the first ADC

  // dosimeter integration
  for (uint16_t i=0; i<(65535); i++)    // 7.34 s (6.88 s integration time excluding dead time)
  {
    // 112 us fo 14 cycles of conversion; it includes cca 7 us of death time
    while (bit_is_clear(ADCSRA, ADIF)); // wait for end of conversion 
    delayMicroseconds(12);            // 12 us wait for 1.5 cycle of 125 kHz ADC clock for sample/hold for next conversion
    uint8_t raising_edge = PINB; // peak of pulse was before S/H? H = raising edge; L = falling edge
    
    DDRB = 0b10011111;                  // Reset peak detector
    delayMicroseconds(7);               // cca 7 us for 2k2 resistor and 100n capacitor in peak detector
    DDRB = 0b10011110;
    sbi(ADCSRA, ADIF);                  // reset interrupt flag from ADC

    // we have to read ADCL first; doing so locks both ADCL
    // and ADCH until ADCH is read.  reading ADCL second would
    // cause the results of each conversion to be discarded,
    // as ADCL and ADCH would be locked when it completed.
    lo = ADCL;
    hi = ADCH;

    // combine the two bytes
    #if CHANNELS==1024
      u_sensor = (hi << 8) | (lo);
    #else
      u_sensor = (hi << 7) | (lo >> 1);    
    #endif
    // manage negative values
    if (u_sensor <= (CHANNELS/2)-1 ) {u_sensor += (CHANNELS/2);} else {u_sensor -= (CHANNELS/2);}
              
    if (previous_sample & 1) 
    {
      suppress++;
    }
    else
    {
      histogram[u_sensor]++;
    }

    previous_sample = raising_edge;
  }  
  
  // Data out
  {
    uint16_t noise = base_offset+2;
    uint32_t flux=0;

    for(int n=noise; n<(base_offset+RANGE); n++)  
    {
      flux += histogram[n]; 
    }

    digitalWrite(LED3, HIGH); 

    // make a string for assembling the data to log:
    String dataString = "";

    readRTC();
    
    // make a string for assembling the data to log:
    dataString += "$HIST,";
    dataString += String(count); 
    dataString += ",";  
    dataString += String(tm); 
    dataString += ".";
    dataString += String(tm_s100); 
    dataString += ",";
    dataString += String(suppress);
    dataString += ",";
    dataString += String(flux);
    
    for(int n=base_offset-1; n<(base_offset-1+RANGE); n++)  
    {
      dataString += ",";
      dataString += String(histogram[n]); 
    }

    if (SDinserted)
    {
      set_power(SD_ON);

      // make sure that the default chip select pin is set to output
      // see if the card is present and can be initialized:
      if (!SD.begin(SS)) 
      {
        Serial.println("#SD init false");
        SDinserted = false;
        // don't do anything more:
      }
      else
      {
        // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another.
        File dataFile = SD.open(filename, FILE_WRITE);
      
        // if the file is available, write to it:
        if (dataFile) 
        {
          digitalWrite(LED2, HIGH);     
          dataFile.println(dataString);  // write to SDcard (800 ms)     
          dataFile.close();
          digitalWrite(LED2, LOW);     
        }  
        // if the file isn't open, pop up an error:
        else 
        {
          Serial.println("#SD false");
          SDinserted = false;
        }
      }  
      set_power(SD_OFF);
    }          
    Serial.println(dataString);   // print to terminal 
    digitalWrite(LED3, LOW);     
    
    count++;
    if (count > MAX_MEASUREMENTS) 
    {
      count = 0;
      fn++;
      filename = String(fn) + ".txt";        
      Serial.print("#Filename,"); 
      Serial.println(filename); 
    } 
  }    
}
