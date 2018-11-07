#include "MakeCoffe.h"
#include "Arduino.h"

#define TIME_LED_ON 1000

MakeCoffe :: MakeCoffe(int pin0, int pin1, int pin2) {
  this->pin[0] = pin0;
  this->pin[1] = pin1;
  this->pin[2] = pin2;
  count = 0;
}

void MakeCoffe :: init(int period) {
  Task::init(period);
  for (int i = 0; i < 3; i++) {
    led[i] = new Led(pin[i]);
  }
  state = -1;
}

void MakeCoffe :: tick() {
  if (state == -1 && newCoffe) {
    state = 0;
    newCoffe = false;
    led[state]->switchOn();
    return;
  }

  if (state >= 0) {
    int prec = state;
    state = count * myPeriod >= TIME_LED_ON ? state + 1 : state;
    count = state != prec ? 0 : count + 1;

    state = state == 3 ? -1 : state;

    if (state == -1) {
      coffeReady = true;
      led[2]->switchOff();
      return;
    }

    if (state != 0) led[state - 1]->switchOff();
    led[state]->switchOn();
  }
}
