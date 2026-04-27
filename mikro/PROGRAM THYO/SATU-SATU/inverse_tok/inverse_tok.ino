float a, b, c, d, e, f, g, h, i, mot0, mot1, mot2;
float Ypos = 0, Xpos = 0, Thet = 0;
int target[] = {0, 0, 0};
int kinRot = 0;

float degree(double deg) { //function declaration
  return (deg * 22) / (7 * 180.0); //calculation for the radian to degree conversion
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

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
  float maxRPM = 100.00;

  target[0] = mot0;
  target[1] = mot1;
  target[2] = mot2;
  int Target[] = {abs(target[0]), abs(target[1]), abs(target[2])};

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

  target[0] = target[0] * variableMax;
  target[1] = target[1] * variableMax;
  target[2] = target[2] * variableMax;

  //        Serial.print(target[0]);
  //        Serial.print(" ");
  //        Serial.print(target[1]);
  //        Serial.print(" ");
  //        Serial.print(target[2]);
  //        Serial.println(" ");

}
