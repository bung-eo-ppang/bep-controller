#include <Arduino.h>
#include "../lib/transmitter.hpp"

const byte address[6] = "00001";
Transmitter transmitter(address);

void setup() {
  Serial.begin(9600);
  transmitter.init();
}

void loop() {
  transmitter.write();
}
