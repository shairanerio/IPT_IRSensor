#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "NERIO_RESIDENCE";
const char* password = "judithsadam031112";

const int irSensorPin = 4; // IR sensor input pin

void setup() {
  Serial.begin(115200);
  pinMode(irSensorPin, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
}

void loop() {
  int sensorValue = digitalRead(irSensorPin);
  String status;

  if (sensorValue == LOW) {
    Serial.println("Object detected: full");
    status = "full";
  } else {
    Serial.println("No object detected: not full");
    status = "not full";
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://wastewise-backend-uqr2.onrender.com/api/bin-status");
    http.addHeader("Content-Type", "application/json");

    String postData = "{\"bio_status\": \"" + status + "\", \"recyclable_status\": \"not full\"}";

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response:");
      Serial.println(response);
    } else {
      Serial.print("Error sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected.");
  }

  delay(5000); // Wait 5 seconds before next detection
}