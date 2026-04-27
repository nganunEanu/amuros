// Pin Encoder dan Motor
const int EncA = 25;
const int EncB = 26;
const int INA = 18;
const int INB = 19;
const int GPIO_pin = 23;

// Setting PWM ESP32
const int PWM_Freq = 1000;
const int PWM_Res = 8;
const int PWM_Ch = 0;
int dutyCycle = 0;

// Setting PID
double error = 0;
double prevError = 0;
double integral = 0;
double derivative = 0;

const float Kp = 0.1; // Parameter Kp  
const float Ki = 0;    // Parameter Ki 
const float Kd = 0.001;    // Parameter Kd  

double currTime;
double deltaTime;
double prevTime;
double p;
double i;
double d;

int motorPWM = 0;
int rpm = 0;
int targetRPM = 30;

// Variabel Baca Encoder 
volatile int pulse = 0;
volatile int lastPulse = 0;

void setup(){
  Serial.begin(9600);
  pinMode(EncA, INPUT_PULLUP);
  pinMode(EncB, INPUT_PULLUP);
  pinMode(PWM_Ch, OUTPUT);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(EncA), nilai_encoder, RISING);
  
  // Setup PWM ESP32
  ledcAttachPin(GPIO_pin, PWM_Ch);
  ledcSetup(PWM_Ch, PWM_Freq, PWM_Res);
}

void loop(){
  rpm = static_cast<int>((double)pulse / 136.0 * 60.0 * 10.0);
  pulse = 0;
  PID();

  digitalWrite (INA, LOW);
  digitalWrite (INB, HIGH);
  ledcWrite(PWM_Ch, dutyCycle);

  Serial.print("RPM: ");
  Serial.print(rpm);
  Serial.print(", Target: ");
  Serial.println(targetRPM);
}

void PID() {
  currTime = millis();
  deltaTime = (double)(currTime - prevTime);
  error = targetRPM - rpm;
  integral += error * deltaTime;
  derivative = (error - prevError) / deltaTime;
  prevError = error;
  prevTime = currTime;

  p = Kp * error;
  i = Ki * integral;
  d = Kd * derivative;

  // Introduce saturation limits
  int pid = p + i + d;
  dutyCycle += constrain(pid, -10, 10);
  dutyCycle = constrain(dutyCycle, 0, 255);
}
  

void nilai_encoder(){
  int b = digitalRead(EncB);
  if(b>0){
    pulse--;
  }
  else {
    pulse++;
  }
  lastPulse = b;
}
