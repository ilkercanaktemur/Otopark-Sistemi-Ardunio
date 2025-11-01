#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Adresin 0x3F olabilir

const int trigPin = 11;
const int echoPin = 12;
const int irSensorPin = 8;

long duration;
int distance;

int carCount = 0;
const int maxCapacity = 5;

bool carEntered = false;
bool carExited = false;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irSensorPin, INPUT);

  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print("Otopark Hazir");
  delay(2000);
  lcd.clear();
}

void loop() {
  // === Giriş Kontrolü (Ultrasonik) ===
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance > 2 && distance <= 10 && !carEntered) {
    if (carCount < maxCapacity) {
      carCount++;
    }
    carEntered = true;
  }

  if (distance > 15) {
    carEntered = false;
  }

  // === Çıkış Kontrolü (IR Sensör) ===
  if (digitalRead(irSensorPin) == LOW && !carExited) {
    if (carCount > 0) {
      carCount--;
    }
    carExited = true;
  }

  if (digitalRead(irSensorPin) == HIGH) {
    carExited = false;
  }

  // === LCD Güncelle ===
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Araclar: ");
  lcd.print(carCount);

  lcd.setCursor(0, 1);
  if (carCount >= maxCapacity) {
    lcd.print("Otopark: DOLU");
  } else {
    lcd.print("Bos Yer: ");
    lcd.print(maxCapacity - carCount);
  }

  delay(500);
}