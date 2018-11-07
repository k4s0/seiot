#ifndef __POTENTIOMETER__
#define __POTENTIOMETER__
class Potentiometer {
  public:
    Potentiometer(int pin, int bot, int top);
    int getValue();
  private:
    int pin;
    int bot;
    int top;
};
#endif
