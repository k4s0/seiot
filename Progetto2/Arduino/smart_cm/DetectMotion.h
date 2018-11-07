#ifndef __DETECTMOTION__
#define __DETECTMOTION__

#include "Task.h"
#include "Pir.h"
#include "Sonar.h"

extern bool isPresent;
extern bool correctDistance;

class DetectMotion : public Task {
    int pinPir;
    int pinTrig;
    int pinEcho;
    int state;
    int count;
    float distance;
    Sonar *sonar;
    Pir *pir;
  private:
    void dmIsNear();
    void dmIsFar();
    void dmIsGone();
  public:
    DetectMotion(int pinPir, int pinTrig, int pinEcho);
    void init(int period);
    void tick();
};

#endif
