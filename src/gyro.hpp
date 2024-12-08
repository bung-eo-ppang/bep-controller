#ifndef GYRO_HPP
#define GYRO_HPP

#include <MPU6050_6Axis_MotionApps20.h>

struct YawPitchRoll {
  float yaw;
  float pitch;
  float roll;
};

struct Accel {
  float x;
  float y;
  float z;
};


class Gyro {
private:
  MPU6050 mpu;
  int interruptPin;
  bool dmpReady = false;
  uint16_t packetSize;
  static void dmpDataReady();

  uint8_t fifoBuffer[64];
  Quaternion q;
  VectorFloat gravity;
public:
  Gyro() : Gyro(0) {};
  Gyro(int interruptPin);
  void init();
  YawPitchRoll getYawPitchRoll();
  Accel getAccel();
};

#endif