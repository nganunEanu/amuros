//=============  Var ODOMETRY ==================

int posi0 = 0;
int posi1 = 0;
int posi2 = 0;
long milseb = 0;
float q = 0;
float w = 0;
float s = 0; 
float thet = 0;
float xx = 0, yy = 0, tt = 0;
//const char* aa1;
long prevT = 0;
int posPrev[] = {0, 0, 0};
int dones = 0;
float xpos = 0, ypos = 0;

float eeprevx, eeprevy, eeprevt;
float eeintgx, eeintgy, eeintgt;

int tipe = 0, cnt = 0;
int pos = 0;
volatile int posi[] = {0, 0, 0};
int posseb[] = {0, 0, 0};
float Ypos = 0, Xpos = 0, Thet = 0;
int langkahPos = 0, odometryStop = 0, PIDodometry = 0;

float degree(double deg) { //function declaration
  return (deg * 22) / (7 * 180.0); //calculation for the radian to degree conversion
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
//=============   ODOMETRY    ==============

  unsigned long mil, mil1, mil2;
  mil = millis();
  mil1 = millis();
  mil2 = millis();

  if (mil2 - milseb >= 100)
  {
    float fa = 0.7; //Variable Pengali yang didapat dari Trial n error terhadap perhitungan odometry dalam milimeter
//    for (int k = 0; k < 3; k++)
//    {
//      noInterrupts();
//      pos = posi[m[k]] - posseb[m[k]];
//      c_rpm[k] = ((pos * 10 * 60) / ppr[k]); //(Perhitungan Encoder(100ms) * 10(variable 100ms * 10 = 1 detik) * 60(RPS to RPM)  / Pulse Per Revolusi)
//      posseb[m[k]] = posi[m[k]];
//    }

    q = posi[0] - posi0;
    w = posi[1] - posi1;
    s = posi[2] - posi2;
    
//    interrupts();
//
//    cnt++;
//    if (cnt > 10) 
//    {
//      Serial.println(tipe);
//      tendang(tipe);
//      cnt = 0;
//    }
//    penggiring();                                   //Mengaktifkan penggiring
//    Serial.print(cnt);
//    Serial.print(' ');
//    Serial.println(detikIsi);

    q = (q * 2.95) * fa;                            //nilai 2.95 didapat dari (keiling lingkaran  / pulse per revolusi), 1 pulse = 2.95mm
    w = (w * 2.88) * fa;                            //nilai 2.88 didapat dari (keiling lingkaran  / pulse per revolusi), 1 pulse = 2.88mm
    s = (s * 2.97) * fa;                            //nilai 2.97 didapat dari (keiling lingkaran  / pulse per revolusi), 1 pulse = 2.97mm
    
    //Dalam Kenyataannya PPR tiap Motor bisa berbeda-beda harus menyesuaikan dengan kondisi yang nyata
    
    posi0 = posi[0];
    posi1 = posi[1];
    posi2 = posi[2];

    //=============   Forward Kinematik Odometry    ==============

    double vv1 = (0.866 * q) - (0.866 * w) + (0 * s);
    double vv2 = -(0.5 * q) - (0.5 * w) + (1 * s);
    double vv3 = -(q + w + s) / 3;

    //=============   Matrix Rotasi Odometry    ==============

    double bun = ((-((q / 0.7) + (w / 0.7) + (s / 0.7))) * 120) / 1256;
    thet = thet + bun;
    ypos = (cos(degree(thet)) * vv1) + (-sin(degree(thet)) * vv2);
    xpos = (sin(degree(thet)) * vv1) + (cos(degree(thet)) * vv2);

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

//    for (int k = 0; k < 3; k++)
//    {
//      PID(c_rpm[k], k);
//    }

    //Serial.println("");
    milseb = mil2;
  }

}

void PIDpos(int y, int x, int thet, float speedd) {
  float kpP = 0.07;

  int errx = x - Xpos;
  int erry = y - Ypos;
  int errthet = thet - Thet;
  //Serial.print(errx);
  //Serial.print(" - ");
  //Serial.print(erry);
  //Serial.print(" - ");
  float ux = kpP * errx;
  float uy = kpP * erry;
  float uthet = -(kpP * errthet);
  //Serial.print(" Kinematik = ");
  //Serial.print(ux);
  //Serial.print(" - ");
  //Serial.print(uy);
  //Serial.print(" - ");
  //Serial.print(uthet);
  //Serial.print(" - ");

//  if (PIDodometry == 0) {
//    if (errx < 150 && errx > -150 && erry < 150 && erry > -150 && errthet < 12 && errthet > -12 ) {
//      kinematik(0, 0, 0, 0);
//      //delay(2000);
//      modes = 1;
//      dones = 1;
//      PIDodometry = 1;
//    }
//    else {
//      if (errx > 400 && erry > 400 && errx > 0 && erry > 0 ) {
//        //  Serial.println("non");
//        kinematik(ux, uy, uthet, speedd);
//      }
//      else {
//        //    Serial.println("yes");
//        kinematik(ux, uy, uthet, speedd);
//      }
//    }
//  }
//  if (PIDodometry == 1) {
//    if (errx < 60 && errx > -60 && erry < 60 && erry > -60 && errthet < 12 && errthet > -12 ) {
//      kinematik(0, 0, 0, 0);
//      //delay(2000);
//      modes = 1;
//      dones = 1;
//      PIDodometry = 2;
//      odometryStop = 1;
//    }
//    else {
//      if (errx > 400 && erry > 400 && errx > 0 && erry > 0 ) {
//        //  Serial.println("non");
//        kinematik(ux, uy, uthet, speedd);
//      }
//      else {
//        //    Serial.println("yes");
//        kinematik(ux, uy, uthet, speedd);
//      }
//    }
//  }
//
//  if (PIDodometry == 3) {
//    if (errx < 60 && errx > -60 && erry < 60 && erry > -60 && errthet < 12 && errthet > -12 ) {
//      kinematik(0, 0, 0, 0);
//      //delay(2000);
//      modes = 1;
//      dones = 1;
//      PIDodometry = 3;
//      odometryStop = 1;
//    }
//    else {
//      if (errx > 400 && erry > 400 && errx > 0 && erry > 0 ) {
//        //  Serial.println("non");
//        kinematik(ux, uy, uthet, speedd);
//      }
//      else {
//        //    Serial.println("yes");
//        kinematik(ux, uy, uthet, speedd);
//      }
//    }
//  }
//
//  if (PIDodometry == 4) {
//    if (errx < 60 && errx > -60 && erry < 60 && erry > -60 && errthet < 12 && errthet > -12 ) {
//      kinematik(0, 0, 0, 0);
//      //delay(2000);
//      modes = 1;
//      dones = 1;
//      PIDodometry = 4;
//      odometryStop = 1;
//    }
//    else {
//      if (errx > 400 && erry > 400 && errx > 0 && erry > 0 ) {
//        //  Serial.println("non");
//        kinematik(ux, uy, uthet, speedd);
//      }
//      else {
//        //    Serial.println("yes");
//        kinematik(ux, uy, uthet, speedd);
//      }
//    }
//  }  

}
