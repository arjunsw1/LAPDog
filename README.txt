This report presents the design and implementation specifications of our project. The goal of this project was to implement an autonomous drone capable of flying to a user-determined altitude, capture the desired image with an integrated camera, and return to the user where the image could be transferred to his/her device; all with minimal effort and skill required by the user.


Files used for LAPDog Flight and Camera:
Flight controller used for Arduino #1:
YMFC-3D_V2/YMFC-3D_V2_Flight_controller/YMFC-3D_V2_Flight_controller.ino

Camera Capture code used for Arduino #2
ArduCAM/examples/mini/ArduCAM_Mini_Capture2SDnew/ArduCAM_Mini_Capture2SDnew.ino



other libraries required for compilation during testing:

IMU Board - https://learn.adafruit.com/adafruit-10-dof-imu-breakout-lsm303-l3gd20-bmp180/software
Adafruit_10DOF-master
Adafruit-BMP085_Unified-master
Adafruit-L3GD20_U-master
Adafruit-LSM303DLHC-master
Adafruit_Sensor-master
GPSTiny++
ArduCAM Arduino files provided by manufacturer
Flight Controller - http://www.brokking.net/ymfc-3d_v2_main.html
