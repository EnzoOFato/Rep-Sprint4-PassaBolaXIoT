#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wifi.h>
#include <PubSubClient.h>

Adafruit_MPU6050 mpuSensor; // Declaração do Sensor MPU

#define PotPin 15


// Configuração da Rede
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Configuração HiveMQ
const char* mqtt_server = "8ba3b8e02cee4063850876e30806f07c.s1.eu.hivemq.cloud";  // endereço do cluster
const int mqtt_port = 8883;  // Porta segura
const char* mqtt_user = "hivemq.webclient.1757562136665";  
const char* mqtt_password = "l.8I!:P3Ve<qWhu4Jv7R"; 
const char* mqtt_topic = "iot/sensores/desempenho";  // tópico central

WiFiClientSecure espClient;
PubSubClient client(espClient);

int bpm = 0; // inicialização da varável de bpm

void setup_wifi() { //Método de conexão wifi via próprio provedor Wokwi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { // Enquanto não estiver conectado ao wifi
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado");
}

void reconectar() { // Reconectar ao HiveMQ
  while (!client.connected()) {
    Serial.print("Tentando conectar ao HiveMQ...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Conectado ao HiveMQ Cluster!"); // Sucesso na Reconexão
    } else {
      Serial.print("Falha, rc= ");
      Serial.println(client.state()); // Mensagem da falha
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200); // Inicalização do Serial, para fins de teste

  setup_wifi(); //Conectar ao Wifi
  espClient.setInsecure(); // Para testes sem certificado CA
  client.setServer(mqtt_server, mqtt_port); // Conexão do Setup ao HiveMQ

  if (!mpuSensor.begin()) { // Caso falhe a inialização do Sensor MPU (Isso em um cenário real, não falhará no Wokwi)
    Serial.println("Falha ao inicializar Sensor MPU");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU encontrado");

  mpuSensor.setAccelerometerRange(MPU6050_RANGE_8_G); // Declarando o raio de acelaração do Sensor
  mpuSensor.setGyroRange(MPU6050_RANGE_500_DEG); // Declarando o raio de rotação do Sensor
  mpuSensor.setFilterBandwidth(MPU6050_BAND_5_HZ); // Declarando o raio de banda do Sensor

  pinMode(PotPin, INPUT);

}

void loop() {

  if(!client.connected()){ // Caso a conexão caía durante o andamento da aplicação
    reconectar();
  }

  client.loop();

  delay(10);
  int valorPot = analogRead(PotPin);
  sensors_event_t a, r, temp; // Declarando todos os eventos coletados pelo sensor Aceleração (a), Rotação (r), Temperatura (temp)
  mpuSensor.getEvent(&a, &r, &temp); // Lendo os eventos captados pelo sensor
  Serial.println("Dados da aceleração"); // imprimindo dados da aceleração de cada eixo
  Serial.print("Aceleração eixo X: ");
  Serial.print(a.acceleration.x);
  Serial.println(" m/s²");
  Serial.print("Aceleração eixo Y: ");
  Serial.print(a.acceleration.y);
  Serial.println(" m/s²");
  Serial.print("Aceleração eixo Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s²");
  

  // imprimindo dados da temperatura local
  Serial.println("Dados da Temperatura");
  Serial.print("Temperatura ambiente: ");
  Serial.print(temp.temperature);
  Serial.println("°");

  bpm = map(valorPot, 0, 4095, 60, 200); // Mapeando valor do potenciômetro para simular um batimento cardíaco real
  Serial.print("Bpm: "); // Imprime o valor simulado
  Serial.println(bpm);

  String envio = "{";
  envio += "\"bpm\": " + String(bpm) + ",";
  envio += "\"temp\": " + String(temp.temperature) + ",";
  envio += "\"ax\": " + String(a.acceleration.x) + ",";
  envio += "\"ay\": " + String(a.acceleration.y) + ",";
  envio += "\"az\": " + String(a.acceleration.z);
  envio += "}"; // Construindo a mensagem a ser enviada ao HiveMQ

  client.publish(mqtt_topic, envio.c_str()); // Publicando a mensagem no Brooker
  Serial.println("Dados enviados: " + envio);

  delay(2000); // Tempo para a próxima mensagem
}