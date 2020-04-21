#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

double temp=100;
double pressure=100;
double hum=100;
double gas=100;

int gas_din=2;
int gas_ain=A0;
int ad_value;

void connectWiFi(void)
{
 sendData("AT+RST\r\n", 2000); // reset
 sendData("AT+CWMODE=1\r\n", 1000);
 sendData("AT+CWJAP=\"NETIASPOT-AA5E40\",\"cokolwiek123\"\r\n", 2000); //polacz do sieci
 delay(5000);
 sendData("AT+CIFSR\r\n", 1000); // pokaz adres
 Serial.println("8266 Connected");
}

void polaczenieTCP(void)
{
  sendData("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", 2000); //polacz do sieci
  String cmd = "GET /update?api_key=WRE39SB8YDIRGC88&field1=";
 cmd+=String(temp)+"&field2="+String(pressure)+"&field3="+String(hum)+"&field4="+String(gas)+"\r\n\r\n";
 Serial.println(cmd);
 sendData("AT+CIPSEND=" +String(cmd.length()+4)+"\r\n",1000);
 Serial1.find(">"); 
 Serial1.print(cmd);
   sendData("AT+CIPCLOSE=0",1000);
}

String sendData(String command, const int timeout)
{
    String response = "";
        
    Serial1.print(command); // wysłanie polecenia do ESP01
        
    long int time = millis();
        
    while( (time+timeout) > millis())
    {
      while(Serial1.available()) //jeśli są jakieś dane z modułu, wtedy następuje ich odczyt
      {
        char c = Serial1.read(); // odczyt kolejnego znaku
        response += c;
      }
    }
    Serial.print(response);
    return response;
}

void setup()  {
  Serial.begin(9600);
  Serial1.begin(9600);
 
   if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);

  pinMode(gas_din,INPUT);
  pinMode(gas_ain,INPUT);
}
  
  connectWiFi(); 
  delay(5000); 
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}





void loop()  {
if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  // Odczyt temperatury: bme.temperature
  // Odczyt cisnienia: bme.pressure
  // Odczyt wilgotnosci: bme.humidity
  // Odczyt gazów: bme.gas_resistance
  
  temp=bme.temperature;
  pressure=bme.pressure / 100.0;
  hum=bme.humidity;
  gas=bme.gas_resistance / 1000.0;
  

  polaczenieTCP();


  // Jeżeli wystąpi wyciek gazu to powiadom o poziomie
//  ad_value=analogRead(gas_ain);
//  if(digitalRead(gas_din)==LOW)
//  {
//    Serial.println("Gas leakage");
//    Serial.println(ad_value);
//  }
//  else
//  {
//    Serial.println("Gas not leak");
//  }

  Serial.println();
  delay(10000);

  
  
}
