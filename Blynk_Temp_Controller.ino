#define BLYNK_TEMPLATE_ID "user5"
#define BLYNK_TEMPLATE_NAME "user5@server.wyns.it"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// temperatuursensor
#define DS18B20_PIN 4
OneWire ourWire(DS18B20_PIN);
DallasTemperature sensor(&ourWire);

// RGB LED pinnen
#define RED_PIN 21
#define GREEN_PIN 22
#define BLUE_PIN 23

// WiFi en Blynk instellingen
char auth[] = "XBKvdONtp-OBExR7sEXPq2y8Xvrvkjvp";
char ssid[] = "***REMOVED***";
char pass[] = "***REMOVED***";

BlynkTimer timer;

// Vastandaardwaarde
float thresholdTemp = 30.0;

// Blynk widget voor het instellen van de temperatuur
BLYNK_WRITE(V1) {
  thresholdTemp = param.asFloat();
  Serial.print("Nieuwe drempelwaarde via Blynk: ");
  Serial.println(thresholdTemp);
}


void sendSensor() {
  sensor.requestTemperatures();
  float tempC = sensor.getTempCByIndex(0);

  Serial.print("Celsius temperature: ");
  Serial.println(tempC);


  Blynk.virtualWrite(V0, tempC);

  if (tempC <= thresholdTemp) {
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, 0);

    Blynk.setProperty(V2, "color", "#00FF00");
    Blynk.virtualWrite(V2, 255);
  } else {
    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 0);

  
    Blynk.setProperty(V2, "color", "#FF0000");
    Blynk.virtualWrite(V2, 255);
  }
}

void setup() {
  Serial.begin(115200);
  sensor.begin();


  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);


  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);


  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);


  timer.setInterval(1000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
