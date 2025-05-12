#include <WiFi.h>
#include <FirebaseESP32.h>
#include "DHT.h"

// Replace with your network credentials
#define WIFI_SSID "Top Niveau"
#define WIFI_PASSWORD "topniveau2017"

// Replace with your Firebase project credentials
#define FIREBASE_HOST "https://bracelelt-io-default-rtdb.firebaseio.com/"//"your-project-id.firebaseio.com"
#define FIREBASE_AUTH "nvotaBT5vxbofYqFq6X3kjNnnfYFzlfJUeCoLJdC"//"your-database-secret"  // Or Firebase API key for new SDK

// DHT Sensor
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Firebase object
FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected.");

  // Connect to Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  delay(2000);

  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();
  float temperatureF = dht.readTemperature(true);

  if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
    Serial.println("Failed to read from DHT11 sensor!");
    return;
  }

  float heatIndexC = dht.computeHeatIndex(temperatureC, humidity, false);
  float heatIndexF = dht.computeHeatIndex(temperatureF, humidity);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperatureC);
  Serial.print("°C ");
  Serial.print(temperatureF);
  Serial.print("°F  Heat Index: ");
  Serial.print(heatIndexC);
  Serial.print("°C ");
  Serial.print(heatIndexF);
  Serial.println("°F");

  // Send data to Firebase
  Firebase.setFloat(firebaseData, "/DHT11/Humidity", humidity);
  Firebase.setFloat(firebaseData, "/DHT11/TemperatureC", temperatureC);
  Firebase.setFloat(firebaseData, "/DHT11/TemperatureF", temperatureF);
  Firebase.setFloat(firebaseData, "/DHT11/HeatIndexC", heatIndexC);
  Firebase.setFloat(firebaseData, "/DHT11/HeatIndexF", heatIndexF);
}
