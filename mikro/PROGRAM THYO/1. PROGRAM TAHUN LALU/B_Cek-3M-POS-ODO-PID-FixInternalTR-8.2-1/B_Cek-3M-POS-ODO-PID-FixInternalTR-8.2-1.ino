//=============  Var ODOMETRY ==================

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
float xpos=0, ypos=0;

int tipe=0, cnt=0;
//=========== VAR MOTOR & CONTROL =============

//Urutan Array Motor [] = {Kiri, Kanan, Belakang}
const int enca[] = {9,11,26};             //Encoder A
const int encb[] = {10,12,27};            //Encoder B
const int pwmm[] = {2,3,4};               //PWM Motor
const int in1[] = {23,21,14};             //Input B Driver Motor
const int in2[] = {22,20,15};             //Input A Driver Motor

//=========== VAR RELAY & Penggiring =============
const int prox1=18, prox2=19, RLY1=5, RLY2=6, RLY3=7, RLY4=8; 
//RLY1 = 12V to Boost Converter       (Active High)
//RLY2 = Boost Converter to Capacitor (Active High)
//RLY3 = Capacitor to Solenoid        (Active Low)
//RLY4 = Motor penggiring             (Active LOW)

//====================   TENDANG BOLA =====================
int detikIsi=0,  tendangSelesai=0;


float eprev[] = {0,0,0};
float eintg[] = {0,0,0};
float a,b,c,d,e,f,g,h,i,mot0,mot1,mot2;
unsigned long milseb22 = 0, milseb11 = 0;
int pos = 0, pwr, dir, t=0;
int posseb[] = {0,0,0};
float  c_rpm[]= {0,0,0};
const float ppr[] = {133.0 ,137.0 ,133.0};
int target[] = {0,0,0};
const int m[] = {0,1,2};
volatile int posi[] = {0,0,0};
float Ypos = 0, Xpos = 0, Thet = 0;
float kp[] = {0.3,0.3,0.3}, kd[]={0.5,0.7,0.5}, ki[]={0.03,0.03,0.03};
float eeprevx, eeprevy, eeprevt;
float eeintgx, eeintgy, eeintgt;
String dataIn;
String dt[15];
boolean parsing=false;
int ubahRot = 0;
int modes =1;
int kinRot = 0;
int langkahPos=0, odometryStop=0, PIDodometry=0, detikIsi1=0;; 

float degree(double deg){  //function declaration
 return (deg*22)/(7*180.0);     //calculation for the radian to degree conversion
}

//=========== FUNGSI BACA ENCODER =============
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
    Serial.begin(9600);     //SERIAL MONITOR UTAMA
    Serial1.begin(9600);    //kOMUNIKASI SERIAL MINI PC TO MICROCONTROL
    delay(3000);

    pinMode(RLY1, OUTPUT);
    pinMode(RLY2, OUTPUT);
    pinMode(RLY3, OUTPUT);
    pinMode(RLY4, OUTPUT);
    pinMode(prox1, INPUT);
    pinMode(prox2, INPUT);  
    
    digitalWrite(RLY1, LOW);
    digitalWrite(RLY2, LOW);
    digitalWrite(RLY3, HIGH);
    digitalWrite(RLY4, HIGH);

    for(int k = 0; k < 3; k++){
      pinMode(enca[k],INPUT_PULLUP);
      pinMode(encb[k],INPUT_PULLUP);
    }
    for(int k = 0; k < 3; k++){
      pinMode(in1[m[k]],OUTPUT);
      pinMode(in2[m[k]],OUTPUT);
    }   
 
    attachInterrupt(digitalPinToInterrupt(enca[m[0]]),readEncoder<0>,RISING);
    attachInterrupt(digitalPinToInterrupt(enca[m[1]]),readEncoder<1>,RISING);
    attachInterrupt(digitalPinToInterrupt(enca[m[2]]),readEncoder<2>,RISING);

}

void loop() {
  
    if(Serial.available()>0)         
    {
      noInterrupts();
      terimadata();
      interrupts(); 
    }  
//    Serial.print(langkahPos);
//    Serial.print(',');
//    Serial.print(odometryStop);
//    Serial.print(',');
//    Serial.println(PIDodometry);
    
    //noInterrupts();
   if(langkahPos==1&&odometryStop==0)
    {
      if(PIDodometry==0){
        PIDpos(0,0,0,4);    //Memanggil fungsi PID Posisi dengan Koreksi ODOMETRY
      }
      else if(PIDodometry==1){
        PIDpos(0,0,0,4);    //Memanggil fungsi PID Posisi dengan Koreksi ODOMETRY
      }      
    }    
//    if(modes==2)
//    {
//      PIDpos(0,0,0,1.3);    //Memanggil fungsi PID Posisi dengan Koreksi ODOMETRY
//    }

    //interrupts();  
    //kinematikManual(0,400,0,0.5);
    //Serial.println(' ');
     
//            if(dones==0)PIDpos(1000, 0, 180, 1.2);
//            if(dones==1)PIDpos(1000, 500, 0, 1.2);
//            if(dones==2)PIDpos(0, 0, 90, 1.2);
    
                  

    
    
    
//=============   ODOMETRY    ==============
    
    unsigned long mil, mil1, mil2;
    mil = millis();
    mil1 = millis();    
    mil2 = millis();
    
    if(mil2 - milseb >= 100)
    {
      float fa = 0.7;                                 //Variable Pengali yang didapat dari Trial n error terhadap perhitungan odometry dalam milimeter    
      for(int k=0; k<3; k++)
      {
        noInterrupts();
        pos = posi[m[k]] - posseb[m[k]];               
        c_rpm[k] = ((pos * 10 * 60)/ppr[k]);          //(Perhitungan Encoder(100ms) * 10(variable 100ms * 10 = 1 detik) * 60(RPS to RPM)  / Pulse Per Revolusi)
        posseb[m[k]] = posi[m[k]];
            
      }
      
      q = posi[0] - posi0;
      w = posi[1] - posi1;
      s = posi[2] - posi2;
      interrupts();  

      cnt++;
      if(cnt>10){
          Serial.println(tipe);
          tendang(tipe);
          cnt=0;
        }
      penggiring();                                   //Mengaktifkan penggiring
      Serial.print(cnt);
      Serial.print(' ');
      Serial.println(detikIsi);         
      q = (q * 2.95) * fa;                            //nilai 2.95 didapat dari (keiling lingkaran  / pulse per revolusi), 1 pulse = 2.95mm 
      w = (w * 2.88) * fa;                            //nilai 2.88 didapat dari (keiling lingkaran  / pulse per revolusi), 1 pulse = 2.88mm 
      s = (s * 2.97) * fa;                            //nilai 2.97 didapat dari (keiling lingkaran  / pulse per revolusi), 1 pulse = 2.97mm 
                                                      //Dalam Kenyataannya PPR tiap Motor bisa berbeda-beda harus menyesuaikan dengan kondisi yang nyata      
      posi0 = posi[0];
      posi1 = posi[1];
      posi2 = posi[2];
      
//=============   Forward Kinematik Odometry    ==============

      double vv1 = (0.866*q) - (0.866*w) + (0*s);
      double vv2 = -(0.5*q) - (0.5*w) + (1*s);
      double vv3 = -(q + w + s)/3;  

//=============   Matrix Rotasi Odometry    ==============   
     
      double bun = ((-((q/0.7) + (w/0.7) + (s/0.7))) * 120) / 1256;      
      thet = thet + bun;                  
      ypos = (cos(degree(thet))*vv1) + (-sin(degree(thet))*vv2);
      xpos = (sin(degree(thet))*vv1) + (cos(degree(thet))*vv2);
      
           
      ypos = ypos + yy;
      xpos = xpos + xx;      
      
      Ypos = ypos;
      Xpos = xpos;
      Thet = thet;
            
      Serial.print("Posisi = ");
      Serial.print(Ypos);
      Serial.print(" ");
      Serial.print(Xpos);
      Serial.print(" ");
      Serial.print(Thet);
      Serial.print(" ");
                       
      xx = xpos;
      yy = ypos;

      
      Serial.println();
        

      for(int k=0; k<3; k++)
      {
        PID(c_rpm[k], k);
      }
  
      //Serial.println(""); 
      milseb = mil2;
    }
        
}

//=============   Fungsi PID RPM Motor    ==============   
void PID(int inp, int qw){

    if(target[qw]>0)
    {
      digitalWrite(in1[m[qw]],HIGH);
      digitalWrite(in2[m[qw]],LOW);    
    }
    
    else if(target[qw]<0)
    {
      digitalWrite(in1[m[qw]],LOW);
      digitalWrite(in2[m[qw]],HIGH);      
    }
    else
    {
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
  
    if(target[qw]>0)
    {
      if(target[qw]<inp-30)
      {
        pwr = 0;    
      }
    }
    
    if(target[qw]<0)
    {
      if(target[qw]>inp+30)
      {
        pwr = 0;      
      }
    }
      
    if(target[qw]==0){analogWrite(pwmm[m[qw]], 0);}   
    else {analogWrite(pwmm[m[qw]], pwr);}
//    Serial.print(target[qw]);
//    Serial.print(',');
//    Serial.print(inp);
//    Serial.print(',');
    
}

void targetManual(float mott0, float mott1, float mott2){
  kinRot=0;

//      x = (x / sec);
//      y = (y / sec);
//      //thet
//
//    float  x1 = (x/376.8)*60;
//    float  y1 = (y/376.8)*60;
//    float tt1 = ((((thet * 1256)/120)*3)*0.7);
//   //   float tt1 = thet * 15;
//    float  x1 = x * spd;
//    float  y1 = y * spd;

//      a = -0.33;  b = 0.577;   c = 0.33;
//      d = -0.33;  e = -0.577;  f = 0.33;
//      g = 0.67;   h = 0;       i = 0.33;


//float Ypos = (cos(degree(Thet))*y) + (sin(degree(Thet))*x);
//float Xpos = (-sin(degree(Thet))*y) + (cos(degree(Thet))*x);

        mot0 = mott0;
        mot1 = mott1;
        mot2 = mott2;

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
         
//        Serial.print(target[0]);
//        Serial.print(" ");
//        Serial.print(target[1]);
//        Serial.print(" ");
//        Serial.print(target[2]);
//        Serial.println(" ");
  
  }

//=============   kinematik Rot sebagai percobaan tanpa matrix rotasi.    ==============     
void kinematikNoRot(float x, float y, float thet, float spd){
  kinRot=1;
//      x = (x / sec);
//      y = (y / sec);
//    float  x1 = (x/376.8)*60;
//    float  y1 = (y/376.8)*60;
    float tt1 = ((((thet * 1256)/120)*3)*0.7);
   //   float tt1 = thet * 15;
    float  Xpos = x ;
    float  Ypos = y ;

      a = -0.33;  b = 0.577;   c = 0.33;
      d = -0.33;  e = -0.577;  f = 0.33;
      g = 0.67;   h = 0;       i = 0.33;

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

//        Serial.print("Target = ");
//        Serial.print(target[0]);
//        Serial.print(" ");
//        Serial.print(target[1]);
//        Serial.print(" ");
//        Serial.print(target[2]);
//        Serial.println(" ");
        
//        Serial.print("Target RPM = ");
//          for(int qw=0; qw<3; qw++){
//               if(target[qw]>0){
//                digitalWrite(in1[m[qw]],HIGH);
//                digitalWrite(in2[m[qw]],LOW);    
//                }
//            
//               else if(target[qw]<0){
//                digitalWrite(in1[m[qw]],LOW);
//                digitalWrite(in2[m[qw]],HIGH);      
//                }
//               else{
//                digitalWrite(in1[m[qw]],LOW);
//                digitalWrite(in2[m[qw]],LOW);     
//                } 
//                pwr = (int) abs(target[qw]);
//                if (pwr <=0 ) pwr = 0;
//                if (pwr >= 255){ pwr = 254;}                
//                analogWrite(pwm[m[qw]], pwr); 
//        Serial.print(pwr);                
//          }  
//
//
//        Serial.println(" ");  
        
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
         
//        Serial.print(target[0]);
//        Serial.print(" ");
//        Serial.print(target[1]);
//        Serial.print(" ");
//        Serial.print(target[2]);
//        Serial.println(" ");
  
  }
  

void kinematikManual(float x, float y, float thet, float spd){
  kinRot=0;

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
        
float variableMax = 1.00;
float maxRPM = 130.00;

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
    if(PIDodometry==0){
      if(errx<150 && errx>-150 && erry<150 && erry>-150 && errthet<12 && errthet>-12 ){
          kinematik(0, 0, 0, 0);
          //delay(2000);
          modes=1;
          dones=1;
          PIDodometry=1;
        }
        else{
          if(errx>400 && erry>400 && errx>0 && erry>0 ){
          //  Serial.println("non");
            kinematik(ux, uy, uthet, speedd);
          }
          else {
          //    Serial.println("yes");
            kinematik(ux, uy, uthet, speedd);}    
      } 
    }
    if(PIDodometry==1){     
      if(errx<60 && errx>-60 && erry<60 && erry>-60 && errthet<12 && errthet>-12 ){
          kinematik(0, 0, 0, 0);
          //delay(2000);
          modes=1;
          dones=1;
          PIDodometry=2;
          odometryStop=1;          
        }
        else{
          if(errx>400 && erry>400 && errx>0 && erry>0 ){
          //  Serial.println("non");
            kinematik(ux, uy, uthet, speedd);
          }
          else {
          //    Serial.println("yes");
            kinematik(ux, uy, uthet, speedd);}        
    }


  
  }
  
  }  

void tendang(int tipes){

  detikIsi1++;
  if(detikIsi1<6){
      digitalWrite(RLY1, HIGH);
      digitalWrite(RLY2, HIGH);      
  }
  else{
      digitalWrite(RLY1, LOW);
      digitalWrite(RLY2, LOW);     
  }

//========  tipe 5 untuk umpan  =============  
 if(tendangSelesai==0){ 
  if(tipes==5){
      detikIsi++;
//      digitalWrite(RLY1, HIGH);
//      digitalWrite(RLY2, HIGH);      
      if(detikIsi>1){
        digitalWrite(RLY1, LOW);
        digitalWrite(RLY2, LOW);
        digitalWrite(RLY3, LOW);
        tendangSelesai=1;
        detikIsi=0;
      }
      
    }


//========  tipe 6 untuk shooting  =============      
  else if(tipes==6){
      detikIsi++;
//      digitalWrite(RLY1, HIGH);
//      digitalWrite(RLY2, HIGH);      
      if(detikIsi>1){
        digitalWrite(RLY1, LOW);
        digitalWrite(RLY2, LOW);
        digitalWrite(RLY3, LOW);
        tendangSelesai=1;
        detikIsi=0;
      }        
    }
//    else{
//        digitalWrite(RLY1, LOW);
//        digitalWrite(RLY2, LOW);
//        digitalWrite(RLY3, HIGH); 
//        //detikIsi=0;             
//    }
  }else{
        digitalWrite(RLY1, LOW);
        digitalWrite(RLY2, LOW);
        digitalWrite(RLY3, HIGH); 
        //detikIsi=0;      
  }

if(tendangSelesai==1){
      detikIsi++;
      if(detikIsi>2){  
        digitalWrite(RLY1, LOW);
        digitalWrite(RLY2, LOW);
        digitalWrite(RLY3, HIGH); 
        detikIsi1=0;
        detikIsi=0; 
        tipes=0;
        tipe=0;
        tendangSelesai=0;
      }          
}
}

void penggiring(){
  int bacaProx1 = (digitalRead(prox1));
  int bacaProx2 = (digitalRead(prox2));

  if(bacaProx1==0 || bacaProx2==0){
    digitalWrite(RLY4, LOW);
  }else{
    digitalWrite(RLY4, HIGH);
  }  
}

void terimadata(){
  // ############### nextion ##########
  if(Serial.available()>0) 
  {
    char inChar = (char)Serial.read();
    Serial.print(inChar);
    dataIn += inChar; 
  //  Serial.print(dataIn);
    if (inChar == '}') 
    {
      parsing = true;
    }       
  } 
  if (parsing){
    parsingData();
    //Serial.flush();
    parsing = false;
    dataIn = "";  
  }
}

void parsingData()
{
  //dataMasuk = 1;
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
// ===============  Data Mengejar Bola   =========
if(odometryStop==1){
  if(mode==1 || mode==0){  
      kinematikManual(Xx, Yy, Tt, Spd1);
    }
  else if(mode==2){
     if(dones==0){
      PIDpos(Yy, Xx, Tt, Spd1);
      modes=2;
     }
    }
  if(mode==3){
      kinematikNoRot(Xx, Yy, Tt, Spd1);
    }  
}
//============  Data Tendang ===================
  if(mode==5){
      //tendangSelesai=0;
      tipe=5;
      kinematikManual(Xx, Yy, Tt, Spd1);
    } 
  else if(mode==6){
      //tendangSelesai=0;
      tipe=6;
      kinematikManual(Xx, Yy, Tt, Spd1);
    } 
  else{
      tipe=0;          
  }

//============  Data langkah Posisi ============
  if(mode==7){
      langkahPos=1;
      //tendangSelesai=0;
      //tipe=5;
      //kinematikManual(Xx, Yy, Tt, Spd1);
    }   
  if(mode==8){
      targetManual(Xx,Yy,Tt);
      //tendangSelesai=0;
      //tipe=5;
      //kinematikManual(Xx, Yy, Tt, Spd1);
    }     
  if(mode==15){
      langkahPos=0;
      odometryStop=0;
      PIDodometry=0;
      thet=0,  yy=0, xx=0, Ypos=0, Xpos=0, Thet=0, xpos=0, ypos=0;       
      kinematikManual(0, 0, 0, Spd1);
      //tendangSelesai=0;
      //tipe=5;
      //kinematikManual(Xx, Yy, Tt, Spd1);
    }     
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

     Serial.print("Mode = ");
     Serial.println(mode);
     Serial.print("Yy = ");
     Serial.println(Yy);
     Serial.print("Xx = ");
     Serial.println(Xx);
     Serial.print("Tt = ");
     Serial.println(Tt);
     Serial.print("Spd = ");
     Serial.println(Spd);     





}    
