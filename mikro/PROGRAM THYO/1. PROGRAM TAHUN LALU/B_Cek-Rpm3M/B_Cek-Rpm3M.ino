const int enca[] = {2,20,3};
const int encb[] = {50,48,51};
const int pwm[] = {5,6,7};
const int in1[] = {38,46,43};
const int in2[] = {39,47,42};
float eprev[] = {0,0,0};
float eintg[] = {0,0,0};
unsigned long milseb = 0, milseb1;
int pos = 0, pwr, dir;
int posseb[] = {0,0,0};
float  c_rpm[]= {0,0,0};
const float ppr[] = {134.0 ,137.0 ,133.0};
int target[] = {60,60,80};
//#define m1 1
//#define m2 2
//#define m3 3
//#define m[] = [0,1,2];
const int m[] = {0,1,2};
volatile int posi[] = {0,0,0};

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
  // put your setup code here, to run once:

    
    Serial.begin(9600);
    delay(2000);

   // digitalWrite(pwm[m], HIGH);
    for(int k = 0; k < 3; k++){
    digitalWrite(in1[m[k]],HIGH);
    digitalWrite(in2[m[k]],LOW);
    pinMode(enca[k],INPUT_PULLUP);
    pinMode(encb[k],INPUT_PULLUP);
  }
 
  attachInterrupt(digitalPinToInterrupt(enca[m[0]]),readEncoder<0>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[m[1]]),readEncoder<1>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[m[2]]),readEncoder<2>,RISING);
 
 // digitalWrite(pwm[m], HIGH);

}

void loop() {
  
  
  unsigned long mil;
  mil = millis();
  if(mil - milseb >= 100){   

     for(int k=0; k<3; k++){
      noInterrupts();
      pos = posi[m[k]] - posseb[m[k]];    
      
        c_rpm[k] = ((pos *10 * 60)/ppr[k]) ;
   //     PID(c_rpm[k], k);
          

                
              
        
//        Serial.print(c_rpm[k]);
//        Serial.print(" ");
      posseb[m[k]] = posi[m[k]];
      interrupts();
     }

     
//     Serial.println("");
      milseb = mil;
    }
            for(int k=0; k<3; k++){
                PID(c_rpm[k], k);
                }
                Serial.println("");

    
  //  PID(pos);
//Serial.println(c_rpm);
}



void PID(int inp, int qw){
  long op = millis();
  float DelT = ((float) (op - milseb1)) / (1000);
  milseb1 = op;
  float kp = 0.8, kd=0.4, ki=0.05;
  

    int e = target[qw] - inp;
 //   if (e <= 0) e=0;
    float dedt = (e-eprev[qw]);
    eintg[qw] = eintg[qw] + e ;

    float u = kp*e + kd*dedt + ki*eintg[qw];
    eprev[qw] = e;

    pwr = (int) abs(u);
    if (pwr <=0 ) pwr = 0;
    if (pwr >= 255) pwr = 254;
analogWrite(pwm[m[qw]], pwr);
  Serial.print(target[qw]);
  Serial.print(" ");
  Serial.print(inp);
  Serial.print(" || ");
//  Serial.print(e);
//  Serial.print(" ");
//  Serial.print(dedt);
//  Serial.print(" ");
 // Serial.print(eintg);
 // Serial.print(" ");
//  Serial.println(pwr);
  

  //return pwr;
  }
