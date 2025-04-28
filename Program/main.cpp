// #include <Arduino.h>
// #include <Servo.h>

// // Pin Configuration
// #define TEMP_POT_PIN A0    // Potensiometer suhu di A0
// #define HUM_POT_PIN A3     // Potensiometer kelembaban di A1
// #define LDR_PIN A1         // LDR di A2
// #define RAIN_BUTTON_PIN 2  // Push button hujan di D2
// #define SERVO_PIN 9        // Servo di D9

// // Threshold
// const int LDR_DARK = 300;    // Nilai LDR saat gelap
// const int RAIN_BUTTON_THRESHOLD = 1; // HIGH = tombol ditekan
// const int UPDATE_INTERVAL = 100;

// Servo myservo;

// struct SensorData {
//   float temperature;
//   float humidity;
//   int light;
//   bool rain;
//   bool decision;
// } data;


// void updateSensors() {
//   // Simulasi suhu dari potensiometer (0-50°C)
//   data.temperature = map(analogRead(TEMP_POT_PIN), 0, 1023, 0, 500) / 10.0;
  
//   // Baca LDR aktual
//   data.light = analogRead(LDR_PIN);
  
//   // Baca push button hujan (aktif LOW)
//   data.rain = digitalRead(RAIN_BUTTON_PIN) == LOW;
// }

// void makeDecision() {
//   bool raining = data.rain;
//   bool dark = data.light < LDR_DARK;
//   bool cold = data.temperature < 25.0;

//   // Logika keputusan
//   data.decision = !(raining || (dark && cold));
// }

// void controlServo() {
//   myservo.write(data.decision ? 45 : 0); // 45° keluar, 0° masuk
// }

// void sendData() {
//   Serial.print("T:");
//   Serial.print(data.temperature);
//   Serial.print("|L:");
//   Serial.print(data.light);
//   Serial.print("|R:");
//   Serial.print(data.rain ? "RAINING" : "DRY");
//   Serial.print("|D:");
//   Serial.println(data.decision ? "OUT" : "IN");
// }

// void setup() {
//   Serial.begin(9600);
//   myservo.attach(SERVO_PIN);
//   pinMode(RAIN_BUTTON_PIN, INPUT_PULLUP); // Push button dengan pull-up internal
// }

// void loop() {
//   static unsigned long lastUpdate = 0;
  
//   if(millis() - lastUpdate > UPDATE_INTERVAL) {
//     updateSensors();
//     makeDecision();
//     sendData();
//     controlServo();
    
//     lastUpdate = millis();
//   }
// }

#include <Arduino.h>
#include <Servo.h>
#include <DHT.h>
// Pin Configuration
#define DHT_PIN 5            // DHT22 di D5
#define DHT_TYPE DHT22
#define LDR_PIN A1           // LDR di A1
#define RAIN_BUTTON_PIN 2    // Push button hujan di D2
#define MODE_BUTTON_PIN 3    // Tombol mode otomatis/manual di D3
#define CONTROL_BUTTON_PIN 4 // Tombol kontrol jemuran manual di D4
#define SERVO_PIN 9          // Servo di D9
// Threshold
const int LDR_DARK = 300;    // Ambang batas LDR gelap
const int UPDATE_INTERVAL = 100; // Update setiap 100ms
DHT dht(DHT_PIN, DHT_TYPE);
Servo myservo;
struct SensorData {
  float temperature;
  float humidity;
  int light;
  bool rain;
  bool decision; // True = keluar, False = masuk
} data;

bool autoMode = true;  // true = otomatis, false = manual
bool manualControl = false; // true = keluar, false = masuk

#define DHT_UPDATE_INTERVAL 2000 // Baca DHT tiap 2 detik
unsigned long lastDHTUpdate = 0;

void updateSensors() {
  // Baca DHT cuma kalau waktunya
  if (millis() - lastDHTUpdate >= DHT_UPDATE_INTERVAL) {
    data.temperature = dht.readTemperature();
    data.humidity = dht.readHumidity();
    lastDHTUpdate = millis();
  }

  // Ini tetap dibaca cepat
  data.light = analogRead(LDR_PIN);
  data.rain = digitalRead(RAIN_BUTTON_PIN) == LOW;
}

void makeDecisionAuto() {
  bool raining = data.rain;
  bool dark = data.light < LDR_DARK;
  bool cold = data.temperature < 25.0;

  // Logika keputusan otomatis
  if (raining || (dark && cold)) {
    data.decision = false; // Jemuran masuk
  } else {
    data.decision = true;  // Jemuran keluar
  }
}

void controlServo() {
  if (data.decision) {
    myservo.write(45); // Keluar
  } else {
    myservo.write(0);  // Masuk
  }
}

void readModeButton() {
  static bool lastModeState = HIGH;
  bool currentModeState = digitalRead(MODE_BUTTON_PIN);
  
  if (lastModeState == HIGH && currentModeState == LOW) {
    // Tombol mode ditekan
    autoMode = !autoMode; // Toggle mode
    Serial.print("Mode berubah: ");
    Serial.println(autoMode ? "OTOMATIS" : "MANUAL");
  } 
  lastModeState = currentModeState;
}

void readControlButton() {
  static bool lastControlState = HIGH;
  bool currentControlState = digitalRead(CONTROL_BUTTON_PIN);  
  if (lastControlState == HIGH && currentControlState == LOW) {
    // Tombol control hanya aktif di mode manual
    if (!autoMode) {
      manualControl = !manualControl; // Toggle jemuran keluar/masuk
      Serial.print("Manual control: ");
      Serial.println(manualControl ? "OUT" : "IN");
    }
  } 
  lastControlState = currentControlState;
}

void sendData() {
  Serial.print("Mode:");
  Serial.print(autoMode ? "AUTO" : "MANUAL");
  Serial.print("|T:");
  Serial.print(data.temperature);
  Serial.print("|H:");
  Serial.print(data.humidity);
  Serial.print("|L:");
  Serial.print(data.light);
  Serial.print("|R:");
  Serial.print(data.rain ? "RAINING" : "DRY");
  Serial.print("|D:");
  Serial.println(data.decision ? "OUT" : "IN");
}
void setup() {
  Serial.begin(9600);
  dht.begin();
  myservo.attach(SERVO_PIN);
  pinMode(RAIN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONTROL_BUTTON_PIN, INPUT_PULLUP);
}
void loop() {
  static unsigned long lastSensorUpdate = 0;
  static unsigned long lastButtonUpdate = 0;
  
  unsigned long now = millis();

  // Update tombol lebih sering
  if (now - lastButtonUpdate > 10) { // Baca tombol tiap 10ms
    readModeButton();
    readControlButton();
    lastButtonUpdate = now;
  }

  // Update sensor lebih lambat
  if (now - lastSensorUpdate > 1000) { // Baca sensor tiap 1 detik
    if (autoMode) {
      updateSensors();
      makeDecisionAuto();
    } else {
      data.decision = manualControl; // Manual tetap pakai data tombol
    }
    sendData();
    controlServo();
    lastSensorUpdate = now;
  }
}
