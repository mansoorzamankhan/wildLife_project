
#include <WiFi.h>
#include <PubSubClient.h>  // https://github.com/knolleary/pubsubclient/
#include <WiFiClientSecure.h>
// WiFi credentials
const char* ssid = "workplace";
const char* password = "devomech12121";
#define BROKER "4513d12687ab46a0b58b0ab2ec75a85d.s2.eu.hivemq.cloud"
#define MQTT_PORT 8883
#define MQTT_USER "Wildlife_project"  // User name in adafruit.io
#define MQTT_PASSWORD "Abbasial2023"  // MQTT Key in adafruit.io
#define TOPIC "Name"                  // User_Name/feeds/Topic_Name_Of_Choice



WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  espClient.setCACert(hivemq_public_broker_ca);
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  String msg = "Hello from ESP32 with TLS";
  client.publish("esp32/test", msg.c_str());

  delay(5000);
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT Broker with TLS...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT Broker with TLS");
      client.subscribe("esp32/receive");
    } else {
      Serial.print("Failed to connect to MQTT Broker with TLS, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}
