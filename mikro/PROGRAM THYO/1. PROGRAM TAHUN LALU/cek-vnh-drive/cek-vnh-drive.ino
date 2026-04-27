const int enca[] = {2,20,3};
const int encb[] = {50,48,51};
const int pwm[] = {5,6,7};
const int in1[] = {38,46,43};
const int in2[] = {39,47,42};
#define m 0   
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
    Serial.begin(9600);
    delay(2000);
    
    //-----KONDISI HIGH DAN LOW DIBALIK UNTUK MEMUTAR ARAH!!!
//    digitalWrite(in1[m],0);       // jika target positiv in1 = 1 || neg = 0
//    digitalWrite(in2[m], 1);      // jika target positiv in1 = 0 || neg = 1
    for(int k = 0; k < 3; k++){
    pinMode(enca[k],INPUT_PULLUP);
    pinMode(encb[k],INPUT_PULLUP);
    }
    attachInterrupt(digitalPinToInterrupt(enca[m]),readEncoder<m>,RISING);
    
  }


void loop() {
  Serial.println(posi[m]);
  //CW
  
    digitalWrite(in1[m],0);       // jika target positiv in1 = 1 || neg = 0
    digitalWrite(in2[m], 1);      // jika target positiv in1 = 0 || neg = 1

    for(int i=0; i<255; i++){
      analogWrite(pwm[m], i);
      delay(20);
    }
    digitalWrite(pwm[m], 0);
    delay(1000);

  //CCW
  
    digitalWrite(in1[m],1);       // jika target positiv in1 = 1 || neg = 0
    digitalWrite(in2[m], 0);      // jika target positiv in1 = 0 || neg = 1

    for(int i=0; i<255; i++){
      analogWrite(pwm[m], i);
      delay(20);
    }
    digitalWrite(pwm[m], 0);
    delay(1000);
        
}
