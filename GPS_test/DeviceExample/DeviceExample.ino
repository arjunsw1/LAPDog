#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 115200;

// The TinyGPS++ object
TinyGPSPlus gps;
char rc;
bool loc_found = 0;
bool loc_pinned = 0;
double initial_lat;
double initial_lng;
double course_back;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
  
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    rc = ss.read();
    //Serial.print(rc);
    if (gps.encode(rc))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
     Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    if(!loc_found)
      loc_found = 1;
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" Altitude: "));
  if (gps.altitude.isValid())
  {
    Serial.print(gps.altitude.meters());
    Serial.print(F("m "));
    Serial.print(gps.altitude.feet());
    Serial.print(F(" feet "));
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  if(loc_pinned)
  {
     Serial.print(F(" Distance to Launchpad: "));
      Serial.print(gps.distanceBetween(initial_lat, initial_lng, gps.location.lat(), gps.location.lng()) );
      Serial.print(F("m "));

     course_back = gps.courseTo(initial_lat, initial_lng, gps.location.lat(), gps.location.lng());
     Serial.print(F(" Angle to Launchpad: "));
     Serial.print(course_back);
     Serial.print(F(" deg "));
     Serial.print(gps.cardinal(course_back));
   }
  
  if( (!loc_pinned)&&(loc_found) )
  {
    initial_lat = gps.location.lat();
    initial_lng = gps.location.lng();
    Serial.print('\n');
    Serial.println("Start Location Pinned!");
    loc_pinned = 1;
  }
  
  Serial.println();
}
