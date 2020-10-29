#include <Arduino.h>
#include <ESP8266Wifi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID     "RoPC"
#define WIFI_PASSWORD "12345678"

#define FIREBASE_HOST "https://dinning-robot.firebaseio.com/"
#define FIREBASE_KEY  "ecleWhEMf3TKjgEcm1gSSX1pSd7Q6DcQ2Ybr6mGO"

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
  delay(1000);\n
  state = !state;
  if(Firebase.getBool(firebaseData, "/wio-link-01/state")) {
    state = firebaseData.boolData();
    Serial.println(state);
    delay(1000);
  }
  else {
    Serial.println("Error : " + firebaseData.errorReason());
  }
}