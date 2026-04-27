#include <Wire.h>

int value = 0;

void setup() {
  Wire.begin(2); // join i2c bus (address optional for master)
  Serial.begin(9600);

  pinMode(5, OUTPUT);
}

void loop() {
  value = 4;

  Wire.beginTransmission(8); // transmit to device #8
  Wire.write("Master value :  ");         // sends these bytes
  Wire.write(value);              // sends one byte
  Wire.endTransmission();    // stop transmitting

  Wire.requestFrom(8, 1);    // request 1 byte from slave device #8
  while (Wire.available()) { // slave may send less than requested
    int c = Wire.read(); // receive a byte as character
    
    analogWrite(5, c);
    Serial.print("Slave value : ");
    Serial.println(c);         // print the character
  }

}
