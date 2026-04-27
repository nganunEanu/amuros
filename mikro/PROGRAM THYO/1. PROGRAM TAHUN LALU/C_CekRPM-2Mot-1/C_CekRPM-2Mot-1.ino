
// Encoder output to Arduino Interrupt pin. Tracks the pulse count.


const int enca[] = {27, 33};
const int encb[] = {26, 32};

int posi0 = 0;
int posi1 = 0;
float Kiri=0, Kanan=0;

unsigned long milseb = 0;
 
// Keep track of the number of right wheel pulses
volatile int posi[] = {0,0};

template <int j>
void readEncoder(){
  int b = digitalRead(encb[j]);
  if(b > 0){
    posi[j]++;
  
  }
  else{
    posi[j]--;
   
  }
}
 
void setup() {
 
  // Open the serial port at 9600 bps
  Serial.begin(9600); 
 
  // Set pin states of the encoder
  for(int i=0; i<2; i++){
      pinMode(enca[i], INPUT_PULLUP);
    }
  for(int i=0; i<2; i++){
      pinMode(encb[i], INPUT_PULLUP);
    }  

 
  // Every time the pin goes high, this is a pulse
  attachInterrupt(digitalPinToInterrupt(enca[0]), readEncoder<0>, RISING);
  attachInterrupt(digitalPinToInterrupt(enca[1]), readEncoder<1>, RISING);
   
}
 
void loop() {
  
    Serial.print(" Pulses right : ");
    Serial.print(posi[0]); 
    Serial.print(" Pulses left : "); 
    Serial.println(posi[1]); 

    if(millis() - milseb >100){
      noInterrupts();
        Odometry();
        
      }
}
 

void Odometry(){
      Kanan = posi[0] - posi0;
      Kiri = posi[1] - posi1;
      interrupts();
      Kanan = (Kanan * 0.303);
      Kiri = (Kiri * 0.303);

      posi0 = posi[0];
      posi1 = posi[1];

      double vv1 = (0.7*Kanan) + (0.7*Kiri);
      double vv2 = -(0.7*Kanan) - (0.7*Kiri);
      double vv3 = -((Kanan/0.1) + (Kiri/0.1));      
      
    }

float degree(double deg){  //function declaration
       return (deg*22)/(7*180.0);     //calculation for the radian to degree conversion
    }    
