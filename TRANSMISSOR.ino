#include <HardwareSerial.h>
#include <TinyGPSPlus.h>

HardwareSerial LoRaSerial(2); // Use UART2 para a comunicação com o módulo LoRa

const int PIN_M0 = 19; // Pino M0
const int PIN_M1 = 18; // Pino M1

TinyGPSPlus gps;

HardwareSerial ss(1);

void setup() {
  // Configuração dos pinos M0 e M1
  Serial.begin(115200);
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);
  ss.begin(9600, SERIAL_8N1, 25, 26);
  // M0 HIGH e M1 LOW para modo transmissor
  digitalWrite(PIN_M0, HIGH);
  digitalWrite(PIN_M1, LOW);

  // Inicializa a serial para comunicação com o módulo LoRa
  LoRaSerial.begin(9600, SERIAL_8N1, 17, 16); // Configuração do UART2
  Serial.begin(115200); // Serial para monitoramento
}

void loop() {
   while (ss.available() > 0) {
    gps.encode(ss.read());  // Decodifica os dados NMEA do GPS

    // Verifica se a localização foi atualizada com dados válidos
    if (gps.location.isUpdated()) {
      double latitude = gps.location.lat();
      double longitude = gps.location.lng();
      String dadosParaEnviar = "{\"longitude\": ";
      dadosParaEnviar += String(longitude, 6);
      dadosParaEnviar += ", \"latitude\": ";
      dadosParaEnviar += String(latitude, 6);  

      Serial.print("Latitude: ");
      Serial.println(latitude, 6);  // Exibe a latitude no Serial Monitor com 6 casas decimais
      Serial.print("Longitude: ");
      Serial.println(longitude, 6); 

      LoRaSerial.println(dadosParaEnviar); // Envia a mensagem via LoRa
      delay(1000); // Aguarda 1 segundo antes de enviar novamente
    }
   }
}

//transmissor
