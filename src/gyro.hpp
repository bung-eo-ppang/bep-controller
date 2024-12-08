#ifndef GYRO_HPP
#define GYRO_HPP

#include <MPU6050_6Axis_MotionApps20.h>

struct YawPitchRoll {
  float yaw;
  float pitch;
  float roll;
};

struct Accel {
  int16_t x;
  int16_t y;
  int16_t z;
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