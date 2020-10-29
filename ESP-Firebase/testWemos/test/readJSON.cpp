#include <Arduino.h>
#include <ESP8266Wifi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"

#define FIREBASE_HOST "FIREBASE_HOST"
#define FIREBASE_KEY "FIREBASE_KEY"

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
  if(Firebase.getJSON(firebaseData, "/20201027/1")) {
    FirebaseJson *json = firebaseData.jsonObjectPtr();
    size_t len = json->iteratorBegin();
    String key, value;
    int type = 0;
    // json->iteratorGet(size_t(1), type, key, value);
    // Serial.println(key + " : " + value);
    for (size_t index = 0; index < len; index++){
        json->iteratorGet(index, type, key, value);
        if (type == FirebaseJson::JSON_OBJECT && key.length() > 1) {
            Serial.print(index);
            Serial.println(" : " + key + " : " + value);
        }
    }
    Serial.println();
    json->iteratorEnd();
    json->clear();
    delay(1000);
  }
  else {
    Serial.println("Error : " + firebaseData.errorReason());
  }
}