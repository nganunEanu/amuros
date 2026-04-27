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
double pid;

const float Kp = 0.2; // Parameter Kp  
const float Ki = 0.0003;   // Parameter Ki 
const float Kd = 0.002;   // Parameter Kd  

double currTime;
double deltaTime;
double prevTime;
double p;
double i;
double d;

int rpm;
int targetRPM = 30;

// Variabel Baca Encoder 
volatile int encoderCounts = 0;
volatile int lastChannelAState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(EncA, INPUT_PULLUP);
  pinMode(EncB, INPUT_PULLUP);
  pinMode(PWM_Ch, OUTPUT);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(EncA), encoderISR, CHANGE);

  // Setup PWM ESP32
  ledcAttachPin(GPIO_pin, PWM_Ch);
  ledcSetup(PWM_Ch, PWM_Freq, PWM_Res);
}

void loop() {
  rpm = static_cast<int>((double)encoderCounts / 270.0 * 60.0 * 10.0);
  PID();

  // Print additional debugging information
  Serial.print("Encoder Counts: ");
  Serial.print(encoderCounts);
  encoderCounts = 0;  // Move this line after calculating rpm
  Serial.print(", RPM: ");
  Serial.print(rpm);
  // delay(10);

  digitalWrite(INA, HIGH);
  digitalWrite(INB, LOW);
  ledcWrite(PWM_Ch, dutyCycle);

  Serial.print(", PID: ");
  Serial.print(pid);
  Serial.print(", error: ");
  Serial.print(error);
  Serial.print(", Duty Cycle: ");
  Serial.println(dutyCycle);
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
  pid = p + i + d;
//  if (pid > 50){
//    pid=50;
//  }
//  else if (pid <-50){
//    pid= -50;
//  }
  dutyCycle = constrain(dutyCycle =+ pid, 0, 255);
}

void encoderISR() {
  int channelAState = digitalRead(EncA);  // Use EncA instead of 25
  int channelBState = digitalRead(EncB);  // Use EncB instead of 26

  if (channelAState != lastChannelAState) {
    if (channelBState == channelAState) {
      encoderCounts++;
    } else {
      encoderCounts--;
    }
  }
  lastChannelAState = channelAState;  // Update lastChannelAState
}
