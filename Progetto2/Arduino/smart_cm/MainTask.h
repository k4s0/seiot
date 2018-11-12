#ifndef __MAINTASK__
#define __MAINTASK__

#include "Task.h"
#include "Potentiometer.h"
#include "Button.h"

extern bool isPresent;
extern bool correctDistance;
extern bool newCoffe;
extern bool coffeReady;
extern bool coffeTaked;
extern int numCoffe;
extern bool reStart;
extern bool maintenanceActive;

enum class MainState{STANDBY, ON, READY, MAKECOFFE, TAKECOFFE,MAINTENANCE};

class MainTask : public Task {
  private:
    int pinPot;
    int pinButton;
    Potentiometer *pot;
    Button *button;
    MainState state;
    int prevPotValue;
  public:
    MainTask(int pinPot, int pinBut);
    void init(int period);
    void tick();
};

#endif
