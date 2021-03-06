#include "MakeCoffe.h"
#include "Arduino.h"
#include "config.h"



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
  /*if maintenance is active system don't do nothing*/
  if (maintenanceActive) {
    return;
  }
  /*start the presentatio*/
  if (state == -1 && newCoffe) {
    state = 0;
    newCoffe = false;
#ifdef __DEBUG__
    Serial.println("LED 1");
#endif

    led[state]->switchOn();
    return;
  }
  
  /*select the next led to turn on*/
  if (state >= 0) {
    int prec = state;
    state = count * myPeriod >= TIME_LED_ON ? state + 1 : state;
    count = state != prec ? 0 : count + 1;

    state = state == 3 ? -1 : state;

    /*stop the presentation*/
    if (state == -1) {
      coffeReady = true;
      led[2]->switchOff();
      return;
    }

    if (state != 0) led[state - 1]->switchOff();
#ifdef __DEBUG__
    Serial.println("LED " + String(state));
#endif
    led[state]->switchOn();
  }
}
