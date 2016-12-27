#include <TinyGPS++.h>

/* 
   This sample sketch should be the first you try out when you are testing a TinyGPS++
   (TinyGPSPlus) installation.  In normal use, you feed TinyGPS++ objects characters from
   a serial NMEA GPS device, but this example uses static strings for simplicity.
*/
// A sample NMEA stream.
/*
const char *gpsStream =

  "$GPRMC,013403.00,A,3309.05398,N,11708.73566,W,0.076,,241216,,,A*64\r\n"
  "$GPGGA,013402.00,3309.05429,N,11708.73568,W,1,05,3.34,218.1,M,-33.2,M,,*6D\r\n"
  
  "$GPRMC,013421.00,A,3309.05432,N,11708.73500,W,0.124,,241216,,,A*65\r\n"
  "$GPGGA,013420.00,3309.05436,N,11708.73503,W,1,05,3.34,218.9,M,-33.2,M,,*66\r\n"
  
  "$GPRMC,013444.00,A,3309.05382,N,11708.73596,W,3.731,,241216,,,A*64\r\n"
  "$GPGGA,013443.00,3309.05248,N,11708.73698,W,1,05,3.33,213.2,M,-33.2,M,,*6A\r\n"

  "$GPVTG,,T,,M,0.009,N,0.016,K,A*2D\r\n"
  "$GPGGA,013402.00,3309.05429,N,11708.73568,W,1,05,3.34,218.1,M,-33.2,M,,*6D\r\n"
  "$GPGSA,A,3,09,26,23,16,07,,,,,,,,4.53,3.34,3.05*0E\r\n"
  "$GPGSV,3,1,10,03,27,179,,06,01,272,,07,46,284,37,08,16,140,18*7F\r\n"
  "$GPGSV,3,2,10,09,60,331,39,16,38,046,38,23,69,083,40,26,11,040,26*7D\r\n"
  "$GPGSV,3,3,10,48,48,208,37,51,50,162,38*79\r\n"
  "$GPGLL,3309.05429,N,11708.73568,W,013402.00,A,A*7C\r\n"
  "$GPRMC,013403.00,A,3309.05398,N,11708.73566,W,0.076,,241216,,,A*64\r\n"
  
  "$GPVTG,,T,,M,3.731,N,6.911,K,A*2A\r\n"
  "$GPGGA,013444.00,3309.05382,N,11708.73596,W,1,03,9.02,216.9,M,-33.2,M,,*67\r\n"
  "$GPGSA,A,2,26,16,07,,,,,,,,,,9.08,9.02,1.00*0C\r\n"
  "$GPGSV,3,1,10,03,27,179,08,06,01,271,,07,46,285,10,08,16,140,08*71\r\n"
  "$GPGSV,3,2,10,09,60,331,08,16,38,046,16,23,69,084,09,26,11,040,14*78\r\n"
  "$GPGSV,3,3,10,48,48,207,,51,50,162,*79\r\n"
  "$GPGLL,3309.05382,N,11708.73596,W,013444.00,A,A*79\r\n"
  "$GPRMC,013445.00,V,,,,,,,241216,,,N*78\r\n"
;
*/

#include <fstream>;
ifstream infile("GPSdata.txt");
string gpsStream
// The TinyGPS++ object
TinyGPSPlus gps;

void setup()
{
  Serial.begin(115200);

  Serial.println(F("BasicExample.ino"));
  Serial.println(F("Basic demonstration of TinyGPS++ (no device needed)"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();

  while (infile >> gpsStream)
    if (gps.encode(gpsStream))
      displayInfo();

  Serial.println();
  Serial.println(F("Done."));
}

void loop()
{
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
