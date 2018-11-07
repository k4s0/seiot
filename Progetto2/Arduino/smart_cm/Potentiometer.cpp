#include "Potentiometer.h"
#include "Arduino.h"

Potentiometer :: Potentiometer(int pin, int bot, int top) {
  this->pin = pin;
  this->bot = bot;
  this->top = top;
  pinMode(pin, INPUT);
}

int Potentiometer :: getValue() {
  return map(digitalRead(pin), 0, 1023, bot, top);
}
