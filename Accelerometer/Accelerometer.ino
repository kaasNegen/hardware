#include "I2Cdev.h"
#include "MPU6050.h"
#include "Ping.h"
#include "ArduinoJson.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define TRIGGER 10
#define ECHO 9

MPU6050 accelgyro;
Ping pingsensor(TRIGGER,ECHO);

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
   root["Distance"] = pingsensor.distanceCalc();

   JsonArray& accelData = root.createNestedArray("Degrees");
   accelData.add(ax);
   accelData.add(ay);
   accelData.add(az);

   root.printTo(Serial);
   Serial.println();
}
