#include "Arduino.h"
#include "Ping.h"

Ping::Ping(int trigPin, int echoPin) {
  _trigPin = trigPin;
  _echoPin = echoPin;
}

void Ping::init() {    
  pinMode(_trigPin, OUTPUT);
  pinMode(_echoPin, INPUT);
}

int Ping::distanceCalc() {
  startTrigger();        
  duration = pulseIn(_echoPin, HIGH);
  distance = duration*0.034/2;
  return distance;
}

void Ping::startTrigger() {
  digitalWrite(_trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);
}




        
