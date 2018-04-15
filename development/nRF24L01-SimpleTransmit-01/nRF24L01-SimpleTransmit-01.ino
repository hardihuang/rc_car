/*
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 7
   4 - CSN to Arduino pin 8
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
/*-----( Import needed libraries )-----*/
#include <SPI.h>   // Comes with Arduino IDE
#include "RF24.h"  // Download and Install (See above)
/*-----( Declare Constants and Pin Numbers )-----*/
#define JOYSTICK_X  A0
#define JOYSTICK_Y  A1
#define JOYSTICK_SW  2

/*-----( Declare objects )-----*/
// (Create an instance of a radio, specifying the CE and CS pins. )
RF24 myRadio (7, 8); // "myRadio" is the identifier you will use in following methods
/*-----( Declare Variables )-----*/
byte addresses[][6] = {"1Node"}; // Create address for 1 pipe.

struct dataStruct {
  unsigned long _micros;
  int Xposition;
  int Yposition;
  bool switchOn;
} myData;


void setup()
{
 Serial.begin(115200);
  pinMode(JOYSTICK_SW, INPUT_PULLUP);
  myRadio.begin();
  myRadio.setChannel(108);
  myRadio.setPALevel(RF24_PA_MIN);
  myRadio.openWritingPipe( addresses[0]); 
  delay(1000);
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  myRadio.stopListening();
  myData.Xposition = analogRead(JOYSTICK_X);
  myData.Yposition = analogRead(JOYSTICK_Y);
  myData.switchOn = !digitalRead(JOYSTICK_SW);

  myData._micros = micros();
  Serial.print("X: ");
  Serial.print(myData.Xposition);
  Serial.print("  Y: ");
  Serial.println(myData.Yposition);

  myRadio.write(&myData, sizeof(myData));
  delay(100);
}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/


//*********( THE END )***********
