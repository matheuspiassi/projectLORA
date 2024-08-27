#include <HardwareSerial.h>

HardwareSerial LoRaSerial(2); // Use UART2 para a comunicação com o módulo LoRa

const int PIN_M0 = 19; // Pino M0
const int PIN_M1 = 18; // Pino M1

void setup() {
  // Configuração dos pinos M0 e M1
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);

  // M0 HIGH e M1 LOW para modo transmissor
  digitalWrite(PIN_M0, HIGH);
  digitalWrite(PIN_M1, LOW);

  // Inicializa a serial para comunicação com o módulo LoRa
  LoRaSerial.begin(9600, SERIAL_8N1, 17, 16); // Configuração do UART2
  Serial.begin(115200); // Serial para monitoramento
}

void loop() {
  String message = "Hello from Transmitter!";
  
  LoRaSerial.println(message); // Envia a mensagem via LoRa
  Serial.println("Mensagem enviada: " + message);
  
  delay(1000); // Aguarda 1 segundo antes de enviar novamente
}

//transmissor