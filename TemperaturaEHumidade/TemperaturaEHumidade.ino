#include <DHT.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
 
#define DHTPIN 4 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11

#define SSID "BDAG"
#define SENHA "bdag2018"

#define IDCANAL 2097376
 
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.print("Conectando a rede: ");
  Serial.println(SSID); 
  WiFi.begin(SSID, SENHA);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  } 
  Serial.println("\nConectado a rede com sucesso!");
  ThingSpeak.begin(client);

  dht.begin();
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  
  ThingSpeak.setField(1, temperatura);
  ThingSpeak.setField(2, umidade);

  int retorno = ThingSpeak.writeFields(2097376, "PLYQYQQC4JLYWEHW"); // IDCANAL E WRITE API KEY
  
  if (retorno == 200) {
    Serial.println("Dados gravados corretamente no ThingSpeak");
  } else {
    Serial.println("Falha na gravação");
  }
  delay(15000);  

  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidade: "));
  Serial.print(h);
  Serial.print(F("%  Temperatura: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}
