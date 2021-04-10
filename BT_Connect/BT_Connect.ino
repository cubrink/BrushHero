// Basic Bluetooth sketch BT_TALK
// Connect the module and communicate using the serial monitor
// Communicate with the BT module at 9600 (comms mode)
 
#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3); // RX | TX
// Connect the HC-05 TX to Arduino pin 2 RX. 
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.
// 
 
char c = ' ';                                 // data type to save imported data as
const int word_length = 8;                    // number of bits to receive
char bits_saved[word_length];                 // array to save imported data in - word_length: number of imported characters
int cur_bit_index = 0;                        // used to keep count of current character input number in array
 
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
    if (BTserial.available())
    {  
        c = BTserial.read();                      // read in from buffer
        
        bits_saved[cur_bit_index] = c;            // store to input array
        cur_bit_index++;                          // increment index
        if(cur_bit_index >= word_length)
        {
          Serial.println("Receive Complete!");    // tell user 8 characters have been received
          for(int i = 0; i < word_length; i++)
          {
            Serial.write(bits_saved[i]);          // print bits to screen
            bits_saved[i] = ' ';                  // clear array so all elements are false
          }
          Serial.println();                       // end line
          cur_bit_index = 0;                      // reset index
          
        }



        // other test code from initial start of file
        //Serial.write(c);
        //Serial.println();
        //Serial.println("Sending back data");
        //BTserial.write(1);
        //Serial.println("Transmission complete!");
    }
 
}
