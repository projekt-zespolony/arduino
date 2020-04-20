void setup()  {
  Serial.begin(9600);
  Serial1.begin(9600);
  connectWiFi();
}
void loop()  {
 while(Serial1.available())
  {
    Serial.write(Serial1.read());
  }
  while(Serial.available())
  {
    Serial1.write(Serial.read());
  }
  
}

void connectWiFi(void)
{
 sendData("AT+RST\r\n", 2000); // reset
 sendData("AT+CWMODE=1\r\n", 1000);
 sendData("AT+CWJAP=\"NETIASPOT-AA5E40\",\"cokolwiek123\"\r\n", 2000); //polacz do sieci
 delay(5000);
 sendData("AT+CIFSR\r\n", 1000); // pokaz adres
 Serial.println("8266 Connected");
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
