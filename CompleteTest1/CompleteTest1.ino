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
  unsigned long start_time = millis();
  //Serial.println(getControllerState());
  //getControllerState();
  BTserial.print(getControllerState());
  unsigned long end_time = millis();
  Serial.println(end_time-start_time);
  delay(25);

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
  if(getStrumStatus())
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
