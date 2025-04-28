#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "Aaron iPhone";
const char* password = "youareaperson1";

// IP of the machine running Mosquitto (on your local network)
const char* mqtt_server = "34.168.240.96";
const int ledPin = 2; // Pin connected to the LED

WiFiClient espClient;
PubSubClient mqttClient(espClient);

//MQTT callback function
void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  //Control the LED based on the message
  if(payload[0] == '1'){
    digitalWrite(ledPin, HIGH);
  }else if (payload[0] == '2'){
    digitalWrite(ledPin, LOW);
  }else if (payload[0] == '3'){
    Serial.println("Ending program...");
    while(true){
      // Infinite loop to "end" the program
      delay(1000);
    }
  }
}

void setup() {
  //Start Serial Comm
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  //Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  //Connect to Mosquitto
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);

  while(!mqttClient.connected()){
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("ESP32Client")){
      Serial.println("connected");
      mqttClient.subscribe("esp32/led");
    }else{
      Serial.print("failed, rc");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
}

void loop() {
  mqttClient.loop();
}
