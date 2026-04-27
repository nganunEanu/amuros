const int enca[] = {2,20,3};
const int encb[] = {50,48,51};
const int pwm[] = {5,6,7};
const int in1[] = {38,46,43};
const int in2[] = {39,47,42};
float eprev[] = {0,0,0};
float eintg[] = {0,0,0};
float a,b,c,d,e,f,g,h,i,mot0,mot1,mot2;
unsigned long milseb = 0, milseb1 = 0;
int pos = 0, pwr, dir, t=0;
int posseb[] = {0,0,0};
float  c_rpm[]= {0,0,0};
const float ppr[] = {134.0 ,137.0 ,133.0};
int target[] = {0,0,0};
//#define m1 1
//#define m2 2
//#define m3 3
//#define m[] = [0,1,2];
const int m[] = {0,1,2};
volatile int posi[] = {0,0,0};

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
  // put your setup code here, to run once:

    
    Serial.begin(9600);
    delay(2000);

   // digitalWrite(pwm[m], HIGH);
    for(int k = 0; k < 3; k++){

    pinMode(enca[k],INPUT_PULLUP);
    pinMode(encb[k],INPUT_PULLUP);
  }
 
  attachInterrupt(digitalPinToInterrupt(enca[m[0]]),readEncoder<0>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[m[1]]),readEncoder<1>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[m[2]]),readEncoder<2>,RISING);
 
 // digitalWrite(pwm[m], HIGH);

}

void loop() {
  
  
  unsigned long mil, mil1;
  mil = millis();
  mil1 = millis();
  if(mil1 - milseb1 >=1000){
      ///////////-------------------------------------KINEMATIKA-----------------------

          if(t<5) {kinematik(1500,0,0,4); if(t==4){kinematik(0,0,0,0); for(int k=0; k<3; k++){eintg[k]=0; eprev[k]=0; }}}
          else if(t<10) {kinematik(0,1500,0,4); if(t==9){kinematik(0,0,0,0); for(int k=0; k<3; k++){eintg[k]=0; eprev[k]=0;}}}
          else if(t<15) {kinematik(-1500,0,0,4); if(t==14){kinematik(0,0,0,0); for(int k=0; k<3; k++){eintg[k]=0; eprev[k]=0;}}}
          else if(t<20) {kinematik(0,-1500,0,4); if(t==19){kinematik(0,0,0,0); for(int k=0; k<3; k++){eintg[k]=0; eprev[k]=0;}}}
          else {kinematik(0,0,0,0);}
            for(int k=0; k<3; k++){
             
               if(target[k]>0){
                digitalWrite(in1[m[k]],HIGH);
                digitalWrite(in2[m[k]],LOW);    
                }
            
               else if(target[k]<0){
                digitalWrite(in1[m[k]],LOW);
                digitalWrite(in2[m[k]],HIGH);      
                }
               else{
                digitalWrite(in1[m[k]],LOW);
                digitalWrite(in2[m[k]],LOW);     
                } 
            }
          
      t++;
      milseb1 = mil1;
    }


  
  if(mil - milseb >= 100){   

     for(int k=0; k<3; k++){
      noInterrupts();
      pos = posi[m[k]] - posseb[m[k]];    
      
        c_rpm[k] = ((pos *10 * 60)/ppr[k]) ;
   //     PID(c_rpm[k], k);
          

                
              
        
//        Serial.print(c_rpm[k]);
//        Serial.print(" ");
      posseb[m[k]] = posi[m[k]];
      interrupts();
     }

     
//     Serial.println("");
      milseb = mil;
    }

    
            for(int k=0; k<3; k++){
                PID(c_rpm[k], k);
                }
                Serial.println("");


    
  //  PID(pos);
//Serial.println(c_rpm);
}



void PID(int inp, int qw){

 
  
//  long op = millis();
//  float DelT = ((float) (op - milseb1)) / (1000);
//  milseb1 = op;
  float kp = 0.3, kd=0.5, ki=0.26;
//    float kp = 0.8, kd=0.4, ki=0.08;

    int e = target[qw] - inp;
 //   if (e <= 0) e=0;
    float dedt = (e-eprev[qw]);
    eintg[qw] = eintg[qw] + e ;

    float u = kp*e + kd*dedt + ki*eintg[qw];
    eprev[qw] = e;

    pwr = (int) abs(u);
    if (pwr <=0 ) pwr = 0;
    if (pwr >= 255) pwr = 254;


if(target==0){analogWrite(pwm[m[qw]], 0);}   
else {analogWrite(pwm[m[qw]], pwr);}
  Serial.print(target[qw]);
  Serial.print(" ");
  Serial.print(inp);
  Serial.print(" ");
  Serial.print(e);
//  Serial.print(" ");
//  Serial.print(dedt);
  Serial.print(" ");
  Serial.print(eintg[qw]);
  Serial.print(" || ");
//  Serial.println(pwr);
  

  //return pwr;
  }

void kinematik(int x, int y, int thet, int sec){


      x = (x / sec);
      y = (y / sec);
      //thet

    float  x1 = (x/376.8)*60;
    float  y1 = (y/376.8)*60;

      a = -0.33;  b = 0.577;   c = 0.33;
      d = -0.33;  e = -0.577;  f = 0.33;
      g = 0.67;  h = 0;      i = 0.33;

        mot0 = (a*x1 + b*y1  + c*thet);
        mot1 = (d*x1 + e*y1  + f*thet);
        mot2 = (g*x1 + h*y1  + i*thet);

         target[0] = mot0;
         target[1] = mot1;
         target[2] = mot2;
         
//        Serial.print(target[0]);
//        Serial.print(" ");
//        Serial.print(target[1]);
//        Serial.print(" ");
//        Serial.print(target[2]);
//        Serial.println(" ");
  
  }
