# 🍇 Sistema de Riego Automático para Viñedos – IoT Cloud Project

Proyecto desarrollado como parte de **Problemáticas Globales y Prototipado (Prueba 2)**.  
Su objetivo es implementar un sistema inteligente que optimice el riego de viñedos mediante **dispositivos IoT**, **comunicación MQTT** y **servicios en la nube**.

---

## 🌱 Descripción del Proyecto

El sistema automatiza el riego del viñedo controlando la humedad del suelo y las condiciones ambientales.  
Cuando los sensores detectan valores bajo un umbral, se activa la válvula de riego automáticamente o bajo control manual desde una **plataforma web**.

El enfoque combina **electrónica, comunicación IoT y análisis en la nube**, mejorando la eficiencia del agua y la producción del cultivo.

---

## 👥 Integrantes
- **Rafael Alarcón**  
- **Joaquin Sánchez**  
- **Sebastián Basaure**  
- **Lenny Norambuena**

📅 **Fecha:** 25/10/2025  
📘 **Sección:** 002D

---

## ⚙️ Tecnologías Principales

| Componente | Función |
|-------------|----------|
| **ESP32 / Arduino Uno** | Nodos de campo: sensores y control de válvulas |
| **Raspberry Pi 4** | Gateway local, cliente MQTT, almacenamiento temporal |
| **Sensores** | Humedad del suelo, temperatura, radiación solar, nivel de agua |
| **Actuadores** | Relé 5V para válvulas y bombas |
| **Alimentación** | Panel solar + batería (para zonas remotas) |

---

## ☁️ Arquitectura en Capas

1. **Capa de Percepción:** Sensores y actuadores que captan y ejecutan acciones.  
2. **Capa de Red (Edge):** Raspberry Pi recopila, filtra y envía datos a la nube.  
3. **Capa de Ingesta:** Comunicación segura mediante **MQTT over TLS**.  
4. **Capa de Procesamiento:** Análisis en **AWS IoT / Azure IoT Hub** e **InfluxDB**.  
5. **Capa de Presentación:** Paneles en **Grafana** y aplicación web.  
6. **Capa de Seguridad:** Certificados, VPN, IAM y auditorías.

---

## 🔗 Protocolos y Comunicaciones

MQTT (telemetría y control): publish/subscribe eficiente; tópicos MQTT over TLS: cifrado y autenticación por certificados para evitar spoofing.
HTTP/HTTPS (REST API): configuración por UI, webhooks de notificación, y consola administrativa.
CoAP (opcional): en dispositivos ultra-limitados si se requiere UDP.
LoRa / LoRa WAN: enlace físico para nodos remotos (baja tasa), con gateway que traduce a MQTT.
TCP/IP, DHCP, DNS: pila de red básica.
NTP: sincronización horaria para registros y políticas de riego.
SSH / VPN: acceso administrativo seguro al Raspberry Pi y servicios backend.

---

## 💻 Ejemplo de Código

### 🌾 ESP32 – Lectura de humedad y envío por MQTT
```cpp
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "TuSSID";
const char* password = "TuPassword";
const char* mqtt_server = "192.168.1.100";

WiFiClient espClient;
PubSubClient client(espClient);

int sensorPin = 34;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) reconnect();
  int humedad = analogRead(sensorPin);
  String payload = String("{\"humedad\":") + humedad + "}";
  client.publish("vina/parcela1/nodo1/telemetry", payload.c_str());
  delay(5000);
}
```

---

### 🐍 Raspberry Pi – Gateway y control de válvula
```python
import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO
import json

VALVULA_PIN = 17
GPIO.setmode(GPIO.BCM)
GPIO.setup(VALVULA_PIN, GPIO.OUT)

def on_message(client, userdata, msg):
    data = json.loads(msg.payload.decode())
    GPIO.output(VALVULA_PIN, GPIO.HIGH if data.get("activar") else GPIO.LOW)

client = mqtt.Client("Gateway")
client.on_message = on_message
client.connect("broker.hivemq.com", 1883, 60)
client.subscribe("vina/parcela1/control")
client.loop_forever()
```

---

## 📊 Visualización y Nube

- **InfluxDB + Grafana** → Base de datos de series temporales y dashboards.  
- **AWS IoT Core / Azure IoT Hub** → Enlace seguro y escalable.  
- **S3 / Blob Storage** → Backups y almacenamiento multimedia.  

---

## 🔒 Seguridad

- Certificados **TLS** por dispositivo.  
- Roles **IAM** para control de acceso.  
- **VPN** para administración.  
- Monitoreo y alertas automáticas.

---

## 🌍 Impacto

Este sistema contribuye a la **agricultura sostenible**, reduciendo el consumo de agua y mejorando la productividad de los viñedos mediante automatización y análisis de datos.

---

## 🛠️ Herramientas de Prototipado
- Fritzing / draw.io (diagramas)
- Node-RED (flujos locales)
- GitHub (control de versiones)


✨ _Desarrollado con pasión por un equipo comprometido con la innovación tecnológica y el cuidado del medio ambiente._
