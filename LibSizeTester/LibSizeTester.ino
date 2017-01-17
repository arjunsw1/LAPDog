#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include <EEPROM.h>
#include <Adafruit_10DOF.h> //contains other adafruit libs
#include "memorysaver.h"

//#include <SD.h>
//#include "Fat16.h"
#include "SdFat.h"

void setup(void)
{
//no SD lib is 4%, 1,458 bytes
//including SD.h is 17%, 5,548 bytes
//including Fat16.h is 6%, 2,130 bytes
//including SdFat.h is 4%, 1,464 bytes

}

void loop(void)
{
  
}

