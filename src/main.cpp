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

  byte buffer[16 * 5];

  int bufferIndex = 0;

  memcpy(buffer + bufferIndex, SIGNATURE, sizeof(SIGNATURE));
  bufferIndex += sizeof(SIGNATURE);

  memcpy(buffer + bufferIndex, &time, sizeof(time));
  bufferIndex += sizeof(time);

  memcpy(buffer + bufferIndex, &count, sizeof(count));
  bufferIndex += sizeof(count);

  memcpy(buffer + bufferIndex, VERSION, sizeof(VERSION));
  bufferIndex += sizeof(VERSION);

  for (short i = 0; i < count; i++) {
    memcpy(buffer + bufferIndex, &i, sizeof(i));
    bufferIndex += sizeof(i);

    memcpy(buffer + bufferIndex, VERSION, sizeof(VERSION));
    bufferIndex += sizeof(VERSION);

    memcpy(buffer + bufferIndex, &time, sizeof(time));
    bufferIndex += sizeof(time);

    memcpy(buffer + bufferIndex, &ypr.yaw, sizeof(ypr.yaw));
    bufferIndex += sizeof(ypr.yaw);

    memcpy(buffer + bufferIndex, &ypr.pitch, sizeof(ypr.pitch));
    bufferIndex += sizeof(ypr.pitch);

    memcpy(buffer + bufferIndex, &ypr.roll, sizeof(ypr.roll));
    bufferIndex += sizeof(ypr.roll);

    float joyX = 0;
    float joyY = 0;
    byte buttons[] = { 0b1111, 0b1111, 0b1111, 0b1111 };

    memcpy(buffer + bufferIndex, &joyX, sizeof(joyX));
    bufferIndex += sizeof(joyX);

    memcpy(buffer + bufferIndex, &joyY, sizeof(joyY));
    bufferIndex += sizeof(joyY);

    memcpy(buffer + bufferIndex, &buttons, sizeof(buttons));
    bufferIndex += sizeof(buttons);
  }

  uint8_t checksum[8];
  for (int i = 0; i < sizeof(checksum) / sizeof(uint8_t); i++) {
    checksum[i] = 0;
    for (int j = 0; j < bufferIndex; j++) {
      checksum[i] ^= buffer[j];
    }
  }
  memcpy(buffer + bufferIndex, checksum, sizeof(checksum));
  bufferIndex += sizeof(checksum);

  memcpy(buffer + bufferIndex, END_SIGNATURE, sizeof(END_SIGNATURE));
  bufferIndex += sizeof(END_SIGNATURE);

  Serial.write(buffer, bufferIndex);
}
