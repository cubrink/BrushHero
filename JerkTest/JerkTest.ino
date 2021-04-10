/*
 * Brush Hero Prototype Test 1
 * 
 * Bluetooth Module HC-06 ZS-040 
 *    Vcc -> 5V
 *    GND -> GND
 *    TXD -> 3
 *    RXD -> 2
 * 
 * Accelerometer MPU6050
 *    Vcc -> 5V
 *    GND -> GND
 *    SCL -> A5
 *    SDA -> A4
 * 
 * Buttons
 *    Green -> 4
 *    Red -> 5
 *    Yellow -> 6
 *    Blue -> 7
 *    Orange -> 8
 * 
 * 
 * Bit Definition (8-bit integer : type int)
 *    0: Strum
 *    1: Green
 *    2: Red
 *    3: Yellow
 *    4: Blue
 *    5: Orange
 * 
 * 
 */


/*
 * Strum detection algorithm:
 * 
 * Calculate
 * 
 */


// Libraries for Bloothtooth Module
#include <SoftwareSerial.h>

// Libraries for Accelerometer
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


// Bluetooth Initialization and Constants
SoftwareSerial BTserial(2, 3); // RX | TX
char* chars[] = {"Hello", "World"};

// Accelerometer Initialization and Constants
Adafruit_MPU6050 mpu;
const float ACCEL_THRESHOLD = 25.0;
const float JERK_THRESHOLD = 5;
const int JERK_COOLDOWN = 2;
const long JERK_TIME_COOLDOWN = 100;

// Button Initialization and Constants
const int green = 4;
const int red = 5;
const int yellow = 6;
const int blue = 7;
const int orange = 8;

// Controller Status/Values
// int controller_state = 0;
const int STRUM_BIT = 0;
const int GREEN_BIT = 1;
const int RED_BIT = 2;
const int YELLOW_BIT = 3;
const int BLUE_BIT = 4;
const int ORANGE_BIT = 5;


// acceleration values for jerk calculation
float x_curr, y_curr, z_curr;
float x_prev, y_prev, z_prev;

bool cooldown_active = false;
int cooldown_count = 0;

unsigned long start = millis();
unsigned long end_ = millis();

unsigned long time_start = millis();


void setup() {
  Serial.begin(9600);
  Serial.println("Arduino Started");

  /* 
   *  Bluetooth Setup
   */
  BTserial.begin(9600); 
  

  /*
   * Accelerometer Setup
   */
  if (!mpu.begin()) 
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1) 
    {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);


  /*
   * Button Setup
   */
  pinMode(green, INPUT);
  pinMode(red, INPUT);
  pinMode(yellow, INPUT);
  pinMode(blue, INPUT);
  pinMode(orange, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  BTserial.print(getControllerState());
//  Serial.print("cooldown_active: ");
//  Serial.print(cooldown_active);
//  Serial.print(", cooldown_count: ");
//  Serial.println(cooldown_count);
//  float jerk = getJerk();
//  if (cooldown_active) {
////    if (jerk < JERK_THRESHOLD) {
////          cooldown_count++;
////    }
//    cooldown_count++;
//    if (cooldown_count >= JERK_COOLDOWN) {
//        cooldown_active = false;
//        cooldown_count = 0;
//    }
//  }
  if (cooldown_active) {
    cooldown_active = (millis() - time_start) < JERK_TIME_COOLDOWN;
  }
  else if (getJerk() > JERK_THRESHOLD) {
    end_ = millis();
    Serial.print("Since last strum ");
    Serial.print(end_ - start);
    Serial.println("ms.");
//    Serial.println("Strum");
    start = millis();
    time_start = millis();
    cooldown_active = true;
  }
  
  delay(25);

}

/*
 * Returns the jerk value at a given time
 */
float getJerk()
{
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  x_prev = x_curr;
  y_prev = y_curr;
  z_prev = z_curr;

  x_curr = a.acceleration.x;
  y_curr = a.acceleration.y;
  z_curr = a.acceleration.z;

  float j_x = (x_curr - x_prev);
  float j_y = (y_curr - y_prev);
  float j_z = (z_curr - z_prev);

//  return sqrt((j_x*j_x)+(j_y*j_y)+(j_z*j_z));
  return (((x_curr - x_prev) + (y_curr - y_prev) + (z_curr - z_prev)));
}

/*
 * Returns wether the user is strumming
 */
bool getStrumStatus()
{
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float x = a.acceleration.x;
  float y = a.acceleration.y;
  float z = a.acceleration.z-9.8;
  float vect_mag = sqrt((x*x)+(y*y)+(z*z)) - 9.8;

  return vect_mag >= ACCEL_THRESHOLD;
}

/*
 * Returns controller state as a character
 * 
 * Does bitwise operations based on device inputs
 */
int getControllerState()
{
  int controller_state = 0;
//  if(getStrumStatus())
  if (false)
  {
    controller_state |= 1 << STRUM_BIT;
  }
  if(digitalRead(green))
  {
    controller_state |= 1 << GREEN_BIT;
  }
  if(digitalRead(red))
  {
    controller_state |= 1 << RED_BIT;
  }
  if(digitalRead(yellow))
  {
    controller_state |= 1 << YELLOW_BIT;
  }
  if(digitalRead(blue))
  {
    controller_state |= 1 << BLUE_BIT;
  }
  if(digitalRead(orange))
  {
    controller_state |= 1 << ORANGE_BIT;
  }
  return controller_state;
}
