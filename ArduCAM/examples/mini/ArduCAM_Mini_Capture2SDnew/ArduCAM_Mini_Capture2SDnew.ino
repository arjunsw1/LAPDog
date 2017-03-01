// ArduCAM demo (C)2016 Lee
// web: http://www.ArduCAM.com
// This program is a demo of how to use most of the functions
// of the library with a supported camera modules, and can run on any Arduino platform.
//
// This demo was made for Omnivision 2MP/5MP sensor.
// It will run the ArduCAM ESP8266 5MP as a real 2MP digital camera, provide both JPEG capture.
// The demo sketch will do the following tasks:
// 1. Set the sensor to JPEG mode.
// 2. Capture and buffer the image to FIFO every 5 seconds 
// 3. Store the image to Micro SD/TF card with JPEG format in sequential.
// 4. Resolution can be changed by myCAM.set_JPEG_size() function.
// This program requires the ArduCAM V4.0.0 (or later) library and ArduCAM 2MP/5MP shield
// and use Arduino IDE 1.5.2 compiler or above

#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include <SD.h>
//#include "SdFat.h"
#include <EEPROM.h>
#include "memorysaver.h"
//This demo can only work on OV2640_MINI_2MP or OV5642_MINI_5MP or OV5642_MINI_5MP_BIT_ROTATION_FIXED platform.
#if !(defined OV5642_MINI_5MP || defined OV5642_MINI_5MP_BIT_ROTATION_FIXED || defined OV2640_MINI_2MP)
  #error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif
const int SD_CS = 14; //9 on Uno
const int SPI_CS = 10;
const int address = 0;
ArduCAM myCAM( OV2640, SPI_CS );
bool pictures_taken = 0;

//SdFat SD;

//5 half turns from all the way screwed in puts my cieling in focus ~13ft

void myCAMSaveToSDFile(){
  char str[8];
  byte buf[256];
  static int i = 0;
  static int file_num = EEPROM.read(address);
  uint8_t temp = 0,temp_last=0;
  uint32_t length = 0;
  bool is_header = false;
  File outFile;
  
  //Flush the FIFO
  myCAM.flush_fifo();
  //Clear the capture done flag
  myCAM.clear_fifo_flag();
  //Start capture
  myCAM.start_capture();
  Serial.println("start Capture");

  
 while(!myCAM.get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK)){
 }
  
 Serial.println("Capture Done!");  
 length = myCAM.read_fifo_length();
 
 Serial.print("The fifo length is :");
 Serial.println(length, DEC);
  if (length >= MAX_FIFO_SIZE) //384K
  {
    Serial.println("Over size.");
    //return 0;
  }
    if (length == 0 ) //0 kb
  {
    Serial.println("Size is 0.");
    //return 0;
  }
  
 //Construct a file name
 file_num++;
 itoa(file_num, str, 10);
 strcat(str, ".jpg");
 //Open the new file
 outFile = SD.open(str, FILE_WRITE);
 Serial.println(str);
 if(!outFile){
  Serial.println("open file failed");
  return;
 }
 
 if (file_num < 200) //added by me for file numbering
    EEPROM.write(address, file_num);
 else{
    file_num = 1;
    EEPROM.write(address, file_num);
 }
 
 myCAM.CS_LOW();
 myCAM.set_fifo_burst();

while ( length-- )
  {
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    //Read JPEG data from FIFO
    if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
    {   Serial.print(temp_last, HEX); //prints out end of JPEG header
        Serial.println(temp,HEX);     //which is FF D9
        buf[i++] = temp;  //save the last  0XD9     
       //Write the remain bytes in the buffer
        myCAM.CS_HIGH();
        outFile.write(buf, i);    
      //Close the file
        outFile.close();
        Serial.println("CAM Save OK!");
        is_header = false;
        i = 0;
    }  
    if (is_header == true)
    { 
       //Write image data to buffer if not full
        if (i < 256)
        buf[i++] = temp;
        else
        {
          //Write 256 bytes image data to file
          myCAM.CS_HIGH();
          outFile.write(buf, 256);
          i = 0;
          buf[i++] = temp;
          myCAM.CS_LOW();
          myCAM.set_fifo_burst();
        }        
    }
    else if ((temp == 0xD8) & (temp_last == 0xFF))
    {
      is_header = true;
      buf[i++] = temp_last;
      buf[i++] = temp;
      Serial.print(temp_last, HEX); //prints out beginning of JPEG header
      Serial.println(temp,HEX);     //which is FF D8 FF   
    } 
  } 
  //Flush the FIFO
  //myCAM.flush_fifo();
  //Clear the capture done flag
  //myCAM.clear_fifo_flag();
}

void setup(){
  uint8_t vid, pid;
  uint8_t temp;

  Wire.begin();
  digitalWrite(A4, LOW); //removes internal pullup resistors
  digitalWrite(A5, LOW); // IMU has built in pull ups
  Serial.begin(115200);
  Serial.println("ArduCAM Start!");

  //set the CS as an output:
  pinMode(SPI_CS,OUTPUT);

  // initialize SPI:
  SPI.begin();
  //Check if the ArduCAM SPI bus is OK
  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);
   
  if (temp != 0x55){
    Serial.println("SPI1 interface Error!");
    while(1);
  }
    //Initialize SD Card
  if(!SD.begin(SD_CS)){
    Serial.println("SD Card Error");
  }
  else
  Serial.println("SD Card detected!");
   
   //Check if the camera module type is OV2640
   myCAM.wrSensorReg8_8(0xff, 0x01);
   myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
   myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
   if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 )))
    Serial.println("Can't find OV2640 module!");
    else
    Serial.println("OV2640 detected.");

   myCAM.set_format(JPEG);
   myCAM.InitCAM();
   myCAM.OV2640_set_JPEG_size(OV2640_1600x1200);
}

void loop(){
  //myCAMSaveToSDFile();
  //delay(5000);

  if((digitalRead(15) == HIGH) )//&& (picturestaken == 0)){
     myCAMSaveToSDFile();
     delay(5000);
     //pictures_taken = 1;
   }
   

   if((digitalRead(15) == LOW) )//&& (picturestaken == 1))
    //pictures_taken = 0;
  
  
}


