/*
  Servos will move to a reset position after its been detached. Because of this, I placed the detach before the server.send. 
  For some reason, having something else happen immediately after helps stop the motors from moving after detached.

*/

// Declare Libraries
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Connection and Verifications
const char* ssid = "WiFi Name";
const char* password = "WiFi Password";
const char* secretKey = "Your Remote Command Password";

// Configure servo, pin, and angle presets
Servo hookServo;    //pin 25
Servo doorServo;    //pin 26
const int hookPin = 25;
const int doorPin = 26;
int angHookOpen = 150;
int angHookClose = 38;
int angDoorOpen = 157;
int angDoorClose = 34;

// Server port
WebServer server(80);

// === HANDLERS ===
void handleDrop() {
  if (server.arg("key") == secretKey) {
    hookServo.attach(hookPin);
    doorServo.attach(doorPin);
    // Open door
    doorServo.write(angDoorOpen);
    delay(1000);

    // Pin and reset pin
    hookServo.write(angHookOpen);
    delay(1000);
    hookServo.write(angHookClose);
    delay(1000);

    // Close door
    doorServo.write(angDoorClose);
    delay(1000);

    // Detach servos
    hookServo.detach();
    doorServo.detach();
 
    server.send(200, "text/plain", "Supply Drop");
  } else {
    server.send(403, "text/plain", "Forbidden");
  }
}

void handleOpenPin() {
  if (server.arg("key") == secretKey) {
    hookServo.attach(hookPin);
    hookServo.write(angHookOpen);
    delay(1000);
    hookServo.detach();
    server.send(200, "text/plain", "Pin Open");
  } else {
    server.send(403, "text/plain", "Forbidden");
  }
}

void handleClosePin() {
  if (server.arg("key") == secretKey) {
    hookServo.attach(hookPin);
    hookServo.write(angHookClose);
    delay(1000);
    hookServo.detach();
    server.send(200, "text/plain", "Pin Closed");
  } else {
    server.send(403, "text/plain", "Forbidden");
  }
}

void handleOpenDoor() {
  if (server.arg("key") == secretKey) {
    doorServo.attach(doorPin);
    doorServo.write(angDoorOpen);
    delay(1000);
    doorServo.detach();
    server.send(200, "text/plain", "Closed");
  } else {
    server.send(403, "text/plain", "Forbidden");
  }
}

void handleCloseDoor() {
  if (server.arg("key") == secretKey) {
    doorServo.attach(doorPin);
    doorServo.write(angDoorClose);
    delay(1000);
    doorServo.detach();
    server.send(200, "text/plain", "Closed");
  } else {
    server.send(403, "text/plain", "Forbidden");
  }
}

void setup() {
  // Begin WiFi connection
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Preset commands
  server.on("/drop", handleDrop);
  server.on("/openPin", handleOpenPin);
  server.on("/closePin", handleClosePin);
  server.on("/openDoor", handleOpenDoor);
  server.on("/closeDoor", handleCloseDoor);
  server.begin();
}

void loop() {
  server.handleClient();
  delay(500);
}
