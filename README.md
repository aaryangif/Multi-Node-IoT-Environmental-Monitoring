# Multi-Node IoT Environmental Monitoring System (MQTT + Pico 2 W + ESP32-S3)

Two Raspberry Pi Pico 2 W nodes publish sensor telemetry over Wi-Fi (YOUR_WIFI_SSID) to a Mosquitto MQTT broker (YOUR_BROKER_IP:1883), and an ESP32-S3 gateway subscribes to `rooms/#` and logs messages over USB Serial at 115200 baud.

## Wiring (3.3 V Logic Only)
- **Pico 1 + HW-507 (Temp/Humidity):** VCC/+ -> 3V3(OUT) (Pin 36), GND/- -> GND, DATA/S -> GP7 (`rooms/pico1/temp`, `rooms/pico1/humidity`)
- **Pico 2 + HW-486 (Light Sensor):** + -> 3V3(OUT) (Pin 36), - -> GND, S -> GP7 (`rooms/pico2/light`)
- **ESP32-S3 Gateway:** Subscribes to `rooms/#` on `YOUR_BROKER_IP:1883` and prints to Serial Monitor at 115200 baud
