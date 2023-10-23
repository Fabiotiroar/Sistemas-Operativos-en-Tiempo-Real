#include <Adafruit_MPL3115A2.h>
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
//#include "semphr.h"

Adafruit_MPL3115A2 mpl;

float tempC = 0.0;
SemaphoreHandle_t tempMutex;

// Tarea UART 01
void TareaUART01(void *parameter) {
  const char msg_1[31] = "Niños, ¿Quieren dulces? 7w7";
  int msg_len = strnlen(msg_1, 27);
  while (1) {
    Serial.println();
    for (int i = 0; i < msg_len; i++) {
      Serial.print(msg_1[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Tarea UART 02
void TareaUART02(void *parameter) {
  const char msg_2[35] = "Si capitan, queremos dulces";
  int msg_len = strnlen(msg_2, 35);
  while (1) {
    Serial.println();
    for (int i = 0; i < msg_len; i++) {
      Serial.print(msg_2[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Tarea UART 03
void TareaUART03(void *parameter) {
  const char msg_3[35] = "No los escuchooooo";
  int msg_len = strnlen(msg_3, 35);
  while (1) {
    Serial.println();
    for (int i = 0; i < msg_len; i++) {
      Serial.print(msg_3[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Tarea para el sensor MPL3115A2
void TareaSensor(void *pvParameters) {
  while (1) {
    while(!mpl.begin()){}; //mientras no se cumple no sale de ahi
    mpl.startOneShot();
    float newTempC = mpl.getTemperature();

    if (xSemaphoreTake(tempMutex, portMAX_DELAY) == pdTRUE) {
      tempC = newTempC;
      xSemaphoreGive(tempMutex);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Adafruit_MPL3115A2 test!");

  if (!mpl.begin()) {
    Serial.println("Could not find sensor. Check wiring.");
    while (1);
  }

  tempMutex = xSemaphoreCreateMutex();

  Serial.println("Setting mode to barometer (pressure).");
  mpl.setMode(MPL3115A2_BAROMETER);

  xTaskCreatePinnedToCore(
    TareaUART01,
    "UART 01",
    4096,
    NULL,
    1,
    NULL,
    1
  );
  xTaskCreatePinnedToCore(
    TareaUART02,
    "UART 02",
    4096,
    NULL,
    1,
    NULL,
    1
  );
  xTaskCreatePinnedToCore(
    TareaUART03,
    "UART 03",
    4096,
    NULL,
    3,
    NULL,
    1
  );
  xTaskCreatePinnedToCore(
    TareaSensor,
    "SensorTask",
    4096,
    NULL,
    2,
    NULL,
    0
  );
  vTaskStartScheduler();
}

void loop() {
  // El loop no se usa ya que todas las tareas se gestionan con FreeRTOS
}
