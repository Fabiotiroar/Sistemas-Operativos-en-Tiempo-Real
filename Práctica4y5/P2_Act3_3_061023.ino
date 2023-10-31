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
//Internet de Jashia
//const char* ssid = "Jashi";
//const char* password = "jashirun18.";

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
static const int Boton = 5;   //Boton     //Sin esto funciona unicamente el potenciomentro
//static const int LED01 = 4;   //Azul
static const int PIN_POT = 34;  //Potenciometro


int libre=1;
int turno=1;
bool jalar=LOW; //si esta prendida envia cosas
float lecpot =0;
int mano_pot=0;
int mano_Boton=0;         //Sin esto funciona unicamente el potenciomentro
bool lectura_boton=0;     //Sin esto funciona unicamente el potenciomentro
String mensage = "";
/////////////////////////////////////////////RTOS
void Potenciometro(void *parameter) {
  if (millis() > lastTimeBotRan + botRequestDelay) {              //Este IF estaba en el void loop y funcionaba el potenciometro, con una nueva tarea se desbordaba
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
  while (1) {
    mano_pot=1; //digo que quiero entrar a la seccion critica
    turno=2;    //Cedo el turno
    lecpot= analogRead(PIN_POT);
    if((libre==1)&&(turno==1)&&(mano_pot==1)){//si esta libre y es mi turno y quiero entrar
      mano_pot=0;
      escribirTelegram(1);
    }
    //digitalWrite(LED01, HIGH);
    //vTaskDelay(323 / portTICK_PERIOD_MS);
    //digitalWrite(LED01, LOW);
    //vTaskDelay(323 / portTICK_PERIOD_MS);
  }
}

void biton(void *parameter) {         //Sin esto funciona unicamente el potenciomentro y se tiene que poner el turno=1
  while (1) {
    lectura_boton=digitalRead(Boton);
    mano_Boton=1;     //pide entrar a la seccion critica
    turno=1;
    if((libre==1)&&(turno==2)&&(mano_Boton==1)){//si esta libre y es mi turno y quiero entrar
      mano_Boton=0;
      escribirTelegram(2);
    }
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
      String welcome = "Bienvenida mi vida, " + from_name + ".\n";
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
    }
    if(quien_llama==2){           //Esto es para el boton, sin esto funciona unicamente el potenciomentro
      ////valor_leido = String(buffer);
      leyo="El Valor del Boton es: ";
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
    /*if(turno==2){
      turno=1;
    }else{
      turno=turno+1;
    }*/
    libre=1;
  }
}

void setup() {
  // Initialize Wi-Fi, LED pins, and other necessary setup
  //Wire.begin();
  Serial.begin(115200);

  #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      
    client.setTrustAnchors(&cert); 
  #endif

  pinMode(PIN_POT, INPUT);
  pinMode(Boton, INPUT);
  //pinMode(LED01, OUTPUT);

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
  xTaskCreatePinnedToCore(Potenciometro, "Leer potenciometro", 8192, NULL, 2, NULL, app_cpu);
  xTaskCreatePinnedToCore(biton, "Conmutar LED 2", 8192, NULL, 2, NULL, app_cpu);
}

void loop() {
  // Additional code for any continuous operations
}
