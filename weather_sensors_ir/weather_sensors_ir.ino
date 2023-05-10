#include <LiquidCrystal.h>
#include <dht_nonblocking.h>
#include "IRremote.h"

int receiver = 11; // Signal Pin of IR receiver to Arduino Digital Pin 11

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
// Set up the LCD module
LiquidCrystal lcd(7, 8, 9, 10, 13, 12);
// Set up the DHT11 sensor
#define DHT_SENSOR_PIN 2
#define DHT_SENSOR_TYPE DHT_TYPE_11
DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
// Set up the water level detection sensor
#define WATER_LEVEL_SENSOR_PIN A0
const int MAX_WATER_LEVEL = 530;

// Initialize the current mode to temperature
int current_mode = 1;

void translateIR() // takes action based on IR code received
{
  // Increment the current mode and wrap around if needed
  switch(results.value)
  {
  case 0xFFFFFFFF:
    Serial.println("REPEAT");
    if(current_mode == 3)
    {
      current_mode = 1;
    }
    else
    {
      current_mode += 1;
    }
  break;
  }
}

void setup() {
  Serial.begin(9600);  // Initialize the serial port for debugging
  lcd.begin(16, 2); // Initialize the LCD
  irrecv.enableIRIn(); // Start the receiver
}

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  float temperature;
  float humidity;
  int water_level;
  float water_percent;

  // Check for IR signal
  if (irrecv.decode(&results))
  {
    translateIR(); // Process the IR signal
    irrecv.resume(); // Receive the next value
  }


  // Measure temperature and humidity
  dht_sensor.measure(&temperature, &humidity);

  // Measure water level and calculate percentage
  water_level = analogRead(WATER_LEVEL_SENSOR_PIN);
  water_percent = (water_level * 100) / MAX_WATER_LEVEL;

  // Update LCD display based on current mode


  if (current_mode == 1)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temperature: ");
      lcd.setCursor(0, 1);
      lcd.print((int)temperature);
      lcd.print("C/");
      lcd.print((int)(temperature * 1.8 + 32));
      lcd.print("F");
    }
    
  if (current_mode == 2)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Humidity: ");
      lcd.setCursor(0, 1);
      lcd.print((int)humidity);
      lcd.print("%  ");  
      lcd.setCursor(0, 1);
    }
  if (current_mode == 3)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Water: ");
      lcd.setCursor(0, 1);
      lcd.print(water_percent);
      lcd.print("%");
      lcd.setCursor(0, 1);
    }
  delay(1000);
}