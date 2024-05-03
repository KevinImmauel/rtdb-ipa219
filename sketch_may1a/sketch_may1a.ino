#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define API_KEY "AIzaSyCF-IW8RfCopdZsNINw3_FIT_GNypR8w4E"
#define DB_URL "https://esp-32-rtdb-default-rtdb.asia-southeast1.firebasedatabase.app/"

#define WIFI_SSID "TP-LINK-2"
#define WIFI_PASSWORD "29823309"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signUpOK = false;
int wifiData = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to the WiFi network");

  config.api_key = API_KEY;
  config.database_url = DB_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("SignUp OK");
    signUpOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  Firebase.begin(&config, &auth);
  Firebase.reconnectWifi(true);
}

void loop() {
  if (Firebase.ready() && signUpOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    int32_t rssi = WiFi.RSSI();
    if (Firebase.RTDB.setInt(&fbdo, "Sensor/Wifi_data", rssi)) {
      Serial.println(); Serial.println(rssi);
    } else {
      Serial.println("Failed: " + fbdo.errorReason());
    }
  }

  delay(10000); // Check every 10 seconds
}
