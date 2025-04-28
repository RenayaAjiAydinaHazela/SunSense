#include <Arduino.h>
#include <Servo.h>

// Pin Configuration
#define TEMP_POT_PIN A0    // Potensiometer suhu di A0
#define HUM_POT_PIN A5     // Potensiometer kelembaban di A1
#define LDR_PIN A1         // LDR di A2
#define RAIN_BUTTON_PIN 2  // Push button hujan di D2
#define SERVO_PIN 9        // Servo di D9

// Threshold
const int LDR_DARK = 300;    // Nilai LDR saat gelap
const int RAIN_BUTTON_THRESHOLD = 1; // HIGH = tombol ditekan
const int UPDATE_INTERVAL = 1000;

Servo myservo;

struct SensorData {
  float temperature;
  float humidity;
  int light;
  bool rain;
  bool decision;
} data;
void updateSensors() {
  // Simulasi suhu dari potensiometer (0-50°C)
  data.temperature = map(analogRead(TEMP_POT_PIN), 0, 1023, 0, 500) / 10.0;
  // Simulasi kelembaban dari potensiometer (0-100%)
  //data.humidity = map(analogRead(HUM_POT_PIN), 0, 1023, 0, 1000) / 10.0;
  // Baca LDR aktual
  data.light = analogRead(LDR_PIN);
  // Baca push button hujan (aktif LOW)
  data.rain = digitalRead(RAIN_BUTTON_PIN) == LOW;
}

void makeDecision() {
  bool raining = data.rain;
  bool dark = data.light < LDR_DARK;
  bool cold = data.temperature < 25.0;
  // Logika keputusan
  data.decision = !(raining || (dark && cold));
}

void controlServo() {
  myservo.write(data.decision ? 45 : 0); // 45° keluar, 0° masuk
}

void sendData() {
  Serial.print("T:");
  Serial.print(data.temperature);
  Serial.print("|L:");
  Serial.print(data.light);
  Serial.print("|R:");
  Serial.print(data.rain ? "RAINING" : "DRY");
  Serial.print("|D:");
  Serial.println(data.decision ? "OUT" : "IN");
}

void setup() {
  Serial.begin(115200);
  myservo.attach(SERVO_PIN);
  pinMode(RAIN_BUTTON_PIN, INPUT_PULLUP); // Push button dengan pull-up internal
}
void loop() {
  static unsigned long lastUpdate = 0;
  if(millis() - lastUpdate > UPDATE_INTERVAL) {
    updateSensors();
    makeDecision();
    sendData();
    controlServo();
    lastUpdate = millis();
  }
}

