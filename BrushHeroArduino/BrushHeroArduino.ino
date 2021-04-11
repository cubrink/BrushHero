/*
 * Brush Hero Final
 * This code should be loaded onto the Arduino that is placed in the toothbrush
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


// Libraries for Bloothtooth Module
#include <SoftwareSerial.h>

// Libraries for Accelerometer
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


// Bluetooth Initialization and Constants
SoftwareSerial BTserial(2, 3); // RX | TX

// Accelerometer Initialization and Constants
Adafruit_MPU6050 mpu;
const float J2_X_THRESH = 40; // Squared jerk, Up and Down movement
const float J2_Y_THRESH = 25; // Squared jerk, Back and forth movement
const int JERK_COOLDOWN = 3;

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

// Holds state of strumming
bool strummed = false;

// acceleration values for jerk calculation
float x_curr, y_curr, z_curr;
float x_prev, y_prev, z_prev;

bool cooldown_active = false;
int cooldown_count = 0;


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
  // Read sensors to determine state
  int state = getControllerState(strummed);
  BTserial.write(state); // This has to be .write()! Using .print() can clip values that are received!

  // There was a problem with over-strumming
  // Setting a countdown between ticks helps prevent accidental rapidfire strums
  //
  // If cooldown active, add to cooldown_count
  if (cooldown_active) {
    cooldown_count++;
    // If cooldown met, mark as active again and reset
    if (cooldown_count >= JERK_COOLDOWN) {
        cooldown_active = false;
        cooldown_count = 0;
    }
  }
  // When strummed, set active
  else if (strummed) {
    cooldown_active = true;
  }
  
  // We don't need to update any faster than this.
  // When we went faster we ran into some inconsistencies when reading
  // the data from the device we connected with
  delay(20);
}


/*
 * Returns whether the user is strumming
 */
bool getStrumStatus()
{
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  x_prev = x_curr;
  y_prev = y_curr;

  x_curr = a.acceleration.x;
  y_curr = a.acceleration.y;

  float j_x = (x_curr - x_prev);
  float j_y = (y_curr - y_prev);

  float j2_x = j_x*j_x;
  float j2_y = j_y*j_y;

  return (j2_x >= J2_X_THRESH) || (j2_y >= J2_Y_THRESH);
}

/*
 * Returns controller state as a character
 * 
 * Does bitwise operations based on device inputs
 */
int getControllerState(bool& strummed)
{
  int controller_state = 0;
  strummed = getStrumStatus();
  if(strummed)
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
