# 1 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
String githash = "379276a";
String FWversion = "L512_02"; // 16 MHz crystal


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

# 67 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 2
# 68 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 2
# 69 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 2

//#define LED_yellow  23   // PC7
# 87 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
uint16_t count = 0;
uint32_t serialhash = 0;
uint16_t base_offset = 512 /* 3th channel is channel 1 (ussually DCoffset or DCoffset+1, for version with noise reduction transistor)*/;
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

uint8_t analog_reference = 3; // DEFAULT, INTERNAL, INTERNAL1V1, INTERNAL2V56, or EXTERNAL

void setup()
{
  pinMode(21 /* PC5*/, 0x1);
  digitalWrite(21 /* PC5*/, 0x1);
  delay(100);

  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  Serial.println("#Cvak...");

  
# 127 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(0x7C)) 
# 127 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
       = (analog_reference << 6) | ((0 | 0x10) & 0x1F);
  
# 128 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(0x7B)) 
# 128 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
        = 0; // Switching ADC to Free Running mode
  (
# 129 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 129 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 |= 
# 129 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (1 << (5))
# 129 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 ); // ADC autotrigger enable (mandatory for free running mode)
  (
# 130 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 130 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 |= 
# 130 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (1 << (6))
# 130 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 ); // ADC start the first conversions
  (
# 131 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 131 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 |= 
# 131 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (1 << (
# 131 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 2
# 131 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 ))
# 131 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 ); // 0x111 = clock divided by 128, 125 kHz, 112 us for 14 cycles of one AD conversion, 12 us for 1.5 cycle for sample-hold
  (
# 132 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 132 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 |= 
# 132 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (1 << (
# 132 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 1
# 132 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 ))
# 132 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 );
  (
# 133 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 133 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 |= 
# 133 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (1 << (
# 133 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 0
# 133 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 ))
# 133 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 );

  pinMode(0 /* PB0*/, 0x1); // reset for peak detetor

  //pinMode(SDpower1, OUTPUT);  // SDcard interface
  //pinMode(SDpower2, OUTPUT);     
  //pinMode(SDpower3, OUTPUT);     
  //pinMode(SS, OUTPUT);     
  //pinMode(MOSI, INPUT);     
  //pinMode(MISO, INPUT);     
  //pinMode(SCK, OUTPUT);  

  
# 145 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 145 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
      = 0b10011110;
  
# 146 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 146 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
       = 0b00000000; // SDcard Power OFF

  
# 148 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)((0x01) + 0x20)) 
# 148 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
      = 0b11111100;
  
# 149 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)((0x02) + 0x20)) 
# 149 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
       = 0b00000000; // SDcard Power OFF
  
# 150 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 150 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
      = 0b11101100;
  
# 151 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)((0x08) + 0x20)) 
# 151 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
       = 0b00000000; // SDcard Power OFF
  
# 152 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 152 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
      = 0b11111100;
  
# 153 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 153 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
       = 0b10000000; // SDcard Power OFF

  Wire.setClock(100000);

  pinMode(21 /* PC5*/, 0x1);
  digitalWrite(21 /* PC5*/, 0x1);
  delay(100);
  Serial.println("#Hmmm...");
  pinMode(22 /* PC6*/, 0x1);
  digitalWrite(22 /* PC6*/, 0x1);
  delay(100);

  {
    uint16_t DCoffset;
    for (uint8_t n=0; n<8; n++)
    {
      // measurement of ADC offset
      
# 170 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (*(volatile uint8_t *)(0x7C)) 
# 170 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
           = (analog_reference << 6) | 0b10001; // Select +A1,-A1 for offset correction
      delay(50);
      
# 172 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (*(volatile uint8_t *)(0x7B)) 
# 172 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
            = 0; // Switching ADC to Free Running mode
      (
# 173 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 173 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     |= 
# 173 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (1 << (5))
# 173 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     ); // ADC autotrigger enable (mandatory for free running mode)
      (
# 174 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 174 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     |= 
# 174 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (1 << (6))
# 174 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     ); // ADC start the first conversions
      (
# 175 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 175 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     |= 
# 175 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (1 << (
# 175 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     2
# 175 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     ))
# 175 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     ); // 0x111 = clock divided by 128
      (
# 176 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 176 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     |= 
# 176 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (1 << (
# 176 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     1
# 176 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     ))
# 176 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     );
      (
# 177 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 177 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     |= 
# 177 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (1 << (
# 177 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     0
# 177 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     ))
# 177 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     );
      (
# 178 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 178 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     |= 
# 178 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (1 << (4))
# 178 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     ); // reset interrupt flag from ADC
      while (
# 179 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
            (!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) & (1 << (4))))
# 179 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
                                      ); // wait for the first conversion 
      (
# 180 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 180 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     |= 
# 180 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
     (1 << (4))
# 180 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
     ); // reset interrupt flag from ADC
      lo = 
# 181 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
          (*(volatile uint8_t *)(0x78))
# 181 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
              ;
      hi = 
# 182 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
          (*(volatile uint8_t *)(0x79))
# 182 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
              ;
      // combine the two bytes
      u_sensor = (hi << 8) | (lo);
      // manage negative values
      if (u_sensor <= (1024 /* number of channels in buffer for histogram, including negative numbers*//2)-1 ) {u_sensor += (1024 /* number of channels in buffer for histogram, including negative numbers*//2);} else {u_sensor -= (1024 /* number of channels in buffer for histogram, including negative numbers*//2);}
      DCoffset += u_sensor;
    }
    base_offset = DCoffset >> 3; // Calculate mean of 8 measurements
  }

  pinMode(23 /* PC7*/, 0x1);
  digitalWrite(23 /* PC7*/, 0x1);
  delay(100);

  // make a string for device identification output
  String dataString = "$DOS,LABDOS01A," + FWversion + "," + String(base_offset) + "," + githash + ","; // FW version and Git hash

  Wire.beginTransmission(0x58); // request SN from EEPROM
  Wire.write((int)0x08); // MSB
  Wire.write((int)0x00); // LSB
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)0x58, (uint8_t)16);
  for (int8_t reg=0; reg<16; reg++)
  {
    uint8_t serialbyte = Wire.read(); // receive a byte
    if (serialbyte<0x10) dataString += "0";
    dataString += String(serialbyte,16);
    serialhash += serialbyte;
  }

  {
    Serial.println(dataString); // print SN to terminal 
  }


  pinMode(21 /* PC5*/, 0x1);
  digitalWrite(21 /* PC5*/, 0x0);
  delay(100);
  pinMode(22 /* PC6*/, 0x1);
  digitalWrite(22 /* PC6*/, 0x0);
  delay(100);
  pinMode(23 /* PC7*/, 0x1);
  digitalWrite(23 /* PC7*/, 0x0);
  delay(100);


  // Initiates RTC
  rtc.autoprobe();
  rtc.resetClock();
}


void loop()
{
  uint16_t histogram[1024 /* number of channels in buffer for histogram, including negative numbers*/];
  for(int n=0; n<1024 /* number of channels in buffer for histogram, including negative numbers*/; n++)
  {
    histogram[n]=0;
  }

  // dummy conversion
  
# 243 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(0x7C)) 
# 243 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
       = (analog_reference << 6) | 0b10000; // Select +A0,-A1 for measurement
  
# 244 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(0x7B)) 
# 244 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
        = 0; // Switching ADC to Free Running mode
  (
# 245 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 245 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 |= 
# 245 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (1 << (5))
# 245 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 ); // ADC autotrigger enable (mandatory for free running mode)
  (
# 246 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 246 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 |= 
# 246 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (1 << (6))
# 246 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 ); // ADC start the first conversions
  (
# 247 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 247 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 |= 
# 247 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (1 << (
# 247 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 2
# 247 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 ))
# 247 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 ); // 0x111 = clock divided by 128
  (
# 248 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 248 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 |= 
# 248 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (1 << (
# 248 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 1
# 248 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 ))
# 248 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 );
  (
# 249 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 249 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 |= 
# 249 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (1 << (
# 249 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 0
# 249 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 ))
# 249 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 );

  
# 251 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 251 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
       = 1; // Set reset output for peak detector to H
  (
# 252 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 252 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 |= 
# 252 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (1 << (4))
# 252 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 ); // reset interrupt flag from ADC
  while (
# 253 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
        (!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) & (1 << (4))))
# 253 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
                                  ); // wait for the first dummy conversion 
  
# 254 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 254 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
      = 0b10011111; // Reset peak detector
  delayMicroseconds(100); // guaranteed reset
  
# 256 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 256 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
      = 0b10011110;

  (
# 258 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 258 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 |= 
# 258 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (1 << (4))
# 258 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 ); // reset interrupt flag from ADC

  uint16_t suppress = 0;

  while (
# 262 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
        (!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) & (1 << (4))))
# 262 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
                                  ); // wait for dummy conversion 
  
# 263 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 263 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
      = 0b10011111; // Reset peak detector
  asm("NOP"); // cca 6 us for 2k2 resistor and 1k capacitor in peak detector
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  
# 269 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 269 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
      = 0b10011110;
  (
# 270 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 270 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 |= 
# 270 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
 (1 << (4))
# 270 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
 ); // reset interrupt flag from ADC

  // dosimeter integration
  for (uint16_t i=0; i<(46000); i++) // cca 10 s
  {
    while (
# 275 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
          (!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) & (1 << (4))))
# 275 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
                                    ); // wait for end of conversion 
    delayMicroseconds(13); // 12 us wait for 1.5 cycle of 125 kHz ADC clock for sample/hold for next conversion

    
# 278 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
   (*(volatile uint8_t *)((0x04) + 0x20)) 
# 278 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
        = 0b10011111; // Reset peak detector
    delayMicroseconds(7); // cca 7 us for 2k2 resistor and 100n capacitor in peak detector
    
# 280 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
   (*(volatile uint8_t *)((0x04) + 0x20)) 
# 280 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
        = 0b10011110;
    (
# 281 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
   (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 281 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
   |= 
# 281 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
   (1 << (4))
# 281 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
   ); // reset interrupt flag from ADC

    // we have to read ADCL first; doing so locks both ADCL
    // and ADCH until ADCH is read.  reading ADCL second would
    // cause the results of each conversion to be discarded,
    // as ADCL and ADCH would be locked when it completed.
    lo = 
# 287 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
        (*(volatile uint8_t *)(0x78))
# 287 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
            ;
    hi = 
# 288 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino" 3
        (*(volatile uint8_t *)(0x79))
# 288 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512/LABDOS512.ino"
            ;

    // combine the two bytes
    u_sensor = (hi << 8) | (lo);

    // manage negative values
    if (u_sensor <= (1024 /* number of channels in buffer for histogram, including negative numbers*//2)-1 ) {u_sensor += (1024 /* number of channels in buffer for histogram, including negative numbers*//2);} else {u_sensor -= (1024 /* number of channels in buffer for histogram, including negative numbers*//2);}

    histogram[u_sensor]++;
/*
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
*/
  }

  // Data out
  {
    rtc.readClock(tm);
    RTCx::time_t t = RTCx::mktime(&tm);

    uint16_t noise = base_offset+3;
    uint32_t dose=0;


    for(int n=noise; n<(base_offset+500); n++)
    {
      dose += histogram[n];
    }

    digitalWrite(23 /* PC7*/, 0x1);

    // make a string for assembling the data to log:
    String dataString = "";

    // make a string for assembling the data to log:
    dataString += "$HIST,";
    dataString += String(count);
    dataString += ",";
    dataString += String(t-946684800);
    dataString += ",";
    dataString += String(suppress);
    dataString += ",";
    dataString += String(dose);

    for(int n=base_offset-1; n<(base_offset-1+500); n++)
    {
      dataString += ",";
      dataString += String(histogram[n]);
    }

    /* calibration
    uint16_t maxener=0; 
    uint16_t maxch=0;     
    for(int n=noise+3; n<(511); n++)  
    {
      if (histogram[n]>maxener) 
      {
        maxener = histogram[n];
        maxch = n; 
      }
    }
    dataString += "#";
    dataString += String(int(maxch-noise+3)); 
    dataString += ",";
    dataString += String(maxener); 
    */

    count++;

    {
      Serial.println(dataString); // print to terminal (additional 700 ms in DEBUG mode)
      digitalWrite(23 /* PC7*/, 0x0);
    }
  }
}
