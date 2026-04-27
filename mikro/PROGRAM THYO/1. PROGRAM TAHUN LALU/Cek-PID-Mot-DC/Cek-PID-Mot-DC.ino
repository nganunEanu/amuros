const int enca[] = {2,20,3};
const int encb[] = {50,48,51};
const int pwm[] = {5,6,7};
const int in1[] = {38,46,43};
const int in2[] = {39,47,42};
unsigned long milseb = 0, milseb1;
int pos = 0, pwr, target = -40, dir, posseb = 0;
#define m 2
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
        digitalWrite(in1[m],HIGH);
    digitalWrite(in2[m],LOW);
   // digitalWrite(pwm[m], HIGH);
    for(int k = 0; k < 3; k++){
    pinMode(enca[k],INPUT_PULLUP);
    pinMode(encb[k],INPUT_PULLUP);
  }
  attachInterrupt(digitalPinToInterrupt(enca[m]),readEncoder<m>,RISING);
  

}

void loop() {
  analogWrite(pwm[m], HIGH);
  Serial.println("aa");
//  SetMOT(dir);
//  Serial.print("Power = ");
//  Serial.println(pwr);
  
  unsigned long mil;
  mil = millis();
  if(mil - milseb >= 200){
    
   // noInterrupts();
     
      pos = posi[m] - posseb;
      
      
      
      
    //  Serial.println(pwr);
   // interrupts();
      
//      Serial.print("POS = ");
//      Serial.println(pos);
      
      
      
      posseb = posi[m];
      milseb = mil;
    }
 //   PID(pos);

}

void PID(int inp){
  long op = millis();
  float DelT = ((float) (op - milseb1)) / (1000);
  milseb1 = op;
  float kp = 1, kd=0, ki=0.4;
  float eprev, eintg;

    int e = target - inp;
    e = abs(e);
    float dedt = (e-eprev);
    eintg += e ;

    float u = kp*e + kd*dedt + ki*eintg;
    eprev = e;

//    dir = 1;
//    if(u<0){dir = -1;}

    pwr = (int) abs(u);
    if (pwr >= 255) pwr = 254;

  Serial.print(inp);
  Serial.print(" ");
  Serial.print(e);
  Serial.print(" ");
  Serial.print(dedt);
  Serial.print(" ");
  Serial.print(eintg);
  Serial.print(" ");
  Serial.println(pwr);
  analogWrite(pwm[m], pwr);

  //return pwr;
  }

//  void SetMOT(int a){
//    if (a == 1){
//    digitalWrite(in1[m],HIGH);
//    digitalWrite(in2[m],LOW);
//      }
//
//    else if (a == -1){
//    digitalWrite(in1[m],LOW);
//    digitalWrite(in2[m],HIGH);
//      }
//
//    else {
//    digitalWrite(in1[m],LOW);
//    digitalWrite(in2[m],LOW);
//      }
//      analogWrite(pwm[m], pwr);
//    }
