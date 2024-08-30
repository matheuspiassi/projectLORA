#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>

HardwareSerial LoRaSerial(2); 
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int PIN_M0 = 19; 
const int PIN_M1 = 18; 

void setup() {
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);
  digitalWrite(PIN_M0, LOW);
  digitalWrite(PIN_M1, HIGH);
  lcd.init();
  lcd.backlight();

  LoRaSerial.begin(9600, SERIAL_8N1, 17, 16); 
  Serial.begin(115200); 
}

void loop() {
  if (LoRaSerial.available()) {
    String receivedMessage = LoRaSerial.readStringUntil('\n');
    Serial.println("recebido: " + receivedMessage);
    lcd.print(receivedMessage);
  }
}
