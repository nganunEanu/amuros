
// Encoder output to Arduino Interrupt pin. Tracks the pulse count.
#define ENC_IN_RIGHT_A1 27
#define ENC_IN_RIGHT_B1 26

#define ENC_IN_RIGHT_A2 33
#define ENC_IN_RIGHT_B2 32



 
// Keep track of the number of right wheel pulses
volatile int right_wheel_pulse_count = 0;
volatile int left_wheel_pulse_count = 0;
 
void setup() {
 
  // Open the serial port at 9600 bps
  Serial.begin(9600); 
 
  // Set pin states of the encoder
  pinMode(ENC_IN_RIGHT_A1 , INPUT_PULLUP);
  pinMode(ENC_IN_RIGHT_B1 , INPUT_PULLUP);

  pinMode(ENC_IN_RIGHT_A2 , INPUT_PULLUP);
  pinMode(ENC_IN_RIGHT_B2 , INPUT_PULLUP);  
 
  // Every time the pin goes high, this is a pulse
  attachInterrupt(digitalPinToInterrupt(ENC_IN_RIGHT_A1), right_wheel_pulse, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_IN_RIGHT_A2), left_wheel_pulse, RISING);
   
}
 
void loop() {
  
    Serial.print(" Pulses right : ");
    Serial.print(right_wheel_pulse_count); 
    Serial.print(" Pulses left : "); 
    Serial.println(left_wheel_pulse_count); 
}
 
// Increment the number of pulses by 1
void right_wheel_pulse() {
  
  int b1 = digitalRead(ENC_IN_RIGHT_B1);
//  Serial.println(b);
  if(b1>0)
  {
    right_wheel_pulse_count++;
  }
  else 
  {
    right_wheel_pulse_count--;
  }
}

void left_wheel_pulse() {
  
  int b2 = digitalRead(ENC_IN_RIGHT_B2);
//  Serial.println(b);
  if(b2>0)
  {
    left_wheel_pulse_count++;
  }
  else 
  {
    left_wheel_pulse_count--;
  }
}
