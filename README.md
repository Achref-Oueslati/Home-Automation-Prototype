# Home-Automation-Prototype
This project enables home automation using an ESP32 with sensors and the Blynk app for real-time control. The project can monitor temperature, humidity, and light intensity and control a relay-connected device.

### Features
- Real-time monitoring with Blynk
- Temperature and humidity readings via DHT11
- Relay control through the Blynk app

### Setup Instructions
1. Add your Wi-Fi and Blynk credentials in `credentials.h`.
2. Connect the DHT sensor and relay to GPIO 4 and 5, respectively.
3. Upload the code to your ESP32 using PlatformIO.

### Libraries
- Blynk Library
- DHT Sensor Library
