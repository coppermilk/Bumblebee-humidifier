// Universal Telegram Bot Library v 1.3.0
// https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include "CTBot.h"

// DHT sensor library version 1.4.2
// https://github.com/adafruit/DHT-sensor-library
#include "DHT.h"

/***********************************PINS***************************************/

// Digital pin connected to the humidity sensor
#define PIN_VIN D2

// Digital pin connected to the pomp mosfet
#define PIN_POMP D3

// Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
#define PIN_DHT D5

/**********************************CONST***************************************/

// Type you're using!
// DHT 11
#define DHTTYPE DHT11

// REPLACE SSID WITH YOUR WIFI SSID
String ssid = "XXXXXXXXXXXX";
// REPLACE Password YOUR WIFI PASSWORD
String pass = "XXXXXXXXXXXX";
// REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN
String token = "XXXXXXXXXXXX";

// REPLACE WITH YOUR TELEGRAM chat id
int myId1 = XXXXXXXXXXXX;
int myId2 = XXXXXXXXXXXX;

unsigned int max_try = 10;
int pomp_try = 0;

// Experimentally derived value
#define MAX_LEVEL 350

// Create obg
DHT dht(PIN_DHT, DHTTYPE);
CTBot myBot;

void setup() {

  pinMode(PIN_VIN, OUTPUT);
  pinMode(PIN_POMP, OUTPUT);

  // Initialize the Serial and DHT11
  Serial.begin(115200);

  Serial.println(F("Starting DHT..."));
  dht.begin();

  Serial.println("Starting TelegramBot...");

  // Connect the ESP8266 to the desired access point
  myBot.wifiConnect(ssid, pass);

  // Set the telegram bot token
  myBot.setTelegramToken(token);

  // Check if all things are ok
  if (myBot.testConnection())
    Serial.println("\nConnection OK");
  else
    Serial.println("\nConnection ERROR");
}

void loop() {
  // A variable to store telegram message data
  TBMessage msg;

  // Read the input on analog pin:
  digitalWrite(PIN_VIN, HIGH);
  delay(100);
  int sensorValue = analogRead(A0);
  digitalWrite(PIN_VIN, LOW);

  // Print out the value you read:
  Serial.println(sensorValue);
  delay(1000 * 30);

  // Pomp activation
  if (sensorValue > MAX_LEVEL) {

    pomp_try = ++pomp_try;
    Serial.print("Starting pomp... ");
    Serial.print("Try: ");
    Serial.println(pomp_try);

    digitalWrite(PIN_POMP, HIGH);
    delay(15000);
    digitalWrite(PIN_POMP, LOW);
  }
// Water cheker
  if (sensorValue < MAX_LEVEL) {
    pomp_try = 0;
    Serial.print("Еnough water.");
    Serial.print("Try reset to: ");
    Serial.println(pomp_try);
  }

  // Read Humidity
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();


// Check DHT sensor
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

// Print sensor data
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));

// Check pomp try
  if (pomp_try > max_try) {
    Serial.println("Watter error. Send message to Telegram...");

    String telegram_answer;
    telegram_answer = "⚠🚨EMERGENCY ALERT FROM ESP⚠🚨\n\nNeed to top up water!\nTemperature: " + (String) t + " °C\nHumidity: " + (String) h + "%";
    myBot.sendMessage(myId1, telegram_answer);
    myBot.sendMessage(myId2, telegram_answer);
    Serial.println("message delivered");

    delay(1000 * 60 * 15);
  }

// delay for stability
  delay(1000);
}
