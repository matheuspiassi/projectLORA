#include <HardwareSerial.h>

HardwareSerial LoRaSerial(2); 

const int PIN_M0 = 19; 
const int PIN_M1 = 18; 

void setup() {
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);
  digitalWrite(PIN_M0, LOW);
  digitalWrite(PIN_M1, HIGH);

  LoRaSerial.begin(9600, SERIAL_8N1, 17, 16); 
  Serial.begin(115200); 
}

void loop() {
  if (LoRaSerial.available()) {
    String receivedMessage = LoRaSerial.readStringUntil('\n');
    Serial.println("recebido: " + receivedMessage);
  }
}
