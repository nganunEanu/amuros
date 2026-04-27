#include <SPI.h>
#include <Wire.h>



boolean RUN = false;

int halaman = 1;
int menuItem = 1;
int cr = 1;
float sp, e, e_temp, e_sum, x;
int temp, count = 0;


//Motor
volatile long pulse_count = 0;
int enc_res = 200;
int val;
int interval = 1000;

float rpm = 0;
double kp, ki, kd = 0;
double deltaT;
long prevT, currT = 0;
boolean directionA = true;

const int Lpwm = 52;
const int Rpwm = 53;
const int pwm = 5;
const int enc_a = 2;
const int enc_b = 3;



void setup() {
 
  Serial.begin(9600);

  /**************************************/
  //Motor
  pinMode(enc_a, INPUT_PULLUP);
  pinMode(enc_b, INPUT_PULLUP);
  pinMode(Lpwm, OUTPUT);
  pinMode(Rpwm, OUTPUT);
    digitalWrite(Lpwm, 0);
  digitalWrite(Rpwm, 1);
  attachInterrupt(digitalPinToInterrupt(enc_a), countPulse, RISING);
  sp= 80;
  kp=0.02;
  ki=0.02;
  kd=0.1;
   
}

void loop() {


  //**********************RPM**************************//
//
//  analogWrite(pwm, 0);


 currT = millis();
// deltaT = (double) (currT - prevT);

 //jika 1 detik terlewati hitung rpm
 if (currT - prevT > interval){
  
  prevT = currT;
  //Perhitungan nilai rpm
  rpm = (float) (pulse_count * 60 / enc_res);

//  Serial.print("Pulses: ");
//  Serial.println(pulse_count);
//  Serial.print("RPM: ");
//  Serial.println(rpm);
//  Serial.print("SP: ");
//  Serial.println(sp);
////  Serial.print("Error: ");
////  Serial.println(x);
//  Serial.println("");
  

  pulse_count = 0;
 }


// if (rpm != temp){
//    temp = rpm;
////    display.clearDisplay();
// }

 if (sp != 0){
    RUN = true;
    
  } 


 /******************************* PID*******************************/
  if (RUN == true){
    e = sp - rpm;
    x = e * kp + e_sum * ki + (e - e_temp) * kd;
    e_temp = e;
    e_sum += e;

    if (x < 255 && x > 0){
      analogWrite(pwm, x);
    
    } else if ( x > 255) {
      analogWrite(pwm, 255);
    
    } 
  }

delay(20);
//  Serial.print("RPM: ");
//  Serial.println(rpm);
//  Serial.print("SP: ");
//  Serial.println(sp);
//  Serial.print("Error: ");
//  Serial.println(x);
//  Serial.println("");

}
void countPulse(){
  int val = digitalRead(enc_b);

  if(val == 0){
    directionA = false; // CCW
  }
  else{
    directionA = true; // CW
  }

  if (directionA){
    pulse_count++;
  }
  else{
    pulse_count--;
  }
}
