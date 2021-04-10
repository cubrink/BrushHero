// Basic Bluetooth sketch BT_TALK
// Connect the module and communicate using the serial monitor
// Communicate with the BT module at 9600 (comms mode)
 
#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX. 
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.
// 

char* chars[] = {"Hello", "World"};

void setup() 
{
    // Initialize the Serial Monitor
    Serial.begin(9600);
    Serial.println("Arduino is ready");
    Serial.println("Remember to select Both NL & CR in the serial monitor");

    // Initialize serial for bluetooth module
    // HC-05 default serial speed for Command mode is 9600
    BTserial.begin(9600);  
}
 
void loop()
{
    
    // Keep reading from H`C-05 and send to Arduino Serial Monitor
    // if data is available - buffer is not empty
    for (int i = 0; i < sizeof(chars)/sizeof(chars[0]); i++) {
      delay(8000);
      BTserial.print(chars[i]);  
      Serial.print("Printed: ");
      Serial.println(chars[i]);
    }
    
    
    

    
 
}
