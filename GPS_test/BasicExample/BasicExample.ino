#include <TinyGPS++.h>

/* 
   This sample sketch should be the first you try out when you are testing a TinyGPS++
   (TinyGPSPlus) installation.  In normal use, you feed TinyGPS++ objects characters from
   a serial NMEA GPS device, but this example uses static strings for simplicity.
*/
// A sample NMEA stream.

char *gpsStream =
/*
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

  $GPRMC,012558.00,A,3309.05716,N,11708.73068,W,0.391,,030117,,,A*6F
  $GPVTG,,T,,M,0.391,N,0.724,K,A*29
  $GPGGA,012558.00,3309.05716,N,11708.73068,W,1,07,1.23,213.8,M,-33.2,M,,*6C
  $GPGSA,A,3,09,23,07,08,27,16,30,,,,,,2.15,1.23,1.77*03
  $GPGSV,4,1,14,03,13,176,21,05,00,327,14,07,53,305,37,08,27,128,26*7C
  $GPGSV,4,2,14,09,74,343,37,16,25,041,21,22,01,167,21,23,58,114,33*7C
  $GPGSV,4,3,14,27,27,084,29,28,10,213,15,30,25,275,23,46,48,147,*7A
  $GPGSV,4,4,14,48,48,207,31,51,50,162,30*7C
  $GPGLL,3309.05716,N,11708.73068,W,012558.00,A,A*79
  
  $GPRMC,012559.00,A,3309.05717,N,11708.73073,W,0.517,,030117,,,A*6D
  $GPVTG,,T,,M,0.517,N,0.957,K,A*2B
  $GPGGA,012559.00,3309.05717,N,11708.73073,W,1,07,1.23,213.6,M,-33.2,M,,*68
  $GPGSA,A,3,09,23,07,08,27,16,30,,,,,,2.15,1.23,1.77*03
  $GPGSV,4,1,14,03,13,176,21,05,00,327,,07,53,305,36,08,27,128,25*7B
  $GPGSV,4,2,14,09,74,343,36,16,25,041,19,22,01,167,22,23,58,114,33*75
  $GPGSV,4,3,14,27,27,084,27,28,10,213,,30,25,275,27,46,48,147,*74
  $GPGSV,4,4,14,48,48,207,31,51,50,162,28*75
  $GPGLL,3309.05717,N,11708.73073,W,012559.00,A,A*73
  */
  "$GPRMC,012600.00,A,3309.05710,N,11708.73072,W,0.118,,030117,,,A*6F\n$GPVTG,,T,,M,0.118,N,0.219,K,A*21\n$GPGGA,012600.00,3309.05710,N,11708.73072,W,1,08,1.23,213.7,M,-33.2,M,,*6F\n$GPGSA,A,3,09,23,07,08,27,16,30,03,,,,,2.15,1.23,1.77*00\n$GPGSV,4,1,14,03,13,176,21,05,00,327,20,07,53,305,37,08,27,128,25*78\n$GPGSV,4,2,14,09,74,343,37,16,25,041,17,22,01,167,21,23,58,114,35*7F\n$GPGSV,4,3,14,27,27,084,25,28,10,213,25,30,25,275,31,46,48,147,*76\n$GPGSV,4,4,14,48,48,207,32,51,50,162,30*7F\n$GPGLL,3309.05710,N,11708.73072,W,012600.00,A,A*7A"

;


// The TinyGPS++ object
TinyGPSPlus gps;
bool loc_found = 0;
bool loc_pinned = 0;
double initial_lat;
double initial_lng;
double course_back;

void setup()
{
  Serial.begin(115200);

  Serial.println(F("BasicExample.ino"));
  Serial.println(F("Basic demonstration of TinyGPS++ (no device needed)"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();

  while (*gpsStream)
    if (gps.encode(*gpsStream++))
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
