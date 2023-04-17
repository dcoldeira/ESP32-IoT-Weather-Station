#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <WiFi.h>
#include <WebServer.h>
#define BMP085_ADDRESS 0x76
#include <SPI.h>
#include <WiFiClientSecure.h>

// Replace with your network credentials
const char* ssid = "VM9840342";
const char* password = "gmjy3VwBvwrk";

// Create an instance of the BMP180 sensor
Adafruit_BMP085 bmp;

// Create an instance of the WebServer
WebServer server(80);

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

  // Initialize the BMP180 sensor
  if (!bmp.begin(BMP085_ADDRESS)) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1);
  }

  // Serve the web page
  server.on("/", handleRoot);
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

  // Wait for a second before taking the next reading
  delay(1000);
}

void handleRoot() {
  // Build the HTML page
  String html = "<html><body>";
  html += "<h1>BMP085 Sensor Readings</h1>";
  html += "<p>Temperature: ";
  html += bmp.readTemperature(), 1;
  html += " °C</p>";
  html += "<p>Pressure: ";
  html += bmp.readPressure() / 100.0F;
  html += " hPa</p>";
  html += "</body></html>";

  // Send the HTML page to the client
  server.send(200, "text/html", html);
}
