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
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(500);
    }
    Serial.print("\nConnected: ");
    Serial.println(WiFi.localIP());
}

int lenIndex(String path){
    if (Firebase.getJSON(firebaseData, path)){
        FirebaseJson *json = firebaseData.jsonObjectPtr();
        size_t len = json->iteratorBegin();
        // Serial.println(len);
        return int(len);
    }
    else{
        Serial.println("Error : " + firebaseData.errorReason());
        return 0;
    }
}

String getKey(String path, int index){
    if (Firebase.getJSON(firebaseData, path)){
        FirebaseJson *json = firebaseData.jsonObjectPtr();
        json->iteratorBegin();
        String key, value;
        int type = 0;
        json->iteratorGet(size_t(index), type, key, value);
        json->iteratorEnd();
        json->clear();
        // Serial.println(key);
        return key;
    }
    else{
        Serial.println("Error : " + firebaseData.errorReason());
    }
}

bool readFinish(String path, int index){
    if (Firebase.getJSON(firebaseData, path)){
        String key = getKey(path, index);
        path += "/" + key + "/finish";
        Firebase.getBool(firebaseData, path);
        bool state = firebaseData.boolData();
        return state;
    }
    else{
        Serial.println("Error : " + firebaseData.errorReason());
    }
}

void updateFinish(String path, int index){
    if (Firebase.getJSON(firebaseData, path)){
        String key = getKey(path, index);
        path += "/" + key + "/finish";
        Firebase.setBool(firebaseData, path, true);
    }
    else{
        Serial.println("Error : " + firebaseData.errorReason());
    }
}

bool readTable(String path, int index=1){
    if (Firebase.getJSON(firebaseData, path)){
        String key = getKey(path, index);
        path += "/" + key + "/finish";
        Firebase.getBool(firebaseData, path);
        bool state = firebaseData.boolData();
        return state;
    }
    else{
        Serial.println("Error : " + firebaseData.errorReason());
    }
}



void setup(){
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    // connectWifi();

    // Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
    // Serial.println("Connected Firebase");
}

void loop(){
    Serial.print("1");
    Serial.print("0");
    delay(1000);

    // int index = lenIndex("/20201027/1");
    // Serial.println("index: " + index);
    // String key = getKey("/20201027/1", 0);
    // Serial.println("key : " + key);
    // readFinish("/20201027/1", 0);
    // delay(3000);
}