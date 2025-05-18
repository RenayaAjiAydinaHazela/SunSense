#include <Arduino.h>
#include <Servo.h>
#include <DHT.h>

// Pin Configuration
#define DHT_PIN 5
#define DHT_TYPE DHT22
#define LDR_PIN A1
#define RAIN_SENSOR_PIN 4
#define SERVO_PIN 9
#define MODE_BUTTON_PIN 3
#define CONTROL_BUTTON_PIN 2

const unsigned long DHT_READ_INTERVAL = 2000;
const int LDR_DARK = 300;
const int UPDATE_INTERVAL = 1000;
const int DEBOUNCE_DELAY = 50;
const int HUMID_THRESHOLD = 70;

DHT dht(DHT_PIN, DHT_TYPE);
Servo myservo;

struct SensorData {
  float temperature = 0;
  float humidity = 0;
  int light;
  bool rain;
  bool decision;
} data;

bool manualMode = false;
bool manualDecision = false;

unsigned long lastUpdate = 0;
unsigned long lastModeButtonPress = 0;
unsigned long lastControlButtonPress = 0;

// Fungsi gerakan servo halus
void smoothServoMove(int targetAngle) {
  int currentAngle = myservo.read();
  if (currentAngle == targetAngle) return;

  int step = (currentAngle < targetAngle) ? 1 : -1;
  for (int angle = currentAngle; angle != targetAngle; angle += step) {
    myservo.write(angle);
    delay(10);  // Sesuaikan kecepatan
  }
  myservo.write(targetAngle);  // Pastikan posisi akhir tepat
}

void updateSensors() {
  static unsigned long lastDHTRead = 0;
  if (millis() - lastDHTRead >= DHT_READ_INTERVAL) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t)) data.temperature = t;
    if (!isnan(h)) data.humidity = h;
    lastDHTRead = millis();
  }

  data.light = analogRead(LDR_PIN);
  data.rain = digitalRead(RAIN_SENSOR_PIN) == LOW;
}

void makeAutomaticDecision() {
  bool raining = data.rain;
  bool dark = data.light < LDR_DARK;
  bool cold = data.temperature < 25.0;
  bool humid = data.humidity > HUMID_THRESHOLD;
  data.decision = !(raining || (dark && cold) || (dark && humid));
}

void controlServo() {
  int targetAngle = 0;
  if (manualMode) {
    targetAngle = manualDecision ? 45 : 0;
  } else {
    targetAngle = data.decision ? 45 : 0;
  }
  smoothServoMove(targetAngle);
}

void sendData() {
  Serial.print("MODE:");
  Serial.print(manualMode ? "MANUAL" : "AUTO");
  Serial.print("|T:");
  Serial.print(data.temperature, 1);
  Serial.print("|H:");
  Serial.print(data.humidity, 1);
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
  if (lastState == HIGH && currentState == LOW) {
    if (millis() - lastModeButtonPress > DEBOUNCE_DELAY) {
      manualMode = !manualMode;
      lastModeButtonPress = millis();
    }
  }
  lastState = currentState;
}

void readControlButton() {
  static bool lastState = HIGH;
  bool currentState = digitalRead(CONTROL_BUTTON_PIN);
  if (manualMode && lastState == HIGH && currentState == LOW) {
    if (millis() - lastControlButtonPress > DEBOUNCE_DELAY) {
      manualDecision = !manualDecision;
      lastControlButtonPress = millis();
    }
  }
  lastState = currentState;
}

void handleSerialCommand() {
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'm') {
      manualMode = !manualMode;
      Serial.println(manualMode ? "MODE:MANUAL" : "MODE:AUTO");
    } else if (cmd == 'c' && manualMode) {
      manualDecision = !manualDecision;
      Serial.println(manualDecision ? "POS:OUT" : "POS:IN");
    }
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  myservo.attach(SERVO_PIN);
  myservo.write(0);  // Posisi awal

  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONTROL_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  readModeButton();
  readControlButton();
  handleSerialCommand();

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
