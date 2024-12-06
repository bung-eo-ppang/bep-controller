#include <Arduino.h>
#include "transmitter.hpp"
#include "gyro.hpp"

const byte address[6] = "00001";
Transmitter transmitter(address);
Gyro gyro(7);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  gyro.init();
  // transmitter.init();
}

void loop() {
  // Serial.println("Hello, World!");

  // transmitter.write();
  auto ypr = gyro.getYawPitchRoll();
  Serial.println("yaw: " + String(ypr.yaw) + " pitch: " + String(ypr.pitch) + " roll: " + String(ypr.roll));
}
