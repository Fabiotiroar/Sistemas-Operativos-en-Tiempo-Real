#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif
// Declaracion de los pines de salida
const int ROJO_01 = 19;
const int AMARILLO_01 = 2;
const int VERDE_01 = 5;
const int ROJO_02 = 21;
const int AMARILLO_02 = 22;
const int VERDE_02 = 4;
const int SENNAL = 13;
//Marcas booleanas de la red de Petri para las luces
static bool marca01 = false;
static bool marca02 = false;
static bool marca03 = false;
static bool marca04 = false;
static bool marca05 = false; //estado de reposo

//CONFIGURACION DEL BOTON
const int SW_01 = 18;
const int SW_02 = 23;

unsigned long tiempoInicio = 0; // Variable para registrar el tiempo inicial
int espera=1;     //while del estado 1
int espera2=1;    //while del estado 2
int espera3=1;    //while del estado 3
bool estado_final=false;    //Bandera estado de emergencia
//Marcas booleanas de la red de Petri para la secuencia de los botones
static bool marca1 = false;
static bool marca2 = false;
static bool marca3 = false;
static bool marca4 = false;
static bool marca5 = false;

// Prototipos de tareas
void estado01(void *parameters);
void estado02(void *parameters);
void estado03(void *parameters);
void estado04(void *parameters);

//TAREAS BOTON
void E1(void *parameters);
void E2(void *parameters);
void E3(void *parameters);
void E4(void *parameters);
void E5(void *parameters);
void salida01(void *parameters);
void salida02(void *parameters);
void salida03(void *parameters);
void salida04(void *parameters);
void salida05(void *parameters);
void salida06(void *parameters);
void salida11(void *parameters);

//creacion semaforos para el semaforo
static SemaphoreHandle_t xSemaphore1,xSemaphore2,xSemaphore3,xSemaphore4;

//semaforos para los botones
static SemaphoreHandle_t xS1,xS2,xS3,xS4,xS5;

void desactivar(int quien){ //desactiva todas las marcas que no son la que se esta ejecutando y deshabilita todas las esperas
  switch(quien){
    case 1:
      marca2=false;
      marca3=false;
      marca4=false;
      marca5=false;   
      espera=0;
      espera2=0;
      espera3=0;
    break;

    case 2:
      marca1=false;
      marca3=false;
      marca4=false;
      marca5=false;
      espera=1;
      espera2=0;
      espera3=0;
    break;

    case 3:
      marca1=false;
      marca2=false;
      marca4=false;
      marca5=false;
      espera=0;
      espera3=0;
    break;

    case 4:
      marca1=false;
      marca2=false;
      marca3=false;
      marca5=false;
      espera=0;
      espera2=0;
    break;

    case 5:
      marca1=false;
      marca2=false;
      marca3=false;
      marca4=false;
      espera=0;
      espera2=0;
    break;
  }
}


void setup() {
  Serial.begin(115200);

  //PINES BOTONES
  pinMode(SW_01, INPUT);
  pinMode(SW_02, INPUT);

  //PINNES DEL SEMAFORO
  pinMode(SENNAL, OUTPUT); //señal para ver si esta en modo de emergencia
  pinMode(ROJO_01, OUTPUT);
  pinMode(AMARILLO_01, OUTPUT);
  pinMode(VERDE_01, OUTPUT);
  pinMode(ROJO_02, OUTPUT);
  pinMode(AMARILLO_02, OUTPUT);
  pinMode(VERDE_02, OUTPUT);

  //Creacion semaforos semaforo
  xSemaphore1 = xSemaphoreCreateBinary();
  xSemaphore2 = xSemaphoreCreateBinary();
  xSemaphore3 = xSemaphoreCreateBinary();
  xSemaphore4 = xSemaphoreCreateBinary();
  //creacion semaforos botones
  xS1 = xSemaphoreCreateBinary();
  xS2 = xSemaphoreCreateBinary();
  xS3 = xSemaphoreCreateBinary();
  xS4 = xSemaphoreCreateBinary();
  xS5 = xSemaphoreCreateBinary();

  //estados del semaforo
  xTaskCreatePinnedToCore(estado01,"estado01",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(estado02,"estado02",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(estado03,"estado03",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(estado04,"estado04",1024,NULL,1,NULL,app_cpu);
  
  //funciones de salida
  xTaskCreatePinnedToCore(salida01,"salida01",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(salida02,"salida02",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(salida03,"salida03",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(salida04,"salida04",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(salida05,"salida05",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(salida06,"salida06",1024,NULL,1,NULL,app_cpu);
  
  //Estados de los botones
  xTaskCreatePinnedToCore(E1,"estado01",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(E2,"estado02",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(E3,"estado03",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(E4,"estado04",1024,NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(E5,"estado04",1024,NULL,1,NULL,app_cpu);
  
  //comutación señal de emergencia
  xTaskCreatePinnedToCore(salida11,"salida11",1024,NULL,1,NULL,app_cpu);

  //comenzar estados 1
  xSemaphoreGive(xSemaphore1);
  xSemaphoreGive(xS1);  
}

void loop() {

}
//REd de petri semaforo
void estado01(void *parameters){
  while (1) {
      xSemaphoreTake(xSemaphore1, portMAX_DELAY);
      marca04=false;
      marca01=true;
      vTaskDelay(3000 / portTICK_PERIOD_MS);
      xSemaphoreGive(xSemaphore2);
  }
}

void estado02(void *parameters){
  while (1) {
      xSemaphoreTake(xSemaphore2, portMAX_DELAY);
      marca01=false;
      marca02=true;
      vTaskDelay(3000 / portTICK_PERIOD_MS);
      xSemaphoreGive(xSemaphore3);
  }
}

void estado03(void *parameters){
  while (1) {
      xSemaphoreTake(xSemaphore3, portMAX_DELAY);
      marca02=false;
      marca03=true;
      vTaskDelay(3000 / portTICK_PERIOD_MS);
      xSemaphoreGive(xSemaphore4);
  }
}

void estado04(void *parameters){
  while (1) {
      xSemaphoreTake(xSemaphore4, portMAX_DELAY);
      marca03=false;
      marca04=true;
      vTaskDelay(3000 / portTICK_PERIOD_MS);
      xSemaphoreGive(xSemaphore1);
  }
}

//Funciones de salida de los leds semaforo
void salida01(void *parameters){
  while(1){
    if(estado_final==false){
      if(marca01 == true ){
        digitalWrite(ROJO_01, HIGH);
      }else{
        digitalWrite(ROJO_01, LOW);
      }
    }else{
      digitalWrite(ROJO_01, LOW);
    }
  }
}
void salida02(void *parameters){
  while(1){
    if(estado_final==false){
      if(marca02 == true){
        digitalWrite(AMARILLO_01, HIGH);
      }
      else{
        digitalWrite(AMARILLO_01, LOW);
      }
  }else{
    digitalWrite(AMARILLO_01, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(AMARILLO_01, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
}
void salida03(void *parameters){
  while(1){
    if(estado_final==false){
      if(marca03 == true || marca04 == true){
        digitalWrite(VERDE_01, HIGH);
      }else{
        digitalWrite(VERDE_01, LOW);
      }
    }else{
      digitalWrite(VERDE_01, LOW);
    }
  }
}
void salida04(void *parameters){
  while(1){
    if(estado_final==false){
      if(marca03 == true){
        digitalWrite(ROJO_02, HIGH);
      }else{
        digitalWrite(ROJO_02, LOW);
      }
    }else{
      digitalWrite(ROJO_02, LOW);
    }

  }
}
void salida05(void *parameters){
  while(1){
    if(estado_final==false){
      if(marca04 == true){
        digitalWrite(AMARILLO_02, HIGH);
      }
      else{
        digitalWrite(AMARILLO_02, LOW);
      }
    }else{
      digitalWrite(AMARILLO_02, HIGH);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        digitalWrite(AMARILLO_02, LOW);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  }
}

void salida06(void *parameters){
  while(1){
    if(estado_final==false){
      if(marca01 == true || marca02 == true){
        digitalWrite(VERDE_02, HIGH);
      }else{
        digitalWrite(VERDE_02, LOW);
      }
    }else{
      digitalWrite(VERDE_02, LOW);
    }
  }
}

///----------------------COSAS DEL BOTON

void E1(void *parameters){
  while (1) {
    xSemaphoreTake(xS1, portMAX_DELAY);
    desactivar(1);
    marca1=true;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    while(digitalRead(SW_01)==LOW);//espera presione boton
    tiempoInicio=millis(); //registra cuando se presiono el boton
    xSemaphoreGive(xS2);
  }
}

void E2(void *parameters){
  while (1) {
    xSemaphoreTake(xS2, portMAX_DELAY);
    desactivar(2);
    marca2=true;
    vTaskDelay(100 / portTICK_PERIOD_MS);
    Serial.println("Estoy aqui");
    if(digitalRead(SW_01)==LOW){
      espera=0;
      xSemaphoreGive(xS1);//regreso al estado1
    }
    while(espera==1){//inicia la cuenta de dos segundos
      if(digitalRead(SW_01)==HIGH){//pasa cuando esta presionado
        if((millis()-tiempoInicio)<2000){//espera dos segundos
         //Serial.println(millis()-tiempoInicio);
        }else{//ya pasaron los dos segundos
          espera=0;
          espera2=1;
         xSemaphoreGive(xS3);
        }
      }else{//lo solte
        espera=0;
        xSemaphoreGive(xS1);//regreso al estado1
      }
    } 
  }
}

void E3(void *parameters){
  while (1) {
    xSemaphoreTake(xS3, portMAX_DELAY);
    desactivar(3);
    marca3=true;
    vTaskDelay(100 / portTICK_PERIOD_MS);
    while(espera2==1){
      if(digitalRead(SW_01)==HIGH){//tienes que seguir presionando el boton 1
        if(digitalRead(SW_02)==HIGH){
          xSemaphoreGive(xS4);
        }
      }
      else{//soltaste el boton1
        xSemaphoreGive(xS1);//si sueltas el boton1 te regresas al estado 1
      }
    }
  }
}

void  E4(void *parameters){
  while (1) {
    xSemaphoreTake(xS4, portMAX_DELAY);
    desactivar(4);
    marca4=true;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    espera3=1;
    while(espera3==1){
      if(digitalRead(SW_01)==HIGH){//si sigue prendido el boton1
        if(digitalRead(SW_02)==LOW){ //suelto el otro boton
          espera3=0;
          xSemaphoreGive(xS5);
        }
      }
      else{
        espera3=0;
        xSemaphoreGive(xS1);
      }
    }
  }
}

void  E5(void *parameters){
  while (1) {
    xSemaphoreTake(xS5, portMAX_DELAY);
    desactivar(5);
    marca5=true;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    while(marca5==true){
      if(digitalRead(SW_02)==HIGH){//si vuelvo a presionar el boton
        xSemaphoreGive(xS1);
      }
      if(digitalRead(SW_01)==LOW){ //suelto el primer boton
          if(estado_final==false){
            estado_final=true;
            digitalWrite(SENNAL,HIGH);
          }
          else{
            estado_final=false;
            digitalWrite(SENNAL,LOW);
          }
          marca5=false;
          xSemaphoreGive(xS1);
      }
    }
  }
}

void salida11(void *parameters){
  while(1){
    if(marca1 == true ){
      Serial.println("Estado1");
      //digitalWrite(ROJO_01,LOW);
    }
    if(marca2==true){
      Serial.println("Estado2");
      //digitalWrite(ROJO_01,LOW);
    }
    if(marca3==true){
      Serial.println("Estado3");
      //digitalWrite(ROJO_01,HIGH);
    }
    if(marca4==true){
      Serial.println("Estado4");
      //digitalWrite(ROJO_01,LOW);
    }
    if(marca5==true){
      Serial.println("Estado5");
      //digitalWrite(ROJO_01,LOW);
    }
  }
}

