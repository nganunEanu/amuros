int EncA = 25;
int EncB = 26;
int lastChannelAState = LOW;  // Declare lastChannelAState as a global variable
volatile int encoderCounts = 0;  // Make encoderCounts volatile since it's modified in an ISR

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(EncA, INPUT_PULLUP);
  pinMode(EncB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EncA), encoderISR, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  
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
  Serial.println(encoderCounts);
}
