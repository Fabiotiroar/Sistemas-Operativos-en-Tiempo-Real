#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

const char msg[25] = "Estas usando tu cerebro ";
const char ID1[20] = "soy el lado derecho";
const char ID2[24] = "soy el lado izquierdo";
const char ID3[17] = "No tengo cerebro";
const char ID4[17] = "... Al chile sí";

static TaskHandle_t Tarea01 = NULL;
static TaskHandle_t Tarea02 = NULL;
static TaskHandle_t Tarea03 = NULL;
static TaskHandle_t Tarea04 = NULL;


bool banno = false;  //donde pregunta si est aocupada

void critico() {
  int msg_len = strnlen(msg,24);
  for (int i = 0; i < msg_len; i++) {
    Serial.print(msg[i]);
  }
}

//Tarea UART 01
void TareaUART01(void *parameter) {
  //paepadeo de led
  while (1) {
    if (banno == false) {
      banno = true;
      
          critico();
          int msg_texto = strnlen(ID1,24);
          for (int i = 0; i < msg_texto; i++) {
            Serial.print(ID1[i]);
          }
        Serial.println();
      
      banno = false;
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void TareaUART02(void *parameter) {
  while (1) {
    if (banno == false) {
      banno = true;
               critico();
          int msg_texto = strnlen(ID2,24);
          for (int i = 0; i < msg_texto; i++) {
            Serial.print(ID2[i]);
  }
      Serial.println();
      banno = false;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void TareaUART03(void *parameter) {
  while (1) {
    if (banno == false) {
      banno = true;
               critico();
          int msg_texto = strnlen(ID3,17);
          for (int i = 0; i < msg_texto; i++) {
            Serial.print(ID3[i]);
  }
      Serial.println();
      banno = false;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void TareaUART04(void *parameter) {
  while (1) {
    if (banno == false) {
      banno = true;
               critico();
          int msg_texto = strnlen(ID4,17);
          for (int i = 0; i < msg_texto; i++) {
            Serial.print(ID4[i]);
  }
      Serial.println();
      banno = false;
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(300);
  xTaskCreatePinnedToCore(
    TareaUART01,  //Funcion que se manda llamar
    "UART 01",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    1,  //Prioridades
    &Tarea01,
    app_cpu);
  xTaskCreatePinnedToCore(
    TareaUART02,  //Funcion que se manda llamar
    "UART 02",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    1,  //Prioridades
    &Tarea02,
    app_cpu);
  xTaskCreatePinnedToCore(
    TareaUART03,  //Funcion que se manda llamar
    "UART 03",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    1,  //Prioridades
    &Tarea03,
    app_cpu);
  xTaskCreatePinnedToCore(
    TareaUART04,  //Funcion que se manda llamar
    "UART 04",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    1,  //Prioridades
    &Tarea04,
    app_cpu);
}

void loop() {
  /*
  for (int i = 0; i < 5; i++) {
    vTaskSuspend(Tarea01);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    vTaskResume(Tarea01);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  */
}
