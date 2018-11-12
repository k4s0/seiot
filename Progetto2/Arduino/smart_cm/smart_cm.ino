#include "Scheduler.h"
#include "MakeCoffe.h"
#include "DetectMotion.h"
#include "MainTask.h"

bool isPresent = false;
bool correctDistance = false;
bool newCoffe = false;
bool coffeReady = false;
bool coffeTaked = false;
int numCoffe = 1;

bool maintenanceActive = false;
bool reStart = false;

Scheduler sched;


void setup() {
  Serial.begin(9600);
  sched.init(50);
  Task* mc = new MakeCoffe(3, 4, 5);
  mc->init(100);
  Task* dm = new DetectMotion(6, 7, 8);
  dm->init(50);
  Task* mt = new MainTask(9, 10);
  mt->init(50);
  sched.addTask(dm);
  sched.addTask(mc);
  sched.addTask(mt);

}

void loop() {
  sched.schedule();
}
