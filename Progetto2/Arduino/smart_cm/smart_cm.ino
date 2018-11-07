#include "Scheduler.h"
#include "MakeCoffe.h"
#include "DetectMotion.h"

bool isPresent = false;
bool correctDistance = false;
bool newCoffe = false;
bool coffeReady = false;
bool coffeTaked = false;
int numCoffe = 10;

Scheduler sched;

void pressButton() {
  newCoffe = true;
}

void setup() {
  Serial.begin(9600);
  sched.init(25);
  Task* mc = new MakeCoffe(3, 4, 5);
  mc->init(50);
  Task* dm = new DetectMotion(6, 7, 8);
  dm->init(500);
  sched.addTask(mc);
  sched.addTask(dm);
  attachInterrupt(0, pressButton, FALLING);
}

void loop() {
  sched.schedule();
}
