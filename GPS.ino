#include <LiquidCrystal_I2C.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

// Inicializa o display LCD com o endereço I2C 0x27 e o tamanho 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Cria uma instância do objeto GPS
TinyGPSPlus gps;

// Configura uma serial dedicada para o GPS
HardwareSerial ss(1);

void setup() {
  Serial.begin(115200);    // Inicializa a Serial para monitoramento
  ss.begin(9600, SERIAL_8N1, 25, 26);  // Configura a serial do GPS: taxa de transmissão, formato, RX, TX
  lcd.init();  // Inicializa o LCD
  lcd.backlight();  // Liga a luz de fundo do LCD
  Serial.println("Inicializando GPS...");
}

void latitude(double valor) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Lat:  ");
  lcd.print(valor, 6);  // Exibe a latitude com 6 casas decimais
}

void longitude(double valor) {
  lcd.setCursor(0, 1);
  lcd.print("Long: ");
  lcd.print(valor, 6);  // Exibe a longitude com 6 casas decimais
}

void loop() {
  // Verifica se há dados disponíveis do GPS
  while (ss.available() > 0) {
    gps.encode(ss.read());  // Decodifica os dados NMEA do GPS

    // Verifica se a localização foi atualizada com dados válidos
    if (gps.location.isUpdated()) {
      double lat = gps.location.lat();  // Obtém a latitude
      double lng = gps.location.lng();  // Obtém a longitude

      Serial.print("Latitude: ");
      Serial.println(lat, 6);  // Exibe a latitude no Serial Monitor com 6 casas decimais
      Serial.print("Longitude: ");
      Serial.println(lng, 6);  // Exibe a longitude no Serial Monitor com 6 casas decimais

      latitude(lat);  // Exibe a latitude no LCD
      longitude(lng);  // Exibe a longitude no LCD

      Serial.print("Satélites: ");
      Serial.println(gps.satellites.value());  // Exibe o número de satélites no Serial Monitor
      Serial.print("Altitude: ");
      Serial.println(gps.altitude.meters());  // Exibe a altitude no Serial Monitor
      Serial.println();
    }
  }
}
