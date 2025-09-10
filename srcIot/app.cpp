#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpuSensor; // Declaração do Sensor MPU

#define PotPin 15

int bpm = 0; // inicialização da varável de bpm

void setup() {
  Serial.begin(115200); // Inicalização do Serial, para fins de teste

  if (!mpuSensor.begin()) { // Caso falhe a inialização do Sensor MPU
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

  delay(5000); // Intervalo do envio de dados
}