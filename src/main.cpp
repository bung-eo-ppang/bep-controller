#include <Arduino.h>
#include "transmitter.hpp"
#include "gyro.hpp"
#include "constants.hpp"

const byte address[6] = "00001";
Transmitter transmitter(address);
Gyro gyro(7);

void setup() {
  Serial.begin(115200);

  gyro.init();
  transmitter.init();

  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}

void loop() {
  auto ypr = gyro.getYawPitchRoll();
  auto accel = gyro.getAccel();
  uint32_t time = millis();
  short count = 1;

  byte buffer[16 * 5];

  int bufferIndex = 0;

  memcpy(buffer + bufferIndex, SIGNATURE, sizeof(SIGNATURE));
  bufferIndex += sizeof(SIGNATURE);

  memcpy(buffer + bufferIndex, &time, sizeof(time));
  bufferIndex += sizeof(time);

  memcpy(buffer + bufferIndex, &count, sizeof(count));
  bufferIndex += sizeof(count);

  memcpy(buffer + bufferIndex, &VERSION, sizeof(VERSION));
  bufferIndex += sizeof(VERSION);

  for (short i = 0; i < count; i++) {
    memcpy(buffer + bufferIndex, &i, sizeof(i));
    bufferIndex += sizeof(i);

    memcpy(buffer + bufferIndex, &VERSION, sizeof(VERSION));
    bufferIndex += sizeof(VERSION);

    memcpy(buffer + bufferIndex, &time, sizeof(time));
    bufferIndex += sizeof(time);

    memcpy(buffer + bufferIndex, &ypr.yaw, sizeof(ypr.yaw));
    bufferIndex += sizeof(ypr.yaw);

    memcpy(buffer + bufferIndex, &ypr.pitch, sizeof(ypr.pitch));
    bufferIndex += sizeof(ypr.pitch);

    memcpy(buffer + bufferIndex, &ypr.roll, sizeof(ypr.roll));
    bufferIndex += sizeof(ypr.roll);

    memcpy(buffer + bufferIndex, &accel.x, sizeof(accel.x));
    bufferIndex += sizeof(accel.x);

    memcpy(buffer + bufferIndex, &accel.y, sizeof(accel.y));
    bufferIndex += sizeof(accel.y);

    memcpy(buffer + bufferIndex, &accel.z, sizeof(accel.z));
    bufferIndex += sizeof(accel.z);

    bufferIndex += 6; // unused padding

    float joyX = 0;
    float joyY = 0;
    uint32_t buttons = 0b0000000000000000;
    buttons |= !digitalRead(4) << 0;
    buttons |= !digitalRead(5) << 1;

    memcpy(buffer + bufferIndex, &joyX, sizeof(joyX));
    bufferIndex += sizeof(joyX);

    memcpy(buffer + bufferIndex, &joyY, sizeof(joyY));
    bufferIndex += sizeof(joyY);

    memcpy(buffer + bufferIndex, &buttons, sizeof(buttons));
    bufferIndex += sizeof(buttons);

    bufferIndex += 4; // unused padding
  }

  uint8_t checksum[8];
  for (unsigned int i = 0; i < sizeof(checksum); i++) {
    checksum[i] = 0;
    for (int j = 0; j < bufferIndex; j++) {
      checksum[i] ^= buffer[j];
    }
  }
  memcpy(buffer + bufferIndex, checksum, sizeof(checksum));
  bufferIndex += sizeof(checksum);

  memcpy(buffer + bufferIndex, END_SIGNATURE, sizeof(END_SIGNATURE));
  bufferIndex += sizeof(END_SIGNATURE);

  transmitter.write(buffer, bufferIndex);
}
