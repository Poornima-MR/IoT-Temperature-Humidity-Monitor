#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#define DHTPIN D4
#define DHTTYPE DHT22
#define TEMP_THRESHOLD 35.0
#define HUMIDITY_THRESHOLD 80.0
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const char* apiKey = "L371856UH4NI7KMW";
unsigned long channelID = 3374921;
WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);
int readingCount = 0;
void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client); 
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}
void loop() {
  readingCount++;
  Serial.println("");
  Serial.print("Reading No: ");
  Serial.println(readingCount);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Sensor read error!");
    return;
  }
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" C  Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  if (temperature > TEMP_THRESHOLD)
    Serial.println("WARNING: High Temp!");
  else
    Serial.println("Temp: OK");
  if (humidity > HUMIDITY_THRESHOLD)
    Serial.println("WARNING: High Humidity!");
  else
    Serial.println("Humidity: OK");
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  int code = ThingSpeak.writeFields(channelID, apiKey);
  if (code == 200)
    Serial.println("Uploaded!");
  else
    Serial.println("Failed: " + String(code));
  delay(15000);
}
