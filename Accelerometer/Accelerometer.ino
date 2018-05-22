#include "I2Cdev.h"
#include "MPU6050.h"
#include "Ping.h"
#include "ArduinoJson.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define TRIGGER_LEFT 10
#define ECHO_LEFT 9
#define TRIGGER_RIGHT 8
#define ECHO_RIGHT 7
#define TRIGGER_FRONT 6
#define ECHO_FRONT 5
#define TRIGGER_DOWN 4
#define ECHO_DOWN 3

MPU6050 accelgyro;
Ping pingLeft(TRIGGER_LEFT,ECHO_LEFT);
Ping pingRight(TRIGGER_RIGHT,ECHO_RIGHT);
Ping pingFront(TRIGGER_FRONT,ECHO_FRONT);
Ping pingDown(TRIGGER_DOWN,ECHO_DOWN);

int16_t ax, ay, az;
int16_t gx, gy, gz;
const int gyroScale = 180;

void setup() {
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(38400);
    accelgyro.initialize();
    pingsensor.init();
}

void loop() {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    dividebyscale();
    sendJSON();
}

void dividebyscale() {
  ax /= gyroScale;
  ay /= gyroScale;
  az /= gyroScale;
}

void sendJSON() {
   DynamicJsonBuffer jBuffer;
   JsonObject& root = jBuffer.createObject();
   JsonArray& pingData = root.createNestedArray("Distance");
   pingData.add(pingLeft.distanceCalc();
   pingData.add(pingRight.distanceCalc();
   pingData.add(pingFront.distanceCalc();
   pingData.add(pingDown.distanceCalc();

   JsonArray& accelData = root.createNestedArray("Degrees");
   accelData.add(ax);
   accelData.add(ay);
   accelData.add(az);

   root.printTo(Serial);
   Serial.println();
}
