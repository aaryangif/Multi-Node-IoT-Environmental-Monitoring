#include <WiFi.h>
#include <PubSubClient.h>

#define LIGHT_PIN 7 // GP7 (HW-486 S pin)

const char* SSID = "YOUR_WIFI_SSID";
const char* PASSWORD = "YOUR_WIFI_PASSWORD";
const char* MQTT_SERVER = "YOUR_BROKER_IP";
const int MQTT_PORT = 1883;

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  delay(2500);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LIGHT_PIN, INPUT);
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
    if (mqtt.connect("Pico2W_Node2")) {
      // Connected
    } else {
      delay(2000);
      return;
    }
  }

  mqtt.loop();

  if (millis() - lastSend >= 3000) {
    lastSend = millis();
    int val = digitalRead(LIGHT_PIN);
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", val);
    mqtt.publish("rooms/pico2/light", buf);
  }
}
