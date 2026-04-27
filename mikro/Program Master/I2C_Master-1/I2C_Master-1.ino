#include <Wire.h>
long target[] = {244, 234, 111};
void sendLong(long value) {
  for (int k = 0; k < 4; k++) {
    byte out = (value >> 8 * (3 - k)) & 0xFF;
    Wire.write(out);
  }
}

long receiveLong() {
  long outValue;
  for (int k = 0; k < 4; k++) {
    byte nextByte = Wire.read();
    outValue = (outValue << 8) | nextByte;
  }
  return outValue;
}

void setup() {
  Wire.begin();        // join i2c bus
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  Wire.beginTransmission(1);
  sendLong(target[0]++);       //mengirim menuju alamat i2C addr 1
  Wire.endTransmission();
  // delay(50);
  Wire.beginTransmission(2);
  sendLong(target[1]++);       //mengirim menuju alamat i2C addr 1
  Wire.endTransmission();
  Wire.beginTransmission(3);
  sendLong(target[2]++);       //mengirim menuju alamat i2C addr 1
  Wire.endTransmission();

  // delay(50);

  long pos[] = {0, 0, 0};
  Wire.requestFrom(1, 8);
  pos[0] = receiveLong();    //meminta dari alamat i2C addr 1
  //Serial.println(pos);
  //  delay(50);
  Wire.requestFrom(2, 8);
  pos[1] = receiveLong();    //meminta dari alamat i2C addr 1
  //Serial.println(pos);
  Wire.requestFrom(3, 8);
  pos[2] = receiveLong();    //meminta dari alamat i2C addr 1
  //Serial.println(pos);

  Serial.print(pos[0]);
  Serial.print(',');
  Serial.print(pos[1]);
  Serial.print(',');
  Serial.println(pos[2]);

  // put your main code here, to run repeatedly:

}
