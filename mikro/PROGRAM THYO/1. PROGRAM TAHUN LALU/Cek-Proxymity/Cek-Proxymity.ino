const int prox1=18, prox2=19, RLY1=5, RLY2=6, RLY3=7, RLY4=8;;

void setup() {
  Serial.begin(9600);
  pinMode(prox1, INPUT);
  pinMode(prox2, INPUT);
  pinMode(RLY1, OUTPUT);
  pinMode(RLY2, OUTPUT);
  pinMode(RLY3, OUTPUT);  
  pinMode(RLY4, OUTPUT);

  digitalWrite(RLY1, LOW);
  digitalWrite(RLY2, LOW);
  digitalWrite(RLY3, HIGH);  
  // put your setup code here, to run once:

}

void loop() {
  int bacaProx1 = (digitalRead(prox1));
  int bacaProx2 = (digitalRead(prox2));

  if(bacaProx1==0 || bacaProx2==0){
    digitalWrite(RLY4, LOW);
  }else{
    digitalWrite(RLY4, HIGH);
  }
  delay(200);
}
