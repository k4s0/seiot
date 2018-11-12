#include "Sonar.h"
#include "Arduino.h"
const float vs = 331.5 + 0.6 * 20;
Sonar :: Sonar(int analogPin1, int analogPin2) {
  this->pinTrig = analogPin1;
  this->pinEcho = analogPin2;
  pinMode(pinEcho, INPUT);
  pinMode(pinTrig, OUTPUT);
  average = 0;
}

float Sonar :: getDistance() {
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(3);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(5);
  digitalWrite(pinTrig, LOW);
  float tu = pulseIn(pinEcho, HIGH);
  float t = tu / 1000.0 / 1000.0 / 2;
  average = average * 0.7 + t * vs * 0.3;
  return average;
}
