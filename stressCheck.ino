#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Wi-Fi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT AWS
const char* mqtt_server = "COLOQUE_IP";
const int mqtt_port = 1883;

// MQTT Tópicos
#define TOPIC_PULSE "/TEF/device/attrs/p"
#define TOPIC_TEMP  "/TEF/device/attrs/t"
#define TOPIC_CMD   "/TEF/device/cmd"
#define TOPIC_ATTRS "/TEF/device/attrs"

// Objetos
WiFiClient espClient;
PubSubClient client(espClient);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensores e atuadores
const int pulsePin = 34;
const int ledPin = 2;
const int buzzerPin = 5;

int pulseValue = 0;

void setup_wifi() {
  lcd.clear();
  lcd.print("Conectando WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }
  lcd.clear();
  lcd.print("WiFi OK");
  delay(600);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];

  if (msg == "led_on") {
    digitalWrite(ledPin, HIGH);
    lcd.clear();
    lcd.print("LED LIGADO");
    client.publish(TOPIC_ATTRS, "LED ligado");
  }
  else if (msg == "led_off") {
    digitalWrite(ledPin, LOW);
    lcd.clear();
    lcd.print("LED DESLIGADO");
    client.publish(TOPIC_ATTRS, "LED desligado");
  }
}

void reconnect() {
  while (!client.connected()) {
    lcd.clear();
    lcd.print("MQTT...");
    if (client.connect("ESP32_Client")) {
      lcd.clear();
      lcd.print("MQTT OK");
      client.subscribe(TOPIC_CMD);
    } else {
      lcd.clear();
      lcd.print("MQTT Erro...");
      delay(1500);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  lcd.clear();
  lcd.print("Sistema Pronto");
  delay(800);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  // Ler pulso (simulado no Wokwi como potenciômetro)
  pulseValue = analogRead(pulsePin);

  // Enviar pulso
  char pulseStr[8];
  dtostrf(pulseValue, 6, 2, pulseStr);
  client.publish(TOPIC_PULSE, pulseStr);

  // Temperatura fake (porque removemos MPU)
  float tempFake = 25.0 + (pulseValue % 20) * 0.1;
  char tempStr[8];
  dtostrf(tempFake, 4, 2, tempStr);
  client.publish(TOPIC_TEMP, tempStr);

  // Exibir no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pulso: ");
  lcd.print(pulseValue);

  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(tempFake);

  // Verificar estresse
  if (pulseValue > 200) {
    lcd.clear();
    lcd.print("ALTO ESTRESSE!");
    lcd.setCursor(0, 1);
    lcd.print("FAZER PAUSA!");
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1200, 300);
  }
  else if (pulseValue > 120) {
    lcd.clear();
    lcd.print("Estresse:");
    lcd.setCursor(0, 1);
    lcd.print("Moderado");
    digitalWrite(ledPin, HIGH);
  }
  else {
    digitalWrite(ledPin, LOW);
  }

  delay(1000);
}
