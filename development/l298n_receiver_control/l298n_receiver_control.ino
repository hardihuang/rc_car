/* 
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 7
   4 - CSN to Arduino pin 8
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
*/

#include <SPI.h>
#include "RF24.h" 

#define enA 10
#define in1 6
#define in2 5
#define enB 9
#define in3 4
#define in4 3

int motorSpeedA = 0;
int motorSpeedB = 0;
int xAxis = 0;
int yAxis = 0;
bool btnA = 0;
bool btnB = 0;
bool btnC = 0;
bool btnD = 0;
bool btnE = 0;


RF24 myRadio (7, 8); 
byte addresses[][6] = {"1Node"};
int dataReceived;

struct dataStruct {
  unsigned long _micros;  // to save response times
  int Xposition;          // The Joystick position values
  int Yposition;
  bool btnA;
  bool btnB;
  bool btnC;
  bool btnD;
  bool btnE;
} myData; 


void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  Serial.begin(115200);
  delay(1000);
  myRadio.begin();  // Start up the physical nRF24L01 Radio
  myRadio.setChannel(108);  // Above most Wifi Channels
  myRadio.setDataRate(RF24_250KBPS); // Fast enough.. Better range
  //myRadio.setPALevel(RF24_PA_MIN);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.openReadingPipe(1, addresses[0]); // Use the first entry in array 'addresses' (Only 1 right now)
  myRadio.startListening();
  
}

void loop() {
      
  if(myRadio.available()){
    while (myRadio.available())  // While there is data ready
    {
      myRadio.read( &myData, sizeof(myData) ); // Get the data payload (You must have defined that already!)
      //Serial.println("Getting Data!");
    }
    xAxis = myData.Xposition;
    yAxis = myData.Yposition;
    btnA = myData.btnA;
    btnB = myData.btnB;
    btnC = myData.btnC;
    btnD = myData.btnD;
    btnE = myData.btnE;
    
    Serial.print(" X: ");
    Serial.print(xAxis);
    Serial.print(" Y: ");
    Serial.print(yAxis);
    Serial.print(" btnA: ");
    Serial.print(btnA);
    Serial.print(" btnB: ");
    Serial.print(btnB);
    Serial.print(" btnC: ");
    Serial.print(btnC);
    Serial.print(" btnD: ");
    Serial.print(btnD);
    Serial.print(" btnE: ");
    Serial.println(btnE);
    
    if(btnA == 1){  //spinning to the left
      motorSpeedA = 255;
      motorSpeedB = 255;
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }else if(btnB == 1){
      motorSpeedA = 255;
      motorSpeedB = 255;
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }else{
      // Y-axis used for forward and backward control
      if (yAxis < 470) {
        //Serial.print(" backward ");
        // Set Motor A backward
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        // Set Motor B backward
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
        // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
        motorSpeedA = map(yAxis, 470, 0, 0, 255);
        motorSpeedB = map(yAxis, 470, 0, 0, 255);
      }else if (yAxis > 550) {
        //Serial.print(" forward ");
        // Set Motor A forward
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        // Set Motor B forward
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
        // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
        motorSpeedA = map(yAxis, 550, 1023, 0, 255);
        motorSpeedB = map(yAxis, 550, 1023, 0, 255);
      } // If joystick stays in middle the motors are not moving
      else {
        //Serial.print(" stop ");
        motorSpeedA = 0;
        motorSpeedB = 0;
      }
      
        // X-axis used for left and right control
      if (xAxis < 470) {
        // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
        int xMapped = map(xAxis, 470, 0, 0, 255);
        // Move to left - decrease left motor speed, increase right motor speed
        motorSpeedA = motorSpeedA - xMapped;
        motorSpeedB = motorSpeedB + xMapped;
        // Confine the range from 0 to 255
        if (motorSpeedA < 0) {
          motorSpeedA = 0;
        }
        if (motorSpeedB > 255) {
          motorSpeedB = 255;
        }
      }
      if (xAxis > 550) {
        // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
        int xMapped = map(xAxis, 550, 1023, 0, 255);
        // Move right - decrease right motor speed, increase left motor speed
        motorSpeedA = motorSpeedA + xMapped;
        motorSpeedB = motorSpeedB - xMapped;
        // Confine the range from 0 to 255
        if (motorSpeedA > 255) {
          motorSpeedA = 255;
        }
        if (motorSpeedB < 0) {
          motorSpeedB = 0;
        }
      }
      // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
      if (motorSpeedA < 20) {
        motorSpeedA = 0;
      }
      if (motorSpeedB < 20) {
        motorSpeedB = 0;
      }
    }
    
    analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
    analogWrite(enB, motorSpeedB); // Send PWM signal to motor B
  }
  
  
} //end loop
