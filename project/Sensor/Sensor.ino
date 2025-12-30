#include "DHT.h"

/* -------- DHT11 CONFIG -------- */
#define DHTPIN 4
#define DHTTYPE DHT11

/* -------- MQ-2 CONFIG -------- */
#define MQ2_PIN 34

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("Temperature | Humidity | Gas");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity    = dht.readHumidity();
  int gasValue      = analogRead(MQ2_PIN);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT11 reading failed");
    delay(2000);
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C  |  Humidity: ");
  Serial.print(humidity);
  Serial.print(" %  |  Gas: ");
  Serial.println(gasValue);

  delay(2000);
}