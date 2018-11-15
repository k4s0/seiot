#include "Pir.h"
#include "Arduino.h"

Pir :: Pir(int pin) {
  this->pin = pin;
  pinMode(pin, INPUT);
  delay(10000);
}

bool Pir:: isPresent() {
  return digitalRead(pin);
}
