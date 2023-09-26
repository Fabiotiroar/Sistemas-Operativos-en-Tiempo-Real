// Actividad 2. Mandar nombre de la unidad

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

const char msg[100] = "Unidad Profesional Interdisciplinaria de Ingeniería Campus Zacatecas IPN";

static TaskHandle_t  Tarea01 = NULL;
static TaskHandle_t  Tarea02 = NULL;
static TaskHandle_t  Tarea03 = NULL;
static TaskHandle_t  Tarea04 = NULL;
static TaskHandle_t  Tarea05 = NULL;
static TaskHandle_t  Tarea06 = NULL;
static TaskHandle_t  Tarea07 = NULL;

static const int LED01 = 15;   //1
static const int LED02 = 2;   //2
static const int LED03 = 4;   //3
static const int LED04 = 5;   //4
static const int LED05 = 18;   //5
static const int LED06 = 19;   //6
static const int LED07 = 21;   //7

//Tarea UART 01
void TareaUART01(void *parameter){
  int msg_len = strnlen(msg,100);
  while(1){
    digitalWrite(LED01, HIGH);
    Serial.println();
    for(int i =0; i< msg_len; i++){
      Serial.print(msg[i]);
    }
    Serial.println();
    digitalWrite(LED01, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//Tarea UART 02
void TareaUART02(void *parameter){
  while(1){
    digitalWrite(LED02, HIGH);
    Serial.println('1');
    digitalWrite(LED02, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//Tarea UART 03
void TareaUART03(void *parameter){
  while(1){
    digitalWrite(LED03, HIGH);
    Serial.println('2');
    digitalWrite(LED03, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//Tarea UART 04
void TareaUART04(void *parameter){
  while(1){
    digitalWrite(LED04, HIGH);
    Serial.println('3');
    digitalWrite(LED04, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//Tarea UART 05
void TareaUART05(void *parameter){
  while(1){
    digitalWrite(LED05, HIGH);
    Serial.println('4');
    digitalWrite(LED05, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//Tarea UART 06
void TareaUART06(void *parameter){
  while(1){
    digitalWrite(LED06, HIGH);
    Serial.println('5');
    digitalWrite(LED06, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//Tarea UART 07
void TareaUART07(void *parameter){
  while(1){
    digitalWrite(LED07, HIGH);
    Serial.println('6');
    digitalWrite(LED07, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  pinMode(LED01, OUTPUT);
  pinMode(LED02, OUTPUT);
  pinMode(LED03, OUTPUT);
  pinMode(LED04, OUTPUT);
  pinMode(LED05, OUTPUT);
  pinMode(LED06, OUTPUT);
  pinMode(LED07, OUTPUT);

  Serial.begin(300);
  xTaskCreatePinnedToCore(
    TareaUART01,  //Funcion que se manda llamar
    "UART 01",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    2,            //Prioridades
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
    1,            //Prioridades
    &Tarea03,
    app_cpu);
  xTaskCreatePinnedToCore(
    TareaUART04,  //Funcion que se manda llamar
    "UART 04",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    1,            //Prioridades
    &Tarea04,
    app_cpu);
  xTaskCreatePinnedToCore(
    TareaUART05,  //Funcion que se manda llamar
    "UART 05",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    1,            //Prioridades
    &Tarea05,
    app_cpu);
  xTaskCreatePinnedToCore(
    TareaUART06,  //Funcion que se manda llamar
    "UART 06",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    1,            //Prioridades
    &Tarea06,
    app_cpu);
  xTaskCreatePinnedToCore(
    TareaUART07,  //Funcion que se manda llamar
    "UART 07",    //El nombre de la tarea
    1024,         //Stack size
    NULL,
    1,            //Prioridades
    &Tarea07,
    app_cpu);
}

void loop() {
  for(int i=0; i<5; i++){
    vTaskSuspend(Tarea01);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    vTaskResume(Tarea01);
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}
