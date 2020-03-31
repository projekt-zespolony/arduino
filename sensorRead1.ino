#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

// Wyjscie cyfrowe czujnika do pinu 2
// Wyjscie analogowe do A0
// ad_value odczyt z A/C
int gas_din=2;
int gas_ain=A0;
int ad_value;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println(F("BME680 test"));

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);

  pinMode(gas_din,INPUT);
  pinMode(gas_ain,INPUT);
  
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void loop() {
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  // Odczyt temperatury: bme.temperature
  // Odczyt cisnienia: bme.pressure
  // Odczyt wilgotnosci: bme.humidity
  // Odczyt gazów: bme.gas_resistance
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

  Serial.print("Gas = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");


  // Jeżeli wystąpi wyciek gazu to powiadom o poziomie
  ad_value=analogRead(gas_ain);
  if(digitalRead(gas_din)==LOW)
  {
    Serial.println("Gas leakage");
    Serial.println(ad_value);
  }
  else
  {
    Serial.println("Gas not leak");
  }

  Serial.println();
  delay(2000);
}
