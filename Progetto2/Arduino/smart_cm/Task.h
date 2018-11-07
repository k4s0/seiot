#ifndef __TASK__
#define __TASK__


class Task {

    int timeElapsed;

  protected:
    int myPeriod;

  public:
    virtual void init(int period) {
      myPeriod = period;
      timeElapsed = 0;
    }

    virtual void tick() = 0;

    bool updateAndCheckTime(int basePeriod) {
      timeElapsed += basePeriod;
      if (timeElapsed >= myPeriod) {
        timeElapsed = 0;
        return true;
      } else {
        return false;
      }
    }

};

#endif
