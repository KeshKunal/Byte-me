#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// --- Pin Map (Verified) ---
#define RED D1
#define GREEN D2
#define BLUE D3
#define BUTTON D5
#define LAMP D6    // Your main White LED Lamp
#define BUZZER D7  
#define SERVO_PIN D4
#define LDR_PIN A0

// --- Globals & Objects ---
Servo nudgeServo;
ESP8266WebServer server(80);
float filteredLight = 500; 
bool lastButtonState = HIGH;
unsigned long sessionStartTime = 0;

// Intervals (Change these via Wi-Fi later!)
long studyDuration = 10000; // 10s Demo
long breakDuration = 5000;  // 5s Demo

enum State { IDLE, STUDY, BREAK };
State currentState = IDLE;

// --- Wi-Fi Webpage ---
void handleRoot() {
  String html = "<html><body style='font-family:sans-serif; text-align:center;'>";
  html += "<h1>Adaptive Lamp Dashboard</h1>";
  html += "<p>Mode: " + String(currentState == STUDY ? "STUDY" : (currentState == BREAK ? "BREAK" : "IDLE")) + "</p>";
  html += "<p>Current Duration: " + String(studyDuration/1000) + "s</p>";
  html += "<hr><a href='/set10'><button style='padding:10px;'>Set 10s Demo</button></a>";
  html += " <a href='/set20'><button style='padding:10px;'>Set 20m Study</button></a>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(RED, OUTPUT); pinMode(GREEN, OUTPUT); pinMode(BLUE, OUTPUT);
  pinMode(LAMP, OUTPUT); pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  // 1. Setup Access Point
  WiFi.softAP("Smart-Study-Lamp", "password123");
  server.on("/", handleRoot);
  server.on("/set10", [](){ studyDuration = 10000; server.send(200, "text/html", "Set to 10s! <a href='/'>Back</a>"); });
  server.on("/set20", [](){ studyDuration = 1200000; server.send(200, "text/html", "Set to 20m! <a href='/'>Back</a>"); });
  server.begin();

  // 2. Initialize Hardware
  moveServo(0); // Hide flag
  resetPins();
  
  Serial.println(">>> SYSTEM ONLINE - CONNECT TO WIFI 'Smart-Study-Lamp' <<<");
}

void loop() {
  server.handleClient();
  checkButton();
  
  if (currentState == STUDY) {
    handleAdaptiveLighting();
    handleFocusBreathing(); // Pulse Red LED for "Do Not Disturb"
    
    if (millis() - sessionStartTime >= studyDuration) {
      startBreak();
    }
  } 
  else if (currentState == BREAK) {
    if (millis() - sessionStartTime >= breakDuration) {
      resetToIdle();
    }
  }
}

// --- Logic Blocks ---

void handleAdaptiveLighting() {
  int raw = analogRead(LDR_PIN);
  filteredLight = (filteredLight * 0.9) + (raw * 0.1);

  // CALIBRATION FIX: 
  // If Dark (200) -> Output Bright (1023)
  // If Bright (900) -> Output Dim (100)
  int targetBrightness = map((int)filteredLight, 200, 900, 1023, 100);
  targetBrightness = constrain(targetBrightness, 100, 1023);

  analogWrite(LAMP, targetBrightness);
}

void handleFocusBreathing() {
  // Smoothly pulse the RED LED to show "Deep Work"
  float val = (exp(sin(millis() / 2000.0 * PI)) - 0.36787944) * 108.0;
  analogWrite(RED, (int)val);
  digitalWrite(BLUE, LOW);
  digitalWrite(GREEN, LOW);
}

void checkButton() {
  bool currentButton = digitalRead(BUTTON);
  if (lastButtonState == HIGH && currentButton == LOW) {
    if (currentState == IDLE) startStudy();
    else resetToIdle();
    delay(250); 
  }
  lastButtonState = currentButton;
}

// --- State Transitions ---

void startStudy() {
  currentState = STUDY;
  sessionStartTime = millis();
  
  // 🔵 Start Flash
  digitalWrite(BLUE, HIGH);
  beep(1);
  delay(300);
  digitalWrite(BLUE, LOW);
  
  moveServo(0);
}

void startBreak() {
  currentState = BREAK;
  sessionStartTime = millis();
  
  // 🟢 Break Status
  resetPins();
  digitalWrite(GREEN, HIGH);
  beep(2);
  
  analogWrite(LAMP, 0); // Turn off lamp to rest eyes
  moveServo(90);        // Raise "Health Flag"
}

void resetToIdle() {
  currentState = IDLE;
  resetPins();
  moveServo(0);
}

// --- Helpers ---

void moveServo(int angle) {
  nudgeServo.attach(SERVO_PIN);
  nudgeServo.write(angle);
  delay(600);
  nudgeServo.detach();
}

void beep(int count) {
  for(int i=0; i<count; i++) {
    digitalWrite(BUZZER, HIGH); delay(100);
    digitalWrite(BUZZER, LOW);  delay(100);
  }
}

void resetPins() {
  digitalWrite(RED, LOW); digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW); digitalWrite(LAMP, LOW);
}