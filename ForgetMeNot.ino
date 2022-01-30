#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>

#define R_PINOUT D7
#define G_PINOUT D6
#define B_PINOUT D5

// Wi-Fi configuration
//const char *ssid = ""; // Enter your WiFi name
//const char *password = "";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic_sub = "<your_sub_topic>";
const char *topic_pub = "<your_pub_topic>";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);


/* Callback function to turn on LED when specific message is received */
void ledControlCallback(char *topic, byte *payload, unsigned int length) {
  char payloadStr[length] = {};
  memcpy(payloadStr, payload, length);
  Serial.printf("rcvd: %s\n", payloadStr);
  
  int payloadInt = atoi(payloadStr);
  if (payloadInt == 951) {
    digitalWrite(R_PINOUT, LOW);
  } else if (payloadInt == 852) {
    digitalWrite(G_PINOUT, LOW);
  } else if (payloadInt == 753) {
    digitalWrite(B_PINOUT, LOW);
  }

  /* Publish the state to topic_pub */
  client.publish(topic_pub, payloadStr);

  /* Turn off the LED after some time */
  delay(10000);                   // Hold the state
  digitalWrite(R_PINOUT, HIGH);   // For the provided schematic, HIGH voltage turns off the LED
  digitalWrite(G_PINOUT, HIGH);   // For the provided schematic, HIGH voltage turns off the LED
  digitalWrite(B_PINOUT, HIGH);   // For the provided schematic, HIGH voltage turns off the LED

  /* Publish the off state to topic_pub */
  client.publish(topic_pub, "0");
}

/* System initialization */
void setup() {
  // Set software serial baud to 115200;
  Serial.begin(115200);

  // TODO: Remove. Digital pin initialization as output
  pinMode(R_PINOUT, OUTPUT);
  pinMode(G_PINOUT, OUTPUT);
  pinMode(B_PINOUT, OUTPUT);
  digitalWrite(R_PINOUT, HIGH);  // For the provided schematic, HIGH voltage turns off the LED
  digitalWrite(G_PINOUT, HIGH);  // For the provided schematic, HIGH voltage turns off the LED
  digitalWrite(B_PINOUT, HIGH);  // For the provided schematic, HIGH voltage turns off the LED

  // Wi-Fi AutoConnect protocol
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("ForgetMeNot_OB", "<wifi_pw>");

  if(!res) {
    /* Failed to connect*/
    Serial.println("Failed to connect");
  } else {
    /* Successfully connected */
    Serial.print("Connected to Wi-Fi: ");
    Serial.println(WiFi.localIP());
  }
  
//  // Connect to specified Wi-Fi network
//  WiFi.begin(ssid, password);
//  Serial.print("Connecting to Wi-Fi");
//  while (WiFi.status() != WL_CONNECTED) {
//      Serial.print(".");
//      delay(500);
//  }
//  Serial.println("");
//  Serial.print("Connected to Wi-Fi: ");
//  Serial.println(WiFi.localIP());
  
  // Connect to MQTT broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(ledControlCallback);
  while (!client.connected()) {
      String client_id = "client-";
      client_id += String(WiFi.hostname());
      Serial.printf("Connecting %s to MQTT broker\n", client_id.c_str());

      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Connected to MQTT broker");
      } else {
          Serial.printf("-- Failed [%d]\n", client.state());
          delay(1000);
      }
  }
  
  // Subscribe
  client.subscribe(topic_sub);
}

/* This will be called continuously */
void loop() {
  client.loop();
  
  if (!client.connected()) {
      String client_id = "client-";
      client_id += String(WiFi.hostname());
      Serial.printf("Reconnecting %s to MQTT broker\n", client_id.c_str());

      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Reconnected to MQTT broker");
      } else {
          Serial.printf("-- Reconnection failed [%d]\n", client.state());
          delay(1000);
      }
  }
}
