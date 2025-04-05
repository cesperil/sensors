// Incluimos librería
#include <DHT.h>
 
// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 2
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11

const int sensorPin = 3;
const int measureInterval = 2500;
volatile int pulseConter;

const float factorK = 7.5;

void ISRCountPulse()
{
  pulseConter++;
}

float GetFrequency()
{
  pulseConter = 0;

  interrupts();
  delay(measureInterval);
  noInterrupts();

  return (float)pulseConter * 1000 / measureInterval;
}

 
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  // Inicializamos comunicación serie
  Serial.begin(9600);
 
  // Comenzamos el sensor DHT
  dht.begin();

  //Inicializamos YF-S201
  attachInterrupt(digitalPinToInterrupt(sensorPin), ISRCountPulse, RISING);

 
}
 
void loop() {
    // Esperamos 5 segundos entre medidas
  delay(5000);
 
  medirTemperatura();

// obtener frecuencia en Hz
  float frequency = GetFrequency();

  // calcular caudal L/min
  float flow_Lmin = frequency / factorK;

  Serial.print("Frecuencia: ");
  Serial.print(frequency, 0);
  Serial.print(" (Hz)\tCaudal: ");
  Serial.print(flow_Lmin, 3);
  Serial.println(" (L/min)");
 
}



void medirTemperatura(){
  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Leemos la temperatura en grados Fahreheit
  float f = dht.readTemperature(true);
 
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }
 
  // Calcular el índice de calor en Fahreheit
  float hif = dht.computeHeatIndex(f, h);
  // Calcular el índice de calor en grados centígrados
  float hic = dht.computeHeatIndex(t, h, false);
 
  mostrarDatosTemperatura(h,t);
}


void mostrarDatosTemperatura(float h, float t ){



  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print("ºC\n");

}