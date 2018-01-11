/*
 * Title: Home Automation
 * Description : Nodemcu Alexa Smart Switch
 */
#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

/* >>WiFi Setting<< */
const char* ssid = "SSID";
const char* password = "PASSWORD";

/* >>YOUR HUB <<*/
// Host Name
const char* host = "SmartHomeSystem";
// This is Not Neccessary to change 
const char* update_path = "/firmware";
// Username to access the openhab web page(your openhab username)
const char* update_username = "############";
// Password to access the openhab page(your openhab password)
const char* update_password = "###########";

/* >>MQTT<< */
// Topic which listens for commands
char* subscribeTopic = "Home-Automation/utilities/SmartHomeSystem"; 
//MQTT Server IP Address
const char* server = "RaspberryPi IP Address";
//Unique device ID 
const char* mqttDeviceID = "SmartHomeDevice";

/* >>Nodemcu pins to relay which will act as Smart Switch << */
int channel1 = 14;
int channel2 = 12;
int channel3 = 13;

/* >> No  need to change further<< */
//webserver 
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

//MQTT
WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

//Connect to WiFI and MQTT
void connect();

//Setup pins, wifi, webserver and MQTT
void setup() 
{
  // set pin modes
  pinMode(channel1, OUTPUT);
  digitalWrite(channel1, LOW);

  pinMode(channel2, OUTPUT);
  digitalWrite(channel2, LOW);

  pinMode(channel3, OUTPUT);
  digitalWrite(channel3, LOW);

  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  client.begin(server, net);
  client.onMessage(messageReceived);

  connect();

  MDNS.begin(host);

  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
}

//Connect to wifi and MQTT
void connect() 
{
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
  }

  while (!client.connect(mqttDeviceID)) 
  {
    delay(1000);
  }

  client.subscribe(subscribeTopic);
}

void loop() 
{
  // MQTT Loop
  client.loop();
  delay(10);

  // Make sure device is connected
  if(!client.connected()) 
  {
    connect();
  }

  httpServer.handleClient();

}

// Change the state of a relay based on the MQTT Message
void messageReceived(String &topic, String &payload) 
{
    String msgString = payload;

  if (msgString == "1ON")
  {
    digitalWrite(channel1, HIGH);
    delay(250);
  }
  else if (msgString == "1OFF")
  {
    digitalWrite(channel1, LOW);
    delay(250);
  }
  else if (msgString == "2ON")
  {
    digitalWrite(channel2, HIGH);
    delay(250);
  }
  else if (msgString == "2OFF")
  {
    digitalWrite(channel2, LOW);
    delay(250);
  }
  else if (msgString == "3ON")
  {
    digitalWrite(channel3, HIGH);
    delay(250);
  }
  else if (msgString == "3OFF")
  {
    digitalWrite(channel3, LOW);
    delay(250);
  }
}


