// include the library code:
#include <LiquidCrystal.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 7   

#define DHTTYPE    DHT11    


// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int sensorPin = A0; 
int sensorValue;  
int limit = 300; 
int button = 0; 
int pumpPin = 10; 

void setup() {
  // set up the LCD's number of columns and rows:
  Serial.begin(9600); 
  pinMode(8, INPUT); 
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(10, OUTPUT);
  digitalWrite(pumpPin, HIGH);  
  lcd.begin(16, 2);
  // Print a message to the LCD.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

int volume = 0; 

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  sensorValue = analogRead(sensorPin); 
  Serial.print(F("Soil Moisture: "));
  Serial.println(sensorValue); 
  if (digitalRead(8) == 0) { 
    Serial.println("button pressed"); 
    if (button == 3) { 
      button = -1; 
    } 
    button += 1; 
    Serial.println(button); }
  if (button == 0) {
    if (sensorValue > 900) { 
      lcd.begin(16, 2); 
      lcd.clear(); 
      lcd.setCursor(0,0); 
      lcd.print("My status: "); 
      lcd.setCursor(0,1); 
      lcd.print("I NEED WATER :("); 
    }
    else {
      lcd.begin(16, 2); 
      lcd.clear(); 
      lcd.setCursor(0,0); 
      lcd.print("My status: "); 
      lcd.setCursor(0,1); 
      lcd.print("DOING WELL RN :)"); 
    }}  
  if (button == 1) {
    lcd.begin(16, 2); 
    lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print("Moisture Level:"); 
    lcd.setCursor(0,1); 
    lcd.print(sensorValue); 
  }
  if (button == 2) {
    lcd.begin(16, 2); 
    sensors_event_t event;
    lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print("Humid.: "); 
    dht.humidity().getEvent(&event);
    lcd.print(event.relative_humidity); 
    lcd.print(F("%"));
    lcd.setCursor(0,1); 
    lcd.print("Temp.: "); 
    dht.temperature().getEvent(&event);
    lcd.print(event.temperature); 
    lcd.print(F(" C"));
  }
  if (button == 3) {
    lcd.begin(16, 2); 
    lcd.clear(); 
    lcd.setCursor(0,0); 
    lcd.print("Sunlight Level: ");
    lcd.setCursor(0,1); 
    lcd.print(analogRead(A1)); 
  } 
  if (sensorValue > 900) { 
    if (volume == 0) { 
      lcd.begin(16,2); 
      lcd.print("WATERING NOW!"); 
      digitalWrite(pumpPin, LOW); 
      delay(1000);  
      digitalWrite(pumpPin, HIGH);  
      volume = 1; 
    }
  } 
  else {
    volume = 0; 
  } 
} 
 
