 // Display of the DED or the PFL on a 254x64 OLED display
 // based on the awesome DEDuino coding by Uri_ba (https://pit.uriba.org/tag/deduino/)

  #include <U8g2lib.h>
  #include "FalconDEDFont.h"   //load font

// Declare screen Object
  U8G2_SSD1322_NHD_256X64_1_4W_SW_SPI dedDisp(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* cs=*/ 5, /* dc=*/ 3, /* reset=*/ 4); 

  
// How many seconds should go by before screens turn off after no response recived to the "I'm alive" signal
  #define SLEEP_TIMER 120

// How many milliseconds go before the displays reset after initillizing (for user inspection)
  #define PRE_BOOT_PAUSE 1000
  #define POST_BOOT_PAUSE 2000

byte DED[5][25] = {{ 0 }};
unsigned long last_comm = millis();
short Run = 0;

///empties the DED display
void ClearDED()
{
 for (byte line=0;line<5;line++)
  { 
    for (byte digit=0;digit<25;digit++)
    {
      DED[line][digit]=0;
    }
  } 
}


//setup the OLED
void SetupDED() 
{
  delay(PRE_BOOT_PAUSE); 
  
  dedDisp.begin();
  dedDisp.setFont(FalconDED);
  dedDisp.setFontPosTop();
  dedDisp.firstPage();
  do {
      dedDisp.drawFrame(0,0,256,64);
      dedDisp.drawFrame(64,16,128,32);
      dedDisp.drawStr(62+DED_H_CONST, DED_V_CONST+14+DED_CHAR_H , "BMSAIT - DED");       
      dedDisp.drawLine(128,0,128,16);
      dedDisp.drawLine(128,48,128,64);
      dedDisp.drawLine(0,32,64,32);
      dedDisp.drawLine(192,32,256,32);
  } while ( dedDisp.nextPage() );
  delay(POST_BOOT_PAUSE); 
  ClearDED();
}



///copies the DED data from the main data container
void ReadDED()
{
  for (byte line=0;line<5;line++)
  {
    for (byte digit=0;digit<24;digit++)
    {
      if (digit<12)
        {
          if (datenfeld[line].wert[digit]=='\0')
            {DED[line][digit]=' ';}
          else
            {DED[line][digit] = datenfeld[line].wert[digit];}
        }
      else
        { 
          if (datenfeld[line+5].wert[digit-12]=='\0')
            {DED[line][digit]=' ';}
          else
            {DED[line][digit] = datenfeld[line+5].wert[digit-12];}}
    }
    DED[line][24]='\0';
  }
}


///updates the output of the DED on the OLED
void DrawDED() 
{
  dedDisp.firstPage();
  do {
    for (byte line = 0; line < 5; line++ ) 
    {
      dedDisp.drawStr(DED_H_CONST, line * DED_CHAR_H + DED_V_CONST, DED[line]);
    }
  } while ( dedDisp.nextPage() );
}


///update the DED
void UpdateDED() 
{ 
  if ((millis()-last_comm) > (5 * 1000))           //turn off display after 5 seconds without fresh data
    { dedDisp.setPowerSave(1);  }
  else
  {
    dedDisp.setPowerSave(0);
    ReadDED();
    DrawDED();
  } 
}