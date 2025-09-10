#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpuSensor; // Declaração do Sensor MPU

#define PotPin 15

int bpm = 0 // inicialização da varável de bpm

void setup() {
  Serial.begin(115200); // Inicalização do Serial, para fins de teste

  if (!mpu.begin()) { // Caso falhe a inialização do Sensor MPU
    Serial.println("Falha ao inicializar Sensor MPU");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU encontrado");

  mpuSensor.setAccelerometerRange(MPU6050_RANGE_8_G); // Declarando o raio de acelaração do Sensor
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); // Declarando o raio de rotação do Sensor
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ); // Declarando o raio de banda do Sensor

  pinMode(PotPin, INPUT);

}

void loop() {
  delay(10);
  int valorPot = analogRead(PotPin);
  eventos_sensor = a, r, temp; // Declarando todos os eventos coletados pelo sensor Aceleração (a), Rotação (r), Temperatura (temp)
  mpuSensor.getEvent(&a, &r, &temp); // Lendo os eventos captados pelo sensor
  Serial.println("Dados da aceleração"); // imprimindo dados da aceleração de cada eixo
  Serial.println("Aceleração eixo X: " + string(a.acceleration.x) + "m/s²");
  Serial.println("Aceleração eixo Y: " + string(a.acceleration.y) + "m/s²");
  Serial.println("Aceleração eixo Z: " + string(a.acceleration.z) + "m/s²");

  // imprimindo dados da temperatura local
  Serial.println("Dados da Temperatura");
  Serial.println("Temperatura do ambiente: " + string(temp.acceleration.x) + "°");

  bpm = map(valorPot, 0, 4095, 60, 200) // Mapeando valor do potenciômetro para simular um batimento cardíaco real
  Serial.println("Bpm: " + string(bpm)); // Imprime o valor simulado
}