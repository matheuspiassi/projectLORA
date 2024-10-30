#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

HardwareSerial LoRaSerial(2);
HardwareSerial ss(1);

const int PIN_M0 = 19;
const int PIN_M1 = 18;

TinyGPSPlus gps;
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);
  
  ss.begin(9600, SERIAL_8N1, 25, 26);
  LoRaSerial.begin(9600, SERIAL_8N1, 17, 16);
  
  digitalWrite(PIN_M0, HIGH);
  digitalWrite(PIN_M1, LOW);

  // Verifica se o módulo GY-521 (MPU6050) está conectado
  if (!mpu.begin()) {
    Serial.println("Falha ao conectar o módulo");
    while (1) {
      delay(10);
    }
  }

  Serial.println("Módulo conectado");
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  while (ss.available() > 0) {
    gps.encode(ss.read());

    if (gps.location.isUpdated()) {
      // Captura os valores do GPS
      double latitude = gps.location.lat();
      double longitude = gps.location.lng();

      // Captura os valores do giroscópio e acelerômetro
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      // Cria a string JSON para enviar
      String dadosParaEnviar = "{\"longitude\": ";
      dadosParaEnviar += String(longitude, 2);
      dadosParaEnviar += ", \"latitude\": ";
      dadosParaEnviar += String(latitude, 2);
      dadosParaEnviar += ", \"accel_x\": ";
      dadosParaEnviar += String(a.acceleration.x, 2);
      dadosParaEnviar += ", \"accel_y\": ";
      dadosParaEnviar += String(a.acceleration.y, 2);
      dadosParaEnviar += ", \"accel_z\": ";
      dadosParaEnviar += String(a.acceleration.z, 2);
      dadosParaEnviar += "}";

      // Imprime os valores no monitor serial
      Serial.print("Latitude: ");
      Serial.println(latitude, 2);
      Serial.print("Longitude: ");
      Serial.println(longitude, 2);

      Serial.print("Aceleração X: ");
      Serial.print(a.acceleration.x);
      Serial.print(", Y: ");
      Serial.print(a.acceleration.y);
      Serial.print(", Z: ");
      Serial.print(a.acceleration.z);
      Serial.println(" m/s^2");

      // Envia os dados via LoRa
      LoRaSerial.println(dadosParaEnviar);
      delay(500);  // Atraso entre as transmissões
    }
  }
}
