// Encoder output to ESP32 Interrupt pin. Tracks the pulse count.
#define EncA 25
#define EncB 26

volatile int encoderCounts = 0;
volatile int lastChannelAState = LOW;

unsigned long prevTime = 0;
unsigned long updateInterval = 1000;  // Update interval in milliseconds

void setup() {
  Serial.begin(9600);
  pinMode(EncA, INPUT_PULLUP);
  pinMode(EncB, INPUT_PULLUP);

  // Every time the pin goes high, this is a pulse
  attachInterrupt(digitalPinToInterrupt(EncA), right_wheel_pulse, CHANGE);
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - prevTime >= updateInterval) {
    int rpm = static_cast<int>((double)encoderCounts / 136.0 * 60.0 * 10.0);
    
    // Print RPM and reset encoder counts
    Serial.print("RPM: ");
    Serial.println(rpm);

    encoderCounts = 0;
    prevTime = currentTime;
  }
  // Your other code here...
}

void right_wheel_pulse() {
  int channelAState = digitalRead(EncA);
  int channelBState = digitalRead(EncB);

  if (channelAState != lastChannelAState) {
    if (channelBState == channelAState) {
      encoderCounts++;
    } else {
      encoderCounts--;
    }
  }
  lastChannelAState = channelAState;  // Update lastChannelAState
}
