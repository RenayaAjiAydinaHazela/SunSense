#include <Arduino.h>
#include <Servo.h>

// Pin Configuration
#define TEMP_POT_PIN A0    // Potensiometer suhu
#define HUM_POT_PIN A5     // Potensiometer kelembaban
#define LDR_PIN A1         // LDR
#define RAIN_BUTTON_PIN 2  // Push button hujan
#define SERVO_PIN 9        // Servo

#define MODE_BUTTON_PIN 3      // Tombol ganti mode Otomatis/Manual
#define CONTROL_BUTTON_PIN 4   // Tombol kontrol jemuran Masuk/Keluar (hanya Manual)

const int LDR_DARK = 300;    // Ambang batas LDR
const int UPDATE_INTERVAL = 1000; // Update tiap 1 detik
const int DEBOUNCE_DELAY = 50;    // Delay debounce tombol

Servo myservo;

struct SensorData {
  float temperature;
  float humidity;
  int light;
  bool rain;
  bool decision;
} data;

bool manualMode = false;    // Default Otomatis
bool manualDecision = false; // Default jemuran masuk (false = IN, true = OUT)

unsigned long lastUpdate = 0;
unsigned long lastModeButtonPress = 0;
unsigned long lastControlButtonPress = 0;

void updateSensors() {
  data.temperature = map(analogRead(TEMP_POT_PIN), 0, 1023, 0, 500) / 10.0;
  data.light = analogRead(LDR_PIN);
  data.rain = digitalRead(RAIN_BUTTON_PIN) == LOW;
}

void makeAutomaticDecision() {
  bool raining = data.rain;
  bool dark = data.light < LDR_DARK;
  bool cold = data.temperature < 25.0;
  data.decision = !(raining || (dark && cold));
}

void controlServo() {
  if (manualMode) {
    myservo.write(manualDecision ? 45 : 0); // Manual kontrol
  } else {
    myservo.write(data.decision ? 45 : 0); // Otomatis
  }
}

void sendData() {
  Serial.print("MODE:");
  Serial.print(manualMode ? "MANUAL" : "AUTO");
  Serial.print("|T:");
  Serial.print(data.temperature);
  Serial.print("|L:");
  Serial.print(data.light);
  Serial.print("|R:");
  Serial.print(data.rain ? "RAINING" : "DRY");
  Serial.print("|D:");
  Serial.println((manualMode ? manualDecision : data.decision) ? "OUT" : "IN");
}

void readModeButton() {
  static bool lastState = HIGH;
  bool currentState = digitalRead(MODE_BUTTON_PIN);

  if (lastState == HIGH && currentState == LOW) { // Tombol ditekan
    if (millis() - lastModeButtonPress > DEBOUNCE_DELAY) {
      manualMode = !manualMode; // Ganti mode
      lastModeButtonPress = millis();
    }
  }
  lastState = currentState;
}

void readControlButton() {
  static bool lastState = HIGH;
  bool currentState = digitalRead(CONTROL_BUTTON_PIN);

  if (manualMode && lastState == HIGH && currentState == LOW) { // Tombol ditekan
    if (millis() - lastControlButtonPress > DEBOUNCE_DELAY) {
      manualDecision = !manualDecision; // Toggle jemuran keluar/masuk
      lastControlButtonPress = millis();
    }
  }
  lastState = currentState;
}

void setup() {
  Serial.begin(9600); // Ini juga buat Bluetooth (pastikan baudrate Bluetooth sama 9600)
  myservo.attach(SERVO_PIN);

  pinMode(RAIN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONTROL_BUTTON_PIN, INPUT_PULLUP);

  myservo.write(0); // Awal jemuran masuk
}

void loop() {
  readModeButton();
  readControlButton();

  if (millis() - lastUpdate > UPDATE_INTERVAL) {
    updateSensors();
    if (!manualMode) {
      makeAutomaticDecision();
    }
    controlServo();
    sendData();
    lastUpdate = millis();
  }
}
