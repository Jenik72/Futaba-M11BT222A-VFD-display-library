
//
// test of vfd display Futaba  library by DENISOFT, Jan Jurcik, 2019 
// here is HW connection on pins   - you can change it in FutbaMT.h
//  EN_PIN 10
//  DIN_PIN 11
//  CLK_PIN 12
//  STB_PIN 13


#include "FutabaMT.h"
#include "Arduino.h"



unsigned char u;
 
char anumDisp [6];
char numDisp [7];
String pomocna;
byte xx=1,i;
long cislo;
byte bnm=0;

// start class vfd display
FutabaMT vfd;


//----------------------------------------

void setup() {

  vfd.Init();
  vfd.DiscRotate(1);
 // Serial.begin(19200);

}

//----------------------------------------
void loop()
{
      vfd.SetMem(0x00); // clear all segments
 
        // naplnenni ramecku kostickami 8 plus 8 bitu
          xx=1;
          for (i=0; i<=0x7; i++)
          {    vfd.WriteDigit(9,xx,0,0);  vfd.WriteDigit(8,xx,0,0);
            xx=xx << 1; ++xx;    delay(100);
          }
           xx=1;
          for (i=0; i<=0x7; i++)
          {    vfd.WriteDigit(9,255,xx,0);
            xx=xx << 1; ++xx;    delay(100);
          }
 

//---- test cela abeceda ----------------------------------------------------

 
  xx=vfd.firstChar;
  for (i=0; i<vfd.charSetLength; i++)
    {
      vfd.PrintChar(0,i+xx);
      vfd.PrintChar(1,i+xx);
      vfd.PrintChar(2,i+xx);
      vfd.PrintChar(3,i+xx);
      vfd.PrintChar(4,i+xx);
      delay(50);
    }

  vfd.DiscRotate(3);
 

//ééééééééééééé test nápis éééééééééééééééééééééééééééééééééééééééééééééééééééééééééééééé
    vfd.PrintTxt5("TAK");
    delay(500);
    vfd.PrintTxt5("TOHLE");
    delay(500);
    vfd.PrintTxt5("JSEM");
    delay(500);
    vfd.PrintTxt5("ZMAKL");
    delay(500);
 
 
//ooooooooooooooooooooooooooooooopocitani na hornim displayi oooooooooooooooooooooooooooooooooooooooo

 
for (cislo=0;cislo<=99;cislo++)  {

      pomocna=String(cislo);
      String(pomocna).toCharArray(numDisp, 7);
     
      vfd.PrintNumbers(numDisp,LOW);
  
      }
 
//oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

for (cislo=0;cislo<99999;cislo++)
    {

      pomocna=String(cislo);
      String(pomocna).toCharArray(numDisp, 7);
      vfd.PrintNumbers(numDisp,HIGH);
      //vfd.StatusLeds(0xff);
     // vfd.On_Auto();
     if(bnm==1) {vfd.Netwrk(0x7f,0x7f);}
     if(bnm==10) {vfd.Netwrk(0xff,0x7f);}
     if(bnm==20) {bnm=0;}
     bnm++;
     
      vfd.Dots(0xa5,0xa5);
      delay(50);
      vfd.PrintNumbers(numDisp,LOW);
    //  vfd.StatusLeds(0x00);
     // vfd.Off_Auto();
       
      if (pomocna.length() ==1) {pomocna="0000"+pomocna; }
      if (pomocna.length() ==2) {pomocna="000"+pomocna; }
      if (pomocna.length() ==3) {pomocna="00"+pomocna; }
      if (pomocna.length() ==4) {pomocna="0"+pomocna; }
      String(pomocna).toCharArray(anumDisp, 6);
       // anumDisp.toUpperCase();
 
      vfd.PrintTxt5(anumDisp);
      vfd.DiscBar(int(cislo/12500));
      vfd.Dots(0x5a,0x5a);
      delay(50);
     
      }

}
