
// Encoder output to Arduino Interrupt pin. Tracks the pulse count.




//  ================  COMPAS  ==========
#include <Wire.h>
#define ADDRESS 0x60
#define CMPS12_ADDRESS 0x60  // Address of CMPS12 shifted right one bit for arduino wire library
#define ANGLE_8  1           // Register to read 8bit angle from
unsigned char high_byte, low_byte, angle8;
char pitch, roll;
unsigned int angle16;
unsigned long milseb=0;

const int enca[] = {27, 33};
const int encb[] = {26, 32};
float c=0, e=0, d=0, o, f;
int posi0 = 0;
int posi1 = 0;
float Kiri=0, Kanan=0, xx=0, yy=0;

 
// Keep track of the number of right wheel pulses
volatile int posi[] = {0,0};

float degree(double deg){  //function declaration
 return (deg*22)/(7*180.0);     //calculation for the radian to degree conversion
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
 
void setup() {
 
  // Open the serial port at 9600 bps
  Serial.begin(9600); 
  Wire.begin();
  delay(2000);

  //  ================  COMPAS  ==========
  Wire.beginTransmission(CMPS12_ADDRESS);  //starts communication with CMPS12
  Wire.write(ANGLE_8);                     //Sends the register we wish to start reading from
  Wire.endTransmission();
  Wire.requestFrom(CMPS12_ADDRESS, 5);       
  
  while(Wire.available() < 5);        // Wait for all bytes to come back
  
  angle8 = Wire.read();               // Read back the 5 bytes
  high_byte = Wire.read();
  low_byte = Wire.read();
  pitch = Wire.read();
  roll = Wire.read();
  angle16 = high_byte;                 // Calculate 16 bit angle
  angle16 <<= 8;
  angle16 += low_byte;
  float u = (angle16 / 10);
  float i = (angle16 % 10);
  o = u + i;
  c=o;
   
  // Set pin states of the encoder
  for(int i=0; i<2; i++){
      pinMode(enca[i], INPUT_PULLUP);
    }
  for(int i=0; i<2; i++){
      pinMode(encb[i], INPUT_PULLUP);
    }  

 
  // Every time the pin goes high, this is a pulse
  attachInterrupt(digitalPinToInterrupt(enca[0]), readEncoder<0>, RISING);
  attachInterrupt(digitalPinToInterrupt(enca[1]), readEncoder<1>, RISING);
   
}
 
void loop() {
  
//    Serial.print(" Pulses right : ");
//    Serial.print(posi[0]); 
//    Serial.print(" Pulses left : "); 
//    Serial.println(posi[1]); 
  
    if(millis() - milseb >150){
      noInterrupts();
        Wire.beginTransmission(CMPS12_ADDRESS);  //starts communication with CMPS12
        Wire.write(ANGLE_8);                     //Sends the register we wish to start reading from
        Wire.endTransmission();
       
        // Request 5 bytes from the CMPS12
        // this will give us the 8 bit bearing, 
        // both bytes of the 16 bit bearing, pitch and roll
        Wire.requestFrom(CMPS12_ADDRESS, 5);       
        
        while(Wire.available() < 5);        // Wait for all bytes to come back
        
        angle8 = Wire.read();               // Read back the 5 bytes
        high_byte = Wire.read();
        low_byte = Wire.read();
        pitch = Wire.read();
        roll = Wire.read();
        
        angle16 = high_byte;                 // Calculate 16 bit angle
        angle16 <<= 8;
        angle16 += low_byte;
          
      
      
        float u = (angle16 / 10);
        float i = (angle16 % 10);
        o = u + i;
       // if(o>360)o=360;
        
//        Serial.print(" angle full: ");     // Display 16 bit angle with decimal place
//        Serial.print(o);
//        Serial.print(" === ");
        
        d=o-c;
        if(d>300){
          if(o>360){d=-((o-360)+c);}
          else {d=-(360-d);}
          }
        if(d<-300){
          if(o>360){d=c+d;}
          else {d=360+d;}
          }
      
          
   //     Serial.print(d);
        f=d+e;  
//        Serial.print("  angle alias:  "); 
//        Serial.println(f);
        //==== MEMORY SEMENTARA ===
        c=o;
        e=f;
  
        Odometry();

        milseb = millis();
      }
}
 

void Odometry(){
      Kanan = posi[0] - posi0;
      Kiri = posi[1] - posi1;
      interrupts();
      Kanan = (Kanan * 0.303);
      Kiri = (Kiri * 0.303);

      posi0 = posi[0];
      posi1 = posi[1];

      double vv1 = -(0.7*Kanan) - (0.7*Kiri);
      double vv2 =  (0.7*Kanan) - (0.7*Kiri);
      double vv3 = -((Kanan/0.1) + (Kiri/0.1)); 
      Serial.print(Kanan);
      Serial.print(' ');
      Serial.print(Kiri);
      
      float ypos = (cos(-degree(f))*vv1) + (-sin(-degree(f))*vv2);
      float xpos = (sin(-degree(f))*vv1) + (cos(-degree(f))*vv2); 
      ypos = ypos + yy;
      xpos = xpos + xx;    

    Serial.print("  Posisi = ");
    Serial.print(ypos);
    Serial.print(" ");
    Serial.print(xpos);
    Serial.print(" ");
    Serial.print(f);
    Serial.println(" ");      

      xx = xpos;
      yy = ypos;      
      
    }

 
