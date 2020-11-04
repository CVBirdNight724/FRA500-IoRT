#include <Arduino.h>

byte last_channel_1, last_channel_2, last_channel_3, last_channel_4;
int receiver_channel_1, receiver_channel_2, receiver_channel_3, receiver_channel_4;
unsigned long timer_1, timer_2, timer_3, timer_4;

ISR(PCINT0_vect){
    if(last_channel_1 == 0 && (PINB & B00000001)){
        last_channel_1 = 1;
        timer_1 = micros();
    }
    else if(last_channel_1 == 1 && !(PINB & B00000001)){
        last_channel_1 = 0;
        receiver_channel_1 = micros() - timer_1;
    }
    
    if(last_channel_2 == 0 && (PINB & B00000010)){
        last_channel_2 = 1;
        timer_2 = micros();
    }
    else if(last_channel_2 == 1 && !(PINB & B00000010)){
        last_channel_2 = 0;
        receiver_channel_2 = micros() - timer_2;
    }

    if(last_channel_3 == 0 && (PINB & B00000100)){
        last_channel_3 = 1;
        timer_3 = micros();
    }
    else if(last_channel_3 == 1 && !(PINB & B00000100)){
        last_channel_3 = 0;
        receiver_channel_3 = micros() - timer_3;
    }

    if(last_channel_4 == 0 && (PINB & B00001000)){
        last_channel_4 = 1;
        timer_4 = micros();
    }
    else if(last_channel_4 == 1 && !(PINB & B00001000)){
        last_channel_4 = 0;
        receiver_channel_4 = micros() - timer_4;
    }
}

void printSignal(){
    Serial.print("CH1 : ");
    Serial.println(receiver_channel_1);
    Serial.print("CH2 : ");
    Serial.println(receiver_channel_2);
    Serial.print("CH3 : ");
    Serial.println(receiver_channel_3);
    Serial.print("CH4 : ");
    Serial.println(receiver_channel_4);
    Serial.println();
}

void setup(){
    PCICR |= (1 << PCIE0);          // set PCIE0 to enable PCMSK0 scan
    PCMSK0 |= (1 << PCINT0);        // set PCINT0 (pin 8) to trigger interrupt
    PCMSK0 |= (1 << PCINT1);        // set PCINT1 (pin 9) to trigger interrupt
    PCMSK0 |= (1 << PCINT2);        // set PCINT2 (pin 10) to trigger interrupt
    PCMSK0 |= (1 << PCINT3);        // set PCINT3 (pin 11) to trigger interrupt
    Serial.begin(115200);
}

void loop(){
    printSignal();
    delay(50);
}