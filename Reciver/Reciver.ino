#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

LiquidCrystal_I2C lcd(0x27,16,2); 
SoftwareSerial bluetooth(14,12);

const int buzzer = 17;
bool state = true;

char auth[] = "YourAuthToken";

char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  bluetooth.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(bluetooth);
  //json.prettyPrintTo(Serial);
  if (json == JsonObject::invalid())
  return;
  Serial.println("JSON received and parsed");
  Serial.print("\n");
  Serial.print("Humidity ");
  float humidity=json["humidity"];
  Serial.print(humidity);
  Serial.println("%");
  Serial.print("Temperature ");
  float temperature=json["temperature"];
  Serial.print(temperature);
  Serial.println("*C");
  Serial.print("RainSensorValue ");
  int rainSensor=map(json["rainSensorValue"], 0, 1024, 0, 100);
  Serial.print(rainSensor);
  Serial.println("%");
  Serial.print("GasSensorValue ");
  int gasSensor=map(json["gasSensorValue"], 0, 1024, 0, 100);
  Serial.print(gasSensor);
  Serial.println("%");
  Serial.println("---------------------xxxxx--------------------");
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, rainSensor);
  Blynk.virtualWrite(V4, gasSensor);
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature);
  lcd.write(223);
  lcd.print("C ");
  lcd.print("GAS:");
  lcd.print(gasSensor);
  lcd.print("%   ");
  lcd.setCursor(0,1);

  if (rainSensor > 70)
  {
    lcd.print("WATER PERSENT   ");
  }
  else
  {
    lcd.print("NO WATER PERSENT");
  }
  if ((temperature > 40.0) | (gasSensor > 70) | (rainSensor > 70))
  {
    digitalWrite(buzzer,HIGH);
    if (state)
    {
      Blynk.notify("Warning limits exceded safety range");
      state = false;
    }
  }
  else
  {
    digitalWrite(buzzer,LOW);
    state = true;
  }
}
