#include <WiFi.h>
#include <PubSubClient.h>

// Configuración Wi-Fi
const char* ssid = "TuSSID";
const char* password = "TuPassword";

// Dirección del broker MQTT (puede ser local o en la nube)
const char* mqtt_server = "192.168.1.100";

WiFiClient espClient;
PubSubClient client(espClient);

int sensorPin = 34; // Pin analógico para sensor de humedad
int humedad;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  // Espera hasta que se conecte al Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  // Lectura del sensor
  humedad = analogRead(sensorPin);

  // Construcción del mensaje en formato JSON
  String payload = String("{"humedad":") + humedad + "}";

  // Publicación en el tópico MQTT
  client.publish("viña/parcela1/nodo1/telemetry", payload.c_str());

  delay(5000); // cada 5 segundos
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("Nodo1")) {
      client.subscribe("viña/parcela1/control");
    } else {
      delay(2000);
    }
  }
}