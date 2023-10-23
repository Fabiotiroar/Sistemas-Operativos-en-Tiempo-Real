#include <Adafruit_MPL3115A2.h>

//Telegram
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   
#include <ArduinoJson.h>

//RTOS
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


//const char* ssid = "Ale";
//const char* password = "rtyu1234.A";
//const char* ssid = "DESKTOP-O044E380279";
//const char* password = "3210*Bx0";
//Casa Diana
const char* ssid = "INFINITUM5D98_2.4";
const char* password = "3288259392";

#define BOTtoken "6336734125:AAGvOhkVyRD29YRL9CrbXhmxdKTFYRz21I8"  
#define CHAT_ID "1502768504"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

static TaskHandle_t BoidLoop=NULL;
/*-------------------------------------Definiciones---------*/
Adafruit_MPL3115A2 mpl;
float tempC=0.0;
const int ledPin = 4;
bool jalar = LOW;

/*---------------------------------------Tareas-------------*/
//Tarea que tiene todo lo que tenia el void loop
void Era_Void_Loop(void *parameter) {
    Serial.println("Starting a conversion.");
    mpl.startOneShot();

    // do something else while waiting
    Serial.println("Counting number while waiting...");
    int count = 0;
    while (!mpl.conversionComplete()) {
      count++;
    }
    Serial.print("Done! Counted to "); Serial.println(count);
    Serial.println("Aqui estoy :D");
    tempC=mpl.getLastConversionResults(MPL3115A2_TEMPERATURE);
    if(jalar==1){
      Serial.print("Temperature = ");
      Serial.println(tempC);
    }
    if (millis() > lastTimeBotRan + botRequestDelay)  {
      int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

      while(numNewMessages) {
        handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
      lastTimeBotRan = millis();
    }
  

    delay(250);
}

/*-------------------Telegram----*/
void handleNewMessages(int numNewMessages) {
  for (int i=0; i<numNewMessages; i++) {
    
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Bienvenida, " + from_name + ".\n";
      welcome += "Estas utilizando la version 2\n Utiliza los siguientes comandos para interactuar con el ESP32.\n\n";
      welcome += "/empezar para comenzar la lectura\n";
      welcome += "/parar para terinar la lectura\n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/empezar") {
      bot.sendMessage(chat_id, "Voy a empezar :D", "");
      jalar = HIGH;
    }
    
    if (text == "/parar") {
      bot.sendMessage(chat_id, "Ya me cayo UnU", "");
      jalar = LOW;
    }
  }
}

void setup(){
  Serial.begin(115200);

  //DEfinir entradas y salidas
  pinMode(ledPin, OUTPUT);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); 
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println(WiFi.localIP());

  //poner el sensor
  while(!Serial);
  Serial.println("Adafruit_MPL3115A2 test!");

  if (!mpl.begin()) {
    Serial.println("Could not find sensor. Check wiring.");
    while(1);
  }

  // set mode before starting a conversion
  Serial.println("Setting mode to barometer (pressure).");
  mpl.setMode(MPL3115A2_BAROMETER);
  
  //crea las tareas
  xTaskCreatePinnedToCore(      // Use xTaskCreate() in vanilla FreeRTOS
          Era_Void_Loop,        // Function to be called
          "BoidLoop",   // Name of task
          1024,               // Stack size (bytes in ESP32, words in FreeRTOS)
          NULL,               // Parameter to pass
          1,                  // Task priority
          &BoidLoop,               // Task handle
          0);           // Run on one core for demo purposes (ESP32 only)
}

void loop(){

}