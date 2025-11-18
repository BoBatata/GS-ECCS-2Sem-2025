#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>

// Wi-Fi
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";

// HiveMQ Cloud
const char* HIVEMQ_BROKER = "ColoqueSeuURLAqui.s1.eu.hivemq.cloud";
const int   HIVEMQ_PORT   = 8883;
const char* HIVEMQ_USER   = "SeuUser";
const char* HIVEMQ_PASS   = "SuaSenha";

// Topics mandados e recebidos.
const char* TOPIC_STRESS  = "/planner/stress_monitor/attrs/s";
const char* TOPIC_BPM     = "/planner/stress_monitor/attrs/b";
const char* TOPIC_TEMP    = "/planner/stress_monitor/attrs/t";
const char* TOPIC_GENERAL = "/planner/stress_monitor/attrs";
const char* TOPIC_CMD     = "/planner/stress_monitor/cmd";
const char* TOPIC_XP      = "/planner/stress_monitor/attrs/xp";   // XP recebido

LiquidCrystal_I2C lcd(0x27, 16, 2);

int LED_PIN    = 4;
int BUZZER_PIN = 5;
int POT_PIN    = 34; // Stress está sendo medido no potenciômetro

WiFiClientSecure espClient;
PubSubClient client(espClient);

bool paused = false;

// Reconectar com o HiveMQ
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao HiveMQ...");
    if (client.connect("ESP32-StressMonitor", HIVEMQ_USER, HIVEMQ_PASS)) {
      Serial.println("OK!");
      client.subscribe(TOPIC_CMD);
    } else {
      Serial.print("Falhou (rc=");
      Serial.print(client.state());
      Serial.println(") tentando em 5s...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];

  Serial.print("CMD recebido: ");
  Serial.println(msg);

  // Pausar dispositivo.
  if (msg == "pause") {
    paused = true;
    lcd.clear();
    lcd.print("Processo PAUSADO");
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);

    client.publish(TOPIC_GENERAL, "Processo pausado");
  }

  // Retomar dispositivo.
  else if (msg == "resume") {
    paused = false;
    lcd.clear();
    lcd.print("Processo ativo");
    client.publish(TOPIC_GENERAL, "Processo retomado");
  }

  // Receber XP - Isso seria via o Site Planner que mandaria para cá.
  else if (msg.startsWith("xp:")) {
    String xpValue = msg.substring(3);
    lcd.clear();
    lcd.print("XP ganho: ");
    lcd.print(xpValue);

    client.publish(TOPIC_XP, xpValue.c_str());
    Serial.println("XP recebido: " + xpValue);
  }
}

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.print("Inicializando...");

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  lcd.clear();
  lcd.print("WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.print("WiFi OK");

  espClient.setInsecure();

  client.setServer(HIVEMQ_BROKER, HIVEMQ_PORT);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) reconnect();
  client.loop();

  // Se estiver pausado, não mede stress
  if (paused) {
    delay(500);
    return;
  }

  // Stress via potenciômetro
  int rawValue = analogRead(POT_PIN);
  int stressLevel = map(rawValue, 0, 4095, 0, 100);

  bool alerta = (stressLevel >= 75);

  // LCD
  lcd.clear();
  lcd.print("Stress: ");
  lcd.print(stressLevel);

  if (alerta) {
    lcd.setCursor(0, 1);
    lcd.print("PAUSA sugerida");

    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 1500, 200);
  } else {
    lcd.setCursor(0, 1);
    lcd.print("OK");
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }

  // Simulações extras
  int bpm  = random(60, 110);
  int temp = random(20, 35);

  // Envio MQTT
  client.publish(TOPIC_STRESS,  String(stressLevel).c_str());
  client.publish(TOPIC_BPM,     String(bpm).c_str());
  client.publish(TOPIC_TEMP,    String(temp).c_str());

  if (alerta)
    client.publish(TOPIC_GENERAL, "ALERTA: estresse alto");
  else
    client.publish(TOPIC_GENERAL, "Normal");

  Serial.println("Telemetria enviada");

  delay(2000);
}
