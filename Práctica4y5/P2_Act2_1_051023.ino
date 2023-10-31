//Decker
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

int turno = 1;

const char msg[9]= "Ya Salii";
const char msgD[30]= "Yaa llegueee.....yo tambieeen";

void imprimir(int quien){
  if (quien==1){
   int msg_texto = strnlen(msg,9);
          for (int i = 0; i < msg_texto; i++) {
            Serial.print(msg[i]);
          }
          Serial.println("");
          turno=2;
  }
  else{
    int msg_texto = strnlen(msgD,30);
          for (int i = 0; i < msg_texto; i++) {
            Serial.print(msgD[i]);
          }
          Serial.println("");
          turno=1;
  }
}

//Tarea UART 01
void Ale(void *parameter) {
  while (1) {
    manoAle = HIGH;

    if (manoDiana == HIGH) {
      if (turno == 1){
        imprimir(1);
        manoAle=LOW;
      }
      else{
        vTaskDelay(100 / portTICK_PERIOD_MS);
      }
  }
    else {
      if(turno==1){
        imprimir(1);
        manoAle=LOW;
      }
    }        
  }
}


void Diana (void *parameter) {
  while (1) {
    digitalWrite(LED01, HIGH);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      digitalWrite(LED01, LOW);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      
      manoDiana = HIGH;

    if (manoAle == HIGH) {
      if (turno == 2){
          imprimir(2);
          manoDiana=LOW;
      }
    else{
      }
  }
    else {
      if(turno==2){
      imprimir(2);
      manoDiana=LOW;
      }
    } 
  }
}

void setup() {
  pinMode(LED01, OUTPUT);
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
