#ifndef __SONAR__
#define __SONAR__

class Sonar {
  public:
    Sonar(int analogPin1, int analogPin2);
    float getDistance();

  private:
    int pinTrig;
    int pinEcho;
};

#endif
