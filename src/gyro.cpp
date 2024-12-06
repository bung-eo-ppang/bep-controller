#include <Arduino.h>
#include <I2Cdev.h>
#include "gyro.hpp"

void Gyro::dmpDataReady() {
}

Gyro::Gyro(int interruptPin) {
  this->interruptPin = interruptPin;
}

void Gyro::init() {
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

#ifdef SERIAL_DEBUG
  Serial.println(F("Initializing I2C devices..."));
#endif
  mpu.initialize();

  if (interruptPin) {
    pinMode(interruptPin, INPUT);
  }

#ifdef SERIAL_DEBUG 
  Serial.println(F("Testing device connections..."));
#endif

  if (mpu.testConnection()) {
#ifdef SERIAL_DEBUG
    Serial.println(F("MPU6050 connection successful"));
#endif
  } else {
    Serial.println(F("MPU6050 connection failed"));
    while (true);
  }


#ifdef SERIAL_DEBUG
  Serial.println(F("Initializing DMP..."));
#endif

  uint8_t devStatus = mpu.dmpInitialize();
  mpu.setXGyroOffset(0);
  mpu.setYGyroOffset(0);
  mpu.setZGyroOffset(0);
  mpu.setXAccelOffset(0);
  mpu.setYAccelOffset(0);
  mpu.setZAccelOffset(0);

  if (devStatus == 0) {
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    mpu.setDMPEnabled(true);

    if (interruptPin) {
      attachInterrupt(digitalPinToInterrupt(interruptPin), dmpDataReady, RISING);
      mpu.getIntStatus();
    }

    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
}



YawPitchRoll Gyro::getYawPitchRoll() {
  float ypr[3];
  mpu.dmpGetCurrentFIFOPacket(fifoBuffer);
  mpu.dmpGetQuaternion(&q, fifoBuffer);
  mpu.dmpGetGravity(&gravity, &q);
  mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

  return {
      ypr[0] * 180 / M_PI,
      ypr[1] * 180 / M_PI,
      ypr[2] * 180 / M_PI
  };
}
