#include <Arduino.h>

#define L_A     13
#define L_B     12
#define L_PWM   11
#define R_PWM   10
#define R_B     9
#define R_A     8
#define IR_0    A0
#define IR_1    A1
#define IR_2    A2
#define IR_3    A3
#define IR_4    A4

#define MIN_PWM   5
#define MAX_PWM   100
#define IR_NUMBER 5
#define WHITE_IR  true
#define BLACK_IR  !WHITE_IR

int command[2];

void setPin(){
  pinMode(L_A, OUTPUT);
  pinMode(L_B, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(R_A, OUTPUT);
  pinMode(R_B, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(IR_0, INPUT);
  pinMode(IR_1, INPUT);
  pinMode(IR_2, INPUT);
  pinMode(IR_3, INPUT);
  pinMode(IR_4, INPUT);
}

void motorL(float pwm){
  if(pwm >= 100){
    digitalWrite(L_A, HIGH);
    digitalWrite(L_B, LOW);
    digitalWrite(L_PWM, HIGH);
  }
  else if(pwm < MAX_PWM && pwm > MIN_PWM){
    digitalWrite(L_A, HIGH);
    digitalWrite(L_B, LOW);
    analogWrite(L_PWM, map(abs(pwm), 0, 100, 0, 255));
  }
  else if(pwm > -MAX_PWM && pwm < -MIN_PWM){
    digitalWrite(L_A, LOW);
    digitalWrite(L_B, HIGH);
    analogWrite(L_PWM, map(abs(pwm), 0, 100, 0, 255));
  }
  else if(pwm < -MAX_PWM){
    digitalWrite(L_A, LOW);
    digitalWrite(L_B, HIGH);
    digitalWrite(L_PWM, HIGH);
  }
  else{
    digitalWrite(L_A, HIGH);
    digitalWrite(L_B, HIGH);
    digitalWrite(L_PWM, LOW);
  }
}

void motorR(float pwm){
  if(pwm >= MAX_PWM){
    digitalWrite(R_A, HIGH);
    digitalWrite(R_B, LOW);
    digitalWrite(R_PWM, HIGH);
  }
  else if(pwm < MAX_PWM && pwm > MIN_PWM){
    digitalWrite(R_A, HIGH);
    digitalWrite(R_B, LOW);
    analogWrite(R_PWM, map(abs(pwm), 0, 100, 0, 255));
  }
  else if(pwm > -MAX_PWM && pwm < -MIN_PWM){
    digitalWrite(R_A, LOW);
    digitalWrite(R_B, HIGH);
    analogWrite(R_PWM, map(abs(pwm), 0, 100, 0, 255));
  }
  else if(pwm < -MAX_PWM){
    digitalWrite(R_A, LOW);
    digitalWrite(R_B, HIGH);
    digitalWrite(R_PWM, HIGH);
  }
  else{
    digitalWrite(R_A, HIGH);
    digitalWrite(R_B, HIGH);
    digitalWrite(R_PWM, LOW);
  }
}

void readIR(){
  Serial.print("IR_0 : ");
  Serial.println(digitalRead(IR_0));
  Serial.print("IR_1 : ");
  Serial.println(digitalRead(IR_1));
  Serial.print("IR_2 : ");
  Serial.println(digitalRead(IR_2));
  Serial.print("IR_3 : ");
  Serial.println(digitalRead(IR_3));
  Serial.print("IR_4 : ");
  Serial.println(digitalRead(IR_4));
  Serial.println();
}

int valueIR(bool inverse = false){
  int *IR = new int[5];
  if(!inverse){
    IR[0] = digitalRead(IR_0);
    IR[1] = digitalRead(IR_1);
    IR[2] = digitalRead(IR_2); 
    IR[3] = digitalRead(IR_3);
    IR[4] = digitalRead(IR_4);
  }
  else{
    IR[0] = digitalRead(IR_4);
    IR[1] = digitalRead(IR_3);
    IR[2] = digitalRead(IR_2);
    IR[3] = digitalRead(IR_1);
    IR[4] = digitalRead(IR_0);
  }
  Serial.println(digitalRead(IR_0));
  int state = 16*IR[0] + 8*IR[1] + 4*IR[2] + 2*IR[3] + IR[4];
  delete[] IR;
  return state;
}

bool inCase(int input_case, int set_case[], int number_case){
  for(int i=0; i < number_case; i++){
    if(input_case == set_case[i]){
      return true;
    }
  }
  return false;
}

void lineTracking(){
  int state = valueIR();
  if(inCase(state, 0b00100, 1)){       // 00100

  }
  else if(state == 8 || state == 12){   // 01000 01100

  }
  else if(state == 2 || state == 6){    // 00010 00110

  }
  else if(state == 16 || state == 24 || state == 30){   // 10000 11000 11100

  }
  else if(state == 1 || state == 3 || state == 7){   // 00001 00011 00111

  }
  else if(state == 1){   // 10001 11001 11101 10010 11010 11110 10011 10111 01001 01011 01111 

  }
}


int readUART(){
  if(Serial.available() > 0){
    String data = Serial.readString();
    command[0] = data[0];
    command[1] = data[1];
  }
}

void cmd(int state){
  if(state == 0){

  }
  else if(state == 1){

  }
  else if(state == 2){
    
  }
}

void setup() {
  setPin();
  Serial.begin(115200);
}

void loop() {
  int a[] = {0b10001};
  bool asd = inCase(17, int({0b10001}), 1);
  Serial.println(asd);
  delay(1000);
}