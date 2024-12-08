#include "transmitter.hpp"
#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>

const byte address[6] = "00001";

Transmitter::Transmitter(const uint8_t* address) : address(address) {
  radio = RF24(8, 9);
}

Transmitter::~Transmitter() {

}

void Transmitter::init() {
  if (!radio.begin()) {
    Serial.println("Radio hardware is not responding.");
    while (1);
  }

  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(address);
  radio.stopListening();

  printf_begin();
  radio.printDetails();
  radio.printPrettyDetails();
}

void Transmitter::write(const void* buffer, size_t size) {
  Serial.println(radio.write(buffer, size));
}
