/*
  MPU6050.h - Library for using.
  Created by Piyush Goyal, November 29, 2019.
  Released into the public domain.
*/

#ifndef MPU6050_h
#define MPU6050_h

#include "Arduino.h"

class MPU6050
{
  
  public:
    MPU6050(int force_range, int rot_range);
    MPU6050();
    void recordAccelRegisters();
    void recordGyroRegisters();
    void printData();
    
  private:
    int _force_range;
    int _rot_range;
    float gForceX, gForceY, gForceZ;
    float rotX, rotY, rotZ;
    float rot_factor, force_factor;

};

#endif