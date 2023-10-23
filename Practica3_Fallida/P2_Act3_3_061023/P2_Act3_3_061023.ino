// Include necessary libraries and define constants for LED pins and Wi-Fi credentials
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


//const char* ssid = "Ale";
//const char* password = "rtyu1234.A";
const char* ssid = "DESKTOP-O044E380279";
const char* password = "3210*Bx0";
//const char* ssid = "NOTEMETAS2.0";
//const char* password = "peth3803";

#define BOTtoken "6336734125:AAGvOhkVyRD29YRL9CrbXhmxdKTFYRz21I8"  
#define CHAT_ID "1502768504"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


int botRequestDelay = 1000;
unsigned long lastTimeBotRan;
// Define LED01, LED02, and other variables and constants
static const int Boton = 5;   //Boton
static const int LED01 = 4;   //Azul
static const int PIN_POT = 34;  //Potenciometro


int libre=1;
int turno=1;
bool jalar=LOW; //si esta prendida envia cosas
float lecpot =0;
int mano_pot=0;
int mano_Boton=0;
bool lectura_boton=0;
String mensage = "";
/////////////////////////////////////////////RTOS
void Potenciometro(void *parameter) {
  while (1) {
    mano_pot=1; //digo que quiero entrar a la seccion critica
    lecpot= analogRead(PIN_POT);
    digitalWrite(LED01, HIGH);
    vTaskDelay(323 / portTICK_PERIOD_MS);
    digitalWrite(LED01, LOW);
    vTaskDelay(323 / portTICK_PERIOD_MS);
  }
}

void biton(void *parameter) {
  while (1) {
    lectura_boton=digitalRead(Boton);
    mano_Boton=1;

  }
}

///////////////////////////////////////////////7telegram
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
    String valor_leido=""; 
    String leyo=""; 
    //mensaje a mandar
    if(quien_llama==1){//es el potenciometro
      dtostrf(lecpot, 5, 2, buffer); // 5 caracteres en total, 2 decimales
      valor_leido = String(buffer);
      leyo="El Valor del Potenciometro es: ";
      mensage=leyo+valor_leido;
    }else if(quien_llama==2){
      ////valor_leido = String(buffer);
      leyo="El Valor del Sensor es: ";
      if(lectura_boton==HIGH){
        valor_leido = "alto";
      }
      else{
        valor_leido = "bajo";
      }
      mensage=leyo+valor_leido;
    }

    //envia el mensaje
    bot.sendMessage(chat_id, mensage, "");
    delay(1000);
    if(turno==2){
      turno=1;
    }else{
      turno=turno+1;
    }
    libre=1;
  }
}

void setup() {
  // Initialize Wi-Fi, LED pins, and other necessary setup
  Wire.begin();
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      
    client.setTrustAnchors(&cert); 
  #endif

  pinMode(PIN_POT, INPUT);
  pinMode(Boton, INPUT);
  pinMode(LED01, OUTPUT);

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

  // Start LED control tasks
  xTaskCreatePinnedToCore(Potenciometro, "Leer potenciometro", 4096, NULL, 2, NULL, app_cpu);
  xTaskCreatePinnedToCore(biton, "Conmutar LED 2", 4096, NULL, 2, NULL, app_cpu);
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
  // Additional code for any continuous operations
    if((libre==1)&&(turno==1)&&(mano_pot==1)){//si esta libre y es mi turno y quiero entrar
      mano_pot=0;
      escribirTelegram(1);
    }
    if((libre==1)&&(turno==2)&&(mano_Boton==1)){//si esta libre y es mi turno y quiero entrar
      mano_Boton=0;
      escribirTelegram(2);
    }

}

