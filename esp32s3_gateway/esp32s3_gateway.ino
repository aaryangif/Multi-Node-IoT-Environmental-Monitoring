#include <WiFi.h>
#include <PubSubClient.h>

const char* SSID = "YOUR_WIFI_SSID";
const char* PASSWORD = "YOUR_WIFI_PASSWORD";
const char* MQTT_SERVER = "YOUR_BROKER_IP";
const int MQTT_PORT = 1883;

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.print("Gateway [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(msg);
}

void connectWiFiAndMQTT() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }

  while (!mqtt.connected()) {
    if (mqtt.connect("ESP32S3_Gateway")) {
      mqtt.subscribe("rooms/#");
    } else {
      delay(500);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1500);
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(onMqttMessage);
}

void loop() {
  connectWiFiAndMQTT();
  mqtt.loop();
}
