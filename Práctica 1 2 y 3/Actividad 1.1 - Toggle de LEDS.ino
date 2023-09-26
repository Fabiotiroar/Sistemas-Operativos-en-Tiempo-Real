// Define los pines de los LED
const int ledPin1 = 4; // Pin 13 (LED integrado en la placa Arduino)
const int ledPin2 = 18; // Puedes cambiar esto a otro pin si lo deseas

// Define los intervalos de parpadeo para cada LED
const unsigned long interval1 = 323/2; // Período de parpadeo del primer LED en milisegundos
const unsigned long interval2 = 500/2; // Período de parpadeo del segundo LED en milisegundos

// Variables para controlar el tiempo
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;

// Variables para almacenar el estado de los LED
int ledState1 = LOW;
int ledState2 = LOW;

void setup() {
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // Parpadeo del primer LED
  if (currentMillis - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis;
    ledState1 = (ledState1 == LOW) ? HIGH : LOW; // Cambia el estado del LED
    digitalWrite(ledPin1, ledState1); // Aplica el estado al LED
  }

  // Parpadeo del segundo LED
  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;
    ledState2 = (ledState2 == LOW) ? HIGH : LOW; // Cambia el estado del LED
    digitalWrite(ledPin2, ledState2); // Aplica el estado al LED
  }
}