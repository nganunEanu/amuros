const int EncA = 25;
const int EncB = 26;
const int in1 = 18;
const int in2 = 19;
const int GPIO_pin = 23;

// Setting PWM ESP32
const int PWM_Freq = 5000;
const int PWM_Res = 8;
const int PWM_Ch = 0;
const int dutycycle = 50;

// Variabel PID
volatile long posi = 0;
float eprev, eintg;
unsigned long milseb = 0, milseb1;
int pos = 0, pwr, target = 60, dir, posseb = 0;
float  c_rpm=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(EncA, INPUT_PULLUP);
  pinMode(EncB, INPUT_PULLUP);
  pinMode(GPIO_pin, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(EncA), nilai_encoder, RISING);

  ledcAttachPin(GPIO_pin, PWM_Ch);
  ledcSetup(PWM_Ch, PWM_Freq, PWM_Res);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(posi);
  unsigned long mil;
  mil = millis();
  if(mil - milseb >= 100){    
      pos = posi - posseb;    
      
        c_rpm = ((pos *10 * 60)/134.0) ;
        
      posseb = posi;
      milseb = mil;
    }
    PID(c_rpm);
  
}

void PID(int inp){
  if(target>0){        
    digitalWrite(in1,HIGH);
    digitalWrite(in2, LOW);}
  else if(target<0){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);    
    }
  else{
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);    
    }
  long op = millis();
  float DelT = ((float) (op - milseb1)) / (1000);
  milseb1 = op;
  float kp = 0.02, kd=0, ki=0.02;
  

    int e = target - inp;
    float dedt = (e-eprev);
    eintg = eintg + e ;

    float u = kp*e + kd*dedt + ki*eintg;
    eprev = e;

    pwr = (int) abs(u);
    if (pwr <=0 ) pwr = 0;
    if (pwr >= 255) pwr = 254;

  Serial.print(target);
  Serial.print(" ");
  Serial.print(c_rpm);
  Serial.println(" ");
//  Serial.print(e);
//  Serial.print(" ");
//  Serial.print(dedt);
//  Serial.print(" ");
//  Serial.print(eintg);
//  Serial.print(" ");
//  Serial.println(pwr);
  ledcWrite(PWM_Ch, pwr);
}
void nilai_encoder(){
  int b = digitalRead(EncB);
  if(b>0){
    posi--;
  }
  else {
    posi++;
  }
}
