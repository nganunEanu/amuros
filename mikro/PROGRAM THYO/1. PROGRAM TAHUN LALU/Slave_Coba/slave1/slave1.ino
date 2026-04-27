/*
  Made on 8 June 2021
  By Amirmohammad Shojaei
  Home
*/

#include <Wire.h>

int x;
int value=0; 

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
//  Wire.onRequest(requestEvent); // register event
//  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);    // start serial for output

  pinMode(9,OUTPUT);
}

void loop() {
 value=33;
  Wire.beginTransmission(2); // transmit to device #8
  Wire.write("Master value :  ");         // sends these bytes
  Wire.write(value);              // sends one byte
  Wire.endTransmission();    // stop transmitting

if(Wire.available()>0){
  int c = Wire.read();
  Serial.println(c);
}
  
}
