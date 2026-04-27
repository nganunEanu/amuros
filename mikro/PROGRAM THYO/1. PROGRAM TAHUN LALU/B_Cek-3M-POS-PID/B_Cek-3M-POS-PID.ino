

// This alternate version of the code does not require
// atomic.h. Instead, interrupts() and noInterrupts() 
// are used. Please use this code if your 
// platform does not support ATOMIC_BLOCK.

#include <Wire.h>
#include <math.h>

/*------------ CLASS ------------*/
class SimplePID{
  private:
    float kp, kd, ki, umax;
    float eprev, eintegral;
    
  public:
    // Default initialization list
    SimplePID() : kp(1), kd(0), ki(0), umax(255), eprev(0.0), eintegral(0.0) {}
    
    // Set the parameters
    void setParams(float kpIn, float kdIn, float kiIn, float umaxIn){
      kp = kpIn; kd = kdIn; ki = kiIn; umax = umaxIn;
    }

    // Evaluate the signal
    void evalu(int value, int target, float deltaT,int &pwr, int &dir){
        
      // error
      int e = target - value;
      
//      float dedt = (e-eprev)/(deltaT);
//      eintegral = eintegral + e*deltaT;

      float dedt = (e-eprev) / 0.1;
      eintegral = eintegral + e * 0.1;

      float u = kp*e + kd*dedt + ki*eintegral;
    
      // motor power
      pwr = (int) fabs(u);
      if( pwr > umax ){
        pwr = umax;
      }

Serial.print(target);
Serial.print(" ");
Serial.print(value);
Serial.print(" ");
Serial.print(e);
Serial.print(" ");
Serial.print(dedt);
Serial.print(" ");
Serial.print(eintegral);
Serial.print(" ");
Serial.println(u);
           
      // motor direction
      dir = 1;
      if(u<0){
        dir = -1;
      }
            
      // store previous error
      eprev = e;
    }
    
};

/*------------ GLOBALS AND DEFINITIONS ------------*/
// Define the motors
#define NMOTORS 3
#define M0 0
#define M1 1
#define M2 2

const int enca[] = {2,20,3};
const int encb[] = {50,48,51};
const int pwm[] = {5,6,7};
const int in1[] = {38,46,43};
const int in2[] = {39,47,42};


// Global variables
float a,b,c,d,e,f,g,h,i;
int posi0 = 0;
int posi1 = 0;
int posi2 = 0;
long milseb = 0;
float q = 0;
float w = 0;
float s = 0;
float xx = 0,yy = 0,tt = 0, ypos =0, xpos = 0;
const char* aa1;
long prevT = 0;
int posPrev[] = {0,0,0};
double bun =0, thet = 0;

// positions
volatile int posi[] = {0,0,0};


// PID classes
SimplePID pid[NMOTORS];


float degree(double deg){  //function declaration
 return (deg*22)/(7*180);     //calculation for the radian to degree conversion
}

/*------------ FUNCTIONS ------------*/
void setMotor(int dir, int pwmVal, int pwm, int in1, int in2){
  analogWrite(pwm,pwmVal);
  if(dir == 1){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
  else if(dir == -1){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
  }
  else{
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }  
}

template <int j>
void readEncoder(){
  int b = digitalRead(encb[j]);
  if(b > 0){
    posi[j]++;
  
  }
  else{
    posi[j]--;
   
  }
}

void sendLong(long value){
  for(int k=0; k<4; k++){
    byte out = (value >> 8*(3-k)) & 0xFF;
    Wire.write(out);
  }
}

long receiveLong(){
  long outValue;
  for(int k=0; k<4; k++){
    byte nextByte = Wire.read();
    outValue = (outValue << 8) | nextByte;
  }
  return outValue;
}

// targets
float target_f[] = {0,0,0,0};
long target[] = {30,30,30,0};

void setTarget(float t, float deltat){

  float positionChange[4] = {0.0,0.0,0.0,0.0};

//z  
  float pulsesPerTurn0 = 134; 
  float pulsesPerMeter0 = pulsesPerTurn0*2.50764;
//y
  
  float pulsesPerTurn1 = 137; 
  float pulsesPerMeter1 = pulsesPerTurn1*2.50764;

//x

  float pulsesPerTurn = 133; 
  float pulsesPerMeter = pulsesPerTurn*2.50764;  

//-------------------------------------------------------------

float x=0;
float y=0;
float o=0;
//float s=1;
float t_=4;
float rd = 6.28;
//float q,w,e,q1,w1,e1;
float mot0, mot1, mot2;




// inverse
a = -0.33;  b = 0.577;   c = 0.33;
d = -0.33;  e = -0.577;  f = 0.33;
g = 0.67;  h = 0;      i = 0.33;

//a = -5.29;  b = 9.17;   c = 1.04;
//d = -5.29;  e = -9.17;  f = 1.04;
//g = 10.58;  h = 0;      i = 1.04;


 
//float velocity = s / t_;
//float velocity = 0.374;
  float velocity = 0.25;
 
  mot0 = (a*x + b*y  + c*o);
  mot1 = (d*x + e*y  + f*o);
  mot2 = (g*x + h*y  + i*o);

  
  
//  q = velocity*deltat*pulsesPerMeter0;
//  q1 = abs((velocity*deltat*pulsesPerMeter0) * mot0);
//
//  w = velocity*deltat*pulsesPerMeter1;
//  w1 = abs((velocity*deltat*pulsesPerMeter1) * mot1);
//
//  e = velocity*deltat*pulsesPerMeter;
//  e1 = abs((velocity*deltat*pulsesPerMeter) * mot2);

  t = fmod(t,20);

  if(t < 4){}

  else if(t < t_ + 4){
x=1;
y=1;

  mot0 = (a*x + b*y  + c*o);
  mot1 = (d*x + e*y  + f*o);
  mot2 = (g*x + h*y  + i*o);
    
      positionChange[0] = (velocity*deltat*pulsesPerMeter0) * mot0 ;
      positionChange[1] = (velocity*deltat*pulsesPerMeter1) * mot1;
      positionChange[2] = (velocity*deltat*pulsesPerMeter) * mot2;  
    x=0;
    y=0;
    
    }

//    x=0;
//
  else if(t < t_ + 8){
    y=1;
    x=0.6;
  mot0 = (a*x + b*y  + c*o);
  mot1 = (d*x + e*y  + f*o);
  mot2 = (g*x + h*y  + i*o);
    
      positionChange[0] = -(velocity*deltat*pulsesPerMeter0) * mot0 ;
      positionChange[1] = -(velocity*deltat*pulsesPerMeter1) * mot1;
      positionChange[2] = -(velocity*deltat*pulsesPerMeter) * mot2;  
    y=0;
    x=0;
    }
//////y=0;
  else if(t < t_ + 12){
        y=1.3;
        x=0.8;
  mot0 = (a*x + b*y  + c*o);
  mot1 = (d*x + e*y  + f*o);
  mot2 = (g*x + h*y  + i*o);
      positionChange[0] = (velocity*deltat*pulsesPerMeter0) * mot0 ;
      positionChange[1] = (velocity*deltat*pulsesPerMeter1) * mot1;
      positionChange[2] = (velocity*deltat*pulsesPerMeter) * mot2;  
    y=0;
    x=0;
    }    
//y=0;


  else {
    x=1;
    
  mot0 = (a*x + b*y  + c*o);
  mot1 = (d*x + e*y  + f*o);
  mot2 = (g*x + h*y  + i*o);
      positionChange[0] = -(velocity*deltat*pulsesPerMeter0) * mot0;
      positionChange[1] = -(velocity*deltat*pulsesPerMeter1) * mot1;
      positionChange[2] = -(velocity*deltat*pulsesPerMeter) * mot2;  
   x=0; 
   
  }

//  x=0;

//  t = fmod(t,14);
//
//  if(t < 4){}
//
//  else if(t < t_ + 5){
//      positionChange[0] = (mot0*deltat*pulsesPerTurn0);
//      positionChange[1] = (mot1*deltat*pulsesPerTurn1);
//      positionChange[2] = (mot2*deltat*pulsesPerTurn);  
//    
//    }
//
//  else {
//      positionChange[0] = -(mot0*deltat*pulsesPerTurn0);
//      positionChange[1] = -(mot1*deltat*pulsesPerTurn1);
//      positionChange[2] = -(mot2*deltat*pulsesPerTurn);  
//  }


 


   
  

//-------------------------------------------------------------

//  t = fmod(t,12); // time is in seconds
//  float velocity = 0.25; // m/s

//  if(t < 4){
//  }
//  else if(t < 8){
//    
//      positionChange[0] = velocity*deltat*pulsesPerMeter0;
//      positionChange[1] = velocity*deltat*pulsesPerMeter1;
//      positionChange[2] = velocity*deltat*pulsesPerMeter;
//      positionChange[3] = velocity*deltat*pulsesPerMeter;
//    
//  }
//  else{
//    
//      positionChange[0] = -velocity*deltat*pulsesPerMeter0;
//      positionChange[1] = -velocity*deltat*pulsesPerMeter1;
//      positionChange[2] = -velocity*deltat*pulsesPerMeter;
//      positionChange[3] = -velocity*deltat*pulsesPerMeter;
//    
//  }  

//  for(int k = 0; k < 4; k++){
//    target_f[k] = target_f[k] + positionChange[k];
//  }
//  target[0] = (long) target_f[0];
//  target[1] = (long) target_f[1];
//  target[2] = (long) target_f[2];
//  target[3] = (long) -target_f[3];
}



//void inv (float x, float y, float o, float t_ , float s){
//float mot0, mot1, mot2;
//
//// inverse
//a = -0.33;  b = 0.577;   c = 0.065;
//d = -0.33;  e = -0.577;  f = 0.065;
//g = 0.66;  h = 0;      i = 0.065;
// 
//float velocity = s / t_;
// 
//  mot0 = abs(a*x + b*y  + c*o);
//  mot1 = abs(d*x + e*y  + f*o);
//  mot2 = abs(g*x + h*y  + i*o);
//
//  t = fmod(t,t_);
//
//  if( t<= t_)
//      positionChange[0] = (velocity*deltat*pulsesPerMeter0) / mot0;
//      positionChange[1] = (velocity*deltat*pulsesPerMeter1) / mot1;
//      positionChange[2] = (velocity*deltat*pulsesPerMeter) / mot2;  
//  }

/*------------ SETUP ------------*/
void setup() {
  Wire.begin();        // join i2c bus
 // Serial2.begin(115200);
  Serial.begin(9600);
  for(int k = 0; k < NMOTORS; k++){
    pinMode(enca[k],INPUT_PULLUP);
    pinMode(encb[k],INPUT_PULLUP);
    pid[k].setParams(0.02,0,0.2,255);
  }
  attachInterrupt(digitalPinToInterrupt(enca[M0]),readEncoder<M0>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[M1]),readEncoder<M1>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[M2]),readEncoder<M2>,RISING);
  
}

/*------------ LOOP ------------*/
void loop() {

  // time difference
  long currT = micros();
  float deltaT = ((float) (currT - prevT))/( 1.0e6 );
  prevT = currT;
    
  // set target position
  setTarget(currT/1.0e6,deltaT);
  
  // Send the requested position to the follower
//  Wire.beginTransmission(1); 
//  sendLong(target[2]);
//  sendLong(target[3]);
//  Wire.endTransmission();

  // Get the current position from the follower
  long pos[4];
//  Wire.requestFrom(1, 8);    
//  pos[2] = receiveLong();
//  pos[3] = receiveLong();

  // Read the position in an atomic block to avoid a potential misread 
  noInterrupts(); // disable interrupts temporarily while reading
  for(int k = 0; k < NMOTORS; k++){
    pos[k] = posi[k];
  }
  interrupts(); // turn interrupts back on

//   Loop through the motors
  for(int k = 0; k < NMOTORS; k++){
    int pwr, dir;
    pid[k].evalu(pos[k],target[k],deltaT,pwr,dir); // compute the position
    setMotor(dir,pwr,pwm[k],in1[k],in2[k]); // signal the motor
  }

//  for(int i = 0; i<4; i++){
//    Serial.print(target[i]);
//    Serial.print(" ");
//  }

//unsigned long mil;
//mil = millis();
//if(mil - milseb >= 5){
//  q = posi[0] - posi0;
//  w = posi[1] - posi1;
//  s = posi[2] - posi2;
//
////if(ypos == posi0) posi0=0;
////if(xpos == posi1) posi1=0;
////if(thet == posi2) posi2=0;
////
//posi0 = posi[0];
//posi1 = posi[1];
//posi2 = posi[2];
//
//q = q * 2.95;
//w = w * 2.88;
//s = s * 2.97;
//
//
////double vv1 = (0.866*q) - (0.866*w) + (0*s);
////double vv2 = -(0.5*q) - (0.5*w) + (1*s);
////double vv3 = (5*q) + (5*w) + (5*s);
////
//double vv1 = ((q/0.866)/2) - ((w/0.866)/2) + (0*s);
//double vv2 = ((-(q*0.5) - (w*0.5) + (s))*0.68);
//double vv3 = (5*q) + (5*w) + (5*s);
//
// bun = ((-(q + w + s)) * 120) / 1256;
// thet = thet + bun;
// 
// ypos = (cos(degree(thet))*vv1) + (-sin(degree(thet))*vv2);
// xpos = (sin(degree(thet))*vv1) + (cos(degree(thet))*vv2);
//
////float thet = 90;
//                                                                                                                                                                           
////double vv1 = (0.866*q) - (0.866*w) + (0*s);
////double vv2 = -(0.5*q) - (0.5*w) + (1*s);
////double vv3 = -(q + w + s)/3;
////
////double bun = (q / 0.2) + (w / 0.2) + (s / 0.2);
////
////float ypos = vv1;
////float xpos = vv2;
////float thet = vv3;
//
//ypos = ypos + yy;
//xpos = xpos + xx;
////thet = thet + tt;
//
//
//
//
//
////  for(int i = 0; i<4; i++){
//    Serial.print(q);
//    Serial.print(" ");
//    Serial.print(w);
//    Serial.print(" ");
//    Serial.print(s);
//    Serial.print(" ");
//    Serial.print("I------I");
//    Serial.print(posi[0]);
//    Serial.print(" ");
//    Serial.print(posi[1]);
//    Serial.print(" ");
//    Serial.print(posi[2]);
//    Serial.print(" ");
//    Serial.print("I------I");
//    Serial.print(ypos);
//    Serial.print(" ");
//    Serial.print(xpos);
//    Serial.print(" ");
//    Serial.print(thet);
//    Serial.print(" ");
//    Serial.print("I------I");
//    Serial.print(bun);
////
////    String ay, ax, at;
////    ay = String(ypos);
////    ax = String(xpos);
////    at = String(thet);
//
////    Serial.print(ay + 'I');
////   // Serial2.print(" ");
////    Serial.print(ax + 'I');
////   // Serial2.print(" ");
////    Serial.print(at);
////    //Serial2.print(" ");
////    Serial.println();
//
////    Serial2.print(ay + 'I');
////   // Serial2.print(" ");
////    Serial2.print(ax + 'I');
////   // Serial2.print(" ");
////    Serial2.print(at);
////    //Serial2.print(" ");
////    Serial2.println();
//
////  String lo = ay + '+' + ax + '+' + at;
////  aa1 = lo.c_str();
////  Serial2.println(aa1);
//// // Serial2.println();
////  Serial.print(aa1);
//
//
//xx = xpos;
//yy = ypos;
////tt = thet;
//
////  }
//  Serial.println();
//
//
//  milseb = mil;
//}
//    Serial.print(pos[0]);
//    Serial.print(" ");
//    Serial.print(pos[1]);
//    Serial.print(" ");
//    Serial.print(pos[2]);
//    Serial.print(" || ");
//    Serial.print(target[0]);
//    Serial.print(" ");
//    Serial.print(target[1]);
//    Serial.print(" ");
//    Serial.print(target[2]);
//    Serial.println(" ");


  
}
