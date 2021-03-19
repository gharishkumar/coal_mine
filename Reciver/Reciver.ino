#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial bluetooth(14,12);
BlynkTimer timer;

const int buzzer = 17;
float humidity, temperature;
int rainSensor, gasSensor;
bool state = true;
String sensorValues;

char auth[] = "yourBlynkToken";

char ssid[] = "yourSSID";
char pass[] = "yourPassword";

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void sensorDataRead(){
  if (bluetooth.available() > 0) {
    sensorValues = bluetooth.readString();
  }
  else
  {
    return;
  }
  humidity=getValue(sensorValues, ':', 0).toFloat();
  temperature=getValue(sensorValues, ':', 1).toFloat();
  rainSensor=getValue(sensorValues, ':', 2).toInt();
  gasSensor=getValue(sensorValues, ':', 3).toInt();
  Serial.println("sensorValues received and parsed");
  Serial.print("\n");
  Serial.print("Humidity ");
  Serial.print(humidity);
  Serial.println("%");
  Serial.print("Temperature ");
  Serial.print(temperature);
  Serial.println("*C");
  Serial.print("RainSensorValue ");
  Serial.print(rainSensor);
  Serial.println("%");
  Serial.print("GasSensorValue ");
  Serial.print(gasSensor);
  Serial.println("%");
  Serial.println("---------------------xxxxx--------------------");
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature,1);
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
  if ((temperature > 40.0) || (gasSensor > 70) || (rainSensor > 70))
  {
    digitalWrite(buzzer,HIGH);
    Serial.println("---------------------Warning--------------------");
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
  Blynk.virtualWrite(V1, (int)humidity);
  Blynk.virtualWrite(V2, (int)temperature);
  Blynk.virtualWrite(V3, rainSensor);
  Blynk.virtualWrite(V4, gasSensor);
}
void setup()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  bluetooth.begin(38400);
  lcd.setCursor(0, 0);
  lcd.print("Connecting to   ");
  lcd.setCursor(0, 1);
  lcd.print("blynk server    ");
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sensorDataRead);
  lcd.setCursor(0, 0);
  lcd.print("Connected to    ");
  lcd.setCursor(0, 1);
  lcd.print("blynk server    ");
}

void loop()
{
  Blynk.run();
  timer.run();
}
