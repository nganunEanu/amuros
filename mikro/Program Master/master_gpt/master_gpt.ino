#include <Wire.h>

int i = 0;
long target[][3] = {{20,-20, 0}, {0, 0, 0,} ,{-20, 20, 0},{0, 0, 0,},{-20,-20,35},{0, 0, 0,},{20,20,-35},{0, 0, 0,}};
unsigned long waktu_seb;
unsigned long waktuseb;

void sendLong(long value) {
  // Send a long value as four bytes over I2C
  for (int k = 0; k < 4; k++) {
    byte out = (value >> 8 * (3 - k)) & 0xFF;
    Wire.write(out);
  }
}

long receiveLong() {
  // Receive a long value as four bytes over I2C
  long outValue = 0;
  for (int k = 0; k < 4; k++) {
    byte nextByte = Wire.read();
    outValue = (outValue << 8) | nextByte;
  }
  return outValue;
}

void setup() {
  Wire.begin();        // Join I2C bus
  Serial.begin(9600);
}

void loop() {
  unsigned long waktusek = millis();
  unsigned long waktu_sek = millis();

  // Send data every 100 milliseconds
  if (waktusek - waktuseb >= 5000) {
  
    if (i > 9) {
      i = 0;
    }

    Wire.beginTransmission(1);
    sendLong(target[i][0]);
    Wire.endTransmission();
    Wire.beginTransmission(2);
    sendLong(target[i][1]);
    Wire.endTransmission();
    Wire.beginTransmission(3);
    sendLong(target[i][2]);
    Wire.endTransmission();

    Serial.print(target[i][0]);
    Serial.print(',');
    Serial.print(target[i][1]);
    Serial.print(',');
    Serial.println(target[i][2]);

    waktuseb = waktusek;
    i++;
  }

  // Receive data every 1000 milliseconds
  if (waktu_sek - waktu_seb >= 1000) {
    long pos[] = {0, 0, 0};
    
    Wire.requestFrom(1, 8);
    pos[0] = receiveLong();
    Wire.requestFrom(2, 8);
    pos[1] = receiveLong();
    Wire.requestFrom(3, 8);
    pos[2] = receiveLong();

    Serial.print(pos[0]);
    Serial.print(',');
    Serial.print(pos[1]);
    Serial.print(',');
    Serial.println(pos[2]);

    waktu_seb = waktu_sek;
  }
}
