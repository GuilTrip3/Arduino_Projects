/*
  DHT11 example for Arduino
  - reads temperature (°C) and humidity (%)
  - prints values to Serial Monitor
  - data pin on A0
  - uses the Adafruit DHT library
*/

#include <DHT.h>          // <-- Make sure the library is installed

// Pin connected to the DHT11 data pin
#define DHTPIN 11          // or 14 – A0 is equivalent to digital pin 14

// Choose the sensor type: DHT11 or DHT22
#define DHTTYPE DHT11

// Create DHT object
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHT11 Test");

  dht.begin();           // Initialise the sensor
}

void loop() {
  // DHT11 requires at least 2 s between readings
  delay(2000);

  // Read humidity
  float h = dht.readHumidity();

  // Read temperature in Celsius
  float t = dht.readTemperature(); // (t = dht.readTemperature(true);) for °F

  // Check if any reads failed
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;               // Skip printing
  }

  // Print the results
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" oC");
}
