# 1 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
String FWversion = "UAV00"; // 8 MHz crystal


/*
  SIPIN for UAV
 
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
// Compiled with: 
// MightyCore
*/

# 62 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 2
# 63 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 2
# 64 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 2
# 65 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 2

//#define LED_yellow  23   // PC7
# 82 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
uint8_t mavlink_info[25];

uint16_t count = 0;
uint32_t serialhash = 0;
uint16_t base_offset = 256 /* 3th channel is channel 1 (ussually DCoffset or DCoffset+1, for version with noise reduction transistor)*/;
uint8_t lo, hi;
uint16_t u_sensor, maximum;

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


HardwareSerial &hs = Serial;

void SendTunnelData(uint8_t *payload_data, uint8_t payload_length, uint8_t payload_type = 0, uint8_t sysid = 0, uint8_t compid = 0){

  mavlink_message_t msgtn;
  mavlink_tunnel_t tunnel;
  uint8_t buftn[(255 /*|< Maximum payload length*/ + ((9 /*|< Length of core header (of the comm. layer)*/ + 1) /*|< Length of all header bytes, including core and stx*/ + 2) + 13) /*|< Maximum packet length*/];
  mavlink_msg_tunnel_pack(1, 10, &msgtn, sysid, compid, payload_type, payload_length, payload_data);
  uint16_t lentn = mavlink_msg_to_send_buffer(buftn, &msgtn);
  hs.write(buftn, lentn);

  return;
}

void setup()
{
  pinMode(21 /* PC5*/, 0x1);
  digitalWrite(21 /* PC5*/, 0x1);
  delay(100);

  memset(mavlink_info, 0, sizeof(mavlink_info));

  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  Serial.println("#Cvak...");

  
# 140 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(0x7C)) 
# 140 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
       = (analog_reference << 6) | ((0 | 0x10) & 0x1F);
  
# 141 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(0x7B)) 
# 141 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
        = 0; // Switching ADC to Free Running mode
  (
# 142 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 142 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 |= 
# 142 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (1 << (5))
# 142 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 ); // ADC autotrigger enable (mandatory for free running mode)
  (
# 143 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 143 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 |= 
# 143 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (1 << (6))
# 143 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 ); // ADC start the first conversions
  (
# 144 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 144 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 |= 
# 144 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (1 << (
# 144 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 2
# 144 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 ))
# 144 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 ); // 0x111 = clock divided by 128, 125 kHz, 104 us for 13 cycles of one AD conversion, 12 us for 1.5 cycle for sample-hold
  (
# 145 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 145 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 |= 
# 145 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (1 << (
# 145 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 1
# 145 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 ))
# 145 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 );
  (
# 146 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 146 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 |= 
# 146 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (1 << (
# 146 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 0
# 146 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 ))
# 146 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 );

  pinMode(0 /* PB0*/, 0x1); // reset for peak detetor

  //pinMode(SDpower1, OUTPUT);  // SDcard interface
  //pinMode(SDpower2, OUTPUT);     
  //pinMode(SDpower3, OUTPUT);     
  //pinMode(SS, OUTPUT);     
  //pinMode(MOSI, INPUT);     
  //pinMode(MISO, INPUT);     
  //pinMode(SCK, OUTPUT);  

  
# 158 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 158 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
      = 0b10011110;
  
# 159 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 159 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
       = 0b00000000; // SDcard Power OFF

  
# 161 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)((0x01) + 0x20)) 
# 161 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
      = 0b11111100;
  
# 162 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)((0x02) + 0x20)) 
# 162 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
       = 0b00000000; // SDcard Power OFF
  
# 163 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)((0x07) + 0x20)) 
# 163 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
      = 0b11101100;
  
# 164 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)((0x08) + 0x20)) 
# 164 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
       = 0b00000000; // SDcard Power OFF
  
# 165 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)((0x0A) + 0x20)) 
# 165 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
      = 0b11111100;
  
# 166 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)((0x0B) + 0x20)) 
# 166 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
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
      
# 183 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (*(volatile uint8_t *)(0x7C)) 
# 183 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
           = (analog_reference << 6) | 0b10001; // Select +A1,-A1 for offset correction
      delay(50);
      
# 185 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (*(volatile uint8_t *)(0x7B)) 
# 185 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
            = 0; // Switching ADC to Free Running mode
      (
# 186 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 186 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     |= 
# 186 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (1 << (5))
# 186 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     ); // ADC autotrigger enable (mandatory for free running mode)
      (
# 187 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 187 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     |= 
# 187 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (1 << (6))
# 187 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     ); // ADC start the first conversions
      (
# 188 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 188 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     |= 
# 188 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (1 << (
# 188 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     2
# 188 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     ))
# 188 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     ); // 0x111 = clock divided by 128
      (
# 189 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 189 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     |= 
# 189 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (1 << (
# 189 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     1
# 189 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     ))
# 189 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     );
      (
# 190 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 190 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     |= 
# 190 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (1 << (
# 190 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     0
# 190 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     ))
# 190 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     );
      (
# 191 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 191 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     |= 
# 191 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (1 << (4))
# 191 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     ); // reset interrupt flag from ADC
      while (
# 192 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
            (!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) & (1 << (4))))
# 192 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
                                      ); // wait for the first conversion 
      (
# 193 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 193 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     |= 
# 193 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
     (1 << (4))
# 193 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
     ); // reset interrupt flag from ADC
      lo = 
# 194 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
          (*(volatile uint8_t *)(0x78))
# 194 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
              ;
      hi = 
# 195 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
          (*(volatile uint8_t *)(0x79))
# 195 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
              ;
      // combine the two bytes
      u_sensor = (hi << 7) | (lo >> 1);
      // manage negative values
      if (u_sensor <= (512 /* number of channels in buffer for histogram, including negative numbers*//2)-1 ) {u_sensor += (512 /* number of channels in buffer for histogram, including negative numbers*//2);} else {u_sensor -= (512 /* number of channels in buffer for histogram, including negative numbers*//2);}
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
}


void loop()
{
  uint16_t histogram[512 /* number of channels in buffer for histogram, including negative numbers*/];
  uint8_t mavlink_buffer_1[128];
  uint8_t mavlink_buffer_2[128];
  uint8_t mavlink_buffer_3[128];

  memset(histogram, 0, sizeof(histogram)); // clear array
  memset(mavlink_buffer_1, 0, sizeof(mavlink_buffer_1));
  memset(mavlink_buffer_2, 0, sizeof(mavlink_buffer_2));
  memset(mavlink_buffer_3, 0, sizeof(mavlink_buffer_3));

  // dummy conversion
  
# 258 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(0x7C)) 
# 258 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
       = (analog_reference << 6) | 0b10000; // Select +A0,-A1 for measurement
  
# 259 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(0x7B)) 
# 259 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
        = 0; // Switching ADC to Free Running mode
  (
# 260 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 260 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 |= 
# 260 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (1 << (5))
# 260 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 ); // ADC autotrigger enable (mandatory for free running mode)
  (
# 261 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 261 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 |= 
# 261 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (1 << (6))
# 261 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 ); // ADC start the first conversions
  (
# 262 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 262 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 |= 
# 262 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (1 << (
# 262 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 2
# 262 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 ))
# 262 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 ); // 0x111 = clock divided by 128
  (
# 263 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 263 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 |= 
# 263 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (1 << (
# 263 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 1
# 263 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 ))
# 263 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 );
  (
# 264 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 264 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 |= 
# 264 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (1 << (
# 264 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 0
# 264 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 ))
# 264 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 );

  
# 266 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)((0x05) + 0x20)) 
# 266 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
       = 1; // Set reset output for peak detector to H
  (
# 267 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 267 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 |= 
# 267 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (1 << (4))
# 267 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 ); // reset interrupt flag from ADC
  while (
# 268 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
        (!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) & (1 << (4))))
# 268 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
                                  ); // wait for the first dummy conversion 
  
# 269 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 269 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
      = 0b10011111; // Reset peak detector
  delayMicroseconds(100); // guaranteed reset
  
# 271 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 271 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
      = 0b10011110;

  (
# 273 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 273 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 |= 
# 273 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (1 << (4))
# 273 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 ); // reset interrupt flag from ADC

  uint16_t suppress = 0;

  while (
# 277 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
        (!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) & (1 << (4))))
# 277 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
                                  ); // wait for dummy conversion 
  
# 278 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 278 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
      = 0b10011111; // Reset peak detector
  asm("NOP"); // cca 6 us for 2k2 resistor and 1k capacitor in peak detector
  asm("NOP");
  asm("NOP");
  asm("NOP");
  asm("NOP");
  
# 284 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)((0x04) + 0x20)) 
# 284 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
      = 0b10011110;
  (
# 285 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 285 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 |= 
# 285 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
 (1 << (4))
# 285 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
 ); // reset interrupt flag from ADC

  // dosimeter integration
  //for (uint16_t i=0; i<(46000); i++)    // cca 10 s
  for (uint16_t i=0; i<(4600*2); i++) // cca 2 s
  {
    while (
# 291 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
          (!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) & (1 << (4))))
# 291 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
                                    ); // wait for end of conversion 
    delayMicroseconds(150); // 12 us wait for 1.5 cycle of 125 kHz ADC clock for sample/hold for next conversion

    
# 294 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
   (*(volatile uint8_t *)((0x04) + 0x20)) 
# 294 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
        = 0b10011111; // Reset peak detector
    delayMicroseconds(7); // cca 7 us for 2k2 resistor and 100n capacitor in peak detector
    
# 296 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
   (*(volatile uint8_t *)((0x04) + 0x20)) 
# 296 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
        = 0b10011110;
    (
# 297 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
   (*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0x7A)))))) 
# 297 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
   |= 
# 297 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
   (1 << (4))
# 297 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
   ); // reset interrupt flag from ADC

    // we have to read ADCL first; doing so locks both ADCL
    // and ADCH until ADCH is read.  reading ADCL second would
    // cause the results of each conversion to be discarded,
    // as ADCL and ADCH would be locked when it completed.
    lo = 
# 303 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
        (*(volatile uint8_t *)(0x78))
# 303 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
            ;
    hi = 
# 304 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino" 3
        (*(volatile uint8_t *)(0x79))
# 304 "/home/runner/work/LABDOS01/LABDOS01/fw/UAVDOS/UAVDOS.ino"
            ;

    // combine the two bytes
    u_sensor = (hi << 7) | (lo >> 1);

    // manage negative values
    if (u_sensor <= (512 /* number of channels in buffer for histogram, including negative numbers*//2)-1 ) {u_sensor += (512 /* number of channels in buffer for histogram, including negative numbers*//2);} else {u_sensor -= (512 /* number of channels in buffer for histogram, including negative numbers*//2);}

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
    uint16_t noise = base_offset+2;
    uint32_t dose=0;


    for(int n=noise; n<(base_offset+252); n++)
    {
      dose += histogram[n];
    }

    digitalWrite(23 /* PC7*/, 0x1);

    mavlink_buffer_1[0] = (count & 0xff00) >> 8;
    mavlink_buffer_1[1] = (count & 0x00ff);
    mavlink_buffer_1[2] = (dose& 0xff000000) >> 24;
    mavlink_buffer_1[3] = (dose& 0x00ff0000) >> 16;
    mavlink_buffer_1[4] = (dose& 0x0000ff00) >> 8;
    mavlink_buffer_1[5] = (dose& 0x000000ff);
    mavlink_buffer_1[6] = (suppress & 0xff00) >> 8;
    mavlink_buffer_1[7] = (suppress & 0x00ff);
    mavlink_buffer_1[8] = (base_offset & 0xff00) >> 8;
    mavlink_buffer_1[9] = (base_offset & 0x00ff);
    mavlink_buffer_1[10] = 0;
    mavlink_buffer_1[11] = 0;


    for(int n=base_offset; n<(128+base_offset); n++)
    {
      mavlink_buffer_2[n-base_offset];
    }
    for(int n=128+base_offset; n<(base_offset+252); n++)
    {
      mavlink_buffer_3[n-128-base_offset];
    }

    // // make a string for assembling the data to log:
    // String dataString = "";

    // // make a string for assembling the data to log:
    // dataString += "$HIST,";
    // dataString += String(count); 
    // dataString += ",";  
    // dataString += String(suppress);
    // dataString += ",";
    // dataString += String(dose);

    // for(int n=base_offset-1; n<(base_offset-1+RANGE); n++)  
    // {
    //   dataString += ",";
    //   dataString += String(histogram[n]); 
    // }

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
      //SendTunnelData(mavlink_info, sizeof(mavlink_info), 10+0, 0, 0);
      SendTunnelData(mavlink_buffer_1, sizeof(mavlink_buffer_1), 10+1, 0, 0);
      SendTunnelData(mavlink_buffer_2, sizeof(mavlink_buffer_2), 10+2, 0, 0);
      SendTunnelData(mavlink_buffer_3, sizeof(mavlink_buffer_3), 10+3, 0, 0);
      digitalWrite(23 /* PC7*/, 0x0);
    }
  }
}
