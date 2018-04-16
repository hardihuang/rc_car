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
 int leftForward = 9;
 int leftBackward = 10;
 int rightForward = 5;
 int rightBackward = 6;
 int joyPin1 = 0;
 int moveDir = 0;	//0 = stop, 1 = forward, 2 = backward, 3 =spinning right, 4, spinning left

 int Xposition;
 int Yposition;
 int btnA;
 int btnB;
 int motorSpeedA = 0;
 int motorSpeedB = 0;
 
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
  bool btnA;          // The Joystick push-down switch
  bool btnB;
} myData; 


void setup()   /****** SETUP: RUNS ONCE ******/
{
  // Use the serial Monitor (Symbol on far right). Set speed to 115200 (Bottom Right)
  pinMode(leftForward, OUTPUT);
  pinMode(leftBackward, OUTPUT); 
  pinMode(rightForward, OUTPUT);
  pinMode(rightBackward, OUTPUT); 
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
	  Xposition = myData.Xposition;
    Yposition = myData.Yposition;
    btnA = myData.btnA;
    btnB = myData.btnB;
    
    if(Yposition < 470){	// backward
		motorSpeedA = map(Yposition, 470, 0, 0, 255);
		motorSpeedB = motorSpeedA;
		moveDir = 2;
	}else if(Yposition > 550 ){	//forward
		motorSpeedA = map(Yposition, 550, 1023, 0, 255);
		motorSpeedB = motorSpeedA;
		moveDir = 1;
	}else if(btnA == 1){
	  moveDir = 4;
	}else if(btnB == 1){
	  moveDir = 3;
	}else{	//stop
		motorSpeedA = 0;
		motorSpeedB = 0;
		moveDir = 0;
	}
	
	if(Xposition < 470){	//move to the left
		int xMapped = map(Xposition, 470, 0, 0, 255);
		motorSpeedA = motorSpeedA - xMapped;
		motorSpeedB = motorSpeedB + xMapped;
		
		if(motorSpeedA < 0){
			motorSpeedA = 0;
		}
		if(motorSpeedB > 255){
			motorSpeedB = 255;
		}
	}if(Xposition > 550){	//move to the right
		int xMapped = map(Xposition, 550, 1023, 0, 255);
		motorSpeedA = motorSpeedA + xMapped;
		motorSpeedB = motorSpeedB - xMapped;
		
		if(motorSpeedA > 255){
			motorSpeedA = 255;
		}
		if(motorSpeedB < 0){
			motorSpeedB = 0;
		}
	}
	
	if(motorSpeedA < 70){
		motorSpeedA = 0;
	}
	if(motorSpeedB < 70){
		motorSpeedB = 0;
	}
	
	if(moveDir == 1){
		Serial.print(" moving Forward ");
		
		analogWrite(leftForward, motorSpeedA);
		analogWrite(leftBackward, 0);
		analogWrite(rightForward, motorSpeedB);
		analogWrite(rightBackward, 0);
	}else if(moveDir == 2){
		Serial.print(" moving Backward ");
		
		analogWrite(leftForward,0);
		analogWrite(leftBackward,motorSpeedA);
		analogWrite(rightForward,0);
		analogWrite(rightBackward,motorSpeedB);
	}else if(moveDir == 3){
  Serial.print(" spinning right ");
	  digitalWrite(leftForward, 1);
    analogWrite(leftBackward, 0);
    analogWrite(rightForward, 0);
    digitalWrite(leftBackward, 1);
	}else if(moveDir == 4){
  Serial.print(" spinning left ");
   analogWrite(leftForward, 0);
    digitalWrite(leftBackward, 1);
    digitalWrite(rightForward, 1);
    analogWrite(leftBackward, 0);
  }else{
    Serial.print(" stop ");
	  analogWrite(leftForward,0);
    analogWrite(leftBackward,0);
    analogWrite(rightForward,0);
    analogWrite(rightBackward,0);
	}
	Serial.print(" left : ");
	Serial.print(motorSpeedA);
	Serial.print(" right : ");
	Serial.println(motorSpeedB);
  
  } //END Radio available

}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/

//*********( THE END )***********
