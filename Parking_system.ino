#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

// Define I2C pins for ESP32-WROOM-32
#define SDA_PIN 21
#define SCL_PIN 22

// Initialize LCD (set the I2C address; adjust if necessary)
LiquidCrystal_PCF8574 lcd(0x27);

// Define pins for ultrasonic sensors
const int TRIG1 = 5;
const int ECHO1 = 18;
const int TRIG2 = 4;
const int ECHO2 = 19;

// WiFi credentials
const char* ssid = "Your_WiFi_Name";
const char* password = "Your_WiFi_Password";

// Create a web server on port 80
WiFiServer server(80);

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Initialize I2C communication for LCD
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Initialize LCD
  lcd.begin(16, 2);          // 16 columns, 2 rows
  lcd.setBacklight(255);     // Turn on backlight
  
  // Set up ultrasonic sensor pins
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  
  // Initial LCD display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Parking");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Start the web server
  server.begin();
}

long getDistance(int trigPin, int echoPin) {
  // Clear the trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Trigger ultrasonic burst
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echo pin (timeout after 30ms)
  long duration = pulseIn(echoPin, HIGH, 30000);
  
  // Check for timeout
  if (duration == 0) {
    return -1; // Indicates timeout/error
  }
  
  // Calculate distance (in cm)
  long distance = duration * 0.034 / 2;
  return distance;
}

void loop() {
  // Measure distances from both sensors
  long distance1 = getDistance(TRIG1, ECHO1);
  long distance2 = getDistance(TRIG2, ECHO2);
  
  // Determine if there's a sensor error
  bool sensorError = (distance1 == -1 || distance2 == -1);
  
  // Calculate parking vacancy only if no sensor error
  bool spot1_vacant = false;
  bool spot2_vacant = false;
  if (!sensorError) {
    spot1_vacant = (distance1 > 10);
    spot2_vacant = (distance2 > 10);
  }
  
  // Debug output on Serial
  Serial.print("Distance 1: ");
  Serial.print(distance1);
  Serial.print(" cm, Distance 2: ");
  Serial.print(distance2);
  Serial.println(" cm");
  
  // Update LCD display
  lcd.clear();
  lcd.setCursor(0, 0);
  if (sensorError) {
    lcd.print("Sensor Error");
    lcd.setCursor(0, 1);
    lcd.print("Check Wiring");
  } else {
    if (spot1_vacant && spot2_vacant) {
      lcd.print("2 Spots Free");
      lcd.setCursor(0, 1);
      lcd.print("Use Spot 1");
      Serial.println("Recommend: Use Spot 1");
    } else if (spot1_vacant) {
      lcd.print("Spot 1 Free");
      lcd.setCursor(0, 1);
      lcd.print("Park Here");
      Serial.println("Recommend: Use Spot 1");
    } else if (spot2_vacant) {
      lcd.print("Spot 2 Free");
      lcd.setCursor(0, 1);
      lcd.print("Park Here");
      Serial.println("Recommend: Use Spot 2");
    } else {
      lcd.print("Parking Full");
      lcd.setCursor(0, 1);
      lcd.print("Please Wait");
      Serial.println("All Spots Full");
    }
  }
  
  // Handle web server client connections
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    // Wait until client sends data
    while (client.connected() && !client.available()) {
      delay(1);
    }
    
    // Read the client's request (not processed further in this simple example)
    String request = client.readStringUntil('\r');
    Serial.println("Request: " + request);
    client.flush();
    
    // Prepare HTML response with sensor data
    String html = "<!DOCTYPE html><html><head><meta http-equiv='refresh' content='5'>";
    html += "<title>Smart Parking System</title></head><body>";
    html += "<h1>Parking Sensor Data</h1>";
    if (sensorError) {
      html += "<p>Error: Sensor Error. Check Wiring.</p>";
    } else {
      html += "<p>Distance 1: " + String(distance1) + " cm</p>";
      html += "<p>Distance 2: " + String(distance2) + " cm</p>";
      
      if (spot1_vacant && spot2_vacant) {
        html += "<p>Recommendation: Use Spot 1</p>";
      } else if (spot1_vacant) {
        html += "<p>Recommendation: Use Spot 1</p>";
      } else if (spot2_vacant) {
        html += "<p>Recommendation: Use Spot 2</p>";
      } else {
        html += "<p>Parking Full: Please Wait</p>";
      }
    }
    html += "</body></html>";
    
    // Send HTTP response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println(html);
    
    delay(1);
    client.stop();
    Serial.println("Client disconnected");
  }
  
  delay(1000); // Update every second
}