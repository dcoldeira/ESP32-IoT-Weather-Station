#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <WiFiClientSecure.h>

// Replace with your network credentials
const char* ssid = "VM9840342";
const char* password = "gmjy3VwBvwrk";

// Create an instance of the BMP180 sensor
Adafruit_BMP085 bmp;

// Create an instance of the WebServer
WebServer server(80);

// Create an instance of the display
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 

// Function prototype for handleRoot()
void handleRoot();

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
  
  // Set up the OLED display
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tf); 
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print("Hello world!");
  u8g2.sendBuffer();

  // Initialize the BMP180 sensor
  if (!bmp.begin(BMP085_ADDRESS)) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1);
  }

  // Serve the web page\n  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  // Handle web requests
  server.handleClient();

  // Read the temperature and pressure from the BMP180 sensor
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;

  // Print the readings to the serial monitor
  Serial.print("Temperature = ");
  Serial.print(temperature, 1);
  Serial.println(" °C");
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  // Display the readings on the OLED display
  u8g2.clearBuffer();
  u8g2.setCursor(0, 10);
  u8g2.print("Temperature = ");
  u8g2.print(temperature, 1);
  u8g2.print(" C");
  u8g2.setCursor(0, 20);
  u8g2.print("Pressure = ");
  u8g2.print(pressure);
  u8g2.print(" hPa");
  u8g2.sendBuffer();

  // Wait for a second before taking the next reading
  delay(1000);
}
void handleRoot() {
  // Build the HTML page
  String html = "<html><body>";
  html += "<h1 style='text-align:center;'>Weather Station with an ESP32 WROOM 32D and a BMP180 Sensor</h1>";
  html += "<div style='display:flex; justify-content:center;'>";
  html += "<div style='margin:20px; text-align:center;'>";
  html += "<h2>Temperature</h2>";
  html += "<p style='font-size:48px;'>" + String(bmp.readTemperature(), 1) + " &deg;C</p>";
  html += "</div>";
  html += "<div style='margin:20px; text-align:center;'>";
  html += "<h2>Pressure</h2>";
  html += "<p style='font-size:48px;'>" + String(bmp.readPressure() / 100.0F) + " hPa</p>";
  html += "</div>";
  html += "</div>";
  html += "</body></html>";

  // Send the HTML page to the client
  server.send(200, "text/html", html);
}

