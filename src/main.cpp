#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

#define TRANSMITTER

RF24 radio(7, 8);
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  Serial.println("testtest");
  if (!radio.begin()) {
    Serial.println("Radio hardware is not responding.");
    while (1);
  }

  radio.setPALevel(RF24_PA_MIN);
#ifdef TRANSMITTER
  radio.openWritingPipe(address);
  radio.stopListening();
#else
  radio.openReadingPipe(0, address);
  radio.startListening();
#endif
  printf_begin();
  radio.printDetails();
  radio.printPrettyDetails();
}

void loop() {
#ifdef TRANSMITTER
  const char* text = "Hello, World!";
  Serial.println(radio.write(text, strlen(text)));
  delay(1000);
#else
  if (radio.available()) {
    char text[32] = "";
    radio.read(text, sizeof(text));
    Serial.println(text);
  }
#endif
}
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

#define TRANSMITTER

RF24 radio(7, 8);
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  Serial.println("testtest");
  if (!radio.begin()) {
    Serial.println("Radio hardware is not responding.");
    while (1);
  }

  radio.setPALevel(RF24_PA_MIN);
#ifdef TRANSMITTER
  radio.openWritingPipe(address);
  radio.stopListening();
#else
  radio.openReadingPipe(0, address);
  radio.startListening();
#endif
  printf_begin();
  radio.printDetails();
  radio.printPrettyDetails();
}

void loop() {
#ifdef TRANSMITTER
  const char* text = "Hello, World!";
  Serial.println(radio.write(text, strlen(text)));
  delay(1000);
#else
  if (radio.available()) {
    char text[32] = "";
    radio.read(text, sizeof(text));
    Serial.println(text);
  }
#endif
}
