#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ESP32Servo.h>

#include "index_html.h"


const int ledPin = 2;
const int servoLeftPin = 18;
const int servoRightPin = 19;

WebServer server(80);
WebSocketsServer webSocket(81);
Servo servoLeft;
Servo servoRight;

const char* ssid = "Fluture_RC";
const char* password = "";


float frequencyHz = 0.0;
int centerL = 0;
int centerR = 0;
int strokeMinL = -30;
int strokeMaxL = 30;
int strokeMinR = -30;
int strokeMaxR = 30;


int waveType = 0; // 0 = Sinusoidal, 1 = Triunghiular
float downstrokeRatio = 0.5; 

float currentActualOffsetL = 0;
float currentActualOffsetR = 0;

unsigned long startTime = 0;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = String((char*)payload);

    if (message.startsWith("F:")) {
      float oldFreq = frequencyHz;
      frequencyHz = message.substring(2).toFloat();
      if (oldFreq == 0.0 && frequencyHz > 0.0) {
        startTime = micros(); 
      }
    }
    else if (message.startsWith("C:")) {
      sscanf((const char*)payload, "C:%d,%d", &centerL, &centerR);
    }
    else if (message.startsWith("A:")) {
      sscanf((const char*)payload, "A:%d,%d,%d,%d", &strokeMinL, &strokeMaxL, &strokeMinR, &strokeMaxR);
    }
  
    else if (message.startsWith("K:")) {
      int ratioPercent = 50;
      sscanf((const char*)payload, "K:%d,%d", &waveType, &ratioPercent);
      downstrokeRatio = ratioPercent / 100.0;
    }
  }
  else if (type == WStype_DISCONNECTED) {
    frequencyHz = 0.0; 
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);

  servoLeft.setPeriodHertz(300);
  servoRight.setPeriodHertz(300);

  servoLeft.attach(servoLeftPin, 500, 2400);
  servoRight.attach(servoRightPin, 500, 2400);

  WiFi.softAP(ssid, password);
  server.on("/", []() { server.send(200, "text/html", index_html); });
  server.begin();
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  startTime = micros();
}

void loop() {
  webSocket.loop();
  server.handleClient();

  int leftFinalAngle = 90;
  int rightFinalAngle = 90;

 
  if (frequencyHz > 0.0) {
    digitalWrite(ledPin, HIGH);

    float period = 1.0 / frequencyHz;
    // fracțiunea de timp din ciclul actual (de la 0.0 la 1.0)
    float fraction = fmod((micros() - startTime) / 1000000.0, period) / period; 

    
    float mappedPhase = 0;
    if (fraction <= downstrokeRatio) {
      // Suntem în faza de coborâre (0 până la PI)
      mappedPhase = (fraction / downstrokeRatio) * PI;
    } else {
      // Suntem în faza de urcare (PI până la 2*PI)
      mappedPhase = PI + ((fraction - downstrokeRatio) / (1.0 - downstrokeRatio)) * PI;
    }

    // 2. Selectăm Forma Undei
    float waveMultiplier = 0;
    if (waveType == 0) {
      // Undă Sinusoidală. Folosim cos() pentru ca faza 0 să înceapă "Sus" (Max = +1)
      waveMultiplier = cos(mappedPhase); 
    } else {
      // Undă Triunghiulară agresivă
      waveMultiplier = (2.0 / PI) * asin(cos(mappedPhase)); 
    }

    // 3. Calculăm Offsetul Real pentru fiecare aripă
    float midPointL = (strokeMinL + strokeMaxL) / 2.0;
    float amplitudeSizeL = (strokeMaxL - strokeMinL) / 2.0;
    currentActualOffsetL = midPointL + (amplitudeSizeL * waveMultiplier);

    float midPointR = (strokeMinR + strokeMaxR) / 2.0;
    float amplitudeSizeR = (strokeMaxR - strokeMinR) / 2.0;
    currentActualOffsetR = midPointR + (amplitudeSizeR * waveMultiplier);
  }
  // ---------------- OPRIRE LINA (Smooth Glide) ----------------
  else {
    digitalWrite(ledPin, LOW);
    
    // Aducem offset-ul treptat spre 0 în loc să smucim servomotorul
    if (currentActualOffsetL > 0) currentActualOffsetL -= 1.0;
    else if (currentActualOffsetL < 0) currentActualOffsetL += 1.0;
    if (abs(currentActualOffsetL) < 1.0) currentActualOffsetL = 0;

    if (currentActualOffsetR > 0) currentActualOffsetR -= 1.0;
    else if (currentActualOffsetR < 0) currentActualOffsetR += 1.0;
    if (abs(currentActualOffsetR) < 1.0) currentActualOffsetR = 0;
  }

  // 4. Calculul Unghiului Final
  leftFinalAngle = 90 + centerL + currentActualOffsetL;
  rightFinalAngle = 90 - centerR - currentActualOffsetR;

  leftFinalAngle = constrain(leftFinalAngle, 20, 160);
  rightFinalAngle = constrain(rightFinalAngle, 20, 160);

  servoLeft.write(leftFinalAngle);
  servoRight.write(rightFinalAngle);

  delay(2);
}