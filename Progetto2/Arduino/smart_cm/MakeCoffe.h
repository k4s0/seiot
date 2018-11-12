#ifndef __MAKECOFFETASK__
#define __MAKECOFFETASK__

#include "Task.h"
#include "Led.h"
extern bool newCoffe;
extern bool coffeReady;
extern bool maintenanceActive;

class MakeCoffe : public Task {

    int pin[3];
    Light* led[3];
    int state;
    int count;

  public:

    MakeCoffe(int pin0, int pin1, int pin2);
    void init(int period);
    void tick();
};

#endif
