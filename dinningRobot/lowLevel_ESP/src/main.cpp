#include <Arduino.h>
#include <ESP8266Wifi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "RoPC"
#define WIFI_PASSWORD "12345678"

#define FIREBASE_HOST "https://dinning-robot.firebaseio.com/"
#define FIREBASE_KEY "ecleWhEMf3TKjgEcm1gSSX1pSd7Q6DcQ2Ybr6mGO"
#define LED 2
#define LED_VALUE LOW

FirebaseData firebaseData;
bool state = false;
String Date;
String Order_number;
char command;
bool FINISH = false;

void connectWifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    // Serial.println("\nConnecting . . .");
    while (WiFi.status() != WL_CONNECTED)
    {
        // Serial.print('.');
        delay(500);
    }
    // Serial.print("\nConnected: ");
    // Serial.println(WiFi.localIP());
}

int lenIndex(String path)
{
    if (Firebase.getJSON(firebaseData, path))
    {
        FirebaseJson *json = firebaseData.jsonObjectPtr();
        size_t len = json->iteratorBegin();
        return int(len);
    }
    else
    {
        Serial.println("Error : " + firebaseData.errorReason());
        return 0;
    }
}

String getKey(String path, int index = 0)
{
    if (Firebase.getJSON(firebaseData, path))
    {
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
    else
    {
        Serial.println("Error : " + firebaseData.errorReason());
    }
}

bool readFinish(String path, int index = 0)
{
    if (Firebase.getJSON(firebaseData, path))
    {
        String key = getKey(path, index);
        path += "/" + key + "/finish";
        Firebase.getBool(firebaseData, path);
        bool state = firebaseData.boolData();
        return state;
    }
    else
    {
        Serial.println("Error : " + firebaseData.errorReason());
    }
}

void updateFinish(String path, int index = 0)
{
    if (Firebase.getJSON(firebaseData, path))
    {
        String key = getKey(path, index);
        path += "/" + key + "/finish";
        Firebase.setBool(firebaseData, path, true);
    }
    else
    {
        Serial.println("Error : " + firebaseData.errorReason());
    }
}

String getTable(String path, int index = 0)
{
    if (Firebase.getJSON(firebaseData, path))
    {
        String key = getKey(path, index);
        path += "/" + key + "/table";
        Firebase.getString(firebaseData, path);
        String table = firebaseData.stringData();
        // Serial.println(table);
        return table;
    }
    else
    {
        Serial.println("Error : " + firebaseData.errorReason());
    }
}

String getStateDate()
{
    String path = "Date";
    Firebase.getString(firebaseData, path);
    String date = firebaseData.stringData();
    // Serial.println(date);
    return date;
}

String getStateOrder()
{
    String path = "Order";
    Firebase.getString(firebaseData, path);
    String orders = firebaseData.stringData();
    // Serial.println(orders);
    return orders;
}

void readState(bool print = false)
{
    Date = getStateDate();
    Order_number = getStateOrder();
    if (print)
    {
        Serial.print("Date: ");
        Serial.println(Date);
        Serial.print("Order: ");
        Serial.println(Order_number);
        Serial.println();
    }
}

void testReadData()
{
    int index = lenIndex("/20201210/1");
    Serial.print("index: ");
    Serial.println(index);
    String key = getKey("/20201210/1");
    Serial.print("key : ");
    Serial.println(key);
    String table = getTable("/20201210/1");
    Serial.print("table: ");
    Serial.println(table);
    bool finish = readFinish("/20201210/1");
    Serial.print("Finished: ");
    Serial.println(finish);
    Serial.println();
}

void readUART(bool print = false)
{
    if (Serial.available() > 0)
    {
        command = Serial.read();
        if (print)
        {
            Serial.print("0: ");
            Serial.print(int(command));
        }
    }
}

void control(bool print = false)
{
    digitalWrite(LED, LED_VALUE);
    readState();
    String path = "/" + Date + "/" + Order_number;
    if (!readFinish(path))
    {
        char table = getTable(path).toInt();
        Serial.print(table); // Send Command to LowLevel_UNO
        if (print)
        {
            Serial.println(path);
            Serial.print("table: ");
            Serial.println(table);
        }
        while (!FINISH)
        {
            readUART(print); // readUART wait for LowLevel_UNO finish
            if (command == 1 or command == '1')
            {
                FINISH = !FINISH;
            }
            delay(3000);
        }
        updateFinish(path);
    }
    digitalWrite(LED, !LED_VALUE);
}

void setup()
{
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    connectWifi();

    Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
    // Serial.println("Connected Firebase");
}

int count = 5;
int i = 0;

void loop()
{
    control(false);
    delay(5000);
}