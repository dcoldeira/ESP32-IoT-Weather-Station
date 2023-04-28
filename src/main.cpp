#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <WiFiClientSecure.h>
#include <RTClib.h>

#define BMP085_ADDRESS 0x76

// Replace with your network credentials
const char* ssid = "zzz";
const char* password = "xxxx";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_PIN 2
#define BUZZER_PIN 16
#define LED_PIN 17
#define PHOTO_PIN A0
#define PHOTO_RESISTOR 10000

// Create an instance of the BMP180 sensor
Adafruit_BME280 bme;

// Create an instance of the WebServer
WebServer server(80);

// Create an instance of the real-time clock
RTC_DS3231 rtc;
// Function prototype for handleRoot()
void handleRoot();

// Function prototype for getFormattedTime()
String getFormattedTime();

int buttonState = 0;
int buzzerState = LOW;
int ledState = LOW;
int mode = 0;
int buzzerThreshold = 500;
int ledThreshold = 2000;

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print the IP address of the board
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Starting up...");
  display.display();
  if (!bme.begin(0x76)) {
    display.println("Could not find a valid BMP180 sensor, check wiring!");
    display.display();
    while (1);
  }
}

String getFormattedTime() {
  // Get the current time
  time_t now = time(nullptr);

  // Convert the current time to a struct tm
  struct tm *timeinfo = localtime(&now);

  // Create a string for the formatted time
  char formattedTime[20];
  strftime(formattedTime, sizeof(formattedTime), "%H:%M:%S", timeinfo);

  // Return the formatted time as a string
  return String(formattedTime);
}

void loop() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F;
  int lightLevel = analogRead(PHOTO_PIN);
  int buzzerOn = (pressure > buzzerThreshold || temperature > buzzerThreshold) ? 1 : 0;
  int ledOn = (pressure > ledThreshold || temperature > ledThreshold) ? 1 : 0;
  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH) {
    mode = (mode + 1) % 3;
    delay(500);
  }
  display.clearDisplay();
  display.setCursor(0, 0);
  switch (mode) {
    case 0:
      display.println("Temperature:");
      display.print(temperature);
      display.println(" *C");
      break;
    case 1:
      display.println("Humidity:");
      display.print(humidity);
      display.println("%");
      break;
    case 2:
      display.println("Pressure:");
      display.print(pressure);
      display.println(" hPa");
      break;
  }
  if (buzzerOn == 1) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
  if (ledOn == 1) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  delay(1000);
}
