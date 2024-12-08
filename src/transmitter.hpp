#ifndef TRANSMITTER_HPP
#define TRANSMITTER_HPP

#include <RF24.h>

class Transmitter {
private:
  RF24 radio;
  const uint8_t* address;
public:
  Transmitter(const uint8_t* address);

  void init();
  void write(const void* buffer, size_t size);

  ~Transmitter();
};

#endif