/*
   Шаг №7
   Запись и чтение параметров конфигурации в файл

*/
#include <ESP8266WiFi.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step1-wifi
#include <ESP8266WebServer.h>   //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step2-webserver
#include <ESP8266SSDP.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step3-ssdp
#include <FS.h>                 //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step4-fswebserver
//                    ПЕРЕДАЧА ДАННЫХ НА WEB СТРАНИЦУ. Видео с уроком http://esp8266-arduinoide.ru/step5-datapages/
//                    ПЕРЕДАЧА ДАННЫХ С WEB СТРАНИЦЫ.  Видео с уроком http://esp8266-arduinoide.ru/step6-datasend/
#include <ArduinoJson.h>        //Установить из менеджера библиотек.
IPAddress apIP(192, 168, 4, 1);

// Web интерфейс для устройства
ESP8266WebServer HTTP(80);
// Для файловой системы
File fsUploadFile;

// Определяем переменные wifi
String _ssid     = "home"; // Для хранения SSID
String _password = "i12345678"; // Для хранения пароля сети
String _ssidAP = "WaterCliner";   // SSID AP точки доступа
String _passwordAP = "89872595699"; // пароль точки доступа
//String SSDP_Name = "SendToWeb"; // Имя SSDP
//int timezone = 3;               // часовой пояс GTM
long _ON1 = 1800;
long _OFF1 = 1800;
long _ON2 = 30;
long _OFF2 = 30;
long Time1 = 0;
long Time2 = 0;
int X=1000;  // Множитель мкс для отладки

int FullPin = 12;  // Пин реле полного цикла
int LongPin = 14;  // Пин реле короткого цикла

String jsonConfig = "{}";

void setup() {
  Serial.begin(115200);
  Serial.println("");
  pinMode(FullPin, OUTPUT);
  pinMode(LongPin, OUTPUT);
  
  //Запускаем файловую систему
  Serial.println("Start 4-FS");
  FS_init();
  Serial.println("Step7-FileConfig");
  loadConfig();
  Serial.println("Start 1-WIFI");
  //Запускаем WIFI
  WIFIinit(); //StartAPMode();
  //Настраиваем и запускаем SSDP интерфейс
  Serial.println("Start 3-SSDP");
//  SSDP_init();
  //Настраиваем и запускаем HTTP интерфейс
  Serial.println("Start 2-WebServer");
  HTTP_init();

}

void loop() {
  HTTP.handleClient();
  delay(1);
  // Full cycle
  if(millis()-Time1 > _ON1*X) 
  {
    Time1 = millis();
    digitalWrite(FullPin, !digitalRead(FullPin));        // то переключаем реле
  }
  // Long cycle
  if(millis()-Time2 > _ON2*X) 
  {
    Time2 = millis();
    digitalWrite(LongPin, !digitalRead(LongPin));        // то переключаем реле
  }        
}



