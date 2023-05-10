#include <LiquidCrystal.h>
#include <dht_nonblocking.h>

// Set up the LCD module
LiquidCrystal lcd(7, 8, 9, 10, 13, 12);

// Set up the DHT11 sensor
#define DHT_SENSOR_PIN 2
#define DHT_SENSOR_TYPE DHT_TYPE_11
DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// Set up the water level detection sensor
#define WATER_LEVEL_SENSOR_PIN A0
const int MAX_WATER_LEVEL = 530;

void setup() {
  // Initialize the LCD
  lcd.begin(16, 2);

  // Initialize the serial port for debugging
  Serial.begin(9600);
}

void loop() {
  // Read temperature and humidity from the DHT11 sensor
  float temperature;
  float humidity;
  if (dht_sensor.measure(&temperature, &humidity)) {
    // Read water level from the sensor
    int water_level = analogRead(WATER_LEVEL_SENSOR_PIN);

    // Display temperature, humidity, and water level on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperature: ");
    lcd.setCursor(0, 1);
    lcd.print((int)temperature);
    lcd.print("C/");
    lcd.print((int)(temperature * 1.8 + 32));
    lcd.print("F");
    delay(6000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Humidity: ");
    lcd.setCursor(0, 1);
    lcd.print((int)humidity);
    lcd.print("%  ");
    delay(6000);
    lcd.clear();
    lcd.setCursor(0, 0);
    int water_percent = (water_level * 100) / MAX_WATER_LEVEL;
    lcd.print("Water: ");
    lcd.setCursor(0, 1);
    lcd.print(water_percent);
    lcd.print("%");
    delay(6000);
    // Print temperature, humidity, and water level to the serial port
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("C/");
    Serial.print(temperature * 1.8 + 32);
    Serial.print("F, Humidity: ");
    Serial.print(humidity);
    Serial.print("%, Water: ");
    Serial.print(water_percent);
    Serial.print("% (");
    Serial.print(water_level);
    Serial.println(")");

    delay(1000);
  }
}
