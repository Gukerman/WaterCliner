void HTTP_init(void) {

  HTTP.on("/configs.json", handle_ConfigJSON); // формирование configs.json страницы для передачи данных в web интерфейс
  // API для устройства
  HTTP.on("/ssidap", handle_Set_Ssidap); // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
  HTTP.on("/WATER", handle_Set_WATER);     // Установить длительность /WATER?ON1=ON1&ON2=ON2 и т.д.
  HTTP.on("/restart", handle_Restart);   // Перезагрузка модуля по запросу вида /restart?device=ok
  // Запускаем HTTP сервер
  HTTP.begin();

}

void handle_Set_WATER() {
   _ON1 = HTTP.arg("ON1").toInt();       // Получаем значение ON1 из запроса сохраняем в глобальной переменной
   _ON2 = HTTP.arg("ON2").toInt();       // Получаем значение ON2 из запроса сохраняем в глобальной переменной
   _OFF1 = HTTP.arg("OFF1").toInt();       // Получаем значение OFF1 из запроса сохраняем в глобальной переменной
   _OFF2 = HTTP.arg("OFF2").toInt();       // Получаем значение OFF2 из запроса сохраняем в глобальной переменной
  saveConfig();                        // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
    Serial.print("ON1 = ");
    Serial.println(_ON1);
    Serial.print("ON2 = ");
    Serial.println(_ON2);
/*    Serial.print("OFF1 = ");
    Serial.println(_OFF1);
    Serial.print("OFF2 = ");
    Serial.println(_OFF2);
*/
}

//Установка параметров внутренней точки доступа по запросу вида http://192.168.0.101/ssidap?ssidAP=home1&passwordAP=8765439 
void handle_Set_Ssidap() {              //
  _ssidAP = HTTP.arg("ssidAP");         // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
  _passwordAP = HTTP.arg("passwordAP"); // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
  saveConfig();                         // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}


// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart() {
  String restart = HTTP.arg("device");          // Получаем значение device из запроса
  if (restart == "ok") {                         // Если значение равно Ок
    HTTP.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
    ESP.restart();                                // перезагружаем модуль
  }
  else {                                        // иначе
    HTTP.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
  }
}

void handle_ConfigJSON() {
  String json = "{";  // Формировать строку для отправки в браузер json формат
  //{"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101"}
  // Имя SSDP
  json += "\"ON1\":\"";
  json += _ON1;
  // Имя сети
  json += "\",\"ON2\":\"";
  json += _ON2;
  // Пароль сети
  json += "\",\"OFF1\":\"";
  json += _OFF1;
  // Имя точки доступа
  json += "\",\"OFF2\":\"";
  json += _OFF2;
  // Имя точки доступа
  json += "\",\"ssidAP\":\"";
  json += _ssidAP;
  // Пароль точки доступа
  json += "\",\"passwordAP\":\"";
  json += _passwordAP;  
  json += "\"}";
  
  HTTP.send(200, "text/json", json);
}





