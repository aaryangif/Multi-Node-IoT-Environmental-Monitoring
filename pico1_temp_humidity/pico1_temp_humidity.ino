#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 7      // GP7
#define DHTTYPE DHT11 // HW-507 Temperature & Humidity Sensor

const char* SSID = "YOUR_WIFI_SSID";
const char* PASSWORD = "YOUR_WIFI_PASSWORD";
const char* MQTT_SERVER = "YOUR_BROKER_IP";
const int MQTT_PORT = 1883;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  delay(2500);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(500);
  }
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }

  if (!mqtt.connected()) {
    if (mqtt.connect("Pico2W_Node1")) {
      // Connected
    } else {
      delay(2000);
      return;
    }
  }

  mqtt.loop();

  if (millis() - lastSend >= 3000) {
    lastSend = millis();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) return;

    char tempBuf[12];
    char humBuf[12];
    snprintf(tempBuf, sizeof(tempBuf), "%.1f", t);
    snprintf(humBuf, sizeof(humBuf), "%.1f", h);

    mqtt.publish("rooms/pico1/temp", tempBuf);
    mqtt.publish("rooms/pico1/humidity", humBuf);
  }
}
