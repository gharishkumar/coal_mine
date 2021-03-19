#include "DHT.h"
#include <SoftwareSerial.h>
SoftwareSerial bluetooth(5,6);

DHT dht(2, DHT11);

void setup() {
  bluetooth.begin(38400);
  dht.begin();
}
 
void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int rainSensorValue = map(analogRead(A1),0,1024,0,100);
  int gasSensorValue = map(analogRead(A0),0,1024,0,100);
  bluetooth.print((String)humidity+":"+(String)temperature+":"+(String)rainSensorValue+":"+(String)gasSensorValue+":");
  delay(1000);
}
