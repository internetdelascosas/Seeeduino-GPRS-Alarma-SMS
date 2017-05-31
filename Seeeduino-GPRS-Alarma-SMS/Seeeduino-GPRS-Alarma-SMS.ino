/*
Sketch     : Seeeduino GPRS Alarma SMS
Autor      : Jose Zorrilla <jzorrilla@iot.cl>
Descripcion: Programa que obtiene la temperatura desde un sensor DS18B20 y envia un SMS 
             cuando supera un umbral predefinido.
Librerias  : Se requieren las librerias DallasTemp y OneWire de Adafruit

********************************************************************************************
Notar: Los siguientes pin no deben ser utilizados ya que son reservados en el Seeeduino GPRS
  pin 8   // tx pin
  pin 7   // rx pin
  pin 9   // power key pin
  pin 12  // power status pin
********************************************************************************************
*/
// Librerias 
#include <stdlib.h>
#include <gprs.h>
#include <SoftwareSerial.h>
#include <OneWire.h> 
#include <DallasTemperature.h>

// Configuracion - Cambiar los siguientes parametros de 
float temp_limite = 25.00;                 // Temperatura limite antes de alarmar
char numero_telefono[20] = "+6144928xxxx"; // Numero movil al que se envia el SMS

// Pin al cual esta conectado el sensor de temperatura DS18B20 
#define ONE_WIRE_BUS 2 

// Crea instancia OneWire para comunicarse con el sensor
OneWire oneWire(ONE_WIRE_BUS); 

// Crea instancia del sensor de temperatura Dallas
DallasTemperature sensors(&oneWire);

// Variables
GPRS gprs;
float tempf;
char sms[140]; 
char temperatura[140];

// Inicializacion del Seeeduino
void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("GPRS - Alarma SMS Temperatura.");
  sensors.begin();
  
  gprs.preInit();
  delay(1000);
  while(0 != gprs.init()) {
      delay(1000);
      Serial.print("Error al iniciar modulo GPRS\r\n");
  }  
  Serial.println("Inicializacion Modulo GPRS exitosa..."); 
}

// Programa principal loop
void loop() {
  Serial.println("Obteniendo la temperatura..."); 
  // Obtiene la temperatura desde el sensor
  sensors.requestTemperatures(); 
  tempf = sensors.getTempCByIndex(0);
  dtostrf(sensors.getTempCByIndex(0),4,2,temperatura);
  Serial.print("La temperatura es: ");
  Serial.print(temperatura);
  Serial.println(" C");
  // Verifica si la temperatura supera el limite
  if (tempf > temp_limite) {
    sprintf(sms,"La temperatura es: %s C", temperatura);
    Serial.print("Numero Movil: ");
    Serial.println(numero_telefono);
    Serial.print("Enviando SMS: ");
    Serial.println(sms);
    // Envia el SMS al numero de telefono configurado
    gprs.sendSMS(numero_telefono,sms); 
    Serial.println("SMS Enviado.");
    // Espera 4 minutos para evitar enviar SMS cada 1 minuto
    // (estos se suman al minuto que espera en cada ciclo)
    delay(240000);
  }
  // Espera 1 minuto antes de hacer la siguiente lectura
  delay(60000);
}
