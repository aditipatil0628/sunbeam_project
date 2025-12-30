#include <WiFi.h>
#include "DHT.h"
#include "ThingSpeak.h"

#define DHTPIN 4
#define DHTTYPE DHT11
#define MQ2_PIN 34

const char* ssid = "Reva";
const char* password = "reva292005";

unsigned long channelID = 3213181;
const char* writeAPIKey = "P1155FCJ1SO7CJO8";

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  ThingSpeak.begin(client);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int gasValue = analogRead(MQ2_PIN);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Sensor read failed");
    return;
  }

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, gasValue);

  int status = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (status == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.print("Error sending data. Code: ");
    Serial.println(status);
  }

  delay(15000); // IMPORTANT
}