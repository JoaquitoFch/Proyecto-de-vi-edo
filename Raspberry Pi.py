import paho.mqtt.client as mqtt
import RPi.GPIO as GPIO
import json

#Configuración del pin GPIO para la válvula
VALVULA_PIN = 17
GPIO.setmode(GPIO.BCM)
GPIO.setup(VALVULA_PIN, GPIO.OUT)

#Función que se ejecuta al recibir un mensaje MQTT
def on_message(client, userdata, msg):
    data = json.loads(msg.payload.decode())
    if "activar" in data and data["activar"] == 1:
        GPIO.output(VALVULA_PIN, GPIO.HIGH)
        print("Riego activado")
    else:
        GPIO.output(VALVULA_PIN, GPIO.LOW)
        print("Riego desactivado")

#Configuración del cliente MQTT
client = mqtt.Client("Gateway")
client.on_message = on_message
client.connect("broker.hivemq.com", 1883, 60)  # Broker público de ejemplo
client.subscribe("viña/parcela1/control")

Bucle principal
client.loop_forever()