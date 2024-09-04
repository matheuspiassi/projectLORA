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
    Serial.println("Recebido: " + receivedMessage);

    // Extrair longitude, latitude, aceleração e giroscópio da string recebida
    double longitude = extrairValor(receivedMessage, "longitude");
    double latitude = extrairValor(receivedMessage, "latitude");
    double accel_x = extrairValor(receivedMessage, "accel_x");
    double accel_y = extrairValor(receivedMessage, "accel_y");
    double accel_z = extrairValor(receivedMessage, "accel_z");

    // Exibir latitude e longitude no display LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lat: ");
    lcd.print(latitude, 2);
    lcd.setCursor(0, 1);
    lcd.print("Lon: ");
    lcd.print(longitude, 2);
    
    delay(2000); // Mostrar por 2 segundos
    
    // Exibir aceleração no display LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Acc X: ");
    lcd.print(accel_x, 2);
    lcd.setCursor(0, 1);
    lcd.print("Y: ");
    lcd.print(accel_y, 2);
    lcd.print(" Z: ");
    lcd.print(accel_z, 2);
    
    delay(2000); // Mostrar por 2 segundos

    // Exibir valores no monitor serial
    Serial.print("Latitude: ");
    Serial.println(latitude, 6);
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);
    Serial.print("Aceleração X: ");
    Serial.println(accel_x, 2);
    Serial.print("Aceleração Y: ");
    Serial.println(accel_y, 2);
    Serial.print("Aceleração Z: ");
    Serial.println(accel_z, 2);
  }
}

double extrairValor(String json, String chave) {
  int chaveInicio = json.indexOf(chave);
  if (chaveInicio == -1) {
    return 0.0;
  }
  int inicio = json.indexOf(":", chaveInicio);
  int fim = json.indexOf(",", inicio);
  if (fim == -1) {
    fim = json.indexOf("}", inicio);
  }
  String valorString = json.substring(inicio + 1, fim);
  valorString.trim(); // Remove espaços em branco
  return valorString.toDouble();
}
