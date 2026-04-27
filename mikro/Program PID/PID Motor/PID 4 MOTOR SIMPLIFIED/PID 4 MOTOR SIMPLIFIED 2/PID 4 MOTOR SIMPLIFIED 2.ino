const int numMotor = 4;
const int numEncoder = 4;
const int SPCW = 20;
const int SPCCW = -20;
const int pinMotor[numMotor][2] = {
  {10, 9},  // Motor 1: pin {CW, CCW}
  {8, 6},   // Motor 2: pin {CW, CCW}
  {4, 5},   // Motor 3: pin {CW, CCW}
  {11, 12}  // Motor 4: pin {CW, CCW}
};

const int pinEncoder[numEncoder][2] = {
  {20, 34},    // Encoder 1: pin {Channel A, Channel B}
  {18, 33},    // Encoder 2: pin {Channel A, Channel B}
  {2, 31},     // Encoder 3: pin {Channel A, Channel B}
  {19, 32}     // Encoder 4: pin {Channel A, Channel B}
};

const int pinMotorEnable[numMotor] = {23, 22, 24, 25};

double error[numMotor] = {0};
double error_I[numMotor] = {0};
double error_D[numMotor] = {0};
double past_error[numMotor] = {0};

const float Kp[numMotor] = {1, 1, 1, 1};
const float Ki[numMotor] = {0.0005, 0.0005, 0.0005, 0.0005};
const float Kd[numMotor] = {5, 5, 5, 5};

double waktuSekarang;
double waktuBerlalu;
double waktuSebelumnya;

double p[numMotor] = {0}, i[numMotor] = {0}, d[numMotor] = {0};
int PWMmotor[numMotor] = {0};
int rpm[numMotor] = {0};
int kecepatanMotorSet[numMotor] = {0, 0, 0, 0}; // Setpoint awal

volatile int hitungEncoder[numEncoder] = {0};
volatile int lastChannelAState[numEncoder] = {0};

int stateMotor[numMotor] = {1, 1, 0, 0}; // Diinisialisasi dengan Motor 1 dan Motor 2 hidup, yang lain mati
unsigned long waktuMulaiMotor = 0;
const unsigned long durasiMotor = 2000; // 2 detik dalam milidetik

// Tentukan langkah waktu tetap untuk perhitungan PID (dalam milidetik)
const unsigned long pidLangkahWaktu = 100; // Sesuaikan jika perlu

unsigned long waktuSebelumnyaPID = 0;

void setup() {
  Serial.begin(9600);

  for (int m1 = 0; m1 < numMotor; ++m1) {
    pinMode(pinMotor[m1][0], OUTPUT);  // pin CW
    pinMode(pinMotor[m1][1], OUTPUT);  // pin CCW
  }

  for (int en = 0; en < numMotor; ++en) {
    pinMode(pinMotorEnable[en], OUTPUT);
    digitalWrite(pinMotorEnable[en], HIGH); // Aktifkan L293D
  }

  for (int e = 0; e < numEncoder; ++e) {
    pinMode(pinEncoder[e][0], INPUT_PULLUP); // Channel A
    pinMode(pinEncoder[e][1], INPUT_PULLUP); // Channel B
    attachInterrupt(digitalPinToInterrupt(pinEncoder[e][0]), encoderISR, CHANGE);
  }
}

void loop() {
  unsigned long waktuSekarang = millis();

  // Periksa apakah ada data serial yang tersedia
  if (Serial.available() > 0) {
    // Baca input serial untuk mengatur arah gerakan
    int arahGerakan = Serial.parseInt();

    // Atur kecepatan motor berdasarkan arah gerakan
    switch (arahGerakan) {
      case 1: // Maju
        move(true, false, true, false);
        setMotorSetpoints(SPCW, SPCCW, SPCW, SPCCW); // Sesuaikan setpoint jika perlu
        break;
      case 2: // Mundur
        move(false, true, false, true);
        setMotorSetpoints(SPCCW, SPCW, SPCCW, SPCW); // Sesuaikan setpoint jika perlu
        break;
      case 3: // Kiri
        move(true, true, false, false);
        setMotorSetpoints(SPCW, SPCW, SPCCW, SPCCW); // Sesuaikan setpoint jika perlu
        break;
      case 4: // Kanan
        move(false, false, true, true);
        setMotorSetpoints(SPCCW, SPCCW, SPCW, SPCW); // Sesuaikan setpoint jika perlu
        break;
      case 5: // Putar Kanan
        rotate(true);
        setMotorSetpoints(SPCW, SPCW, SPCW, SPCW); // Sesuaikan setpoint jika perlu
        break;
      case 6: // Putar Kiri
        rotate(false);
        setMotorSetpoints(SPCCW, SPCCW, SPCCW, SPCCW); // Sesuaikan setpoint jika perlu
        break;
      default: // Berhenti
        stopMotors();
        setMotorSetpoints(0, 0, 0, 0); // Atur setpoint menjadi 0 untuk berhenti
        break;
    }
  }

  calculateRPM();

  // Lakukan perhitungan PID pada langkah waktu tetap
  if (waktuSekarang - waktuSebelumnyaPID >= pidLangkahWaktu) {
    computePID();
    waktuSebelumnyaPID = waktuSekarang;
  }

  // Tampilkan informasi motor
  for (int m2 = 0; m2 < numMotor; ++m2) {
    Serial.print("Motor ");
    Serial.print(m2 + 1);
    Serial.print(": RPM: ");
    Serial.print(rpm[m2]);
    Serial.print(", Error: ");
    Serial.print(error[m2]);
    Serial.print(", PWM: ");
    Serial.print(PWMmotor[m2]);
    Serial.print(", p: ");
    Serial.print(p[m2]);
    Serial.print(", i: ");
    Serial.print(i[m2]);
    Serial.print(", d: ");
    Serial.println(d[m2]);
  }
}

void kontrolMotor(int indeksMotor, bool arahCW) {
  digitalWrite(pinMotor[indeksMotor][0], arahCW ? HIGH : LOW);
  digitalWrite(pinMotor[indeksMotor][1], arahCW ? LOW : HIGH);
}

void setMotorSetpoints(int mm1, int mm2, int mm3, int mm4) {
  kecepatanMotorSet[0] = mm1;
  kecepatanMotorSet[1] = mm2;
  kecepatanMotorSet[2] = mm3;
  kecepatanMotorSet[3] = mm4;
}

void move(int motor1, int motor2, int motor3, int motor4) {
  kontrolMotor(0, motor1);
  kontrolMotor(1, motor2);
  kontrolMotor(2, motor3);
  kontrolMotor(3, motor4);
}

void stopMotors() {
  for (int m = 0; m < numMotor; ++m) {
    digitalWrite(pinMotor[m][0], LOW);
    digitalWrite(pinMotor[m][1], LOW);
  }
}

void rotate(bool arahCW) {
  move(arahCW, arahCW, arahCW, arahCW);
}

void encoderISR() {
  for (int e1 = 0; e1 < numEncoder; ++e1) {
    int channelAState = digitalRead(pinEncoder[e1][0]);
    int channelBState = digitalRead(pinEncoder[e1][1]);

    if (channelAState != lastChannelAState[e1]) {
      if (channelBState == channelAState) {
        hitungEncoder[e1]++;
      } else {
        hitungEncoder[e1]--;
      }
    }

    lastChannelAState[e1] = channelAState;
  }
}

void calculateRPM() {
  const int pulsaPerPutaran = 360;

  for (int m3 = 0; m3 < numMotor; ++m3) {
    rpm[m3] = static_cast<int>((double)hitungEncoder[m3] / pulsaPerPutaran * 60.0);
    hitungEncoder[m3] = 0;
  }
}

void computePID() {
  waktuSekarang = millis();
  waktuBerlalu = (double)(waktuSekarang - waktuSebelumnya);

  for (int m4 = 0; m4 < numMotor; ++m4) {
    error[m4] = kecepatanMotorSet[m4] - rpm[m4];
    error_I[m4] += error[m4] * waktuBerlalu;
    error_D[m4] = (error[m4] - past_error[m4]) / waktuBerlalu;
    past_error[m4] = error[m4];
    p[m4] = Kp[m4] * error[m4];
    i[m4] = Ki[m4] * error_I[m4];
    d[m4] = Kd[m4] * error_D[m4];
    PWMmotor[m4] = PWMmotor[m4] + p[m4] + i[m4] + d[m4];

    if (PWMmotor[m4] > 255) {
      PWMmotor[m4] = 255;
    } else if (PWMmotor[m4] < 0) {
      PWMmotor[m4] = 0;
    }
    analogWrite(pinMotorEnable[m4], PWMmotor[m4]);
  }

  waktuSebelumnya = waktuSekarang;
}
