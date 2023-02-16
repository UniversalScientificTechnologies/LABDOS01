# 1 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
String githash = "aaaaaaa";
String FWversion = "B512_01"; // 16 MHz crystal


/*
  LABDOS for balloon Fik-9
 
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

# 67 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 2
# 68 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 2
# 69 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 2

//#define LED_yellow  23   // PC7
# 87 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
uint16_t count = 0;
uint32_t serialhash = 0;
uint16_t base_offset = 512 /* 3th channel is channel 1 (ussually DCoffset or DCoffset+1, for version with noise reduction transistor)*/;
uint8_t lo, hi;
uint16_t u_sensor, maximum;
uint32_t tm, tm_old;


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

uint8_t bcdToDec(uint8_t b)
{
  return ( ((b >> 4)*10) + (b%16) );
}

void readRTC()
{
  Wire.beginTransmission(0x51);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(0x51, 6);
  Wire.read();
  //tm_s100 = bcdToDec(Wire.read());
  uint8_t tm_sec = bcdToDec(Wire.read() & 0x7f);
  uint8_t tm_min = bcdToDec(Wire.read() & 0x7f);
  tm = bcdToDec(Wire.read());
  tm += bcdToDec(Wire.read()) * 100;
  tm += bcdToDec(Wire.read()) * 10000;
  tm = tm * 60 * 60 + tm_min * 60 + tm_sec;
}


void setup()
{
  pinMode(21 /* PC5*/, 0x1);
  digitalWrite(21 /* PC5*/, 0x1);
  delay(100);

  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  Serial.println("#Cvak...");

  
# 151 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(0x7C)) 
# 151 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
       = (analog_reference << 6) | ((0 | 0x10) & 0x1F);
  
# 152 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(0x7B)) 
# 152 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
        = 0; // Switching ADC to Free Running mode
  (
# 153 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 153 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 |= 
# 153 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (1 << (5))
# 153 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 ); // ADC autotrigger enable (mandatory for free running mode)
  (
# 154 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 154 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 |= 
# 154 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (1 << (6))
# 154 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 ); // ADC start the first conversions
  (
# 155 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 155 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 |= 
# 155 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (1 << (
# 155 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 2
# 155 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 ))
# 155 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 ); // 0x111 = clock divided by 128, 125 kHz, 112 us for 14 cycles of one AD conversion, 12 us for 1.5 cycle for sample-hold
  (
# 156 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 156 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 |= 
# 156 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (1 << (
# 156 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 1
# 156 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 ))
# 156 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 );
  (
# 157 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 157 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 |= 
# 157 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (1 << (
# 157 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 0
# 157 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 ))
# 157 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 );

  pinMode(0 /* PB0*/, 0x1); // reset for peak detetor

  //pinMode(SDpower1, OUTPUT);  // SDcard interface
  //pinMode(SDpower2, OUTPUT);     
  //pinMode(SDpower3, OUTPUT);     
  //pinMode(SS, OUTPUT);     
  //pinMode(MOSI, INPUT);     
  //pinMode(MISO, INPUT);     
  //pinMode(SCK, OUTPUT);  

  
# 169 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 169 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
      = 0b10011110;
  
# 170 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 170 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
       = 0b00000000; // SDcard Power OFF

  
# 172 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)((0x01) + 0x20)) 
# 172 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
      = 0b11111100;
  
# 173 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)((0x02) + 0x20)) 
# 173 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
       = 0b00000000; // SDcard Power OFF
  
# 174 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 174 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
      = 0b11101100;
  
# 175 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)((0x08) + 0x20)) 
# 175 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
       = 0b00000000; // SDcard Power OFF
  
# 176 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 176 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
      = 0b11111100;
  
# 177 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 177 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
       = 0b10000000; // SDcard Power OFF

  Wire.setClock(100000);

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
      
# 224 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (*(volatile uint8_t *)(0x7C)) 
# 224 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
           = (analog_reference << 6) | 0b10001; // Select +A1,-A1 for offset correction
      delay(50);
      
# 226 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (*(volatile uint8_t *)(0x7B)) 
# 226 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
            = 0; // Switching ADC to Free Running mode
      (
# 227 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 227 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     |= 
# 227 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (1 << (5))
# 227 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     ); // ADC autotrigger enable (mandatory for free running mode)
      (
# 228 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 228 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     |= 
# 228 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (1 << (6))
# 228 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     ); // ADC start the first conversions
      (
# 229 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 229 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     |= 
# 229 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (1 << (
# 229 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     2
# 229 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     ))
# 229 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     ); // 0x111 = clock divided by 128
      (
# 230 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 230 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     |= 
# 230 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (1 << (
# 230 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     1
# 230 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     ))
# 230 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     );
      (
# 231 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 231 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     |= 
# 231 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (1 << (
# 231 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     0
# 231 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     ))
# 231 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     );
      (
# 232 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 232 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     |= 
# 232 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (1 << (4))
# 232 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     ); // reset interrupt flag from ADC
      while (
# 233 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
            (!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) & (1 << (4))))
# 233 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
                                      ); // wait for the first conversion 
      (
# 234 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 234 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     |= 
# 234 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
     (1 << (4))
# 234 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
     ); // reset interrupt flag from ADC
      lo = 
# 235 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
          (*(volatile uint8_t *)(0x78))
# 235 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
              ;
      hi = 
# 236 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
          (*(volatile uint8_t *)(0x79))
# 236 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
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
    
# 267 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
   (*(volatile uint8_t *)((0x04) + 0x20)) 
# 267 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
        = 0b10111110;
    
# 268 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 268 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
         = 0b00001111; // SDcard Power ON
    if (SD.begin(4 /* PB4*/))
    {
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      File dataFile = SD.open("DATALOG.TXT", (O_READ | O_WRITE | O_CREAT));
      // if the file is available, write to it:
      if (dataFile)
      {
        dataFile.println(dataString); // write to SDcard (800 ms) 
        dataFile.close();
      }
    }
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
}


void loop()
{
  uint16_t histogram[1024 /* number of channels in buffer for histogram, including negative numbers*/];
  for(int n=0; n<1024 /* number of channels in buffer for histogram, including negative numbers*/; n++)
  {
    histogram[n]=0;
  }

  // dummy conversion
  
# 306 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(0x7C)) 
# 306 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
       = (analog_reference << 6) | 0b10000; // Select +A0,-A1 for measurement
  
# 307 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(0x7B)) 
# 307 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
        = 0; // Switching ADC to Free Running mode
  (
# 308 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 308 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 |= 
# 308 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (1 << (5))
# 308 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 ); // ADC autotrigger enable (mandatory for free running mode)
  (
# 309 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 309 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 |= 
# 309 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (1 << (6))
# 309 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 ); // ADC start the first conversions
  (
# 310 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 310 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 |= 
# 310 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (1 << (
# 310 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 2
# 310 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 ))
# 310 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 ); // 0x111 = clock divided by 128
  (
# 311 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 311 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 |= 
# 311 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (1 << (
# 311 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 1
# 311 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 ))
# 311 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 );
  (
# 312 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 312 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 |= 
# 312 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (1 << (
# 312 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 0
# 312 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 ))
# 312 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 );

  
# 314 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 314 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
       = 1; // Set reset output for peak detector to H
  (
# 315 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 315 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 |= 
# 315 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (1 << (4))
# 315 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 ); // reset interrupt flag from ADC
  while (
# 316 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
        (!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) & (1 << (4))))
# 316 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
                                  ); // wait for the first dummy conversion 
  
# 317 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 317 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
      = 0b10011111; // Reset peak detector
  delayMicroseconds(100); // guaranteed reset
  
# 319 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 319 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
      = 0b10011110;

  (
# 321 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 321 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 |= 
# 321 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (1 << (4))
# 321 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 ); // reset interrupt flag from ADC

  uint16_t suppress = 0;

  while (
# 325 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
        (!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) & (1 << (4))))
# 325 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
                                  ); // wait for dummy conversion 
  
# 326 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 326 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
      = 0b10011111; // Reset peak detector
  asm("NOP"); // cca 6 us for 2k2 resistor and 1k capacitor in peak detector
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  
# 332 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 332 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
      = 0b10011110;
  (
# 333 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 333 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 |= 
# 333 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
 (1 << (4))
# 333 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
 ); // reset interrupt flag from ADC

  // dosimeter integration
  for (uint16_t i=0; i<(46000); i++) // cca 10 s
  {
    while (
# 338 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
          (!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) & (1 << (4))))
# 338 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
                                    ); // wait for end of conversion 
    delayMicroseconds(13); // 12 us wait for 1.5 cycle of 125 kHz ADC clock for sample/hold for next conversion

    
# 341 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
   (*(volatile uint8_t *)((0x04) + 0x20)) 
# 341 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
        = 0b10011111; // Reset peak detector
    delayMicroseconds(7); // cca 7 us for 2k2 resistor and 100n capacitor in peak detector
    
# 343 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
   (*(volatile uint8_t *)((0x04) + 0x20)) 
# 343 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
        = 0b10011110;
    (
# 344 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
   (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 344 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
   |= 
# 344 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
   (1 << (4))
# 344 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
   ); // reset interrupt flag from ADC

    // we have to read ADCL first; doing so locks both ADCL
    // and ADCH until ADCH is read.  reading ADCL second would
    // cause the results of each conversion to be discarded,
    // as ADCL and ADCH would be locked when it completed.
    lo = 
# 350 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
        (*(volatile uint8_t *)(0x78))
# 350 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
            ;
    hi = 
# 351 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
        (*(volatile uint8_t *)(0x79))
# 351 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
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
    readRTC();

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
    dataString += String(tm);
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

    
# 427 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
   (*(volatile uint8_t *)((0x04) + 0x20)) 
# 427 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
        = 0b10111110;
    
# 428 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino" 3
   (*(volatile uint8_t *)((0x05) + 0x20)) 
# 428 "/home/runner/work/LABDOS01/LABDOS01/fw/LABDOS512_SD/LABDOS512_SD.ino"
         = 0b00001111; // SDcard Power ON
    if (SD.begin(4 /* PB4*/))
    {
      // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      File dataFile = SD.open("DATALOG.TXT", (O_READ | O_WRITE | O_CREAT));
      // if the file is available, write to it:
      if (dataFile)
      {
        dataFile.println(dataString); // write to SDcard (800 ms) 
        dataFile.close();
      }
    }

    {
      Serial.println(dataString); // print to terminal (additional 700 ms in DEBUG mode)
      digitalWrite(23 /* PC7*/, 0x0);
    }
  }
}
