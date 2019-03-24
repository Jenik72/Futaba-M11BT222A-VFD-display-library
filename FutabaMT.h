/*
 Copyright (c) 2019, Denisoft, CZ
 Original code (pre-library): unknown, based on pt6311 driver code from Internet

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

*/



#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif


#ifndef FutabaMT11BT222
#define FutabaMT11BT222

// here is HW connection on pins !
#define EN_PIN 10
#define DIN_PIN 11
#define CLK_PIN 12
#define STB_PIN 13

#define DODELAY __asm__("nop\n\t""nop\n\t")

#ifndef FASTIO
        #define PTDinHigh digitalWrite(DIN_PIN, HIGH)
        #define PTDinLow digitalWrite(DIN_PIN, LOW)
        // !!!CLK and STB are INVERTED in the hardware!!! //
        #define PTClkLow digitalWrite(CLK_PIN, HIGH)
        #define PTClkHigh digitalWrite(CLK_PIN, LOW)
        #define PTStbLow digitalWrite(STB_PIN, HIGH)
        #define PTStbHigh digitalWrite(STB_PIN, LOW)
#endif

#define CMD1_DMSC_08_20 0x00
#define CMD1_DMSC_09_19 0x08
#define CMD1_DMSC_10_18 0x09
#define CMD1_DMSC_11_17 0x0A
#define CMD1_DMSC_12_16 0x0B
#define CMD1_DMSC_13_15 0x0C
#define CMD1_DMSC_14_14 0x0D
#define CMD1_DMSC_15_13 0x0E
#define CMD1_DMSC_16_12 0x0F

#define CMD2_DSC_WDDM_INC (0x40 | 0x00)
#define CMD2_DSC_WDDM_FIX (0x40 | 0x04)
#define CMD2_DSC_WDLP (0x40 | 0x01)
#define CMD2_DSC_RKD (0x40 | 0x02)
#define CMD2_DSC_RSD (0x40 | 0x03)

#define CMD3_ASC 0xC0

#define CMD3_DCC_01_16 (0x88 | 0 )
#define CMD3_DCC_02_16 (0x88 | 1 )
#define CMD3_DCC_04_16 (0x88 | 2 )
#define CMD3_DCC_10_16 (0x88 | 3 )
#define CMD3_DCC_11_16 (0x88 | 4 )
#define CMD3_DCC_12_16 (0x88 | 5 )
#define CMD3_DCC_13_16 (0x88 | 6 )
#define CMD3_DCC_14_16 (0x88 | 7 )
#define CMD3_DCC_OFF 0x80

//*************************************************************************************************


class FutabaMT
{
public:
  FutabaMT();
  void Init();                                            // init of vfd display
  void SetMem(unsigned char dat);                         // fills vfd memeory with dat byte
  // --------user functions ------------
  void DiscRotate(unsigned int pocet);                    // rotates segment disc, pocet= number of repetitions
  void PrintChar(unsigned char aaa, unsigned char bbb);   // this output 1char on alphanumeric display    aaa= position 0-4, bbb - ascii code
  void PrintTxt5(const char *data);                       // this output 5char txt on alphanumeric display
  void PrintNumbers(const char *data,bool colons);     // print up to 6 digit numbers on 7segment positions, colons is flag for turning colons ON
  void DiscValue(unsigned char pocet);                    // light one of 8 segments of disc
  void DiscBar(unsigned int pocet);                       // light bargraph in 8 segments of disc
  void ClearNumDisp();                                    // clear number display
  void On_Auto();                                         // turn on "AUTO" sign
  void Off_Auto();                                        // turn off "AUTO" sign
  void On_Epg();                                          // turn on "EPG"
  void Off_Epg();                                         // turn off "EPG"
  void On_Trans();                                        // turn on etc.
  void Off_Trans();                                       // turn off
  void On_Mail();                                         // turn on
  void Off_Mail();                                        // turn off
  void On_Rate();                                         // turn on
  void Off_Rate();                                        // turn off
  void On_Title();                                        // turn on
  void Off_Title();                                       // turn off
  void On_Chp();                                          // turn on
  void Off_Chp();                                         // turn off
  void On_Trk();                                          // turn on
  void Off_Trk();                                         // turn off
  void On_Ch();                                           // turn on
  void Off_Ch();                                          // turn off
  void On_Usb();                                          // turn on
  void Off_Usb();                                         // turn off
  void Netwrk(unsigned char s1,unsigned char s2);         // light some segments of Network circle
  void StatusLeds(unsigned char pocet);                   // light some segments DVD/REC/HDD/REC etc.
  void Dots(unsigned char s1,unsigned char s2);           // light some segments of dot boxes - 8 + 8bits, displayed from bottom

//--------------------------------------
  byte charSetLength;
  byte firstChar;

private:
  //-----------internal control functions of chip pt6311
  void Write (unsigned char dat);
  void Cmd (unsigned char cmd);

  void Write3Digit(unsigned char digit, unsigned char s1,unsigned char s2,unsigned char s3);  // writes 3 following memory cells at once
  void Write2Digit(unsigned char digit, unsigned char s1,unsigned char s2);                   // writes 2 following memory cells at once
  void WriteADigit(unsigned char digit, unsigned char s1);                   // writes 1 memory cell at once - position A
  void WriteBDigit(unsigned char digit, unsigned char s2);                   // writes 1 memory cell at once - position A
  void WriteCDigit(unsigned char digit, unsigned char s3);                   // writes 1 memory cell at once - position A
  void WriteDisplayRam(unsigned char addr, unsigned char *data, unsigned char len);

// ----------------------- char tables  - first two lines for alphanumeric 5 position display , third row digits for 7segment number display -------
  uint8_t _charSetUpper[43] =  { 49,24,209,209,240,225,225,17,241,241,0,0,0,0,0,0,0,241,213,33,149,161,161,225,240,132,16,168,32,186,178,49,241,177,241,225,133,48,168,176,138,138,137};
  uint8_t _charSetLower[43] = { 70,2,69,66,3,67,71,2,71,67,0,0,0,0,0,0,0,7,82,68,82,69,5,70,7,16,70,37,68,6,38,70,5,102,37,67,16,70,12,46,40,16,72};
  uint8_t _charSetNumber[10] = {119,18,107,91,30,93,125,19,127,31};

  uint8_t _znakHorni, _znakDolni;
  byte  _znak7digit1, _znak7digit2;
  unsigned char _u,_iii,_ww,_ee,_eex, _pocitadlo2;
  char _numberDisp[7];
  byte _x,_pocitadlo,_temp;
  const char *_p;
  String pomocna;

};
#endif
