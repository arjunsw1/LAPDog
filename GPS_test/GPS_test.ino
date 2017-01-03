// GPS Module - GP-20U7 - Arduino UNO Example
//
// Author: Gustavo Bertoli
//
// References:
// https://cdn.sparkfun.com/datasheets/GPS/GP-20U7.pdf
// https://www.arduino.cc/en/Reference/SoftwareSerial
// http://forum.arduino.cc/index.php?topic=288234.0
// 

#include <SoftwareSerial.h> 

SoftwareSerial GPS_Serial(4, 3); // RX, TX

void setup() {
  Serial.begin(115200);
  GPS_Serial.begin(9600);
 Serial.println("Goodnight moon!"); 
}

void loop() {
   char rc;

   if (GPS_Serial.available()){
        rc = GPS_Serial.read();
        Serial.print(rc);
   }
   //Serial.println(' ');
}
