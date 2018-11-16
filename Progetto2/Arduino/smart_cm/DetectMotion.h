#ifndef __DETECTMOTION__
#define __DETECTMOTION__

#include "Task.h"
#include "Pir.h"
#include "Sonar.h"


extern bool isPresent;
extern bool correctDistance;
extern bool maintenanceActive;
extern bool reStart;
extern bool coffeReady;
extern bool coffeTaked;
extern bool newCoffe;

/*
 * Task that handle the user movement.
 */
class DetectMotion : public Task {
private:
    int pinPir; 
    int pinTrig;
    int pinEcho;
    int state;
    int count;
    float distance;
    Sonar *sonar;
    Pir *pir;
    void dmIsNear();
    void dmIsFar();
    void dmIsGone();
    void dmWait();
    void dmTakingCoffe();
  public:
    DetectMotion(int pinPir, int pinTrig, int pinEcho);
    void init(int period);
    void tick();
};

#endif
