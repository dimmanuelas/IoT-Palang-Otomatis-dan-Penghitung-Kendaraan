#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

const int SENSOR1_TrgPIN = 2;
const int SENSOR1_EchoPIN = 3;

const int SENSOR2_TrgPIN = 5;
const int SENSOR2_EchoPIN = 6;

const int Buzzer = 11;

int jumlah_kendaraan = 0;
int max_kendaraan =  10; 
int eeAddress = 0; 

bool entry_gate_status = false;
bool entry_waiting_to_close = false;
unsigned long entryGateTimer = 0;

bool exit_gate_status = false;
bool exit_waiting_to_close = false;
unsigned long exitGateTimer = 0;

unsigned long previousMillis = 0;
const long readInterval = 50;

Servo MotorMasuk;
Servo MotorKeluar;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(SENSOR1_TrgPIN, OUTPUT);
  pinMode(SENSOR2_TrgPIN, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(SENSOR1_EchoPIN, INPUT);
  pinMode(SENSOR2_EchoPIN, INPUT);

  MotorMasuk.attach(4);
  MotorMasuk.write(90);
  MotorKeluar.attach(7);
  MotorKeluar.write(90);
  
  Wire.begin();
  lcd.init();
  lcd.backlight();
  
  Serial.begin(9600);

  EEPROM.get(eeAddress, jumlah_kendaraan);
  
  if (jumlah_kendaraan < 0 || jumlah_kendaraan > max_kendaraan) {
    jumlah_kendaraan = 0;
    EEPROM.put(eeAddress, jumlah_kendaraan);
  }
  
  updateLCD();
}

long l_read(int TrgPIN, int EchPIN) {
  digitalWrite(TrgPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TrgPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrgPIN, LOW);

  long duration = pulseIn(EchPIN, HIGH, 20000);
  if (duration == 0) return 999;
  
  long distance = duration * 0.034 / 2;
  return distance;
}

void updateLCD() {
  lcd.clear();
  if (jumlah_kendaraan >= max_kendaraan) {
    lcd.setCursor(0, 0);
    lcd.print("Maaf Parkiran");
    lcd.setCursor(0, 1);
    lcd.print("Penuh :(");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("PARKIR UMUM");
    lcd.setCursor(0, 1);
    lcd.print("Tersisa " + String(max_kendaraan - jumlah_kendaraan));
  }
}

void loop() {  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= readInterval) {
    previousMillis = currentMillis;

    long distance_1 = l_read(SENSOR1_TrgPIN, SENSOR1_EchoPIN);
    long distance_2 = l_read(SENSOR2_TrgPIN, SENSOR2_EchoPIN);

    if (distance_1 > 0 && distance_1 <= 10 && !entry_gate_status && jumlah_kendaraan < max_kendaraan) {
      MotorMasuk.write(0);
      entry_gate_status = true;
      tone(Buzzer, 323, 100);
    } 
    else if (distance_1 > 10 && entry_gate_status && !entry_waiting_to_close) {
      jumlah_kendaraan += 1;
      EEPROM.put(eeAddress, jumlah_kendaraan); 
      updateLCD();
      entry_waiting_to_close = true;
      entryGateTimer = currentMillis; 
    }

    if (distance_2 > 0 && distance_2 <= 10 && !exit_gate_status && jumlah_kendaraan > 0) {
      MotorKeluar.write(0);
      exit_gate_status = true; 
      tone(Buzzer, 323, 100);
    } 
    else if (distance_2 > 10 && exit_gate_status && !exit_waiting_to_close) {
      if (jumlah_kendaraan > 0) {
        jumlah_kendaraan -= 1;
        EEPROM.put(eeAddress, jumlah_kendaraan);
      }
      updateLCD();
      exit_waiting_to_close = true;
      exitGateTimer = currentMillis;
    }
  }

  if (entry_waiting_to_close && (currentMillis - entryGateTimer >= 3000)) {
    MotorMasuk.write(90);
    entry_gate_status = false;
    entry_waiting_to_close = false;
  }
  if (exit_waiting_to_close && (currentMillis - exitGateTimer >= 3000)) {
    MotorKeluar.write(90);
    exit_gate_status = false;
    exit_waiting_to_close = false;
  }
}