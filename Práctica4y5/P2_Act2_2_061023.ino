//Peterson

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static TaskHandle_t Ale8 = NULL;
static TaskHandle_t Diana4 = NULL;

bool manoAle = LOW;
bool manoDiana = LOW;

static const int LED01 = 4;
static const int LED02 = 2;
static const int LED03 = 15;

int turno = 0;

const char msg[10]= "AASFGDELA"; 
const char msgD[15]= "arawarieooowri"; //"buenas tardes" en idioma zombie

void imprimir(int quien){
  if (quien==0){
   int msg_texto = strnlen(msg,10);
          for (int i = 0; i < msg_texto; i++) {
            Serial.print(msg[i]);
          }
          Serial.println("");
  }
  else{
    int msg_texto = strnlen(msgD,15);
          for (int i = 0; i < msg_texto; i++) {
            Serial.print(msgD[i]);
          }
          Serial.println("");
  }
}

//Tarea UART 01
void Ale(void *parameter) {
  while (1) {
    manoAle = HIGH;
    turno=1;
    while (manoDiana == HIGH&&turno==1) {
      digitalWrite(LED02, HIGH);
      digitalWrite(LED03, HIGH);
      vTaskDelay(323 / portTICK_PERIOD_MS);
      digitalWrite(LED02, LOW);
      digitalWrite(LED03, LOW);
      vTaskDelay(323 / portTICK_PERIOD_MS);
    }
        imprimir(0);
        manoAle=LOW;
        vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}


void Diana (void *parameter) {
while (1) {
    manoDiana = HIGH;
    turno=0;
    while (manoAle == HIGH&&turno==0) {
      digitalWrite(LED01, HIGH);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      digitalWrite(LED01, LOW);
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
        imprimir(1);
        manoDiana=LOW;
        vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  pinMode(LED01, OUTPUT);
  pinMode(LED02, OUTPUT);
  pinMode(LED03, OUTPUT);
  Serial.begin(300);
  xTaskCreatePinnedToCore(
    Ale,  //Funcion que se manda llamar
    "Ale",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    1,  //Prioridades
    &Ale8,
    app_cpu);
  xTaskCreatePinnedToCore(
    Diana,  //Funcion que se manda llamar
    "Diana",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    1,  //Prioridades
    &Diana4,
    app_cpu);

}

void loop() {
  // put your main code here, to run repeatedly:

}

