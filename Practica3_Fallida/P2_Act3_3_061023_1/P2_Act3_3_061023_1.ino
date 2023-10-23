// Definicion de librerias
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   
#include <ArduinoJson.h>
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif
#include <Adafruit_MPL3115A2.h> //libreria del sensor
#include <Wire.h>
#include <Adafruit_Sensor.h> //libreria que tiene marlon

//Internet celular Ale
//const char* ssid = "Ale";
//const char* password = "rtyu1234.A";
//Internet profe
//const char* ssid = "IPN_UPIIZ";
//const char* password = "Electronica123.";
//Casa Diana
const char* ssid = "INFINITUM5D98_2.4";
const char* password = "3288259392";

#define BOTtoken "6336734125:AAGvOhkVyRD29YRL9CrbXhmxdKTFYRz21I8"  
#define CHAT_ID "1502768504"

/*
#ifdef ESP8266 //libreria para leer tiempo, ahorita no la ocupamos
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif
*/
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

//handlers de tareas
TaskHandle_t Sensor1;
TaskHandle_t Sensor2;
TaskHandle_t Escritura;
TaskHandle_t Loopi;
//Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();//Definimos las costantes de lectura del sensor


// Define LED01, LED02, and other variables and constants
//static const int LED01 = 5;   //Verde
//static const int LED02 = 4;   //Azul
static const int PIN_POT = 34;  //Potenciometro


int libre=1; //si la funcion critica esta libre
int turno=1; //turno de quien esta en la funcion critica
bool jalar=LOW; //si esta prendida envia cosas
float lecpot =0; //lectura potenciometro
float tempC=0; //lectura temperatura
int mano_pot=0; //bandera potenciometro
int mano_temp=0; //bandera sensor
String mensage = "";

//////////////////////////////////////////////////////////////////////////////////RTOS
static void Antes_Void_Loop(void *parameter){
  //esto es todo lo que estaba en el void loop
  //tempC = baro.getTemperature();

  //para que reciba mensajes de telegram
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
  // Additional code for any continuous operations
  //vTaskDelay(500/portTICK_PERIOD_MS);
}

void Potenciometro(void *parameter) {
  while (1) {
    mano_pot=1; //digo que quiero entrar a la seccion critica
    turno=1; //sede el turno al siguiente
    lecpot= analogRead(PIN_POT);

    //SECCION CRITICA
    if((libre==1)&&(turno==1)&&(mano_pot==1)){//si esta libre y es mi turno y quiero entrar
      escribirTelegram(1); //llamo funcion
      mano_pot=0; //bajo a mano
    }

    vTaskDelay(250/portTICK_PERIOD_MS); //espera un ratito
  }
}
/*
void temperatura(void *parameter) {
  while (1) {
    mano_temp=1; //quiere entrar seccion critica  
    turno=1; //le cedo el turno al otro 
    //seccion critica
    if((libre==1)&&(turno==2)&&(mano_temp==1)){//si esta libre y es mi turno y quiero entrar
      //escribirTelegram(2);
      mano_temp=0;
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
*/
///////////////////////////////////////////////telegram
void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Bienvenido, " + from_name + ".\n";
      welcome += "Utilice los siguientes comando para el muestreo hecho con el ESP32.\n\n";
      welcome += "/prender para leer mensajes\n";
      welcome += "/apagar para dejar de leer mensajes\n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/prender") {
      jalar=HIGH;
      bot.sendMessage(chat_id, "VOY A EMPEZAR :D", "");
    }
    
    if (text == "/apagar") {
      jalar=LOW;
      bot.sendMessage(chat_id, "Ya me cayo UnU", "");
    }
    /*
    if (text == "/state") {
      if (digitalRead(ledPin)){
        bot.sendMessage(chat_id, "LED esta encendido", "");
      }
      else{
        bot.sendMessage(chat_id, "Ay do babes", "");
      }
    }
    if (text == "/tasbien") {
      bot.sendMessage(chat_id, "ño, el LED se mudio", "");
      
    }
    */
  }
}

void escribirTelegram(int quien_llama) { //seccion critica
  if(jalar==HIGH){
    libre=0;
    int i=0;
    String chat_id = String(bot.messages[i].chat_id);
    char buffer[20];
    String valor_leido=""; //limpia variables
    String leyo=""; //limpia variables

    //mensaje a mandar
    if(quien_llama==1){//es el potenciometro
      dtostrf(lecpot, 5, 2, buffer); // 5 caracteres en total, 2 decimales, float a string
      valor_leido = String(buffer);
      leyo="El Valor del Potenciometro es: ";
    }else if(quien_llama==2){//si es el sensor
      dtostrf(tempC, 5, 2, buffer); // 5 caracteres en total, 2 decimales
      valor_leido = String(buffer);
      leyo="El Valor del Sensor een grados es: ";
    }

    //envia el mensaje
    mensage=leyo+valor_leido;
    bot.sendMessage(chat_id, mensage, "");
    libre=1;
  }
}

void setup() {
  // Initialize uart
  Wire.begin();
  Serial.begin(115200);
  pinMode(PIN_POT, INPUT);

  //conectas al internet
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
  //vTaskDelay(1000 / portTICK_PERIOD_MS); //retardo para que no haya problema con el internet

  //este es para saber la hora
  /*
  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      
    client.setTrustAnchors(&cert); 
  #endif
  */
  
  //pinMode(LED02, OUTPUT);
  //pinMode(LED01, OUTPUT);
  xTaskCreatePinnedToCore(
        Antes_Void_Loop,
        "Void loop",
        4096,
        NULL,
        1,
        &Loopi,
        NULL); //pin task to core 0
  //xTaskCreatePinnedToCore(escribirTelegram, "Leer potenciometro", 4096, NULL, 2, &Sensor1, NULL);
  xTaskCreatePinnedToCore(
          Potenciometro, 
          "Leer potenciometro",
          4096,
          NULL,
          1,
          &Sensor1,
          NULL);
 /* xTaskCreatePinnedToCore(
          temperatura,
          "Leer barometro",
          4096,
          NULL,
          1,
          &Sensor2,
          NULL);
  */
}

void loop(){
}



