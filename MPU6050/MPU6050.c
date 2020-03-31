#include "Arduino.h"
#include "MPU6050.h"

#include <Wire.h>


MPU6050::MPU6050()
{

   Wire.begin();
   Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
   Wire.write(0x6B); //Accessing the register 6B - Power Management (Sec. 4.28)
   Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
   Wire.endTransmission();  
   
   Wire.beginTransmission(0b1101000); //I2C address of the MPU
   Wire.write(0x1B); //Accessing the register 1B - Gyroscope Configuration (Sec. 4.4) 
   Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
   Wire.endTransmission(); 
   
   Wire.beginTransmission(0b1101000); //I2C address of the MPU
   Wire.write(0x1C); //Accessing the register 1C - Acccelerometer Configuration (Sec. 4.5) 
   Wire.write(0b00000000); //Setting the accel to +/- 2g
   Wire.endTransmission(); 

}


MPU6050::MPU6050(int force_range, int rot_range)
{ 
   Wire.begin();
   Wire.beginTransmission(0b1101000); //This is the I2C address of the MPU (b1101000/b1101001 for AC0 low/high datasheet sec. 9.2)
   Wire.write(0x6B); 
   Wire.write(0b00000000); //Setting SLEEP register to 0. (Required; see Note on p. 9)
   Wire.endTransmission();  
   
   if (force_range == 2){

    _force_range = force_range;
    force_factor = 16384.0;

    Wire.beginTransmission(0b1101000); //I2C address of the MPU
    Wire.write(0x1C);
    Wire.write(0b00000000); //Setting the accel to +/- 2g
    Wire.endTransmission(); 
    
   }
   
   else if (force_range == 4) {

    _force_range = force_range;
    force_factor = 8192.0;

    Wire.beginTransmission(0b1101000); //I2C address of the MPU
    Wire.write(0x1C); 
    Wire.write(0b00001000); //Setting the accel to +/- 4g
    Wire.endTransmission(); 
    
   }
   
   else if (force_range == 8) {

    _force_range = force_range;
    force_factor = 4096.0;

    Wire.beginTransmission(0b1101000); //I2C address of the MPU
    Wire.write(0x1C); 
    Wire.write(0b00010000); //Setting the accel to +/- 8g
    Wire.endTransmission(); 
    
   }
   
   else if (force_range == 16) {
    
    _force_range = force_range;
    force_factor = 2048.0;

    Wire.beginTransmission(0b1101000); //I2C address of the MPU
    Wire.write(0x1C); 
    Wire.write(0b00011000); //Setting the accel to +/- 16g
    Wire.endTransmission(); 
    
   }
   
   else{
    printf("Wrong Input. Force Range must be either 2, 4, 8 or 16.");
    return;
   }

   
   if (acc_range == 250){ 
    
    _rot_range = rot_range;
    rot_factor = 131.0;
    
    Wire.beginTransmission(0b1101000); //I2C address of the MPU
    Wire.write(0x1B); 
    Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg./s 
    Wire.endTransmission(); 
   
   }
   
   else if (acc_range == 500){ 
    _rot_range = rot_range;
    rot_factor = 65.5;
    Wire.beginTransmission(0b1101000); //I2C address of the MPU
    Wire.write(0x1B); 
    Wire.write(0x00001000); //Setting the gyro to full scale +/- 500deg./s 
    Wire.endTransmission(); 
   }
   
   else if (acc_range == 500){ 
    _rot_range = rot_range;
    rot_factor = 32.8;
    Wire.beginTransmission(0b1101000); //I2C address of the MPU
    Wire.write(0x1B); 
    Wire.write(0x00010000); //Setting the gyro to full scale +/- 1000deg./s 
    Wire.endTransmission(); 
    }
   
   else if (acc_range == 500){ 
    _rot_range = rot_range;
    rot_factor = 16.4;
    Wire.beginTransmission(0b1101000); //I2C address of the MPU
    Wire.write(0x1B); 
    Wire.write(0x00011000); //Setting the gyro to full scale +/- 2000deg./s 
    Wire.endTransmission(); 
   }

   else{
    printf("Wrong Input. Acceleration Range must be either 250, 500, 1000, 20000.");
    return;
   }

}


void MPU6050::recordAccelRegisters() {
  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Accel Registers (3B - 40)

  long accelX, accelY, accelZ;
  
  while(Wire.available() < 6);
  
  accelX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  accelY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  accelZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  
  gForceX = accelX / force_factor;
  gForceY = accelY / force_factor; 
  gForceZ = accelZ / force_factor;
  
}


void MPU6050::recordGyroRegisters() {
  
  Wire.beginTransmission(0b1101000); //I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); //Request Gyro Registers (43 - 48)
  
  long gyroX, gyroY, gyroZ;
  
  while(Wire.available() < 6);
  
  gyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into accelX
  gyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into accelY
  gyroZ = Wire.read()<<8|Wire.read(); //Store last two bytes into accelZ
  
  rotX = gyroX / rot_factor;
  rotY = gyroY / rot_factor; 
  rotZ = gyroZ / rot_factor;
  
}


void MPU6050::printData() {
  
  Serial.println("Gyro (deg)");
  Serial.print(" X=");
  Serial.println(rotX);
  Serial.print(" Y=");
  Serial.println(rotY);
  Serial.print(" Z=");
  Serial.println(rotZ);
  
  Serial.println(" Accel (g)");
  Serial.print(" X=");
  Serial.println(gForceX);
  Serial.print(" Y=");
  Serial.println(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
  
}
