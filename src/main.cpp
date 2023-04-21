#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <WiFiClientSecure.h>

#define BMP085_ADDRESS 0x76

// Replace with your network credentials
const char* ssid = "VM9840342";
const char* password = "gmjy3VwBvwrk";

// Create an instance of the BMP180 sensor
Adafruit_BMP085 bmp;

// Create an instance of the WebServer
WebServer server(80);

<<<<<<< HEAD
// Create an instance of the display
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
=======
// Create an instance of the OLED display
Adafruit_SSD1306 display(128, 64, &Wire, -1);
>>>>>>> refs/remotes/origin/main

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

<<<<<<< HEAD
  // Serve the web page\n  server.on("/", handleRoot);
=======
  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }

  // Serve the web page
  server.on("/", handleRoot);
>>>>>>> refs/remotes/origin/main
  server.begin();

  // Display a message on the OLED screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Weather Station");
  display.println("with ESP32 WROOM");
  display.println("and BMP180 Sensor");
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  // Handle web requests
  server.handleClient();

  // Read the temperature and pressure from the BMP180 sensor
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;

  // Display the readings on the OLED screen
  display.clearDisplay();
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("Temperature: ");
  display.print("\n");
  display.print(temperature, 1);
  display.println("C");
  display.setCursor(0, 40);
  display.print("Pressure: ");
  display.print("\n");
  display.print(pressure);
  display.println("hPa");
  display.display();

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

