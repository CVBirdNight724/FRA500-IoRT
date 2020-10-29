#include <Arduino.h>
#include <ESP8266Wifi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID     "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"

#define FIREBASE_HOST "FIREBASE_HOST"
#define FIREBASE_KEY  "FIREBASE_KEY"

#define LED D13

FirebaseData firebaseData;
bool state = false;

void connectWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("\nConnecting . . .");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print('.');
    delay(500);
  }
  Serial.print("\nConnected: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  connectWifi();

  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
  Serial.println("Connected Firebase");
}

void loop() {
  delay(1000);
  state = !state;
  if(Firebase.getBool(firebaseData, "/20201027/1/Status")) {
    state = firebaseData.boolData();
    Serial.println(state);
    delay(1000);
  }
  else {
    Serial.println("Error : " + firebaseData.errorReason());
  }
}