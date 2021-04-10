

const int green = 5;
const int red = 6;
const int yellow = 7;
const int blue = 8;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello, World!");

  // initialize button pins as inputs
  pinMode(green, INPUT);
  pinMode(red, INPUT);
  pinMode(yellow, INPUT);
  pinMode(blue, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(digitalRead(green))
  {
    Serial.print("G");
  }
  if(digitalRead(red))
  {
    Serial.print("R");
  }
  if(digitalRead(yellow))
  {
    Serial.print("Y");
  }
  if(digitalRead(blue))
  {
    Serial.print("B");
  }
  Serial.println();
  delay(50);


}
