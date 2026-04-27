//ODOMETRY
int posi0 = 0;
int posi1 = 0;
int posi2 = 0;
long milseb = 0;
float q = 0;
float w = 0;
float s = 0, thet = 0;
float xx = 0,yy = 0,tt = 0;
const char* aa1;
long prevT = 0;
int posPrev[] = {0,0,0};
int dones=0;

//VAR MOTOR & CONTROL
const int enca[]   = {3,18,2};
const int encb[] = {51,50,48};
const int pwm[] = {5,6,7};
const int in1[] = {38,46,43};
const int in2[] = {39,47,42};
float eprev[] = {0,0,0};
float eintg[] = {0,0,0};
int pos = 0, pwr, dir, t=0;
int posseb[] = {0,0,0};
float  c_rpm[]= {0,0,0};
const float ppr[] = {133.0 ,137.0 ,133.0};
int target[] = {0,0,0};  
const int m[] = {0,1,2};
volatile int posi[] = {0,0,0};
float kp[] = {0.25,0.25,0.25}, kd[]={0.8,0.8,0.8}, ki[]={0.025,0.025,0.25};
float a,b,c,d,e,f,g,h,i,mot0,mot1,mot2;
unsigned long milseb22 = 0, milseb1 = 0;
int count = 0;
int inpt[] = {0,0,0};
float Ypos = 0, Xpos = 0, Thet = 0;
int kinRot = 0;
String dataIn;
String dt[15];
bool parsing=false;


float degree(double deg){  //function declaration
 return (deg*22)/(7*180.0);     //calculation for the radian to degree conversion
}

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
  Serial.begin(9600);
   delay(3000);

    for(int k = 0; k < 3; k++){
    pinMode(pwm[k], OUTPUT);
    pinMode(enca[k],INPUT_PULLUP);
    pinMode(encb[k],INPUT_PULLUP);
    pinMode(in1[k], OUTPUT);
    pinMode(in2[k], OUTPUT);
  }

  attachInterrupt(digitalPinToInterrupt(enca[m[0]]),readEncoder<0>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[m[1]]),readEncoder<1>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[m[2]]),readEncoder<2>,RISING);
}


void loop() {

bacaSerial();  
//kinematik(0,1,0,50);
//if (millis() - milseb>1000){
//      if(dones==0){
//        PIDpos(2000,0,0,0.9);
//      }
//      if(dones==1){
//        PIDpos(2000,500,90,0.9);
//      }       
//      if(dones==2){
//        PIDpos(0,0,0,0.9);
//      }        
////      count ++;
////      if(count < 4){
////        kinematikNoRot(0, 1, 0, 50);
////      }
////      else if(count == 4 || count<7){
////        kinematikNoRot(1, 0, 0, 50); 
////        }
////      else if(count == 8 || count<10){
////        kinematikNoRot(0, -1, 0, 50); 
////        }
////       else if(count == 11 || count<13){
////        kinematikNoRot(-1, 0, 0, 50); 
////        }
////        else{
////          kinematikNoRot(0, 0, 0, 50);
////        }
//      milseb = millis();  
//     }
     


  for(int k=0; k<3; k++){
      PID(c_rpm[k], k);
//      Serial.print(inpt[k]);
//      Serial.print(',');
//      Serial.print(target[k]);
//      Serial.print(',');
      }
      
    Serial.println();
 //delay();  

 //==ODOMETRY==
  unsigned long mil, mil1;
  mil = millis();
  mil1 = millis();

  unsigned long mil2;
mil2 = millis();
if(mil2 - milseb1 >= 100){

for(int k=0; k<3; k++){
      noInterrupts();
      pos = posi[m[k]] - posseb[m[k]];    
      
        c_rpm[k] = ((pos * 10 * 60)/ppr[k]) ;
   
      posseb[m[k]] = posi[m[k]];
      
     }
  q = posi[0] - posi0;
  w = posi[1] - posi1;
  s = posi[2] - posi2;
interrupts();

  
  float fa = 0.7;


q = (q * 2.95) * fa;
w = (w * 2.88) * fa;
s = (s * 2.97) * fa;

posi0 = posi[0];
posi1 = posi[1];
posi2 = posi[2];


double vv1 = (0.866*q) - (0.866*w) + (0*s);
double vv2 = -(0.5*q) - (0.5*w) + (1*s);
double vv3 = -(q + w + s)/3;

double bun = ((-((q/0.7) + (w/0.7) + (s/0.7))) * 120) / 1256;
thet = thet + bun;



float ypos = (cos(degree(thet))*vv1) + (-sin(degree(thet))*vv2);
float xpos = (sin(degree(thet))*vv1) + (cos(degree(thet))*vv2);


ypos = ypos + yy;
xpos = xpos + xx;
thet = thet + tt;


Ypos = ypos;
Xpos = xpos;
Thet = thet;



//    Serial.print(vv1);
//    Serial.print(" ");
//    Serial.print(vv2);
//    Serial.print(" ");
//    Serial.print(bun);
//    Serial.print(" ");
//    Serial.print("I------I");
//    Serial.print(posi[0]);
//    Serial.print(" ");
//    Serial.print(posi[1]);
//    Serial.print(" ");
//    Serial.print(posi[2]);
//    Serial.print(" ");
//    Serial.println("I------I");

//    Serial.println("Posisi = ");
//    Serial.print(Ypos);
//    Serial.print(" ");
//    Serial.print(Xpos);
//    Serial.print(" ");
//    Serial.print(Thet);
//    Serial.print(" ");
 


    String ay, ax, at;
    ay = String(ypos);
    ax = String(xpos);
    at = String(thet);

xx = xpos;
yy = ypos;
//tt = thet;

  milseb1 = mil2;
}

}

void PID(int inp, int qw){
  
               if(target[qw]>0){
                digitalWrite(in1[m[qw]],HIGH);
                digitalWrite(in2[m[qw]],LOW);    
                }
            
               else if(target[qw]<0){
                digitalWrite(in1[m[qw]],LOW);
                digitalWrite(in2[m[qw]],HIGH);      
                }
               else{
                digitalWrite(in1[m[qw]],LOW);
                digitalWrite(in2[m[qw]],LOW);     
                } 

    int e = target[qw] - inp; 
    float dedt = (e-eprev[qw]);
    eintg[qw] = eintg[qw] + e ;

if(target[qw]<0 && eintg[qw]>0){eintg[qw] = eintg[qw] * -1;}
if(target[qw]>0 && eintg[qw]<0){eintg[qw] = eintg[qw] * -1;}


    float u = kp[qw]*e + kd[qw]*dedt + ki[qw]*eintg[qw];
    eprev[qw] = e;

    pwr = (int) abs(u);
    if (pwr <=0 ) pwr = 0;
    if (pwr >= 255){ pwr = 254;}
    if (pwr > 100 && abs(eintg[qw]) > 1500){eintg[qw]=0; pwr=0;} 

if(target[qw]>0){
  if(target[qw]<inp-30){
      pwr = 0;

    }
  }

if(target[qw]<0){
  if(target[qw]>inp+30){
      pwr = 0;

    }
  }
inpt[qw]=inp;
  Serial.print(inp);
  Serial.print(',');
  Serial.print(target[qw]);
  Serial.print(',');
//  Serial.print(dedt);
//  Serial.print('=');
//  Serial.print(eintg[qw]);
//  Serial.print('=');
//  Serial.print(pwr);
//  Serial.println('}');

  if(target[qw]==0){analogWrite(pwm[m[qw]], 0);}   
else {analogWrite(pwm[m[qw]], pwr);}

//  Serial.print(posi[qw]);
//  Serial.print(',');
}

void kinematik(float x, float y, float thet, float spd){
    kinRot=0;
//      x = (x / sec);
//      y = (y / sec);
//      //thet
//
//    float  x1 = (x/376.8)*60;
//    float  y1 = (y/376.8)*60;
    float tt1 = ((((thet * 1256)/120)*3)*0.7);
   //   float tt1 = thet * 15;
    float  x1 = x * spd;
    float  y1 = y * spd;

      a = -0.33;  b = 0.577;   c = 0.33;
      d = -0.33;  e = -0.577;  f = 0.33;
      g = 0.67;   h = 0;       i = 0.33;

 float Ypos = (cos(degree(Thet))*y) + (sin(degree(Thet))*x);
 float Xpos = (-sin(degree(Thet))*y) + (cos(degree(Thet))*x);

        mot0 = (a*Xpos* spd + b*Ypos* spd  + c*tt1* spd);
        mot1 = (d*Xpos* spd + e*Ypos* spd  + f*tt1* spd);
        mot2 = (g*Xpos* spd + h*Ypos* spd  + i*tt1* spd);

float variableMax = 1.00;
float maxRPM = 100.00;

 target[0] = mot0;
 target[1] = mot1;
 target[2] = mot2;
int Target[] = {abs(target[0]), abs(target[1]), abs(target[2])};
 
         
if(Target[0]>maxRPM){
    if(Target[0]>=Target[1] && Target[0]>=Target[2]){
        variableMax = (maxRPM/Target[0]);
      }
  }
if(Target[1]>maxRPM){
    if(Target[1]>=Target[0] && Target[1]>=Target[2]){
        variableMax = (maxRPM/Target[1]);
      }
  }
if(Target[2]>maxRPM){
    if(Target[2]>=Target[1] && Target[2]>=Target[0]){
        variableMax = (maxRPM/Target[2]);
      }
  }   
  
target[0] = target[0] * variableMax;
target[1] = target[1] * variableMax;
target[2] = target[2] * variableMax;

 
        
  }

void kinematikManual(int x, int y, int thet, int spd=0){
    kinRot=0;
//      x = (x / sec);
//      y = (y / sec);
//      //thet
//
//    float  x1 = (x/376.8)*60;
//    float  y1 = (y/376.8)*60;
    float tt1 = ((((thet * 1256)/120)*3)*0.7);
   //   float tt1 = thet * 15;
    float  x1 = x * spd;
    float  y1 = y * spd;

      a = -0.33;  b = 0.577;   c = 0.33;
      d = -0.33;  e = -0.577;  f = 0.33;
      g = 0.67;   h = 0;       i = 0.33;

 float Ypos = (cos(degree(Thet))*y) + (sin(degree(Thet))*x);
 float Xpos = (-sin(degree(Thet))*y) + (cos(degree(Thet))*x);

        mot0 = (a*Xpos* spd + b*Ypos* spd  + c*tt1* spd);
        mot1 = (d*Xpos* spd + e*Ypos* spd  + f*tt1* spd);
        mot2 = (g*Xpos* spd + h*Ypos* spd  + i*tt1* spd);

float variableMax = 1.00;
float maxRPM = 100.00;

 target[0] = mot0;
 target[1] = mot1;
 target[2] = mot2;
int Target[] = {abs(target[0]), abs(target[1]), abs(target[2])};
 
         
if(Target[0]>maxRPM){
    if(Target[0]>=Target[1] && Target[0]>=Target[2]){
        variableMax = (maxRPM/Target[0]);
      }
  }
if(Target[1]>maxRPM){
    if(Target[1]>=Target[0] && Target[1]>=Target[2]){
        variableMax = (maxRPM/Target[1]);
      }
  }
if(Target[2]>maxRPM){
    if(Target[2]>=Target[1] && Target[2]>=Target[0]){
        variableMax = (maxRPM/Target[2]);
      }
  }   
  
target[0] = target[0] * variableMax;
target[1] = target[1] * variableMax;
target[2] = target[2] * variableMax;

 target[0] = x;
 target[1] = y;
 target[2] = thet;

 for(int i=0; i<3; i++){
  Serial.print(target[i]);
  Serial.print(' ');
 }
 Serial.println();
 
        
  }

void PIDpos(int y,int x, int thet, float speedd){
  float kpP = 0.05;
    
  int errx = x - Xpos;
  int erry = y - Ypos;
  int errthet = thet - Thet;
//Serial.print(errx);
//Serial.print(" - ");
//Serial.print(erry);
//Serial.print(" - ");
  float ux = kpP*errx;
  float uy = kpP*erry;
  float uthet = -(kpP*errthet);
//Serial.print(" Kinematik = ");
//Serial.print(ux);
//Serial.print(" - ");
//Serial.print(uy);
//Serial.print(" - ");
//Serial.print(uthet);
//Serial.print(" - ");


//if(x>0){
//     if(x<Xpos){
//        kinematik(0, 0, 0, 0);
//      }
//  }

Serial.println(dones);  
  if(errx<50 && errx>-50 && erry<50 && erry>-50 && errthet<12 && errthet>-12 ){
      kinematik(0, 0, 0, 0);
      dones++;
      
//      delay(2000);
//      dones=1;
//      modes=1;
//      job = 1;
//      if(job == 2) job =3;
//      if (job == 1)conjob =1;
      
      
    }

  else{
    if(errx>400 && erry>400 && errx>0 && erry>0 ){
    //  Serial.println("non");
      kinematik(ux, uy, 0, speedd);
    }
    else {
    //    Serial.println("yes");
      kinematik(ux, uy, uthet, speedd);}
  
  }
}  

void bacaSerial(){
  if(Serial.available()>0){
    char inChar = (char)Serial.read();
    dataIn+= inChar;
    if(inChar=='}'){
      parsing=true;
    }
  }

  if(parsing){
    parsingData();
    parsing=false;
    dataIn="";
  }
}

void parsingData(){

  int j=0;
  int i=0;
  
  dt[j]="";
  for (i=0;i<dataIn.length();i++) 
  {
    if (dataIn[i] == ',')
    {
      j++;
      dt[j]="";     
    //delay(10);
    }
    else
    {
      dt[j] = dt[j] + dataIn[i];
    }
  }
  // Konversi nilai data string menjadi integer
  int mode=dt[0].toInt();
  int Xx=dt[1].toInt();
  int Yy=dt[2].toInt();
  int Tt=dt[3].toInt(); 
  int Spd=dt[4].toInt();
  float Spd1 = Spd / 10.00;

  if(mode==1){
    kinematikManual(Xx, Yy, Tt);
  }
  
}
