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
long _ON1 = 1800000;
long _OFF1 = 1800000;
long _ON2 = 60000;
long _OFF2 = 10000;
long _ON3 = 180000;
long Time1 = 0;
long Time2 = 0;

int X=1000;  // Множитель мкс для отладки
boolean flag1 = true;
boolean flag2 = true;
boolean Long = true;
boolean Full = true;


int FullPin = 12;  // Пин реле полного цикла
int LongPin = 14;  // Пин реле короткого цикла

String jsonConfig = "{}";

void setup() {
  Serial.begin(115200);
  Serial.println("");
  pinMode(FullPin, OUTPUT);
  pinMode(LongPin, OUTPUT);
  digitalWrite(FullPin, 0);        // выключаем реле   
  digitalWrite(LongPin, 0);        // выключаем реле  

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
  if (flag1)
  {
        if ((flag2==true)&&(millis()-Time2 > _ON2 )) 
          {
          Time2 = millis();
          Long = 1;
          //digitalWrite(LongPin, 1);        // то переключаем реле
          flag2 = false;
          }
          
        if ((flag2==false)&&(millis()-Time2 > _OFF2 )) 
          {
          Time2 = millis();
          Long = 0;
          //digitalWrite(LongPin, 0);        // то переключаем реле
          flag2 = true;
          }      
          
        if (millis()-Time1 > _ON1 )
          {
            Time1 = millis();
            Full = 1;
            //digitalWrite(FullPin, 1);        // то переключаем реле
            flag1 = false;
          }
          
        if (millis()-Time1 > _ON1 - _ON3) 
          {
          Long = 0;
          //digitalWrite(LongPin, 0);        // то переключаем реле
          }            
  }
  else   
  {
  if (millis()-Time1 > _OFF1 )
      {
        Time1 = millis();
        Full = 0;
        //digitalWrite(FullPin, 0);        // то переключаем реле
        flag1 = true;
      }    
      else
      {
       Long = 1;
       //digitalWrite(LongPin, 1);        // то переключаем реле
       flag2 = true;
      }
  }

 digitalWrite(LongPin, Long);        // то переключаем реле
 digitalWrite(FullPin, Full);        // то переключаем реле
  
}



