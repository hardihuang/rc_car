 int leftForward = 9;
 int leftBackward = 10;
 int rightForward = 5;
 int rightBackward = 6;
 int joyPin1 = 0;

 int value1 = 0; 
 int pwm = 0;

 void setup() {
  pinMode(leftForward, OUTPUT);
  pinMode(leftBackward, OUTPUT); 
  pinMode(rightForward, OUTPUT);
  pinMode(rightBackward, OUTPUT); 
  Serial.begin(9600);
 }

 void loop() {
  value1 = analogRead(joyPin1);  
  pwm = map(value1, 0, 1023, 0, 255); 
  //Serial.println(pwm);
  if(pwm > 136){
    pwm = map(pwm-128, 0, 128, 100, 255);
    forward();
  }else if(pwm < 120){
    pwm = map(pwm, 0, 128, 255, 100);
    backward();
  }else{
    stop();
  }
}

void forward(){
  analogWrite(leftBackward,0);
  analogWrite(leftForward,pwm);
  analogWrite(rightBackward,0);
  analogWrite(rightForward,pwm);
}
void backward(){
  analogWrite(leftForward,0);
  analogWrite(leftBackward,pwm);
  analogWrite(rightForward,0);
  analogWrite(rightBackward,pwm);
}
void stop(){
  analogWrite(leftForward,0);
  analogWrite(leftBackward,0);
  analogWrite(rightForward,0);
  analogWrite(rightBackward,0);
}
