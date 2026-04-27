const int enca[] = {2,20,3};
const int encb[] = {50,48,51};
const int pwm[] = {5,6,7};
const int in1[] = {38,46,43};
const int in2[] = {39,47,42};
unsigned long milseb = 0;
int pos = 0;
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
    digitalWrite(pwm[m], HIGH);
    for(int k = 0; k < 3; k++){
    pinMode(enca[k],INPUT_PULLUP);
    pinMode(encb[k],INPUT_PULLUP);
  }
  attachInterrupt(digitalPinToInterrupt(enca[m]),readEncoder<m>,RISING);
  

}

void loop() {
  unsigned long mil;
  mil = millis();
  if(mil - milseb >= 200){
      pos = posi[m] - pos;
      Serial.println(pos);
      pos = posi[m];
      milseb = mil;
    }

}
