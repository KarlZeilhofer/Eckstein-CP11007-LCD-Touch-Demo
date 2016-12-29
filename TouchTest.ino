#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include <UTFT.h>
#include <Filters.h>

/*
 * (c) Karl Zeilhofer
 * 16.12.2015
 * 
 * Zeichnen mit weißer Farbe auf schwarzem Hintergrund. 
 * Touchpanel test. 
 * 
 * Funktioniert nicht so schlecht. Die Filter für die 
 * x/y-Kooridnaten bringen recht viel. 
 * Eine verbesserte touch-erkennung, also ob ein Stift 
 * aufliegt oder nicht wäre wünschenswert. 
 * Evt. mit schneller und langsamer z-filterung + hysterese möglich. 
 */


// Declare which fonts we will be using
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Due       : <display model>,25,26,27,28
// Teensy 3.x TFT Test Board                   : <display model>,23,22, 3, 4
// ElecHouse TFT LCD/SD Shield for Arduino Due : <display model>,22,23,31,33
//
// Remember to change the model parameter to suit your display module!
UTFT myGLCD(ITDB43,25,26,27,28);

FilterOnePole filterX( LOWPASS, 5 );
FilterOnePole filterY( LOWPASS, 5 );
FilterOnePole filterZ( LOWPASS, 5 );

#define TOUCH_CS_PIN  6 // MOSI, MISO, SCK

XPT2046_Touchscreen ts(TOUCH_CS_PIN);

void setup() {
  Serial.begin(9600);
  Serial.print("Welcome to the XPT2046 Touch Controller Test Program\n");
  
  Serial.print("Touchpanel initializing...");
  ts.begin();
  Serial.print("OK\n");

  Serial.print("LCD initializing...");
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  Serial.print("OK\n");

  myGLCD.setBackColor(0, 0, 0);
  myGLCD.clrScr();
  myGLCD.setColor(VGA_WHITE);
}

void loop() {
  TS_Point p = ts.getPoint();
  static int pressed=0;
//  Serial.print("Pressure = ");
//  Serial.print(p.z);

  filterZ.input(p.z);
  if (ts.touched()) {
//    Serial.print(", x = ");
//    Serial.print(p.x);
//    Serial.print(", y = ");
//    Serial.print(p.y);

    pressed++;
    
    if(pressed>10 && pressed <20)
    {
      filterX.setToNewValue(p.x);
      filterY.setToNewValue(p.y);
    }
    filterX.input(p.x);
    filterY.input(p.y);

    if(pressed > 50)
    {
      myGLCD.drawPixel(480-(filterY.output()-220)*480/3700, 272-(filterX.output()-280)*272/3550);
    }
  }else
  {
    if(pressed > 0)
      pressed--;
  }
//  Serial.println();
//  delay(100);
}
