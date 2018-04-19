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

int btnA = 2; // right spinning
int btnB = 3; //left spinning
int btnC = 4; //joystick btn
int btnD = 5; //undefined
int btnE = 6; //undefined

/*-----( Declare objects )-----*/
// (Create an instance of a radio, specifying the CE and CS pins. )
RF24 myRadio (7, 8); // "myRadio" is the identifier you will use in following methods
/*-----( Declare Variables )-----*/
byte addresses[][6] = {"1Node"}; // Create address for 1 pipe.

struct dataStruct {
  unsigned long _micros;
  int Xposition;
  int Yposition;
  bool btnA;
  bool btnB;
  bool btnC;
  bool btnD;
  bool btnE;
} myData;


void setup()
{
 Serial.begin(115200);
  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  pinMode(btnC, INPUT_PULLUP);
  pinMode(btnD, INPUT_PULLUP);
  pinMode(btnE, INPUT_PULLUP);
  myRadio.begin();
  myRadio.setChannel(108);
  myRadio.setDataRate(RF24_250KBPS); // Fast enough.. Better range
  //myRadio.setPALevel(RF24_PA_MIN);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.openWritingPipe( addresses[0]); 
  delay(1000);
}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
  myRadio.stopListening();
  myData.Xposition = analogRead(JOYSTICK_X);
  myData.Yposition = analogRead(JOYSTICK_Y);
  myData.btnA = !digitalRead(btnA);
  myData.btnB = !digitalRead(btnB);
  myData.btnC = !digitalRead(btnC);
  myData.btnD = !digitalRead(btnD);
  myData.btnE = !digitalRead(btnE);
  myData._micros = micros();
  /*
  Serial.print("X: ");
  Serial.print(myData.Xposition);
  Serial.print("  Y: ");
  Serial.println(myData.Yposition);
  Serial.print(" btnA: ");
  Serial.println(myData.btnA);
  Serial.print(" btnB: ");
  Serial.println(myData.btnB);
  */
  myRadio.write(&myData, sizeof(myData));
  delay(100);
}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/


//*********( THE END )***********
