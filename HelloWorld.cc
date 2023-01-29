#include <Arduino.h>
#include <serial-logger.hpp>

const auto kBaud {115200};
auto serial = &Serial;

using logger = logging::Logger;
using loglevel = logging::Level;

void setup() {
  serial->begin(kBaud);
  while (!(*serial) );

  logger::setSerial(serial);
  logger::setLevel(loglevel::kInfo);

  logger::info("Example to show the serial-logger usage.");
  logger::logLevel(loglevel::kInfo);

  logger::info("Hello world.");
  logger::warning("Hello World.");
  logger::error("Hello world.");
  logger::critical("Hello world.");

  logger::info("Using '%s' syntax is also allowed.", "printf");
}

void loop() {
  // put your main code here, to run repeatedly:
}