//Combining ArduCAM pictures and IMU compass for 360 footage at top of flight
#include <Wire.h>
#include <ArduCAM.h>
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include <Adafruit_10DOF.h> //contains other adafruit libs
#include "memorysaver.h"

//Mag I2C address is 0x1E

const int SD_CS = 9; //SD Card slave select
const int SPI_CS = 10; //camera slave select
const int address = 0; //EEPROM address for file labeling
ArduCAM myCAM( OV2640, SPI_CS );

//labels sensors, lets you use the functions and data associated with them
Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA; //real sea level for accurate altitude

sensors_event_t event;
sensors_vec_t   orientation;

void initSensors(){
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    //Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    //while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    //Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    //while(1);
  }
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    //Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    //while(1);
  }
}

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
  //Serial.println("start Capture");

  
 while(!myCAM.get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK)){
 }
  
 //Serial.println("Capture Done!");  
 length = myCAM.read_fifo_length();
 
 //Serial.print("The fifo length is :");
 //Serial.println(length, DEC);
  if (length >= MAX_FIFO_SIZE) //384K
  {
    //Serial.println("Over size.");
    return 0;
  }
    if (length == 0 ) //0 kb
  {
    //Serial.println("Size is 0.");
    return 0;
  }
  
 //Construct a file name
 file_num++;
 itoa(file_num, str, 10);
 strcat(str, ".jpg");
 //Open the new file
 outFile = SD.open(str, FILE_WRITE); //, FILE_WRITE
 Serial.println(str);
 //Serial.println(SD.exists(str));

 if(!outFile){
  //Serial.println("open file failed");
  return;
 }
 
 if (file_num < 101) //added by me for file numbering
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
    {   //Serial.print(temp_last, HEX); //prints out end of JPEG header
        //Serial.println(temp,HEX);     //which is FF D9
        buf[i++] = temp;  //save the last  0XD9     
       //Write the remain bytes in the buffer
        myCAM.CS_HIGH();
        outFile.write(buf, i);    
      //Close the file
        outFile.close();
        //Serial.println("CAM Save OK!");
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
      //Serial.print(temp_last, HEX); //prints out beginning of JPEG header
      //Serial.println(temp,HEX);     //which is FF D8 FF   
    } 
  } 
  //Flush the FIFO
  myCAM.flush_fifo();
  //Clear the capture done flag
  myCAM.clear_fifo_flag();
}

void pictureLoop(){
  bool pictures_taken = 0;
  int pic_count = 0;
  myCAMSaveToSDFile();
  
  while(pictures_taken == 0)
  {
      //rotates camera 30 deg to the right
      rotate_right();

      //takes pictures, will take 12 in total, 30 deg between snap
      myCAMSaveToSDFile();
      pic_count++;
      
      //diagnostic text
      //Serial.print(F("Picture Count: "));
      //Serial.println(pic_count);
      //Serial.print(F("Heading: "));
      //Serial.print(orientation.heading);
      //Serial.println('\n');
      
    //takes us out of turning for pictures
    if(pic_count==12)
      pictures_taken = 1;
  }
}

void rotate_right() {
  //pulls original heading and starting point
  mag.getEvent(&event);
  dof.magGetOrientation(SENSOR_AXIS_Z, &event, &orientation);
  int orig_heading = orientation.heading;
  int curr_heading = orig_heading;    

  //rotates right until current heading is 30 deg forward of starting point
  while( !((curr_heading < (orig_heading + 32)) && (curr_heading > orig_heading + 28)) ){
    
    //accounts if starting point is >30 deg so you dont miss a picture
    if( (orig_heading+30) > 360)
      orig_heading=orig_heading - 360;
      
    //motor rotate right some amount

    //repolls location to see if need to go more
    mag.getEvent(&event);
    dof.magGetOrientation(SENSOR_AXIS_Z, &event, &orientation);
    curr_heading = orientation.heading;
    
  }
  return;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  initSensors();

  //camera init
  uint8_t vid, pid;
  uint8_t temp;
  Wire.begin();
  //digitalWrite(A4, LOW); //removes internal pullup resistors
  //digitalWrite(A5, LOW); // IMU has built in pull ups
  //Serial.println("ArduCAM Start!");

  //set the CS as an output:
  pinMode(SPI_CS,OUTPUT);

  // initialize SPI:
  SPI.begin();
  //Check if the ArduCAM SPI bus is OK
  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);
   
  if (temp != 0x55){
    //Serial.println("SPI1 interface Error!");
    while(1);
  }
    //Initialize SD Card
  if(!SD.begin(SD_CS)){
    //Serial.println("SD Card Error");
  }
  else
  //Serial.println("SD Card detected!");
   
   //Check if the camera module type is OV2640
   myCAM.wrSensorReg8_8(0xff, 0x01);
   myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
   myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
   if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
    //Serial.println("Can't find OV2640 module!");
   }
   else
    //Serial.println("OV2640 detected.");

   myCAM.set_format(JPEG);
   myCAM.InitCAM();
   myCAM.OV2640_set_JPEG_size(OV2640_1600x1200);

   //pictureLoop(); 

}

void loop() {
  
  // put your main code here, to run repeatedly:
   myCAMSaveToSDFile(); //added to test picture taking with IMU 
   
  //Wire.beginTransmission(0x1E);
   mag.getEvent(&event);
   dof.magGetOrientation(SENSOR_AXIS_Z, &event, &orientation);
   Serial.print(F("Heading: "));
   Serial.print(orientation.heading);
   //Serial.println('\n');
  // Wire.endTransmission();
   //fifo is constantly 524287, possible I2C error even when IMU unplugged
   
   delay(5000);

}
