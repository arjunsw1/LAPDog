#include <Servo.h>

Servo FL_ESC, FR_ESC, RL_ESC, RR_ESC; //Create Servo object for each ESC
int Init_PB = 2;  // Flight initiation PB input
int Init_LED = 3; // Flight initiation PB LED output
int Grn_LED = 12; // Green indicator LED

void setup() {
  Serial.begin(9600);  // start serial at 9600 baud
  
  // ESC minimum speed = 1ms pulse width, ESC maximum speed = 2ms
  FL_ESC.attach(7,1000,2000);    // Front left ESC attached to pin 9
  FR_ESC.attach(4,1000,2000);    // Front left ESC attached to pin 5
  RL_ESC.attach(6,1000,2000);    // Front left ESC attached to pin 6
  RR_ESC.attach(5,1000,2000);    // Front left ESC attached to pin 3
}

void setSpeed(int speed){
  int Scaled_Speed = map(speed, 0, 100, 0, 180); // maps speed 0-100 to a value from 0-180
  FL_ESC.write(Scaled_Speed);
  FR_ESC.write(Scaled_Speed);
  RL_ESC.write(Scaled_Speed);
  RR_ESC.write(Scaled_Speed);
}

void loop() {
  if(digitalRead(Init_PB)) {
    analogWrite(Init_LED, 255);
    analogWrite(Grn_LED, 255);
    setSpeed(25);
  }
  
  else {
    analogWrite(Init_LED, 0);
    analogWrite(Grn_LED, 0);
    setSpeed(0);
  }    
}

