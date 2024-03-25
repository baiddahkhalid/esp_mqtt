
///////////////////////////////////////
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "La_Fibre_dOrange_2.4G_A7BE_Ext"; // Enter your Wi-Fi name
const char *password = "xxxxxxxxxxx";  // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "196.115.108.101";
const char *topic = "emqx/esp32";
const char *mqtt_username = "khalid01";
const char *mqtt_password = "khalid01";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const int output26 = 26;
String myString = "";

void setup() {
    pinMode(output26, OUTPUT);
    // Set software serial baud to 115200;
    Serial.begin(115200);
    // Connecting to a WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the Wi-Fi network");
    //connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());
        Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public EMQX MQTT broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    // Publish and subscribe
    client.publish(topic, "Hi, I'm ESP32 ^^");
    client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
    if((char) payload[0] == 'A'){
      digitalWrite(output26, HIGH);
      client.publish("ws/esp32", "relay 1 ON");
    }else if((char) payload[0] == 'B'){
      digitalWrite(output26, LOW);
      client.publish("ws/esp32", "relay 1 OFF");
    }
    else{
      
    }
}

void loop() {
    client.loop();
}

