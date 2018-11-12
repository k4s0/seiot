#include "DetectMotion.h"
#include "Arduino.h"
#include "config.h"


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
#ifdef __DEBUG__
  Serial.println("DM1");
#endif
  distance = sonar->getDistance();
  if (distance <= DIST1) {
#ifdef __DEBUG__
    Serial.println("[DM1]Distanza giusta dist-> " + String(distance));
#endif
    count++;
  } else {
    count = 0;
#ifdef __DEBUG__
    Serial.println("[DM1]Distanza sbagliata");
#endif
    correctDistance = false;
    isPresent = pir->isPresent();
    if (!isPresent) {
#ifdef __DEBUG__
      Serial.println("[DM1]Nessuna presenza sul PIR");
#endif
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
#ifdef __DEBUG__
  Serial.println("[DM2] " + String(newCoffe));
#endif
  if (newCoffe) {
    count = 0;
    state = 4;
    return;
  }
  distance = sonar->getDistance();
  count = distance > DIST1 ? count + 1 : 0;
  if (count * myPeriod > DT1) {
    count = 0;
    correctDistance = false;
    state = 3;
  }
  return;
}

void DetectMotion :: dmIsGone() {
#ifdef __DEBUG__
  Serial.println("[DM3]");
#endif

  if (count * myPeriod > DT2B) {
    isPresent = false;
    count = 0;
    state = 0;
    return;
  }
  if (!pir->isPresent()) {
    count++;
  } else {
    count = 0;
    state = 1;
  }
  return;
}

void DetectMotion :: dmWait() {
#ifdef __DEBUG__

  Serial.println("[DM4]");
#endif

  if (coffeReady) {
    state = 5;
  }
  return;
}

void DetectMotion :: dmTakingCoffe() {
#ifdef __DEBUG__
  Serial.println("[DM5]");
#endif
  distance = sonar->getDistance();
  count = distance > DIST2 ? count + 1 : 0;
  if (distance < DIST2 || count * myPeriod >= DT4) {
    coffeTaked = true;
    coffeReady = false;
    state = 2;
    count = 0;
  }

}

void DetectMotion :: tick() {
  if (maintenanceActive) {
    return;
  }
  if (reStart) {
    state = 0;
    count = 0;
    isPresent = false;
    correctDistance = false;
    coffeTaked = false;
    coffeReady = false;
    reStart = false;
    return;
  }
  if (pir->isPresent() && state == 0) {
#ifdef __DEBUG__
    Serial.println("Presenza Rilevata dal PIR");
#endif
    isPresent = true;
    state = 1;
    count = 0;
    return;
  }
  switch (state) {
    case 0:
#ifdef __DEBUG__
      Serial.println("Presenza Rilevata dal PIR");
#endif

      break;
    case 1:
      dmIsNear();
      break;
    case 2:
      dmIsFar();
      break;
    case 3:
      dmIsGone();
      break;
    case 4:
      dmWait();
      break;
    case 5:
      dmTakingCoffe();
      break;
  }
}
