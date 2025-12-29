#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// -------- WiFi ----------
const char* ssid = "Raks";
const char* password = "12345678";

// -------- MQTT ----------
const char* mqtt_server = "broker.hivemq.com";
const char* pubTopic = "home/env/data";
const char* subTopic = "home/env/cmd";

// -------- DHT ----------
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// -------- Clients ----------
WiFiClient espClient;
PubSubClient client(espClient);

// -------- MQTT callback ----------
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }

  Serial.println(msg);

  // Example command handling
  if (String(topic) == subTopic) {
    if (msg == "ON") {
      Serial.println("Command: ON received");
    } else if (msg == "OFF") {
      Serial.println("Command: OFF received");
    }
  }
}

// -------- MQTT reconnect ----------
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    String clientId = "ESP32_Env_";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(subTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  // Read DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    String payload = "{";
    payload += "\"temperature\":";
    payload += t;
    payload += ",\"humidity\":";
    payload += h;
    payload += "}";

    client.publish(pubTopic, payload.c_str());
    Serial.println("Published: " + payload);
  }

  delay(5000);
}
