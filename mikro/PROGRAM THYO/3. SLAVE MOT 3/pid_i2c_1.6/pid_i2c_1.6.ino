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
double dutyCycle = 0;

// Setting PID
double error = 0;
double prevError = 0;
double integral = 0;
double derivative = 0;
double pid;
const double Kp = 0.2; // Parameter Kp  
const double Ki = 0.0001; // Parameter Ki 
const double Kd = 30; // Parameter Kd  
unsigned long currTime;
unsigned long prevTime;
unsigned long waktusek;
double p;
double i;
double d;
int waktuseb = 0;
int interval = 100;

#include <Wire.h>
volatile long pos = 0;
volatile long target = 0;
volatile int rpm;
int pwm;

// Variabel Baca Encoder 
volatile int encoderCounts = 0;
volatile int lastChannelAState = LOW;
int PPR = 135;

void sendLong(long value) {
  for (int k = 0; k < 4; k++) {
    byte out = (value >> 8 * (3 - k)) & 0xFF;
    Wire.write(out);
  }
}

long receiveLong() {
  long outValue;
  for (int k = 0; k < 4; k++) {
    byte nextByte = Wire.read();
    outValue = (outValue << 8) | nextByte;
  }
  return outValue;
}

void requestEvent() {
  sendLong(pos); // fungsi ini baru dikirim saat master meminta
}

void receiveEvent(int howMany) {
  target = receiveLong(); // fungsi ini akan menerima saat master mengirim data
  Serial.print(target);
}

void setup() {
  Serial.begin(9600);
  pinMode(EncA, INPUT_PULLUP);
  pinMode(EncB, INPUT_PULLUP);
  pinMode(PWM_Ch, OUTPUT);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(EncA), encoderISR, CHANGE);
  Wire.begin(3); // join I2C
  Wire.onRequest(requestEvent); 
  Wire.onReceive(receiveEvent);

  // Setup PWM ESP32
  ledcAttachPin(GPIO_pin, PWM_Ch);
  ledcSetup(PWM_Ch, PWM_Freq, PWM_Res);
}

void loop() {
  waktusek = millis();
  if (waktusek - waktuseb >= interval) {
    rpm = (encoderCounts / 360.0 * 60.0 * 10.0);
    PID();
//    Serial.print(", pulse: ");
//    Serial.print(encoderCounts);
    Serial.print(", rpm: ");
    Serial.print(rpm);
    Serial.print(", PID: ");
    Serial.print(pid);
    Serial.print(", target: ");
    Serial.print(target);
    waktuseb = millis(); // Update waktuseb to the current time
    encoderCounts = 0; // Move this line after calculating rpm
  }

    if (target < 0) {
    digitalWrite(INA, LOW);
    digitalWrite(INB, HIGH);
    ledcWrite(PWM_Ch, pwm);
  } else if (target > 0) {
    digitalWrite(INA, HIGH);
    digitalWrite(INB, LOW);
    ledcWrite(PWM_Ch, pwm);
  }
    else if (target==0){
    digitalWrite(INA, LOW);
    digitalWrite(INB, LOW);
    pid=0;
    dutyCycle=0;
  }
}

void PID() {
  currTime = millis();
  unsigned long deltaTime = currTime - prevTime;
  error = target - rpm;
  Serial.print(", error: ");
  Serial.print(error);
  integral += error * deltaTime;
  derivative = (error - prevError) / deltaTime;
  Serial.print(", derivative: ");
  Serial.print(derivative);
  
  p = Kp * error;
  i = Ki * integral;
  d = Kd * derivative;
  
  // Introduce saturation limits
  pid = p + i + d;
 
  Serial.print(", Duty Cycle: ");
  Serial.print(dutyCycle);
  dutyCycle += pid;
  
  
  if (dutyCycle > 255) {
    dutyCycle = 255;
  }
  if (dutyCycle < -255) {
    dutyCycle = -255 ;
  }

  pwm = abs(dutyCycle);
  Serial.print(", preverror: ");
  Serial.print(prevError); 
  prevError = error;
  prevTime = currTime;

  Serial.print(", pwm: ");
  Serial.print(pwm);
  Serial.print(", delta time: ");
  Serial.print(deltaTime);
  Serial.print(", p: ");
  Serial.print(p);
  Serial.print(", i: ");
  Serial.print(i);
  Serial.print(", d: ");
  Serial.println(d);

}

void encoderISR() {
  int channelAState = digitalRead(EncA);
  int channelBState = digitalRead(EncB);

  if (channelAState != lastChannelAState) {
    if (channelBState == channelAState) {
      encoderCounts++;
      pos++;
    } else {
      encoderCounts--;
      pos--;
    }
  }
  lastChannelAState = channelAState;
}
