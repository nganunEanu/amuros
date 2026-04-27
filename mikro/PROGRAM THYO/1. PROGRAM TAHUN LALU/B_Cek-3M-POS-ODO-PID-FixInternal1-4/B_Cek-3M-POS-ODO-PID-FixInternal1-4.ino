//Var ODOMETRY

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
const int enca[] = {2,20,3};
const int encb[] = {50,48,51};
const int pwm[] = {5,6,7};
const int in1[] = {38,46,43};
const int in2[] = {39,47,42};
float eprev[] = {0,0,0};
float eintg[] = {0,0,0};
float a,b,c,d,e,f,g,h,i,mot0,mot1,mot2;
unsigned long milseb22 = 0, milseb11 = 0;
int pos = 0, pwr, dir, t=0;
int posseb[] = {0,0,0};
float  c_rpm[]= {0,0,0};
const float ppr[] = {133.0 ,137.0 ,133.0};
int target[] = {0,0,0};
//#define m1 1
//#define m2 2
//#define m3 3
//#define m[] = [0,1,2];
const int m[] = {0,1,2};
volatile int posi[] = {0,0,0};
float Ypos = 0, Xpos = 0, Thet = 0;
float kp[] = {0.3,0.3,0.3}, kd[]={0.5,0.7,0.5}, ki[]={0.03,0.03,0.03};
int job = 0, conjob = 0;
float eeprevx, eeprevy, eeprevt;
float eeintgx, eeintgy, eeintgt;
String dataIn;
String dt[15];
boolean parsing=false;

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
      Serial1.begin(9600);
    delay(3000);

   // digitalWrite(pwm[m], HIGH);
    for(int k = 0; k < 3; k++){

    pinMode(enca[k],INPUT_PULLUP);
    pinMode(encb[k],INPUT_PULLUP);
  }
 
  attachInterrupt(digitalPinToInterrupt(enca[m[0]]),readEncoder<0>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[m[1]]),readEncoder<1>,RISING);
  attachInterrupt(digitalPinToInterrupt(enca[m[2]]),readEncoder<2>,RISING);

}

void loop() {

//  terimadata();
kinematikManual(0,6000,0,0.5);
Serial.println(' ');

//        if(dones==0)PIDpos(1000, 0, 90, 1.2);
//        if(dones==1)PIDpos(1000, 500, 0, 1.2);
//        if(dones==2)PIDpos(0, 0, 90, 1.2);

              
  unsigned long mil, mil1;
  mil = millis();
  mil1 = millis();



//=======ODOMETRY======

unsigned long mil2;
mil2 = millis();
if(mil2 - milseb >= 100){


for(int k=0; k<3; k++){
      noInterrupts();
      pos = posi[m[k]] - posseb[m[k]];    
      
        c_rpm[k] = ((pos * 10 * 60)/ppr[k]) ;
   //     PID(c_rpm[k], k);
          

                
              
        
//        Serial.print(c_rpm[k]);
//        Serial.print(" ");
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
//thet = thet + tt;


Ypos = ypos;
Xpos = xpos;
Thet = thet;


//  for(int i = 0; i<4; i++){
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
//    Serial.print("I------I");

    Serial.print("Posisi = ");
    Serial.print(Ypos);
    Serial.print(" ");
    Serial.print(Xpos);
    Serial.print(" ");
    Serial.print(Thet);
    Serial.print(" ");



    String ay, ax, at;
    ay = String(ypos);
    ax = String(xpos);
    at = String(thet);

xx = xpos;
yy = ypos;
//tt = thet;

//  }
  Serial.println();
  



//    if(dones==0)  {PIDpos(1000, 0, 90, 1.3);}
//    else  {PIDpos(0, 0, 0, 1.3);}

       
//          if(t<5) {kinematik(0,1500,0,4); if(t==4){kinematik(0,0,0,0); for(int k=0; k<3; k++){eintg[k]=0; eprev[k]=0; }}}
////          else if(t<10) {kinematik(0,1500,0,4); if(t==9){kinematik(0,0,0,0); for(int k=0; k<3; k++){eintg[k]=0; eprev[k]=0;}}}
////          else if(t<15) {kinematik(-1500,0,0,4); if(t==14){kinematik(0,0,0,0); for(int k=0; k<3; k++){eintg[k]=0; eprev[k]=0;}}}
////          else if(t<20) {kinematik(0,-1500,0,4); if(t==19){kinematik(0,0,0,0); for(int k=0; k<3; k++){eintg[k]=0; eprev[k]=0;}}}
//          else {kinematik(0,0,0,0);}
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

  milseb = mil2;
}

//============================================
            for(int k=0; k<3; k++){
                PID(c_rpm[k], k);
                }
        //        Serial.println(""); 


}

void PID(int inp, int qw){

 
  
//  long op = millis();
//  float DelT = ((float) (op - milseb1)) / (1000);
//  milseb1 = op;

//    float kp = 0.5, kd=0.5, ki=0.08;

    int e = target[qw] - inp;
 //   if (e <= 0) e=0;


 
    float dedt = (e-eprev[qw]);
    eintg[qw] = eintg[qw] + e ;

if(target[qw]<0 && eintg[qw]>0){eintg[qw] = eintg[qw] * -1;}
if(target[qw]>0 && eintg[qw]<0){eintg[qw] = eintg[qw] * -1;}



    float u = kp[qw]*e + kd[qw]*dedt + ki[qw]*eintg[qw];
    eprev[qw] = e;

    pwr = (int) abs(u);
    if (pwr <=0 ) pwr = 0;
    if (pwr >= 255){ pwr = 254;}

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
  
if(target[qw]==0){analogWrite(pwm[m[qw]], 0);}   
else {analogWrite(pwm[m[qw]], pwr);}
  Serial.print(target[qw]);
  Serial.print(',');
  Serial.print(inp);
  Serial.print(',');
//  Serial.print(e);
//  Serial.print(" ");
//  Serial.print(dedt);
////  Serial.print(" ");
////  Serial.print(eintg[qw]);
//  Serial.print(" ");
  Serial.print(pwr);
  Serial.print('}');
//  Serial.println(pwr);
  

  //return pwr;
  }

void kinematik(float x, float y, float thet, float spd){


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

void kinematikManual(float x, float y, float thet, float spd){


//      x = (x / sec);
//      y = (y / sec);
//      //thet
//
//    float  x1 = (x/376.8)*60;
//    float  y1 = (y/376.8)*60;
    float tt1 = ((((thet * 1256)/120)*3)*0.7);
   //   float tt1 = thet * 15;
//    float  Xpos = x * spd;
//    float  Ypos = y * spd;

      a = -0.33;  b = 0.577;   c = 0.33;
      d = -0.33;  e = -0.577;  f = 0.33;
      g = 0.67;   h = 0;       i = 0.33;


float Ypos = (cos(degree(thet))*y) + (sin(degree(thet))*x);
float Xpos = (-sin(degree(thet))*y) + (cos(degree(thet))*x);

        mot0 = (a*Xpos* spd + b*Ypos* spd  + c*tt1* spd);
        mot1 = (d*Xpos* spd + e*Ypos* spd  + f*tt1* spd);
        mot2 = (g*Xpos* spd + h*Ypos* spd  + i*tt1* spd);

         target[0] = mot0;
         target[1] = mot1;
         target[2] = mot2;

//        Serial.print("Target RPM = ");
//        Serial.print(target[0]);
//        Serial.print(" ");
//        Serial.print(target[1]);
//        Serial.print(" ");
//        Serial.print(target[2]);
//        Serial.println(" ");
  
  }  

void PIDpos(int y,int x, int thet, float speedd){
  float kpP = 0.07;
    
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

  
  if(errx<30 && errx>-30 && erry<30 && erry>-30 && errthet<12 && errthet>-12 ){
      kinematik(0, 0, 0, 0);
      //delay(2000);
      dones++;
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

void terimadata(){
  // ############### nextion ##########
  if(Serial.available()>0) 
  {
    char inChar = (char)Serial.read();
    dataIn += inChar; 
  //  Serial.print(dataIn);
    if (inChar == '}') 
    {
      parsing = true;
    }       
  } 
  if (parsing){
    parsingData();
    parsing = false;
    dataIn = "";  
  }
}

void parsingData()
{
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

//  float ttt = ((((Tt * 1256)/120)*3)*0.7);
  
  kinematikManual(Xx, Yy, Tt, Spd1);
//  String PP = (String)mode;
//  PP = PP + ',';
//  PP = PP + Yy;
//  PP = PP + ',';
//  PP = PP + Xx;
//  PP = PP + ',';
//  PP = PP + Tt;
//  PP = PP + ',';
//  PP = PP + Spd;
//  PP = PP + '}';
//  Serial.println(PP);
//  Serial1.println(PP);
//     Serial1.print("Mode = ");
//     Serial1.println(mode);
//     Serial1.print("Yy = ");
//     Serial1.println(Yy);
//     Serial1.print("Xx = ");
//     Serial1.println(Xx);
//     Serial1.print("Tt = ");
//     Serial1.println(Tt);
//     Serial1.print("Spd = ");
//     Serial1.println(Spd);     





}    
