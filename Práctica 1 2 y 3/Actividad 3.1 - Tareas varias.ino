// Actividad 3.1. Generar tareas

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

const char msg_1[30] = "Niños, ¿Quieren dulces? 7w7";
const char msg_2[35] = "Si capitan, queremos dulces";
const char msg_3[35] = "No los escuchooooo";

static TaskHandle_t  Tarea01 = NULL;
static TaskHandle_t  Tarea02 = NULL;
static TaskHandle_t  Tarea03 = NULL;


//Tarea UART 01
void TareaUART01(void *parameter){
  int msg_len = strnlen(msg_1,30);
  while(1){
    Serial.println();
    for(int i =0; i< msg_len; i++){
      Serial.print(msg_1[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    vTaskDelete(NULL);
  }
}

//Tarea UART 02
void TareaUART02(void *parameter){
  int msg_len = strnlen(msg_2,35);
  while(1){
    Serial.println();
    for(int i =0; i< msg_len; i++){
      Serial.print(msg_2[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//Tarea UART 03
void TareaUART03(void *parameter){
  int msg_len = strnlen(msg_3,35);
  while(1){
    Serial.println();
    for(int i =0; i< msg_len; i++){
      Serial.print(msg_3[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(300);
  xTaskCreatePinnedToCore(
    TareaUART01,  //Funcion que se manda llamar
    "UART 01",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    1,            //Prioridades
    &Tarea01,
    app_cpu);
  xTaskCreatePinnedToCore(
    TareaUART02,  //Funcion que se manda llamar
    "UART 02",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    1,            //Prioridades
    &Tarea02,
    app_cpu);
  xTaskCreatePinnedToCore(
    TareaUART03,  //Funcion que se manda llamar
    "UART 03",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    3,            //Prioridades
    &Tarea03,
    app_cpu);
}

void loop() {


  for(int i=0; i<5; i++){
    vTaskSuspend(Tarea02);
    vTaskDelay(2000/portTICK_PERIOD_MS);
    vTaskResume(Tarea02);
    vTaskDelay(2000/portTICK_PERIOD_MS);
  }
  vTaskSuspend(Tarea02);




}