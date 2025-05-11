#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <IRremote.h>
#include <ESP32Servo.h>
#include "secrets.h"


const char* ssid = "Liadlevi";
const char* password = "307857607"; // 🛑 עדכן פה את הסיסמה שלך

const int RECV_PIN = 27;
const int SERVO_PIN = 13;

const unsigned long SINGLE_HIT_CODE = 0xE619FF00;
const unsigned long RAPID_MODE_CODE = 0xF20DFF00;
const unsigned long STOP_CODE = 0xE916FF00;

WebServer server(80);
Servo myServo;

bool rapidMode = false;
unsigned long lastTapTime = 0;
const int tapInterval = 400;
void doTap();
void handleRoot();

void setup() {
  Serial.begin(115200);

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  // IR
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);

  // Servo
  myServo.attach(SERVO_PIN);
  myServo.write(0);

  // Webserver routes
  server.on("/", handleRoot);
  server.on("/single", []() {
    Serial.println("💥 דפיקה מהדף");
    doTap();
    server.send(200, "text/plain", "Single tap");
  });
  server.on("/rapid", []() {
    Serial.println("🔥 מצב מהיר מהדף");
    rapidMode = true;
    server.send(200, "text/plain", "Rapid mode ON");
  });
  server.on("/stop", []() {
    Serial.println("🛑 עצירה מהדף");
    rapidMode = false;
    server.send(200, "text/plain", "Stopped");
  });

  server.begin();
  Serial.println("HTTP Server Ready");
}

void loop() {
  server.handleClient();

  // מצב מהיר
  if (rapidMode && millis() - lastTapTime > tapInterval) {
    doTap();
    lastTapTime = millis();
  }

  // שלט IR
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("IR: ");
    Serial.println(code, HEX);

    if (code == SINGLE_HIT_CODE) doTap();
    else if (code == RAPID_MODE_CODE) rapidMode = true;
    else if (code == STOP_CODE) rapidMode = false;

    IrReceiver.resume();
  }
}

void doTap() {
  myServo.write(90);
  delay(150);
  myServo.write(0);
}

void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Ceiling Tapper</title>
      <style>
        button {
          width: 200px; height: 60px;
          font-size: 20px;
          margin: 10px;
        }
      </style>
    </head>
    <body>
      <h2>Tap Controller</h2>
      <button onclick="fetch('/single')"> Single Tap</button><br>
      <button onclick="fetch('/rapid')"> Rapid Mode</button><br>
      <button onclick="fetch('/stop')"> Stop</button>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}
