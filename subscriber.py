import json
import paho.mqtt.client as mqtt
import mysql.connector
import requests

# ThingSpeak
THINGSPEAK_API = "3OME5ZV83GT6NSP6"

# MySQL Connection
db = mysql.connector.connect(
    host="localhost",
    user="root",
    password="root",
    database="environment_project"
)
cursor = db.cursor()

def on_message(client, userdata, msg):
    data = json.loads(msg.payload.decode())

    temperature = data['temperature']
    humidity = data['humidity']
    gas = data['gas']

    # Insert into MySQL
    sql = """INSERT INTO parameter_readings
             (temperature, humidity, gas)
             VALUES (%s, %s, %s)"""
    cursor.execute(sql, (temperature, humidity, gas))
    db.commit()

    print("Inserted:", temperature, humidity, gas)

    # Send to ThingSpeak
    url = (
        f"https://api.thingspeak.com/update?"
        f"api_key={THINGSPEAK_API}"
        f"&field1={temperature}"
        f"&field2={humidity}"
        f"&field3={gas}"
    )
    requests.get(url)

# MQTT Client (NEW API – NO WARNING)
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)

client.connect("test.mosquitto.org", 1883, 60)
client.subscribe("iot/environment")
client.on_message = on_message

print("Subscriber started...")
client.loop_forever()