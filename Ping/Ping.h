#ifndef Ping_h
#define Ping_h

#include "Arduino.h"

class Ping{
  public:
  Ping(int trigPin, int echoPin);
  void init();
  int distanceCalc();
  
  private:
  void startTrigger();
  int distance;
  int duration;
  int _trigPin;
  int _echoPin;
};

#endif
