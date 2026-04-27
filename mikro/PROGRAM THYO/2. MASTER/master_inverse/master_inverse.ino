#include <Wire.h>

/*===== VARIABEL INVERSE KINEMATICS =====*/
float a, b, c, d, e, f, g, h, i, mot0, mot1, mot2;
float Ypos = 0, Xpos = 0, Thet = 0;
int targetkin[] = {0, 0, 0};
int kinRot = 0;

float degree(double deg) { //function declaration
  return (deg * 22) / (7 * 180.0); //calculation for the radian to degree conversion
}
/*=======================================*/

int j = 0;
long target[][3] = {{20, -20, 0}, {0, 0, 0,} ,{-20, 20, 0},{0, 0, 0,},{0, 0, 0,}};
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

  kinematik(0, 50, 0 , 2);
  
  // Send data every 100 milliseconds
  if (waktusek - waktuseb >= 5000) {
    j++;
    if (j > 2) {
      j = 0;
    }

    Wire.beginTransmission(1);
    sendLong(targetkin[0]);
    Wire.endTransmission();
    Wire.beginTransmission(2);
    sendLong(targetkin[1]);
    Wire.endTransmission();
    Wire.beginTransmission(3);
    sendLong(targetkin[2]);
    Wire.endTransmission();

    Serial.print(targetkin[0]);
    Serial.print(" ");
    Serial.print(targetkin[1]);
    Serial.print(" ");
    Serial.print(targetkin[2]);
    Serial.println(" ");

//    Serial.print(target[j][0]);
//    Serial.print(',');
//    Serial.print(target[j][1]);
//    Serial.print(',');
//    Serial.println(target[j][2]);

    waktuseb = waktusek;
  }

  // Receive data every 1000 milliseconds
  if (waktu_sek - waktu_seb >= 1000) 
  {
    long pos[] = {0, 0, 0};
    
    Wire.requestFrom(1, 8);
    pos[0] = receiveLong();
    Wire.requestFrom(2, 8);
    pos[1] = receiveLong();
    Wire.requestFrom(3, 8);
    pos[2] = receiveLong();

//    Serial.print(pos[0]);
//    Serial.print(',');
//    Serial.print(pos[1]);
//    Serial.print(',');
//    Serial.println(pos[2]);

    waktu_seb = waktu_sek;
  }
}

void kinematik(float x, float y, float thet, float spd) {
  kinRot = 0;
  
  float tt1 = ((((thet * 1256) / 120) * 3) * 0.7);
  float  x1 = x * spd;
  float  y1 = y * spd;

  a = -0.33;  b = 0.577;   c = 0.33;
  d = -0.33;  e = -0.577;  f = 0.33;
  g = 0.67;   h = 0;       i = 0.33;

  float Ypos = (cos(degree(Thet)) * y) + (sin(degree(Thet)) * x);
  float Xpos = (-sin(degree(Thet)) * y) + (cos(degree(Thet)) * x);

  mot0 = (a * Xpos * spd + b * Ypos * spd  + c * tt1 * spd);
  mot1 = (d * Xpos * spd + e * Ypos * spd  + f * tt1 * spd);
  mot2 = (g * Xpos * spd + h * Ypos * spd  + i * tt1 * spd);

  float variableMax = 1.00;
  float maxRPM = 30.00;

  targetkin[0] = mot0;
  targetkin[1] = mot1;
  targetkin[2] = mot2;
  int Target[] = {abs(targetkin[0]), abs(targetkin[1]), abs(targetkin[2])};

  if (Target[0] > maxRPM) 
  {
    if (Target[0] >= Target[1] && Target[0] >= Target[2]) 
    {
      variableMax = (maxRPM / Target[0]);
    }
  }
  
  if (Target[1] > maxRPM) 
  {
    if (Target[1] >= Target[0] && Target[1] >= Target[2]) 
    {
      variableMax = (maxRPM / Target[1]);
    }
  }
  
  if (Target[2] > maxRPM) 
  {
    if (Target[2] >= Target[1] && Target[2] >= Target[0]) 
    {
      variableMax = (maxRPM / Target[2]);
    }
  }

  targetkin[0] = int(targetkin[0] * variableMax);
  targetkin[1] = int(targetkin[1] * variableMax);
  targetkin[2] = int(targetkin[2] * variableMax);

  //        Serial.print(targetkin[0]);
  //        Serial.print(" ");
  //        Serial.print(targetkin[1]);
  //        Serial.print(" ");
  //        Serial.print(targetkin[2]);
  //        Serial.println(" ");
}
