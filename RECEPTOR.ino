#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>

HardwareSerial LoRaSerial(2);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int PIN_M0 = 19;
const int PIN_M1 = 18;
const int BTN_UP = 12;     
const int BTN_DOWN = 14;   
const int BTN_SELECT = 13; 


double longitude = 0.0, latitude = 0.0;
double accel_x = 0.0, accel_y = 0.0, accel_z = 0.0;

int menuIndex = 0;          
const int numOptions = 2;   // Total de opções no menu
bool displaySelected = false;
int lastMenuIndex = -1;     
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 2000; 

void setup() {
  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);
  digitalWrite(PIN_M0, LOW);
  digitalWrite(PIN_M1, HIGH);
  lcd.init();
  lcd.backlight();

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);

  LoRaSerial.begin(9600, SERIAL_8N1, 17, 16);
  Serial.begin(115200);
}

void loop() {
  
  if (LoRaSerial.available()) {
    String receivedMessage = LoRaSerial.readStringUntil('\n');
    Serial.println("Recebido: " + receivedMessage);

    
    longitude = extrairValor(receivedMessage, "longitude");
    latitude = extrairValor(receivedMessage, "latitude");
    accel_x = extrairValor(receivedMessage, "accel_x");
    accel_y = extrairValor(receivedMessage, "accel_y");
    accel_z = extrairValor(receivedMessage, "accel_z");
  }

  
  if (!displaySelected) {
    if (digitalRead(BTN_UP) == LOW) {
      menuIndex = (menuIndex - 1 + numOptions) % numOptions;
      delay(200);
    } else if (digitalRead(BTN_DOWN) == LOW) {
      menuIndex = (menuIndex + 1) % numOptions;
      delay(200); 
    } else if (digitalRead(BTN_SELECT) == LOW) {
      displaySelected = true;
      delay(200); 
    }

    
    if (menuIndex != lastMenuIndex) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Menu:");
      lcd.setCursor(0, 1);
      
      
      switch (menuIndex) {
        case 0:
          lcd.print("> Coordenadas");
          break;
        case 1:
          lcd.print("> Giroscopio");
          break;
      }
      lastMenuIndex = menuIndex; 
    }
  } else {
    
    if (millis() - lastUpdate > updateInterval) { 
      lcd.clear();
      switch (menuIndex) {
        case 0:
          // Exibir latitude e longitude
          lcd.setCursor(0, 0);
          lcd.print("Lat: ");
          lcd.print(latitude, 2);
          lcd.setCursor(0, 1);
          lcd.print("Lon: ");
          lcd.print(longitude, 2);
          break;
        case 1:
          // Exibir dados do giroscópio
          lcd.setCursor(0, 0);
          lcd.print("Acc X: ");
          lcd.print(accel_x, 2);
          lcd.setCursor(0, 1);
          lcd.print("Y: ");
          lcd.print(accel_y, 2);
          lcd.print(" Z: ");
          lcd.print(accel_z, 2);
          break;
      }
      lastUpdate = millis(); 
      displaySelected = false; 
    }
  }

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
  valorString.trim(); 
  return valorString.toDouble();
}
