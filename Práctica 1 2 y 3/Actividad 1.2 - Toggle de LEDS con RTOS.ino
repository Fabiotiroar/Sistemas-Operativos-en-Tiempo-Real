// Use only core 1
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// LEDs
static const int LED01 = 5;   //Verde
static const int LED02 = 4;   //Azul

//*********************************************************************
// Tarea 01
void toggleLED01(void *parameter) {
  while (1) {
      digitalWrite(LED01, HIGH);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      digitalWrite(LED01, LOW);
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  }
//Tarea 02
void toggleLED02(void *parameter) {
  while (1) {
      digitalWrite(LED02, HIGH);
      vTaskDelay(323 / portTICK_PERIOD_MS);
      digitalWrite(LED02, LOW);
      vTaskDelay(323 / portTICK_PERIOD_MS);
    }
}

void setup() {
  // Configurar pin
  pinMode(LED01, OUTPUT);
  pinMode(LED02, OUTPUT);

  // Start blink task 1
  xTaskCreatePinnedToCore(      // Use xTaskCreate() in vanilla FreeRTOS
            toggleLED01,        // Function to be called
            "Conmutar LED 1",   // Name of task
            1024,               // Stack size (bytes in ESP32, words in FreeRTOS)
            NULL,               // Parameter to pass
            2,                  // Task priority
            NULL,               // Task handle
            app_cpu);           // Run on one core for demo purposes (ESP32 only)

         
// Start blink task 2
  xTaskCreatePinnedToCore(      // Use xTaskCreate() in vanilla FreeRTOS
            toggleLED02,        // Function to be called
            "Conmutar LED 2",   // Name of task
            1024,               // Stack size (bytes in ESP32, words in FreeRTOS)
            NULL,               // Parameter to pass
            2,                  // Task priority
            NULL,               // Task handle
            app_cpu);           // Run on one core for demo purposes (ESP32 only)
/*
  // Delete "setup and loop" task
  vTaskDelete(NULL);*/
}

void loop() {
  // Execution should never get here
}