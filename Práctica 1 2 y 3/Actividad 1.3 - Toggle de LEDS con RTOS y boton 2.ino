// Use only core 1
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// LEDs
static const int LED01 = 5;   // VERDE
static const int LED02 = 4;   // AZUL
static const int SW01 = 18;

int frec1=750;
int frec2=162;
int boton=0;

int status = 0;

//*********************************************************************
// Tarea 01
void toggleLED01(void *parameter) {
  while (1) {
    status = digitalRead(SW01);
    if(status == HIGH){
      if(boton == 0){
        boton=1;
        frec1=1500;
      }else{
        frec1=500;
        boton=0;
      }
    }
    digitalWrite(LED01, HIGH);
    vTaskDelay(frec1 / portTICK_PERIOD_MS);
    digitalWrite(LED01, LOW);
    vTaskDelay(frec1 / portTICK_PERIOD_MS);
    }
  }

//Tarea 02
void toggleLED02(void *parameter) {
  while (1) {
    status = digitalRead(SW01);
    if(status == HIGH){
      if(boton == 0){
        boton=1;
        frec2=250;
      }else{
        frec2=162;
        boton=0;
      }
    }
    digitalWrite(LED02, HIGH);
    vTaskDelay(frec2 / portTICK_PERIOD_MS);
    digitalWrite(LED02, LOW);
    vTaskDelay(frec2 / portTICK_PERIOD_MS);
    }
}

void setup() {
  // Configurar pin
  pinMode(SW01, INPUT);
  pinMode(LED01, OUTPUT);
  pinMode(LED02, OUTPUT);

  // Start blink task 1
  xTaskCreatePinnedToCore(      // Use xTaskCreate() in vanilla FreeRTOS
            toggleLED01,        // Function to be called
            "Conmutar LED 1",   // Name of task
            1024,               // Stack size (bytes in ESP32, words in FreeRTOS)
            NULL,               // Parameter to pass
            1,                  // Task priority
            NULL,               // Task handle
            app_cpu);           // Run on one core for demo purposes (ESP32 only)

         
// Start blink task 2
  xTaskCreatePinnedToCore(      // Use xTaskCreate() in vanilla FreeRTOS
            toggleLED02,        // Function to be called
            "Conmutar LED 2",   // Name of task
            1024,               // Stack size (bytes in ESP32, words in FreeRTOS)
            NULL,               // Parameter to pass
            1,                  // Task priority
            NULL,               // Task handle
            app_cpu);           // Run on one core for demo purposes (ESP32 only)
/*
  // Delete "setup and loop" task
  vTaskDelete(NULL);*/
}

void loop() {
  // Execution should never get here
}