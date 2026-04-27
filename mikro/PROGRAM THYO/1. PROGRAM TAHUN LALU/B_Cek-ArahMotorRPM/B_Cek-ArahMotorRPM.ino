

/*__________________________UNTUK MELAKUKAN PUTARAN ARAH PADA MOTOR TINGGAL DIATUR PADA in1[m] dan in2[m], (PADA VOID SETUP)*/

const int enca[] = {9,11,26};             //Encoder A
const int encb[] = {10,12,27};            //Encoder B
const int pwmm[] = {2,3,4};               //PWM Motor
const int in1[] = {23,21,14};             //Input B Driver Motor
const int in2[] = {22,20,15};             //Input A Driver Motor
float eprev, eintg;
unsigned long milseb = 0, milseb1;
int pos = 1, pwr, target = 40, dir, posseb = 0;
float  c_rpm=0;
#define m 2                          // Pilih Urutan Motor
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
    //-----KONDISI HIGH DAN LOW DIBALIK UNTUK MEMUTAR ARAH!!!
    digitalWrite(in1[m],0);       // jika target positiv in1 = 1 || neg = 0
    digitalWrite(in2[m], 1);      // jika target positiv in1 = 0 || neg = 1
   // digitalWrite(pwm[m], HIGH);
    for(int k = 0; k < 3; k++){
    pinMode(enca[k],INPUT_PULLUP);
    pinMode(encb[k],INPUT_PULLUP);
  }
  attachInterrupt(digitalPinToInterrupt(enca[m]),readEncoder<m>,RISING);
 // digitalWrite(pwm[m], HIGH);

}

void loop() {
  
  
  unsigned long mil;
  mil = millis();
  if(mil - milseb >= 100){    
      pos = posi[m] - posseb;    
      Serial.println(posi[m]);
        c_rpm = ((pos *10 * 60)/134.0) ;
        
      posseb = posi[m];
      milseb = mil;
    }
    PID(c_rpm);
  //  PID(pos);
//Serial.println(c_rpm);
}



void PID(int inp){
  long op = millis();
  float DelT = ((float) (op - milseb1)) / (1000);
  milseb1 = op;
  float kp = 0.02, kd=0.1, ki=0.02;
  

    int e = target - inp;
 //   if (e <= 0) e=0;
    float dedt = (e-eprev);
    eintg = eintg + e ;

    float u = kp*e + kd*dedt + ki*eintg;
    eprev = e;

    pwr = (int) abs(u);
    if (pwr <=0 ) pwr = 0;
    if (pwr >= 255) pwr = 254;

//  Serial.print(target);
//  Serial.print(" ");
//  Serial.print(c_rpm);
//  Serial.print(" ");
//  Serial.print(e);
//  Serial.print(" ");
//  Serial.print(dedt);
//  Serial.print(" ");
// // Serial.print(eintg);
// // Serial.print(" ");
//  Serial.println(pwr);
  analogWrite(pwmm[m], pwr);

  //return pwr;
  }
