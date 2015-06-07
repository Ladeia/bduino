// Bduino
// Uma aplicaçao para para captar dados do
// contexto urbano e/ou natural na perspectiva
// do uso da bicicleta.
// Repositório: https://github.com/Ladeia/tcc

// Criado por Antonio Ladeia com base em diversos
// outros codigos encontrados na internet.
// Email: contato@antonioladeia.com
// Site: www.antonioladeia.com

// Este software esta licensiado sobre a
// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
// mais detalhes encontram-se no arquivo license.txt
// na raiz desse pacote.

// Includes
#include <dht11.h>
#include <TinyGPS.h>

dht11 DHT11;
TinyGPS gps;

// Defines digital pins
#define DHT11Pin 2
#define HCTrigPin 13
#define HCEchoPin 12
#define GPSRXPin 3
#define GPSTXPin 4

// Define analogic pins
#define LDRPin 0
#define SoundPin 1
#define XPin 2
#define YPin 3
#define ZPin 4

void setup()
{
    Serial.begin(9600);
    Serial1.begin(38400);
    Serial1.flush();
    pinMode(HCTrigPin, OUTPUT);
    pinMode(HCEchoPin, INPUT);
}

void loop()
{
    sendJSONToSerial();
}

// Pegar o valor do sensor de iluminaço
int getLightValue()
{
    return analogRead(LDRPin);
}

// Pegar o valor do sensor de temperatura
float getTemperatureValue()
{
    int chk = DHT11.read(DHT11Pin);

    switch (chk)
    {
      case DHTLIB_OK:
  		return ((float)DHT11.temperature, 2);;
  		break;
      case DHTLIB_ERROR_CHECKSUM:
  		return -1;
  		break;
      case DHTLIB_ERROR_TIMEOUT:
  		return -2;
  		break;
      default:
  		return -3;
  		break;
    }

}

// Pegar o valor do sensor de umidade
float getHumidityValue()
{
    int chk = DHT11.read(DHT11Pin);
    switch (chk)
    {
        case DHTLIB_OK:
            return ((float)DHT11.humidity, 2);
  	    break;
        case DHTLIB_ERROR_CHECKSUM:
  		return -1;
  		break;
      case DHTLIB_ERROR_TIMEOUT:
  		return -2;
  		break;
      default:
  		return -3;
  		break;
    }
    return ((float)DHT11.humidity, 2);
}

// Pegar o valor do sensor de distancia
float getDistanceValue()
{
    digitalWrite(HCTrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(HCTrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(HCTrigPin, LOW);
    int duration = pulseIn(HCEchoPin, HIGH);

    return (duration/2) / 29.1;
}

// Pegar o valor do sensor de som
int getSoundValue()
{
    return analogRead(SoundPin);
}

// Pegar o valor do sensor de trepidacao
int getShakeValue()
{
    int x = analogRead(XPin);
    int y = analogRead(YPin);
    int z = analogRead(ZPin);
    return y;
}

// Controla o envio de dados pela serial
void sendJSONToSerial()
{
  char longitute = 'I';
  char latitude = 'I';

  if (Serial1.available())
  {
    bool newData = false;
    unsigned long chars;
    unsigned short sentences, failed;

    for (unsigned long start = millis(); millis() - start < 5;)
    {
      while (Serial1.available())
      {
        start = millis();
        char c = Serial1.read();
        if (gps.encode(c))
          newData = true;
      }
    }

    if (newData)
    {
      float flat, flon;
      unsigned long age;
      gps.f_get_position(&flat, &flon, &age);
      latitude = (flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
      longitute = (flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    }

  }
    int light = getLightValue();
    float temperature = getTemperatureValue();
    float humidity = getHumidityValue();
    float distance = getDistanceValue();
    int sound = getSoundValue();
    int shake = getShakeValue();

      Serial.print("{\"light\":\"");
      Serial.print(light);
      Serial.print("\", \"temperature\":\"");
      Serial.print (temperature );
      Serial.print("\", \"humidity\":\"");
      Serial.print(humidity);
      Serial.print("\", \"distance\":\"");
      Serial.print(distance);
      Serial.print("\", \"longitute\":\"");
      Serial.print(longitute);
      Serial.print("\", \"latitude\":\"");
      Serial.print(latitude);
      Serial.print("\", \"sound\":\"");
      Serial.print(sound);
      Serial.print("\", \"shake\":\"");
      Serial.print(shake);
      Serial.print("\"}");
      Serial.println();
}
