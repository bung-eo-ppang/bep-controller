#include <Arduino.h>
#include "transmitter.hpp"
#include "gyro.hpp"
#include "constants.hpp"

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
  // transmitter.write();
  auto ypr = gyro.getYawPitchRoll();
  uint32_t time = millis();
  int count = 1;

  Serial.write(SIGNATURE, sizeof(SIGNATURE));
  Serial.write((byte*)&time, sizeof(time));
  Serial.write((byte*)&count, sizeof(count));
  Serial.write(VERSION, sizeof(VERSION));

  for (short i = 0; i < count; i++) {
    Serial.write((byte*)&i, sizeof(i));
    Serial.write(VERSION, sizeof(VERSION));
    Serial.write((byte*)&time, sizeof(time));

    Serial.write((byte*)&ypr.yaw, sizeof(ypr.yaw));
    Serial.write((byte*)&ypr.pitch, sizeof(ypr.pitch));
    Serial.write((byte*)&ypr.roll, sizeof(ypr.roll));

    float joyX = 0;
    float joyY = 0;
    byte buttons[] = { 0b1111, 0b1111, 0b1111, 0b1111 };
    Serial.write((byte*)&joyX, sizeof(joyX));
    Serial.write((byte*)&joyY, sizeof(joyY));
    Serial.write((byte*)&buttons, sizeof(buttons));
  }
}
