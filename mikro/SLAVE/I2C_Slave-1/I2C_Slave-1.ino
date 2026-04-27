#include <Wire.h>
volatile long pos = 123;
volatile long target = 0;

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

void requestEvent(){
  sendLong(pos);      // fungsi ini baru dikirim saat master meminta
  
}

void receiveEvent(int howMany){
  target = receiveLong();   // fungsi ini akan menerima saat master mengirim data
  Serial.println(target);
}

void setup() {
  Serial.begin(9600);
  Wire.begin(1); // join I2C
  //Wire.onRequest(requestEvent); 
  Wire.onReceive(receiveEvent);  
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
