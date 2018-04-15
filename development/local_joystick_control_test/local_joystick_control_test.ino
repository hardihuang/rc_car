 int leftForward = 9;
 int leftBackward = 10;
 int rightForward = 5;
 int rightBackward = 6;
 int joyPinY = 0;
 int joyPinX = 1;
 

 int valueX = 0; 
 int valueY = 0;
 int leftSpeed = 0;
 int rightSpeed = 0;
 int moveDir = 0; // 0 = forward , 1 = backward


 void setup() {
  pinMode(leftForward, OUTPUT);
  pinMode(leftBackward, OUTPUT); 
  pinMode(rightForward, OUTPUT);
  pinMode(rightBackward, OUTPUT); 
  Serial.begin(9600);
 }

 void loop() {
  valueX = analogRead(joyPinX);
  valueY = analogRead(joyPinY);
  
  if(valueY<512){ //move forward
    moveDir = 0;
    leftSpeed = 512 - valueY;
    rightSpeed = leftSpeed;
    if(valueX > 512){ //turn left
      leftSpeed = leftSpeed - (valueX - 512);
    }else if(valueX < 512){   //turn right
      rightSpeed = rightSpeed -(512 - valueX);
    }
  }else if(valueY > 512){ //move backward
    moveDir = 1;
    leftSpeed = valueY - 512;
    rightSpeed = leftSpeed;
    if(valueX > 512){ //turn left
      leftSpeed = leftSpeed - (valueX - 512);
    }else if(valueX < 512){ //turn right
      rightSpeed = rightSpeed -(512 - valueX);
    }
  }
  
  
  leftSpeed = map(leftSpeed, 0, 512, 0, 255);
  rightSpeed = map(rightSpeed, 0, 512, 0, 255);
  
  if(moveDir == 0){
    Serial.print(" Forward ");
    analogWrite(leftForward,leftSpeed);
    analogWrite(leftBackward,0);
    analogWrite(rightForward,rightSpeed);
    analogWrite(rightBackward,0);
  }else if(moveDir == 1){
    Serial.print(" Backward ");
    analogWrite(leftForward,0);
    analogWrite(leftBackward,leftSpeed);
    analogWrite(rightForward,0);
    analogWrite(rightBackward,rightSpeed);
  }
    Serial.print(" Left speed: ");
    Serial.print(leftSpeed);
    Serial.print(" Right speed: ");
    Serial.println(rightSpeed);
    //delay(1000);
}








