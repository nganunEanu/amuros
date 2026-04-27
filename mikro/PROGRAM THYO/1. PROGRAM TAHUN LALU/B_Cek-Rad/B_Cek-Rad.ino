const int enca[] = {2,20,3};
const int encb[] = {50,48,51};
const int pwm[] = {5,6,7};
const int in1[] = {38,46,43};
const int in2[] = {39,47,42};
float eprev, eintg;
unsigned long milseb = 0, milseb1;
int pos = 0, pwr, dir, posseb = 0;
float  c_rad=0 , target = 8.1;
#define m 1
volatile int posi[] = {0,0,0};
float phe =  2 * 3.14;
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
  digitalWrite(pwm[m], HIGH);

}

void loop() {
  
  
  unsigned long mil;
  mil = millis();
  if(mil - milseb >= 100){    
      pos = posi[m] - posseb;    
      
        c_rad = ((pos * phe * 10)/134.0) ;
        
      posseb = posi[m];
      milseb = mil;
    }
    PID(c_rad);
  //  PID(pos);
//Serial.println(c_rad);
}



void PID(float inp){
  long op = millis();
  float DelT = ((float) (op - milseb1)) / (1000);
  milseb1 = op;
  float kp = 0.8, kd=1, ki=0.2;
  

    float e = target - inp;
 //   if (e <= 0) e=0;
    float dedt = (e-eprev);
    eintg = eintg + e ;

    float u = kp*e + kd*dedt + ki*eintg;
    eprev = e;

    pwr = (int) abs(u);
    if (pwr <=0 ) pwr = 0;
    if (pwr >= 255) pwr = 254;

  Serial.print(target);
  Serial.print(" ");
  Serial.print(c_rad);
  Serial.print(" ");
  Serial.print(e);
  Serial.print(" ");
  Serial.print(dedt);
  Serial.print(" ");
//  Serial.print(eintg);
//  Serial.print(" ");
//  Serial.println(pwr);
  analogWrite(pwm[m], pwr);
  Serial.println("");

  //return pwr;
  }
