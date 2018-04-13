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
//None yet
/*-----( Declare objects )-----*/
// (Create an instance of a radio, specifying the CE and CS pins. )
RF24 myRadio (7, 8); // "myRadio" is the identifier you will use in following methods
/*-----( Declare Variables )-----*/
byte addresses[][6] = {"1Node"}; // Create address for 1 pipe.
int dataReceived;  // Data that will be received from the transmitter

struct dataStruct {
  unsigned long _micros;  // to save response times
  int Xposition;          // The Joystick position values
  int Yposition;
  bool switchOn;          // The Joystick push-down switch
} myData; 


void setup()   /****** SETUP: RUNS ONCE ******/
{
  // Use the serial Monitor (Symbol on far right). Set speed to 115200 (Bottom Right)
  Serial.begin(115200);
  delay(1000);
  myRadio.begin();  // Start up the physical nRF24L01 Radio
  myRadio.setChannel(108);  // Above most Wifi Channels
  myRadio.setPALevel(RF24_PA_MIN);
  myRadio.openReadingPipe(1, addresses[0]); // Use the first entry in array 'addresses' (Only 1 right now)
  myRadio.startListening();

}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{

  if ( myRadio.available()) // Check for incoming data from transmitter
  {
    while (myRadio.available())  // While there is data ready
    {
      myRadio.read( &myData, sizeof(myData) ); // Get the data payload (You must have defined that already!)
    }
    
    // DO something with the data, like print it
    Serial.print(F("X = "));
    Serial.println(myData.Xposition);
    Serial.print(F("Y = "));
    Serial.println(myData.Yposition);
    if(myData.switchOn == 1){
      Serial.println(F(" Switch ON"));
    }else{
      Serial.println(F(" Switch OFF"));
    }
  } //END Radio available

}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

//None yet
//*********( THE END )***********
