#include <Arduino.h>

#define L_A     7
#define L_B     6
#define L_PWM   5
#define R_PWM   3
#define R_B     4
#define R_A     2 
#define IR_0    A0
#define IR_1    A1
#define IR_2    A2
#define IR_3    A3
#define IR_4    A4

#define MIN_PWM   0
#define MAX_PWM   100
#define MOTOR_L_INVERSE             false
#define MOTOR_R_INVERSE             false
#define REMOTE_CHANNEL_1_INVERSE    false
#define REMOTE_CHANNEL_2_INVERSE    false
#define REMOTE_CHANNEL_3_INVERSE    false
#define REMOTE_CHANNEL_4_INVERSE    false
#define REMOTE_L_X_MIN      1000
#define REMOTE_L_Y_MIN      1000
#define REMOTE_R_X_MIN      1000
#define REMOTE_R_Y_MIN      1000
#define REMOTE_L_X_MEAN     1500
#define REMOTE_L_Y_MEAN     1500
#define REMOTE_R_X_MEAN     1500
#define REMOTE_R_Y_MEAN     1500
#define REMOTE_L_X_MAX      2000
#define REMOTE_L_Y_MAX      2000
#define REMOTE_R_X_MAX      2000
#define REMOTE_R_Y_MAX      2000
#define REMOTE_L_X_THRESHOLD  100
#define REMOTE_L_Y_THRESHOLD  100
#define REMOTE_R_X_THRESHOLD  100
#define REMOTE_R_Y_THRESHOLD  100
#define IR_NUMBER 5
#define IR_WHITE_VALUE      true
#define IR_REVERSE          true

int LINE_CASE_0[] = {0b00100};
int LINE_CASE_1[] = {0b01000, 0b01100};
int LINE_CASE_2[] = {0b00010, 0b00110};
int LINE_CASE_3[] = {0b10000, 0b11000, 0b11100};
int LINE_CASE_4[] = {0b00001, 0b00011, 0b00111};
int LINE_CASE_5[] = {0b10001, 0b11001, 0b11101, 0b10010, 
                    0b11010, 0b11110, 0b10011, 0b10111, 
                    0b01001, 0b01011, 0b01111, 0b11111}; 

bool MANUAL_MODE = true;
bool last_channel_1, last_channel_2, last_channel_3, last_channel_4;
int REMOTE_L_X_VALUE; 
int REMOTE_L_Y_VALUE;
int REMOTE_R_X_VALUE;
int REMOTE_R_Y_VALUE;
unsigned long timer_1, timer_2, timer_3, timer_4;
int command[2];

void setPin(){
  PCICR |= (1 << PCIE0);          // set PCIE0 to enable PCMSK0 scan
  PCMSK0 |= (1 << PCINT0);        // set PCINT0 (pin 8) to trigger interrupt
  PCMSK0 |= (1 << PCINT1);        // set PCINT1 (pin 9) to trigger interrupt
  PCMSK0 |= (1 << PCINT2);        // set PCINT2 (pin 10) to trigger interrupt
  PCMSK0 |= (1 << PCINT3);        // set PCINT3 (pin 11) to trigger interrupt
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

void motorL(int pwm){
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

void motorR(int pwm){
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

void driveMotor(int PWM_L, int PWM_R){
  if(MOTOR_L_INVERSE){
    PWM_L = -PWM_L;
  }
  if(MOTOR_R_INVERSE){
    PWM_R = -PWM_R;
  }
  motorL(PWM_L);
  motorR(PWM_R);
}

void testMotor(){
  driveMotor(MAX_PWM, MIN_PWM);
  delay(500);
  driveMotor(-MAX_PWM, MIN_PWM);
  delay(500);
  driveMotor(MIN_PWM, MAX_PWM);
  delay(500);
  driveMotor(MIN_PWM, -MAX_PWM);
  delay(500);
}

ISR(PCINT0_vect){
  if(last_channel_1 == false && (PINB & B00000001)){
    last_channel_1 = !last_channel_1;
    timer_1 = micros();
  }
  else if(last_channel_1 == true && !(PINB & B00000001)){
    last_channel_1 = !last_channel_1;
    REMOTE_R_X_VALUE = micros() - timer_1;
  }  
  if(last_channel_2 == false && (PINB & B00000010)){
    last_channel_2 = !last_channel_2;
    timer_2 = micros();
  }
  else if(last_channel_2 == true && !(PINB & B00000010)){
    last_channel_2 = !last_channel_2;
    REMOTE_R_Y_VALUE = micros() - timer_2;
  }
  if(last_channel_3 == false && (PINB & B00000100)){
    last_channel_3 = !last_channel_3;
    timer_3 = micros();
  }
  else if(last_channel_3 == true && !(PINB & B00000100)){
    last_channel_3 = !last_channel_3;
    REMOTE_L_Y_VALUE = micros() - timer_3;
  }
  if(last_channel_4 == false && (PINB & B00001000)){
    last_channel_4 = !last_channel_4;
    timer_4 = micros();
  }
  else if(last_channel_4 == true && !(PINB & B00001000)){
    last_channel_4 = !last_channel_4;
    REMOTE_L_X_VALUE = micros() - timer_4;
  }
}

void printSignal(){
  Serial.print("L_X : ");
  Serial.println(REMOTE_L_X_VALUE);
  Serial.print("L_Y : ");
  Serial.println(REMOTE_L_Y_VALUE);
  Serial.print("R_X : ");
  Serial.println(REMOTE_R_X_VALUE);
  Serial.print("R_Y : ");
  Serial.println(REMOTE_R_Y_VALUE);
  Serial.println();
  delay(200);
}

void manualControl(){
  if(abs(REMOTE_R_X_VALUE - REMOTE_R_X_MEAN) < REMOTE_R_X_THRESHOLD && abs(REMOTE_R_Y_VALUE - REMOTE_R_Y_MEAN) < REMOTE_R_Y_THRESHOLD){
    Serial.println("Manual Stop");
    driveMotor(MIN_PWM, MIN_PWM);
  }
  else if(abs(REMOTE_R_X_VALUE - REMOTE_R_X_MEAN) < REMOTE_R_X_THRESHOLD && REMOTE_R_Y_VALUE - REMOTE_R_Y_MEAN > REMOTE_R_Y_THRESHOLD){
    Serial.println("Manual Forward"); 
    int Y_VALUE = map(REMOTE_R_Y_VALUE, REMOTE_R_Y_MEAN + REMOTE_R_Y_THRESHOLD, REMOTE_R_Y_MAX, MIN_PWM, MAX_PWM);
    driveMotor(Y_VALUE, Y_VALUE);
  }
  else if(REMOTE_R_X_VALUE - REMOTE_R_X_MEAN > REMOTE_R_X_THRESHOLD && REMOTE_R_Y_VALUE - REMOTE_R_Y_MEAN > REMOTE_R_Y_THRESHOLD){
    Serial.println("Manual Forward-Right");
  }
  else if(REMOTE_R_X_VALUE - REMOTE_R_X_MEAN > REMOTE_R_X_THRESHOLD && abs(REMOTE_R_Y_VALUE - REMOTE_R_Y_MEAN) < REMOTE_R_Y_THRESHOLD){
    Serial.println("Manual Rotate CW");
    int X_VALUE = map(REMOTE_R_X_VALUE, REMOTE_R_X_MEAN + REMOTE_R_X_THRESHOLD, REMOTE_R_X_MAX, MIN_PWM, MAX_PWM);
    driveMotor(X_VALUE, -X_VALUE);
  }
  else if(REMOTE_R_X_VALUE - REMOTE_R_X_MEAN > REMOTE_R_X_THRESHOLD && REMOTE_R_Y_VALUE - REMOTE_R_Y_MEAN < -REMOTE_R_Y_THRESHOLD){
    Serial.println("Manual Backward-Right"); 
  }
  else if(abs(REMOTE_R_X_VALUE - REMOTE_R_X_MEAN) < REMOTE_R_X_THRESHOLD && REMOTE_R_Y_VALUE - REMOTE_R_Y_MEAN < -REMOTE_R_Y_THRESHOLD){
    Serial.println("Manual Backward");
    int Y_VALUE = map(REMOTE_R_Y_VALUE, REMOTE_R_Y_MIN, REMOTE_R_Y_MEAN - REMOTE_R_Y_THRESHOLD, -MAX_PWM, MIN_PWM);
    driveMotor(Y_VALUE, Y_VALUE);
  }
  else if(REMOTE_R_X_VALUE - REMOTE_R_X_MEAN < -REMOTE_R_X_THRESHOLD && REMOTE_R_Y_VALUE - REMOTE_R_Y_MEAN < -REMOTE_R_Y_THRESHOLD){
    Serial.println("Manual Backward-Left"); 
  }
  else if(REMOTE_R_X_VALUE - REMOTE_R_X_MEAN < -REMOTE_R_X_THRESHOLD && abs(REMOTE_R_Y_VALUE - REMOTE_R_Y_MEAN) < REMOTE_R_Y_THRESHOLD){
    Serial.println("Manual Rotate CCW");
    int X_VALUE = map(REMOTE_R_X_VALUE, REMOTE_R_X_MIN, REMOTE_R_X_MEAN + REMOTE_R_X_THRESHOLD, -MAX_PWM, MIN_PWM);
    driveMotor(X_VALUE, -X_VALUE);
  }
  else if(REMOTE_R_X_VALUE - REMOTE_R_X_MEAN < -REMOTE_R_X_THRESHOLD && REMOTE_R_Y_VALUE - REMOTE_R_Y_MEAN > REMOTE_R_Y_THRESHOLD){
    Serial.println("Manual Forward-Left"); 
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

int valueIR(bool inverse = IR_REVERSE){
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
  if(IR_WHITE_VALUE){
    IR[0] = !IR[0];
    IR[1] = !IR[1];
    IR[2] = !IR[2];
    IR[3] = !IR[3];
    IR[4] = !IR[4];
  }
  int state = 16*IR[0] + 8*IR[1] + 4*IR[2] + 2*IR[3] + IR[4];
  delete[] IR;
  return state;
}

bool inCase(int input_case, int set_case[], int number_case){
  for(int i=0; i < number_case; i++){
    if(input_case == set_case[i]){
      return true;
    }
    // Serial.println(set_case[i]);
  }
  return false;
}

void lineTracking(){
  int state = valueIR();
  if(inCase(state, LINE_CASE_0, 1)){       // 00100
    Serial.println("Line Case 0");
  }
  else if(inCase(state, LINE_CASE_1, 2)){   // 01000 01100
    Serial.println("Line Case 1");
  }
  else if(inCase(state, LINE_CASE_2, 2)){    // 00010 00110
    Serial.println("Line Case 2");
  }
  else if(inCase(state, LINE_CASE_3, 3)){   // 10000 11000 11100
    Serial.println("Line Case 3");
  }
  else if(inCase(state, LINE_CASE_4, 3)){   // 00001 00011 00111
    Serial.println("Line Case 4");
  }
  else if(inCase(state, LINE_CASE_5, 12)){   // 10001 11001 11101 10010 11010 11110 10011 10111 01001 01011 01111 11111
    Serial.println("Line Case 5");
  }
}


int readUART(){
  if(Serial.available() > 0){
    String data = Serial.readString();
    command[0] = data[0];
    command[1] = data[1];
    Serial.print("0: ");
    Serial.print(command[0]);
    Serial.print(" ,1: ");
    Serial.println(command[1]);
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
  readUART();
  delay(1000);

  // lineTracking();
  // delay(300);

  // if(MANUAL_MODE){
  //   manualControl();
  // }
}