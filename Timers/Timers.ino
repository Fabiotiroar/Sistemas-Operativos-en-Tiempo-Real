// Use only core 1 for demo purposes

#if CONFIG_FREERTOS_UNICORE

static const BaseType_t app_cpu = 0;

#else

static const BaseType_t app_cpu = 1;

#endif


static TimerHandle_t auto_reload_timer = NULL;

static TimerHandle_t auto_reload_timer2 = NULL;

static TimerHandle_t auto_reload_timer3 = NULL;

static TimerHandle_t auto_reload_timer4 = NULL;

static TimerHandle_t auto_reload_timer5 = NULL;

static TimerHandle_t auto_reload_timer6 = NULL;

static TimerHandle_t auto_reload_timer7 = NULL;

//***************************

// Callbacks


// Called when one of the timers expires

void myTimerCallback(TimerHandle_t xTimer) {


// Print message if timer 1 expired

if ((uint32_t)pvTimerGetTimerID(xTimer) == 1) {

Serial.println("Timer 1");

}

}


// Called when one of the timers expires

void myTimerCallback2(TimerHandle_t xTimer) {


// Print message if timer 1 expired

if ((uint32_t)pvTimerGetTimerID(xTimer) == 1) {

Serial.println("Timer 2");

}

}


// Called when one of the timers expires

void myTimerCallback3(TimerHandle_t xTimer) {


// Print message if timer 1 expired

if ((uint32_t)pvTimerGetTimerID(xTimer) == 1) {

Serial.println("Timer 3");

}

}

//tareas timer 4
void myTimerCallback4(TimerHandle_t xTimer) {


// Print message if timer expired

if ((uint32_t)pvTimerGetTimerID(xTimer) == 1) {

Serial.println("Timer 4");

}

}
 //config timer 5

void myTimerCallback5(TimerHandle_t xTimer) {


// Print message if timer expired

if ((uint32_t)pvTimerGetTimerID(xTimer) == 1) {

Serial.println("Timer 5");

}

}

//config timer 6

void myTimerCallback6(TimerHandle_t xTimer) {


// Print message if timer expired

if ((uint32_t)pvTimerGetTimerID(xTimer) == 1) {

Serial.println("Timer 6");

}

}

//config timer 7

void myTimerCallback7(TimerHandle_t xTimer) {


// Print message if timer expired

if ((uint32_t)pvTimerGetTimerID(xTimer) == 1) {

Serial.println("Timer 7");

}

}

void setup() {


// Configure Serial

Serial.begin(115200);


// Create an auto-reload timer

auto_reload_timer = xTimerCreate(

"Auto-reload timer", // Name of timer

1000 / portTICK_PERIOD_MS, // Period of timer (in ticks)

pdTRUE, // Auto-reload

(void *)1, // Timer ID

myTimerCallback); // Callback function


auto_reload_timer2 = xTimerCreate(

"Auto-reload timer", // Name of timer

1500 / portTICK_PERIOD_MS, // Period of timer (in ticks)

pdTRUE, // Auto-reload

(void *)1, // Timer ID

myTimerCallback2); // Callback function


auto_reload_timer3 = xTimerCreate(

"Auto-reload timer", // Name of timer

1200 / portTICK_PERIOD_MS, // Period of timer (in ticks)

pdTRUE, // Auto-reload

(void *)1, // Timer ID

myTimerCallback3); //  function

//inicializo timer 4
auto_reload_timer4 = xTimerCreate(

"Auto-reload timer", // Name of timer

1200 / portTICK_PERIOD_MS, // Period of timer (in ticks)

pdTRUE, // Auto-reload

(void *)1, // Timer ID

myTimerCallback4); //  function

//inicializo timer 5
auto_reload_timer5 = xTimerCreate(

"Auto-reload timer", // Name of timer

1200 / portTICK_PERIOD_MS, // Period of timer (in ticks)

pdTRUE, // Auto-reload

(void *)1, // Timer ID

myTimerCallback5); //  function

//inicializacion timer 6
auto_reload_timer6 = xTimerCreate(

"Auto-reload timer", // Name of timer

1200 / portTICK_PERIOD_MS, // Period of timer (in ticks)

pdTRUE, // Auto-reload

(void *)1, // Timer ID

myTimerCallback6); //  function

//inicializo timer 7
auto_reload_timer7 = xTimerCreate(

"Auto-reload timer", // Name of timer

1200 / portTICK_PERIOD_MS, // Period of timer (in ticks)

pdTRUE, // Auto-reload

(void *)1, // Timer ID

myTimerCallback7); //  function


xTimerStart(auto_reload_timer, portMAX_DELAY);

xTimerStart(auto_reload_timer2, portMAX_DELAY);

xTimerStart(auto_reload_timer3, portMAX_DELAY);

xTimerStart(auto_reload_timer4, portMAX_DELAY);

xTimerStart(auto_reload_timer5, portMAX_DELAY);

xTimerStart(auto_reload_timer6, portMAX_DELAY);

xTimerStart(auto_reload_timer7, portMAX_DELAY);

// Delete self task to show that timers will work with no user tasks

//vTaskDelete(NULL);

}



void loop() {

// Execution should never get here

}