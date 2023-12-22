#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif
/*--------------------------------------------*/
//Indicaciones:
/* Hay un codigo de colores en las conexiones, de abajo hacia arriba
  El Potenciometro junto al motor es para leer las revoluciones por minuto 
  El led naranja es el testigo de la cajuela, esta junto a su sensor
  El potenciometro a la izquierda del ESP es el sensor de resistencia para ver si hay alguien en el asiento del conductor
  El led azul a la izquierda del asiento es el testigo de la puerta del conductor
  Arriba del potenciometro del asiento se encuentra la "llave del carro" destacada por un led, cuyo cable es amarillo
  El led verde arrriba del encendido es el testigo del cinturon de seguridad
  El led rosa arriba del testigo del cinturos es el testigo de las bolsas de aire
  A la misma altura pero más a la derecha se encuentra un led rojo que es el testigo de temperatura
  A la derecha del testigo de temperatura se encuentra el testigo de aceite, led blanco, junto con él también esta su sensor analogico
  A la derecha del testigo de aceite se encuentra el testigo de gasolina, led amarillo, junto con el sensor Flotador del tanque
  Delante de todos ellos se encuentra el testigo del capo, con un ledmorado, al igual que la puerta y la cajuela funciona con un dip switch
  Justo en la parte superior se encuentran dos leds azules que son las direccionales, debajo de ellos estan los botones que los activan, el dip swtich a su derecha es para activar las intermitentes
*/

//crear semaforos
static SemaphoreHandle_t dos1,dos2,dos3;

bool arrancado=LOW; //indica si se ha encendido el carro

int leido_conductor=0; //variable que guarda lectura analogica asiento conductor
bool conductor=LOW; //bandera de si hay un conductor
float voltaje_conductor=0; //convierte la lectura a analogico
float resistencia_conductor=0; //variable que guarda la resistencia del sensor de presion del asiento del conductor
bool error_conductor=LOW;

int leido_rpm=0; //guarda lectura analogica senwsor hall
float rpm=0;
float velocidad=0;

int leido_temperatura=0;
float voltaje_temperatura=0;
float resistencia_temperatura=0;

int leido_aceite=0;
int porcentaje_aceite=0;

int leido_gasolina=0;
float voltaje_gasolina=0;
float resistencia_gasolina=0;

int ledPuerta=18;  //Es el LED que se va a enceder para avisar que la puerta esta abierta
int ledCajuela=5;  //Es el LED que se va a enceder para avisar que la cajuela esta abierta
int ledCapo=17;  //Es el LED que se va a enceder para avisar que la capo esta abierta
int ledTemperatura=1;
int ledAceite=3;
int ledBolsas=27;
int ledCinto=14;
int LuzIzquierda=22;
int LuzDerecha=23;
int LuzGasolina=15;
int prende=0;

//para simular el encendido del vehiculo se puso un switch
void encendido(void *parameters){
  while(1){
    if(digitalRead(35)==HIGH){
      //Serial.println("BRRRRM");
      arrancado=HIGH;
      prende=1;
    }else{
      //Serial.println("zzz");
      arrancado=LOW;
      prende=0;
    }
  }
}

void asientoconductor(void *parameters){
  while(1){
    xSemaphoreTake(dos3,portMAX_DELAY);

    vTaskDelay(100);

    leido_conductor=analogRead(36); //ADC1_0, VN
    voltaje_conductor=(leido_conductor*5.0)/4095.0; //convierto el entero en voltaje
    resistencia_conductor=(5000.0-(voltaje_conductor*1000.0))/voltaje_conductor;
    //Serial.println(resistencia_conductor);
    
    if(resistencia_conductor>=480){
      error_conductor=HIGH; //falla en el sensor del asiento
      //Serial.println("Error");
    }else{
      if(resistencia_conductor>=120){
        conductor=LOW; //no hay nadie sentado
        error_conductor=LOW;
          digitalWrite(ledBolsas, LOW);
        //Serial.println("Nadie");
      }else{
        conductor=HIGH;
        if(prende==1){  
          digitalWrite(ledBolsas, HIGH);
        }else{
          digitalWrite(ledBolsas, LOW);
        }
        //Serial.println("Se sentaron");
        error_conductor=LOW;
      }
    }

    xSemaphoreGive(dos1);
  }
}

//en lugar de usar un sensor hall para el tacometro, el potenciometro 
//manda la cantidad de pulsos leidos en un minuto 
void RPM(void *parameters){
  while(1){
    leido_rpm=analogRead(4); //ADC2_0, D04
    rpm=(leido_rpm*6000.0)/4095; //para que lo maximo que pueda leer sean 6x1000r/min
    //Serial.println(rpm);
  }
}

//se considera una llanta de 200m en un rin de 16".
//Que aproximadamente da un diametro de 80 cm
void velocimetro(void *parameters){
  while(1){
    //el radio es de 40cm - 0.4m. por lo tanto por cada revolucion avanza 2.51 m 
    velocidad=((rpm*2.51327412287)/1000)*60;//formula para convertir rpm a km/h
    //Serial.println(velocidad);
  }
}

/*Vamos a utilizar un dip switch para simular el sensor magnetico de la puerta como abierta o cerrada,
se elige en lugar de un boton porque permite mantenerse en un estado*/
void puerta (void*parameters){
  while(1){
      if(digitalRead(32)==LOW){
        digitalWrite(ledPuerta, HIGH);
        //Serial.println("Cierra la puerta");
      }else{
        digitalWrite(ledPuerta, LOW);
        //Serial.println("Vamonooos");
      }
  }
}

/*Vamos a utilizar un dip switch para simular el sensor magnetico de la cajuela como abierta o cerrada,
se elige en lugar de un boton porque permite mantenerse en un estado*/
void cajuela (void*parameters){
  while(1){
      if(digitalRead(33)==LOW){ //cuando no detecta señal es porque no hay nada en el sensor magnetico por lo tanto esta abierta
        digitalWrite(ledCajuela, HIGH);
        //Serial.println("Cierra la puerta");
      }else{
        digitalWrite(ledCajuela, LOW);
        //Serial.println("Vamonooos");
      }
  }
}

/*Vamos a utilizar un dip switch para simular el sensor magnetico de la cajuela como abierta o cerrada,
se elige en lugar de un boton porque permite mantenerse en un estado*/
void capo (void*parameters){
  while(1){
    if(digitalRead(25)==LOW){
      digitalWrite(ledCapo, HIGH);
      //Serial.println("Cierra la puerta");
    }else{
      digitalWrite(ledCapo, LOW);
      //Serial.println("Vamonooos");
    }
  }
}

//el sensor de un automovil tambien se enfoca en medir la resistencia
void temperatura(void *parameters){
  while(1){
    xSemaphoreTake(dos1,portMAX_DELAY);
    
    vTaskDelay(400);

    leido_temperatura=analogRead(26); //ADC2_9, VN
    voltaje_temperatura=(leido_temperatura*5.0)/4095.0; //convierto el entero en voltaje
    resistencia_temperatura=(5000.0-(voltaje_temperatura*1000.0))/voltaje_temperatura;
    //Serial.println(resistencia_temperatura);
    
    if(prende==1){
      if(resistencia_temperatura<=200){
        //Serial.println("Me estoy quemando");
        digitalWrite(ledTemperatura,HIGH);
      }else{
        //Serial.println("todo ok");
        digitalWrite(ledTemperatura,LOW);
      }
    }else{
      digitalWrite(ledTemperatura,LOW);
    }

   xSemaphoreGive(dos2);
  }
}

//se cambia el manometro de un automovil por un potenciometro que representa la lectura analogica
//y se convertira en porcentaje que es como funciona elautomovil
void aceite(void *parameters){
  while(1){
    xSemaphoreTake(dos2,portMAX_DELAY);

   vTaskDelay(100);

    leido_aceite=analogRead(0); //ADC2_1, VN
    porcentaje_aceite=(leido_aceite*100)/4095.0; //convierto el entero a porcentaje
    //Serial.println(porcentaje_aceite);
    
    if(prende==1){
      if(porcentaje_aceite<=20){
        //Serial.println(porcentaje_aceite);
        digitalWrite(ledAceite,HIGH);
      }else{
        //Serial.println("todo ok");
        digitalWrite(ledAceite,LOW);
      }
    }else{
      digitalWrite(ledAceite,LOW);
    }

    xSemaphoreGive(dos3);
  }
}

void cinturon(void *parameters){
  while(1){
    if((prende==1)&&(conductor==HIGH)&&(digitalRead(12)==LOW)){
      digitalWrite(ledCinto,HIGH);
    }else{
      digitalWrite(ledCinto,LOW);
    }
  }
}

//se supone que si las intermitentes estan apagadas parpadean 60 veces por minuto y 
//cuando se activan las intermitentes parpadean 90 veces por minuto
//como los botones generan problemas se cambiaron por un switch
void luces(void *parameters){
  while(1){
    if(digitalRead(16)==HIGH){
      while(digitalRead(16)==HIGH){//las intermitentes tienen priodidad, si estan prendidas no funciona la direccional
        digitalWrite(LuzIzquierda,HIGH);
        digitalWrite(LuzDerecha,HIGH);
        vTaskDelay(500);//parpadeo de 90 veces por minuto
        digitalWrite(LuzIzquierda,LOW);
        digitalWrite(LuzDerecha,LOW);
        vTaskDelay(500);//parpadeo de 90 veces por minuto
      }  
    }else{
      while((digitalRead(21)==LOW)&&(digitalRead(19)==HIGH)&&(digitalRead(16)==LOW)){//giro a la izquierda
        digitalWrite(LuzIzquierda,HIGH);
        vTaskDelay(1000);//parpadeo de 60 veces por minuto
        digitalWrite(LuzIzquierda,LOW);
        vTaskDelay(1000);
      }
    
      while((digitalRead(19)==LOW)&&(digitalRead(21)==HIGH)&&(digitalRead(16)==LOW)){
        digitalWrite(LuzDerecha,HIGH);
        vTaskDelay(1000);//parpadeo de 60 veces por minuto
        digitalWrite(LuzDerecha,LOW);
          vTaskDelay(1000);
      }
    }
    
    digitalWrite(LuzIzquierda,LOW);
    digitalWrite(LuzDerecha,LOW);
  }
}

//el sensor de nivel de gasolina es un flotador de resistencia variable mientras mayor gasolina menos resistencia tendra
void gasolina(void *parameters){
  while(1){
    leido_gasolina=analogRead(2); //ADC2_2
    voltaje_gasolina=(leido_gasolina*5.0)/4095.0; //convierto el entero en voltaje
    resistencia_gasolina=(5000.0-(voltaje_gasolina*1000.0))/voltaje_gasolina;
   if(prende==1){//adc 2_2 
      if(resistencia_gasolina<=200){
        digitalWrite(LuzGasolina,HIGH);
      }else{
        digitalWrite(LuzGasolina,LOW);
      }
   }else{
    //apaga led
    digitalWrite(LuzGasolina,LOW);
   }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(35, INPUT); //encendido
  pinMode(ledPuerta, OUTPUT);
  pinMode(32, INPUT); //puerta
  pinMode(ledCajuela, OUTPUT);
  pinMode(33,INPUT);//cajuela
  pinMode(ledCapo, OUTPUT);
  pinMode(25,INPUT);//capo
  pinMode(ledTemperatura, OUTPUT);
  pinMode(ledAceite, OUTPUT);
  pinMode(ledBolsas, OUTPUT);
  pinMode(ledCinto, OUTPUT);
  pinMode(12,INPUT);//intermitentes
  pinMode(LuzDerecha, OUTPUT);
  pinMode(21,INPUT);//direccional izquierda
  pinMode(LuzIzquierda, OUTPUT);
  pinMode(19,INPUT);//direccional derecha
  pinMode(16,INPUT);//intermitentes
  pinMode(LuzGasolina, OUTPUT);

  dos1 = xSemaphoreCreateBinary();
  dos2 = xSemaphoreCreateBinary();
  dos3 = xSemaphoreCreateBinary();

  //declaracion de tareas
  xTaskCreatePinnedToCore(encendido,"Arranque?",1024,NULL,1,NULL,app_cpu); //adc en 2
  xTaskCreatePinnedToCore(asientoconductor,"Hay alguien en el asiento de conductor?",1024,NULL,1,NULL,0);
  xTaskCreatePinnedToCore(RPM,"RPM",1024,NULL,1,NULL,1); //adc en 2
  xTaskCreatePinnedToCore(velocimetro,"A que velocidad voy",1024,NULL,1,NULL,app_cpu); //adc en 2
  xTaskCreatePinnedToCore(puerta, "abierta", 1024, NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(cajuela, "abierta", 1024, NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(capo, "abierta", 1024, NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(temperatura, "Que mide la temperatura?", 1024, NULL,1,NULL,1);
  xTaskCreatePinnedToCore(aceite, "Porcentaje de aceite?", 1024, NULL,1,NULL,1);
  xTaskCreatePinnedToCore(cinturon, "Ponte el cituron", 1024, NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(luces, "luces", 1024, NULL,1,NULL,app_cpu);
  xTaskCreatePinnedToCore(gasolina, "lectura del flotador", 1024, NULL,1,NULL,1);

  xSemaphoreGive(dos1);
}

void loop() {
}

