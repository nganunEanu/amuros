const int RLY1=5, RLY2=6, RLY3=7, RLY4=8;

//RLY1 = 12V to Boost Converter       (Active High)
//RLY2 = Boost Converter to Capacitor (Active High)
//RLY3 = Capacitor to Solenoid        (Active Low)
//RLY4 = Motor penggiring             (Active LOW)

void setup() {
  pinMode(RLY1, OUTPUT);
  pinMode(RLY2, OUTPUT);
  pinMode(RLY3, OUTPUT);
  pinMode(RLY4, OUTPUT);

  digitalWrite(RLY1, LOW);
  digitalWrite(RLY2, LOW);
  digitalWrite(RLY3, HIGH);
  digitalWrite(RLY4, LOW);
  Serial.begin(9600);
}

void loop() {
  delay(5000);
  digitalWrite(RLY1, HIGH);
  digitalWrite(RLY2, HIGH);
  Serial.println('H');
  delay(4000);
  digitalWrite(RLY1, LOW);
  digitalWrite(RLY2, LOW);
  delay(500);
  digitalWrite(RLY3, LOW);
  delay(1000);
  digitalWrite(RLY3, HIGH);
  


//  digitalWrite(RLY1, HIGH);
//  Serial.println('H');
//  delay(4000);
//  digitalWrite(RLY1, LOW);
//  Serial.println('L');
//  delay(4000);  
  
//  digitalWrite(RLY2, HIGH);
//  Serial.println('H');
//  delay(2000);
//  digitalWrite(RLY2, LOW);
//  Serial.println('L');
//  delay(2000); 
//
//  digitalWrite(RLY3, HIGH);
//  Serial.println('H');
//  delay(2000);
//  digitalWrite(RLY3, LOW);
//  Serial.println('L');
//  delay(2000);  
//  
//  digitalWrite(RLY4, HIGH);
//  Serial.println('H');
//  delay(2000);
//  digitalWrite(RLY4, LOW);
//  Serial.println('L');
//  delay(2000);   
  

}
