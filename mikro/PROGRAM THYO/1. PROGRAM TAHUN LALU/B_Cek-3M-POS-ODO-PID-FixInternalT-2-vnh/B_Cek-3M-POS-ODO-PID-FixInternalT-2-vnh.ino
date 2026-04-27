const int pwmm[] = {2,3,4};
const int in1[] = {22,20,15};
const int in2[] = {23,21,14};
 int p = 2;

void setup() {
  Serial.begin(9600);
  for(int o=0; o<3; o++){
  pinMode(pwmm[o], OUTPUT);
  pinMode(in1[o], OUTPUT);
  pinMode(in2[o], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
 for(int u=0; u<3; u++){
  digitalWrite(in1[u], HIGH);
  digitalWrite(in2[u], LOW);
 }
  for(int i=0; i<255; i++){
      analogWrite(pwmm[0], i);
      analogWrite(pwmm[1], i);
      analogWrite(pwmm[2], i);
      delay(50);
    }
//  analogWrite(pwmm[p], 0);  
  delay(1000);  
//  p++;
//  if(p>2){p=0;}
//  Serial.println(p);
}
