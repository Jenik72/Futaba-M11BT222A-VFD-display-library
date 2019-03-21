

#include "FutabaMT.h"

#define DODELAY __asm__("nop\n\t""nop\n\t")

//*************************************************************


uint8_t _znakHorni, _znakDolni;
byte charSetLength;
byte firstChar;
byte  _znak7digit1, _znak7digit2 ;
byte _x,_temp;
char _numberDisp [7];
unsigned char _u,_iii,_ww,_pocitadlo, _pocitadlo2,_ee,_eex;


FutabaMT::FutabaMT()
  {
    charSetLength=43;
    firstChar=48;

  };



//----------------------------------------------------------------------------
void FutabaMT::Write (unsigned char dat)
{

  for (_iii=0;_iii<8;_iii++)
  {
    PTClkLow;
    if(dat & 0x01)
      PTDinHigh;
    else
      PTDinLow;
    dat >>= 1;
    PTClkHigh;
    DODELAY;
  }
}
//----------------------------------------------------------------------------
void FutabaMT::Cmd (unsigned char cmd)
{
  PTStbLow;
  //DODELAY;
  _x=cmd;

  for (_iii=0;_iii<8;_iii++)
  {
    PTClkLow;
    if(_x & 0x01)
      PTDinHigh;
    else
      PTDinLow;
    _x >>= 1;
    PTClkHigh;
    DODELAY;
  }
  PTStbHigh;
  DODELAY;
}

//----------------------------------------------------------------------------
void FutabaMT::SetMem(unsigned char dat)
{
//------------rewritten cmd (_x)---------
  PTStbLow;
  _x=CMD2_DSC_WDDM_INC;

  for (_iii=0;_iii<8;_iii++)
      {
        PTClkLow;
        if(_x & 0x01)
          PTDinHigh;
        else
          PTDinLow;
        _x >>= 1;
        PTClkHigh;
        DODELAY;
      }
  PTStbHigh;
  DODELAY;

    //-----------
    PTStbLow;
    Write(CMD3_ASC);
    for (_pocitadlo=0;_pocitadlo<=0x2f;_pocitadlo++)
        {
           Write(dat);
        }
    PTStbHigh;
    DODELAY;

}
//----------------------------------------------------------------------------
void FutabaMT::Write3Digit(unsigned char digit, unsigned char s1,unsigned char s2,unsigned char s3)
{
      Cmd( CMD2_DSC_WDDM_INC);
      PTStbLow;
      Write( CMD3_ASC | (digit * 3));
      Write( s1); //SG1-SG4 SG5-SG8
      Write( s2); //SG9-SG12 SG13-SG16
      Write( s3 & 0x0f); //SG17-SG20
      PTStbHigh;
      DODELAY;
}
//----------------------------------------------------------------------------
void FutabaMT::Write2Digit(unsigned char digit, unsigned char s1,unsigned char s2)
{
      Cmd( CMD2_DSC_WDDM_INC);
      PTStbLow;
      Write( CMD3_ASC | (digit * 3));
      Write( s1); //SG1-SG4 SG5-SG8
      Write( s2); //SG9-SG12 SG13-SG16
      // Write( s3 & 0x0f); //SG17-SG20
      PTStbHigh;
      DODELAY;
}
//----------------------------------------------------------------------------
void FutabaMT::WriteADigit(unsigned char digit, unsigned char s1)
{
      Cmd( CMD2_DSC_WDDM_INC);
      PTStbLow;
      Write( CMD3_ASC | (digit * 3 ));
      Write( s1); //SG1-SG4 SG5-SG8
      //Write( s2); //SG9-SG12 SG13-SG16
      //Write( s3 & 0x0f); //SG17-SG20
      PTStbHigh;
      DODELAY;
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void FutabaMT::WriteBDigit(unsigned char digit, unsigned char s2)
{
      Cmd( CMD2_DSC_WDDM_INC);
      PTStbLow;
      Write( CMD3_ASC | (digit * 3 + 1));
      //Write( s1); //SG1-SG4 SG5-SG8
      Write( s2); //SG9-SG12 SG13-SG16
      //Write( s3 & 0x0f); //SG17-SG20
      PTStbHigh;
      DODELAY;
}
//----------------------------------------------------------------------------
void FutabaMT::WriteCDigit(unsigned char digit, unsigned char s3)
{
      Cmd( CMD2_DSC_WDDM_INC);
      PTStbLow;
      Write( CMD3_ASC | (digit * 3 + 2));
      //Write( s1); //SG1-SG4 SG5-SG8
      //Write( s2); //SG9-SG12 SG13-SG16
      Write( s3 & 0x0f); //SG17-SG20
      PTStbHigh;
      DODELAY;
}
//----------------------------------------------------------------------------
void FutabaMT::WriteDisplayRam(unsigned char addr, unsigned char *data, unsigned char len)
{
      Cmd( CMD2_DSC_WDDM_INC);
      PTStbLow;
      Write( CMD3_ASC | addr);
      while(len--)
        {
          Write( *data++);
        }
      PTStbHigh;
      DODELAY;
}
//----------------------------------------------------------------------------
void FutabaMT::Init()
{
      pinMode(DIN_PIN, OUTPUT);
      pinMode(CLK_PIN, OUTPUT);
      pinMode(STB_PIN, OUTPUT);
      pinMode(EN_PIN, OUTPUT);
      digitalWrite(EN_PIN, HIGH); // enable the HV+Filament
      delay(200);
      PTStbHigh;
      PTClkHigh;
      PTDinHigh;
      DODELAY;
      SetMem(0x00);
      Cmd(CMD1_DMSC_11_17);
      Cmd(CMD3_DCC_04_16);
}
//----------------------------------------------------------------------------
void FutabaMT::PrintChar(unsigned char aaa, unsigned char bbb)
{
      _temp=bbb-firstChar;
      _znakHorni =  _charSetUpper[_temp];
      _znakDolni =  _charSetLower[_temp];
      Write2Digit(aaa,_znakHorni, _znakDolni);

}
//----------------------------------------------------------------------------
void FutabaMT::ClearNumDisp()
{
      Write2Digit(5,0,0);
      Write2Digit(6,0,0);
      Write2Digit(7,0,0);
}
//----------------------------------------------------------------------------
void FutabaMT::PrintTxt5(const char *data)
{
      const char *_p;
      _p = data;
      _pocitadlo=0;

      while (*_p && _pocitadlo<=4) {
          if (*_p==32) {
              PrintChar(4-_pocitadlo,60);
              _p++;_pocitadlo++;}   // replace SPACE with some empty character
          else {
              PrintChar(4-_pocitadlo,*_p);
              _p++;_pocitadlo++;
              }
      }
      // fill empty spaces to the end of displayed txt
      if (_pocitadlo<=4)  {
            for (_ee=_pocitadlo; _ee<=4; _ee++) {
            PrintChar(4-_ee,60);
            }
        }

}
//----------------------------------------------------------------------------
void FutabaMT::DiscRotate(unsigned int pocet)      // animace kolecka tam a zpet
{
        for (_pocitadlo=1; _pocitadlo<=pocet; _pocitadlo++)
          { _temp=1;
              for (_ww=0; _ww<=0x7; _ww++)
                {     WriteADigit(8,_temp);
                  _temp=_temp << 1;  _temp=_temp+1;   delay(70);
                }
          _temp=255;
              for (_ww=0; _ww<=0x7; _ww++)
                {    WriteADigit(8,_temp);
                  _temp=_temp >> 1;  delay(70);
                }
          WriteADigit(8,0x00);
          delay(70);
        }
}
//----------------------------------------------------------------------------
void FutabaMT::DiscValue(unsigned char pocet)      // light one segment of circle
{
   WriteADigit(8,pocet);
}
//----------------------------------------------------------------------------
void FutabaMT::StatusLeds(unsigned char pocet)      // light some segments DVD/REC/HDD/REC etc.
{
   WriteBDigit(8,pocet);
}
//----------------------------------------------------------------------------
void FutabaMT::Netwrk(unsigned char s1,unsigned char s2)      // light some segments of Network circle
{
   Write2Digit(10,s1,s2);
}
// -----------------------------------------------------------------------------
void FutabaMT::Dots(unsigned char s1,unsigned char s2)      // light some segments of box with dots  - 8+8 bits
{
   Write2Digit(9,s1,s2);
}
// -----------------------------------------------------------------------------

void FutabaMT::DiscBar(unsigned int pocet)      // light bar graph in circle
{
    _temp=0;
    if (pocet > 0 && pocet < 8) {
         for (_ww=1; _ww<=pocet; _ww++)
           {
             _temp=_temp << 1;  _temp=_temp+1;
           }
           WriteADigit(8,_temp);
     }
}
//----------------------------------------------------------------------------
void FutabaMT::PrintNumbers(const char *data, bool colons)
{
      const char *_p;
      _p = data;
      _pocitadlo2=0;
      for (_ee=0; _ee<=6; _ee++)  // copy input const string to normal array to allow manipulations
       {
          _numberDisp[_ee]=_p[_ee];
          if(_numberDisp[_ee]==0 && _pocitadlo2==0)  {_pocitadlo2=_ee;}
      }
      // trying to fomat digits to right
      for (_ee=0; _ee<=(5-_pocitadlo2); _ee++)
      {
        for   (_eex=0; _eex<5; _eex++)    //rotate char 1 step right and fill zero from left
        {
         _numberDisp[5-_eex]=_numberDisp[5-_eex-1];
        }
        _numberDisp[0]=48;
      }    //now decode 6 digits into 3 separate weird Futaba registers, it is a mess....has to be done manually
             _znak7digit1 =  _charSetNumber[_numberDisp[0]-firstChar];
             _znak7digit2 =  _charSetNumber[_numberDisp[5]-firstChar];
             Write2Digit(5,_znak7digit1, _znak7digit2);
             _znak7digit1 =  _charSetNumber[_numberDisp[1]-firstChar];
             _znak7digit2 =  _charSetNumber[_numberDisp[2]-firstChar];
            if (colons)   {_znak7digit1=_znak7digit1+128; _znak7digit2=_znak7digit2+128; }  // if flag of lighting colons is TRUE
             Write2Digit(7,_znak7digit1,_znak7digit2);
             _znak7digit1 =  _charSetNumber[_numberDisp[3]-firstChar];
             _znak7digit2 =  _charSetNumber[_numberDisp[4]-firstChar];
            if (colons)   {_znak7digit1=_znak7digit1+128; _znak7digit2=_znak7digit2+128; }  // if flag of lighting colons is TRUE
             Write2Digit(6,_znak7digit1, _znak7digit2);

}
//----------------------------------------------------------------------------
void FutabaMT::On_Auto()
{      WriteCDigit(6,1);   }
//----------------------------------------------------------------------------
void FutabaMT::Off_Auto()
{      WriteCDigit(6,0);   }
//----------------------------------------------------------------------------
void FutabaMT::On_Epg()
{      WriteCDigit(7,1);   }
//----------------------------------------------------------------------------
void FutabaMT::Off_Epg()
{      WriteCDigit(7,0);   }
//----------------------------------------------------------------------------
void FutabaMT::On_Trans()
{      WriteCDigit(5,1);   }
//----------------------------------------------------------------------------
void FutabaMT::Off_Trans()
{      WriteCDigit(5,0);   }
//----------------------------------------------------------------------------
void FutabaMT::On_Mail()
{      WriteCDigit(10,1);   }
//----------------------------------------------------------------------------
void FutabaMT::Off_Mail()
{      WriteCDigit(10,0);   }
//----------------------------------------------------------------------------
void FutabaMT::On_Rate()
{      WriteCDigit(3,1);   }
//----------------------------------------------------------------------------
void FutabaMT::Off_Rate()
{      WriteCDigit(3,0);   }
//----------------------------------------------------------------------------
void FutabaMT::On_Title()
{      WriteCDigit(4,1);   }
//----------------------------------------------------------------------------
void FutabaMT::Off_Title()
{      WriteCDigit(4,0);   }
//----------------------------------------------------------------------------
void FutabaMT::On_Chp()
{      WriteCDigit(2,1);   }
//----------------------------------------------------------------------------
void FutabaMT::Off_Chp()
{      WriteCDigit(2,0);   }
//----------------------------------------------------------------------------
void FutabaMT::On_Trk()
{      WriteCDigit(1,1);   }
//----------------------------------------------------------------------------
void FutabaMT::Off_Trk()
{      WriteCDigit(1,0);   }
//----------------------------------------------------------------------------
void FutabaMT::On_Ch()
{      WriteCDigit(0,1);   }
//----------------------------------------------------------------------------
void FutabaMT::Off_Ch()
{      WriteCDigit(0,0);   }
//----------------------------------------------------------------------------
void FutabaMT::On_Usb()
{      WriteCDigit(9,1);   }
//----------------------------------------------------------------------------
void FutabaMT::Off_Usb()
{      WriteCDigit(9,0);   }
//----------------------------------------------------------------------------
