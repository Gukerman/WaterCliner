// Загрузка данных сохраненных в файл  config.json
bool loadConfig() {
  // Открываем файл для чтения
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
  // если файл не найден  
    Serial.println("Failed to open config file");
  //  Создаем файл запиав в него аные по умолчанию
    saveConfig();
    return false;
  }
  // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

// загружаем файл конфигурации в глобальную переменную
  jsonConfig = configFile.readString();
  // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
    DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  //  строку возьмем из глобальной переменной String jsonConfig
    JsonObject& root = jsonBuffer.parseObject(jsonConfig);
  // Теперь можно получить значения из root  
    _ssidAP = root["ssidAPName"].as<String>(); // Так получаем строку
    _passwordAP = root["ssidAPPassword"].as<String>();
      
    _ON1 = root["ON1"];               // Так получаем число
    _ON2 = root["ON2"];               // Так получаем число
    _ON3 = root["ON3"];               // Так получаем число
    _OFF1 = root["OFF1"];               // Так получаем число
    _OFF2 = root["OFF2"];               // Так получаем число
    _ON1 = _ON1 * X;
    _ON2 = _ON2 * X;
    _ON3 = _ON3 * X;
    _OFF1 = _OFF1 * X;
    _OFF2 = _OFF2* X;
    return true;
    

}

// Запись данных в файл config.json
bool saveConfig() {
  // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.parseObject(jsonConfig);
  // Заполняем поля json 
  json["ssidAPName"] = _ssidAP;
  json["ssidAPPassword"] = _passwordAP;
    
  json["ON1"] = _ON1/X;  
  json["ON2"] = _ON2/X;  
  json["ON3"] = _ON3/X;  
  json["OFF1"] = _OFF1/X;  
  json["OFF2"] = _OFF2/X;  
  
  // Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
  json.printTo(jsonConfig);
  // Открываем файл для записи
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    //Serial.println("Failed to open config file for writing");
    return false;
  }
  // Записываем строку json в файл 
  json.printTo(configFile);
  return true;
  }


