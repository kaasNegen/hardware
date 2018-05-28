#include "I2Cdev.h"
#include "MPU6050.h"
#include "Ping.h"
#include "ArduinoJson.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  #include "Wire.h"
#endif


#define TRIGGER_FRONT 3
#define ECHO_FRONT 4
#define TRIGGER_LEFT 5
#define ECHO_LEFT 6
#define TRIGGER_RIGHT 7
#define ECHO_RIGHT 8
#define TRIGGER_DOWN 9
#define ECHO_DOWN 10


MPU6050 accelgyro;
Ping sensor_front(TRIGGER_RIGHT,ECHO_RIGHT);
Ping sensor_left(TRIGGER_RIGHT,ECHO_RIGHT);
Ping sensor_right(TRIGGER_RIGHT,ECHO_RIGHT);
Ping sensor_down(TRIGGER_DOWN,ECHO_DOWN);


int16_t ax, ay, az;
int16_t gx, gy, gz;
const float gyroscale = 131;
const float radianstodegrees = 180/3.14159;
float accel_angle_y,accel_angle_x,gyro_x,gyro_y,gyro_z,
last_read_time,last_x_angle,last_y_angle,last_z_angle,alpha,angle_x,angle_y;
double dt;


void setup() {
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

   Serial.begin(19200);
   accelgyro.initialize();
   set_last_read_angle_data(millis(), 0, 0, 0);
   pingsensor.init();  
}


void loop() {
   dividebyscale();
   sendJSON();
}


void dividebyscale() {
   accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
   unsigned long t_now = millis();
   accel_angle_y = atan(-1*ax/sqrt(pow(ay,2) + pow(az,2)))*radianstodegrees;
   accel_angle_x = atan(ay/sqrt(pow(ax,2) + pow(az,2)))*radianstodegrees;
   gx = gx/gyroscale;
   gy = gy/gyroscale;
   gz = gz/gyroscale;
   dt = (t_now - last_read_time)/1000;
   gyro_x = gx*dt + last_x_angle;
   gyro_y = gy*dt + last_y_angle;
   gyro_z = gz*dt + last_z_angle;
   alpha = 0.96;
   angle_x = alpha*gyro_x + (1.0 - alpha)*accel_angle_x;
   angle_y = alpha*gyro_y + (1.0 - alpha)*accel_angle_y;
   set_last_read_angle_data(t_now, angle_x, angle_y, gyro_z);
}


void set_last_read_angle_data(unsigned long time, float x, float y, float z) {
  last_read_time = time;
  last_x_angle = x;
  last_y_angle = y;
  last_z_angle = z;
}


void sendJSON() {
  DynamicJsonBuffer jBuffer;
  JsonObject& root = jBuffer.createObject();
  JsonArray& accelData = root.createNestedArray("degrees");
  JsonArray& distData = root.createNestedArray("distance");
  accelData.add(angle_x);
  accelData.add(angle_y);
  distData.add(sensor_front.distanceCalc());
  distData.add(sensor_left.distanceCalc());
  distData.add(sensor_right.distanceCalc());
  distData.add(sensor_down.distanceCalc());
  root.printTo(Serial);
  
  Serial.println();
}
