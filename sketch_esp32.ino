#include <WiFi.h>
#include <WebServer.h>
#include "LittleFS.h"

// Wi-Fi credentials
const char* ssid = "Extender-d6056c";
const char* password = "MRAC4thf";

// GPIO setup
const int relayPin = 18;         // Relay IN (#Active LOW)
const int statusLedPin = 5;      // System status LED = ON always
const int relayLedPin = 4;       // NEW: LED indicator for relay ON
const int buttonPin = 12;        // Button to GND

// Button debounce
bool lastButtonState = HIGH;
bool buttonPressed = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// Relay timing
bool relayActive = false;
unsigned long relayStartTime = 0;

WebServer server(80);

// ===== Relay Trigger (1s pulse) =====
void triggerRelay() {
  Serial.println("Triggering relay...");
  digitalWrite(relayPin, LOW);          // Relay ON
  digitalWrite(relayLedPin, HIGH);      // LED ON (active HIGH)
  relayStartTime = millis();
  relayActive = true;
}

// ===== Serve Static Files =====
void serveFile(const char* path, const char* type) {
  if (LittleFS.exists(path)) {
    File file = LittleFS.open(path, "r");
    server.streamFile(file, type);
    file.close();
  } else {
    server.send(404, "text/plain", "File Not Found");
  }
}

void setup() {
  Serial.begin(115200);

  // Pin modes
  pinMode(relayPin, OUTPUT);
  pinMode(statusLedPin, OUTPUT);
  pinMode(relayLedPin, OUTPUT);      // NEW
  pinMode(buttonPin, INPUT_PULLUP);

  // Initial states
  digitalWrite(relayPin, HIGH);       // Relay OFF
  digitalWrite(statusLedPin, HIGH);   // System LED ON
  digitalWrite(relayLedPin, LOW);     // Relay LED OFF

  // Wi-Fi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // LittleFS init
  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS mount failed!");
    return;
  }

  // === Web Routes ===
  server.on("/", []() { serveFile("/index.html", "text/html"); });
  server.on("/pc.html", []() { serveFile("/pc.html", "text/html"); });
  server.on("/style.css", []() { serveFile("/style.css", "text/css"); });

  server.on("/trigger", HTTP_POST, []() {
    triggerRelay();
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "Relay triggered!");
  });

  server.on("/trigger", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  });

  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();

  // Button debounce
  bool reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && !buttonPressed) {
      Serial.println("Physical button press detected.");
      triggerRelay();
      buttonPressed = true;
    }
    if (reading == HIGH) {
      buttonPressed = false;
    }
  }
  lastButtonState = reading;

  // Relay auto-release after 1 second
  if (relayActive && millis() - relayStartTime >= 1000) {
    digitalWrite(relayPin, HIGH);         // Relay OFF
    digitalWrite(relayLedPin, LOW);       // LED OFF
    relayActive = false;
    Serial.println("Relay released.");
  }
}
