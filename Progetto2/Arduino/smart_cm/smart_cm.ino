#include "Scheduler.h"
#include "MakeCoffe.h"
#include "DetectMotion.h"
#include "UserHandler.h"

/*someone is in front of the machine*/
bool isPresent = false;
/*someone stand in front of the machine*/
bool correctDistance = false;
/*user asks a new coffe*/
bool newCoffe = false;
/*a coffe is ready*/
bool coffeReady = false;
/*user take coffe*/
bool coffeTaked = false;
int numCoffe = 1;
/*true if there is no more coffe*/
bool maintenanceActive = false;
/*when is true system restart*/
bool reStart = false;

Scheduler sched;

void setup() {
  Serial.begin(9600);
  sched.init(50);
  Task* mc = new MakeCoffe(3, 4, 5);
  mc->init(100);
  Task* dm = new DetectMotion(6, 7, 8);
  dm->init(50);
  Task* mt = new UserHandler(A0, 9);
  mt->init(50);
  sched.addTask(dm);
  sched.addTask(mc);
  sched.addTask(mt);
}

void loop() {
  sched.schedule();
}
