#include <WiFi.h>
#include <FirebaseESP32.h>
#include <Wire.h> // Required for I2C communication
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// Firebase configuration
#define FIREBASE_HOST "https://esp-32-rtdb-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "AIzaSyCF-IW8RfCopdZsNINw3_FIT_GNypR8w4E"

// WiFi credentials
#define WIFI_SSID "TP-LINK-2"
#define WIFI_PASSWORD "29823309"

// Initialize Firebase
FirebaseData firebaseData;

// Initialize ADXL345 sensor
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345); // Some arbitrary sensor ID

void setup() {
  Serial.begin(115200);

  // Initialize I2C communication
  Wire.begin();

  // Initialize ADXL345 sensor
  if (!accel.begin()) {
    Serial.println("Failed to initialize ADXL345 sensor!");
    while (1);
  }

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // Read WiFi signal strength
  int wifiStrength = WiFi.RSSI();

  // Read accelerometer data
  sensors_event_t event;
  accel.getEvent(&event);

  // Upload WiFi signal strength to Firebase
  Firebase.setInt(firebaseData, "/wifiStrength", wifiStrength);

  // Upload accelerometer data to Firebase
  Firebase.setInt(firebaseData, "/acceleration/x", event.acceleration.x);
  Firebase.setInt(firebaseData, "/acceleration/y", event.acceleration.y);
  Firebase.setInt(firebaseData, "/acceleration/z", event.acceleration.z);

  if (firebaseData.httpCode() == 200) {
    Serial.println("Data uploaded to Firebase successfully!");
  } else {
    Serial.println("Failed to upload data to Firebase!");
    Serial.println(firebaseData.errorReason());
  }

  delay(5000); // Upload data every 5 seconds
}
