#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 19
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "iPhone";
const char* password = "12345678";

const String serverUrl = "http://172.20.10.3:5000/dht11";

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C ");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);

    String jsonData = "{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + "}";

    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server response: " + response);
    } else {
      Serial.println("Error in sending POST request");
    }

    http.end();
  }

  delay(10000);
}
