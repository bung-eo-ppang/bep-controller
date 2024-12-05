#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

RF24 radio(7, 8);
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
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

void loop() {
  const char* text = "Hello, World!";
  Serial.println(radio.write(text, strlen(text)));
  delay(1000);
}
