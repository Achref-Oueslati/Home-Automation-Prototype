#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
// Define pins for sensors and modules
#define DHTPIN 4            // GPIO for DHT sensor
#define DHTTYPE DHT11       // DHT sensor type (DHT11)
#define GAS_SENSOR_PIN 36   // GPIO for Gas/Fire sensor (analog input)
#define PIR_SENSOR_PIN 34   // GPIO for motion sensor (PIR sensor)
#define RELAY_LIGHT_PIN 5   // GPIO for light control (relay)
#define RELAY_GARAGE_PIN 18 // GPIO for garage control (relay)
#define BUZZER_PIN 19       // GPIO for buzzer alarm
#define WIFI_SSID "**"
#define WIFI_PASSWORD "***"
#define BLYNK_AUTH_TOKEN "****"

// Initialize sensors
DHT dht(DHTPIN, DHTTYPE);
bool isPersonPresent = false;

// Gas/Fire threshold level
const int GAS_THRESHOLD = 500; // Adjust based on the sensor and environment

void setup() {
    Serial.begin(9600);
    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);
    
    dht.begin();
    pinMode(RELAY_LIGHT_PIN, OUTPUT);
    pinMode(RELAY_GARAGE_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(PIR_SENSOR_PIN, INPUT);
    pinMode(GAS_SENSOR_PIN, INPUT);

    digitalWrite(RELAY_LIGHT_PIN, LOW);
    digitalWrite(RELAY_GARAGE_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
    Blynk.run();
    
    // Read temperature and humidity
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    // Read gas/fire sensor value
    int gasLevel = analogRead(GAS_SENSOR_PIN);
    
    // Motion sensor status
    bool motionDetected = digitalRead(PIR_SENSOR_PIN);

    // Update Blynk with sensor data
    Blynk.virtualWrite(V1, temperature);
    Blynk.virtualWrite(V2, humidity);
    Blynk.virtualWrite(V3, gasLevel);
    
    // Temperature and Humidity Monitoring
    if (!isnan(temperature) && !isnan(humidity)) {
        Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
        Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
    }

    // Gas/Fire Detection and Alarm Trigger
    if (gasLevel > GAS_THRESHOLD) {
        Serial.println("High gas/fire levels detected!");
        digitalWrite(BUZZER_PIN, HIGH);  // Activate alarm
    } else {
        digitalWrite(BUZZER_PIN, LOW);   // Deactivate alarm
    }

    // Motion-Based Light Control
    if (motionDetected && !isPersonPresent) {
        isPersonPresent = true;
        digitalWrite(RELAY_LIGHT_PIN, HIGH);  // Turn on light
        Serial.println("Person detected, light on.");
    } else if (!motionDetected && isPersonPresent) {
        isPersonPresent = false;
        digitalWrite(RELAY_LIGHT_PIN, LOW);   // Turn off light
        Serial.println("No person detected, light off.");
    }
}

// Blynk button to control garage door relay
BLYNK_WRITE(V4) {
    int garageControl = param.asInt();
    digitalWrite(RELAY_GARAGE_PIN, garageControl ? HIGH : LOW);
}
