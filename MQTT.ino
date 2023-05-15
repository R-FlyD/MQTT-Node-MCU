#include "ESP8266WiFi.h"
#include <PubSubClient.h>

//Parametros de conexão
const char* ssid = "HomeRouter 2G"; // REDE
const char* password = "1234ab1188@@##"; // SENHA


// MQTT Broker
const char *mqtt_broker = "192.168.15.202";       //Host do broket
const char *topic = "RFID";                       //Topico a ser subscrito e publicado
const char *mqtt_username = "";                   //Usuario
const char *mqtt_password = "";                   //Senha
const int mqtt_port = 1883;                       //Porta

//Variáveis
bool mqttStatus = 0;
String client_id = "ESP";

//Objetos
WiFiClient espClient;
PubSubClient client(espClient);

//Prototipos
bool connectMQTT();

void setup(void)
{
  Serial.begin(9600);

  // Conectar
  WiFi.begin(ssid, password);

  //Aguardando conexão
  Serial.println();
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //Envia IP através da UART
  Serial.println(WiFi.localIP());

  mqttStatus =  connectMQTT();
}

void loop() {if (mqttStatus){
  client.publish(topic, "{\"name\": \"Matheus\"}");
  delay(1000);
}}

bool connectMQTT() {
  client.setServer(mqtt_broker, mqtt_port);

  int tentativa = 0;
  do {
    client_id += String(WiFi.macAddress());

    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Exito na conexão:");
      Serial.printf("Cliente %s conectado ao broker\n", client_id.c_str());
      return 1;
    } else {
      Serial.print("Falha ao conectar: ");
      Serial.print(client.state());
      Serial.println();
      Serial.print("Tentativa: ");
      Serial.println(tentativa);
      delay(2000);
    }
    tentativa++;
  } while (!client.connected() && tentativa < 5);

  if(tentativa >= 5)
    Serial.println("Não conectado");    
  return 0;
}