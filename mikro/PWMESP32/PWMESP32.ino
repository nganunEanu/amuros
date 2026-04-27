#define EncA 25
#define EncB 26
#define INA 18
#define INB 19
#define GPIO_pin 23

// Setting PWM ESP32
const int PWM_Freq = 1000;
const int PWM_Ch = 0;
const int PWM_Res = 8;
//int dutycycle = 200;

void setup() {
  // put your setup code here, to run once:
  pinMode(EncA, INPUT);
  pinMode(EncB, INPUT);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(GPIO_pin, OUTPUT);
  Serial.begin(115200);
  
  ledcAttachPin(GPIO_pin, PWM_Ch);
  ledcSetup(PWM_Ch, PWM_Freq, PWM_Res);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(INA, HIGH);
  digitalWrite(INB, LOW);
  for (int i=0; i<200; i++) 
  {
    ledcWrite(PWM_Ch, i);
    Serial.println(i);
    delay(100);
  }
  
  for (int i=0; i>200; i--) 
  {
    ledcWrite(PWM_Ch, i);
    Serial.println(i);
    delay(100);
  }
  
  digitalWrite(INA, LOW);
  digitalWrite(INB, HIGH);
  for (int i=0; i<200; i++) 
  {
    ledcWrite(PWM_Ch, i);
    Serial.println(i);
    delay(100);
  }
  for (int i=0; i>200; i--) 
  {
    ledcWrite(PWM_Ch, i);
    Serial.println(i);
    delay(100);
  }
}
