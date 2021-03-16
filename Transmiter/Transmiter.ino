#include "DHT.h"
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial bluetooth(5,6);

DHT dht(2, DHT11);

void setup() {
  bluetooth.begin(9600);
  dht.begin();
}
 
void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int rainSensorValue = analogRead(A1);
  int gasSensorValue = analogRead(A0);
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["humidity"] = humidity;
  json["temperature"] = temperature;
  json["rainSensorValue"] = rainSensorValue;
  json["gasSensorValue"] = gasSensorValue;
  json.printTo(bluetooth);
  delay(1000);
}
