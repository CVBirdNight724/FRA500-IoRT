#include <Arduino.h>

#define CH_1    2

volatile int nThrottleIn = 0;
volatile unsigned long ulStartPeriod = 0;
volatile boolean bNewThrottleSignal = false;

void calcInput(){
    if(digitalRead(CH_1) == HIGH){
        ulStartPeriod = micros();
    }
    else{
        if(ulStartPeriod && (bNewThrottleSignal == false)){
            nThrottleIn = int(micros() - ulStartPeriod);
            ulStartPeriod = 0;
            bNewThrottleSignal = true;
        }
    }
}


void setup(){
    Serial.begin(115200);
    pinMode(CH_1, INPUT);
    attachInterrupt(digitalPinToInterrupt(CH_1), calcInput, CHANGE);
}

void loop(){
    if(bNewThrottleSignal){
        Serial.println(nThrottleIn);
        bNewThrottleSignal = false;
    }
}