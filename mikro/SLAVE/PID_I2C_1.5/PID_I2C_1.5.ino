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

const float Kp = 0.002; // Parameter Kp  
const float Ki = 0.00003; // Parameter Ki 
const float Kd = 0.0003; // Parameter Kd  

unsigned long currTime;
unsigned long prevTime;
double p;
double i;
double d;
int waktuseb = 0;
int interval = 100;

#include <Wire.h>
volatile long pos = 0;
volatile long target = 20;
volatile int rpm;

// Variabel Baca Encoder 
volatile int encoderCounts = 0;
volatile int lastChannelAState = LOW;

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
  //target = receiveLong(); // fungsi ini akan menerima saat master mengirim data
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
  //Wire.onRequest(requestEvent); 
  Wire.onReceive(receiveEvent);
  // put your setup code here, to run once:

  // Setup PWM ESP32
  ledcAttachPin(GPIO_pin, PWM_Ch);
  ledcSetup(PWM_Ch, PWM_Freq, PWM_Res);
}

void loop() {
  unsigned long waktusek = millis();
  if (waktusek - waktuseb >= interval) {
    rpm = static_cast<int>((double)encoderCounts / 270.0 * 60.0 * 10.0);
    PID();

    // Print additional debugging information
    // Serial.print("Encoder Counts: ");
    // Serial.print(encoderCounts);
    encoderCounts = 0; // Move this line after calculating rpm
    // Serial.print(", RPM: ");
    // Serial.print(rpm);
    // delay(10);
    waktuseb = waktusek; // Update waktuseb to the current time
  }

  if (target < 0) {
    digitalWrite(INA, LOW);
    digitalWrite(INB, HIGH);
    ledcWrite(PWM_Ch, dutyCycle);
  } else {
    digitalWrite(INA, HIGH);
    digitalWrite(INB, LOW);
    ledcWrite(PWM_Ch, dutyCycle);
  }

  Serial.print(", PID: ");
  Serial.print(pid);
  Serial.print(", error: ");
  Serial.print(error);
  Serial.print(", target: ");
  Serial.print(target);
  Serial.print(", rpm: ");
  Serial.print(rpm);
  Serial.print(", preverror: ");
  Serial.print(prevError);
  Serial.print(", pwm: ");
  Serial.print(pwm);
  Serial.print(", Duty Cycle: ");
  Serial.println(dutyCycle);
}

void PID() {
  currTime = millis();
  unsigned long deltaTime = currTime - prevTime;
  error = target - rpm;
  integral += error * deltaTime;
  derivative = (error - prevError) / deltaTime;
  prevError = error;
  Serial.print(", delta time: ");
  Serial.print(deltaTime);
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
  dutyCycle = constrain(dutyCycle += pid, 0, 255);
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
