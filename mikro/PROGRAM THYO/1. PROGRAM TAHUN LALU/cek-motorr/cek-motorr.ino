const int enca[] = {9,11,26};             //Encoder A
const int encb[] = {10,12,27};            //Encoder B
const int pwmm[] = {2,3,4};               //PWM Motor
const int in1[] = {23,21,14};             //Input B Driver Motor
const int in2[] = {22,20,15};             //Input A Driver Motor
int m=2 ;
void setup() {
  pinMode(in1[m], OUTPUT);
  pinMode(in2[m], OUTPUT);
  pinMode(pwmm[m], OUTPUT);

}

void loop() {
  digitalWrite(in1[m], 1);
   digitalWrite(in2[m], 0);

   for(int i=0; i<255; i++){
    analogWrite(pwmm[m], i);
    delay(30);
   }
   digitalWrite(pwmm[m], 0);
   delay(1000);

  digitalWrite(in1[m], 0);
   digitalWrite(in2[m], 1);

   for(int i=0; i<255; i++){
    analogWrite(pwmm[m], i);
    delay(30);
   }
   digitalWrite(pwmm[m], 0);
   delay(1000);   

}
