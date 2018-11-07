#include "DetectMotion.h"
#include "Arduino.h"

#define DIST1 0.3
#define DIST2 0.1
#define DT1 500
#define DT2A 500
#define DT2B 500

DetectMotion :: DetectMotion(int pinPir, int pinTrig, int pinEcho) {
  this->pinPir = pinPir;
  this->pinTrig = pinTrig;
  this->pinEcho = pinEcho;
  count = 0;

}

void DetectMotion :: init(int period) {
  Task::init(period);
  state = 0;
  distance = 0;
  pir = new Pir(pinPir);
  sonar = new Sonar(pinTrig, pinEcho);
}
void DetectMotion :: dmIsNear() {
  Serial.println("Stato 1");
  distance = sonar->getDistance();
  if (distance <= DIST1) {
    Serial.println("Distanza di Engage ! dist-> " + String(distance));
    count++;
  } else {
    count = 0;
    Serial.println("Distanza di Engage Fallita");
    isPresent = pir->isPresent();
    if (!isPresent) {
      Serial.println("Nessuna presenza sul PIR");
      state = 0;
      return;
    }
  }
  if (count * myPeriod >= DT1) {
    count = 0;
    correctDistance = true;
    state = 2;
  }
  return;
}
void DetectMotion :: dmIsFar() {
  Serial.println("Stato 2");
  distance = sonar->getDistance();
  count = distance < DIST1 ? count + 1 : 0;
  if (count * myPeriod > DT1) {
    count = 0;
    correctDistance = false;
    state = 3;
  }
  return;
}
void DetectMotion :: dmIsGone() {
  Serial.println("Stato 3");
  isPresent = pir->isPresent();
  if (count * myPeriod > DT2B) {
    isPresent = false;
    count = 0;
    state = 0;
  }
  if (!isPresent) {
    count++;
  } else {
    count = 0;
    state = 1;
  }
  return;
}

void DetectMotion :: tick() {
  isPresent = pir->isPresent();
  if (isPresent) {
    Serial.println("Presenza Rilevata dal PIR");
    state = 1;
    count = 0;
    return;
  }
  switch (state) {
    case 1:
      dmIsNear();
      break;
    case 2:
      dmIsFar();
      break;
    case 3:
      dmIsGone();
      break;
  }
}
