#include<Wire.h>
#include<SoftwareSerial.h>

#define MOTOR_N1 10
#define MOTOR_N2 11
#define MOTOR_N3 5
#define MOTOR_N4 6
#define ENABLE_A 2
#define ENABLE_B 3
#define TIMER 500

int speed = 180;
int control_spd = 1;
char val = 0;
SoftwareSerial BTSerial(8, 9);

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_N1,OUTPUT);
  pinMode(MOTOR_N2,OUTPUT);
  pinMode(MOTOR_N3,OUTPUT);
  pinMode(MOTOR_N4,OUTPUT);
  pinMode(ENABLE_A, OUTPUT);
  pinMode(ENABLE_B, OUTPUT);
  BTSerial.begin(9600);
  delay(TIMER);
}

void loop() {
  if (BTSerial.available() || Serial.available() ) {
    char c = BTSerial.read();
    if (c == 'S') { // Stop command
      Stop();
    } else {
      val = c;
      switch (val) {
        case '1':
          Forward();
          //delay(5000);
          break;
        case '3':
          Back();
          break;
        case '4':
          Left();
          break;
        case '2':
          Right();
          break;
        case '5':
          Change_Speed_And_Print();
          break;
        case '6':
          Stop();
          break;
        case 'R':
         Resetmode();
         break;

      }
    }
  }
}
//A=Left
//B=Right
void Forward() {
  analogWrite(ENABLE_A, speed-15);
  analogWrite(ENABLE_B, speed);
  digitalWrite(MOTOR_N1, HIGH);
  digitalWrite(MOTOR_N2, LOW);
  digitalWrite(MOTOR_N3, HIGH);
  digitalWrite(MOTOR_N4, LOW);
}

void Back() {
  analogWrite(ENABLE_A, speed-15);
  analogWrite(ENABLE_B, speed);
  digitalWrite(MOTOR_N1, LOW);
  digitalWrite(MOTOR_N2, HIGH);
  digitalWrite(MOTOR_N3, LOW);
  digitalWrite(MOTOR_N4, HIGH);
}

void Stop() {
  analogWrite(ENABLE_A, speed);
  analogWrite(ENABLE_B, speed);
  digitalWrite(MOTOR_N1, LOW);
  digitalWrite(MOTOR_N2, LOW);
  digitalWrite(MOTOR_N3, LOW);
  digitalWrite(MOTOR_N4, LOW);
}

void Left() {
  analogWrite(ENABLE_A, speed);
  analogWrite(ENABLE_B, speed);
  digitalWrite(MOTOR_N1, HIGH);
  digitalWrite(MOTOR_N2, LOW);
  digitalWrite(MOTOR_N3, LOW);
  digitalWrite(MOTOR_N4, HIGH);
}

void Right() {
  analogWrite(ENABLE_A, speed-20);
  analogWrite(ENABLE_B, speed);
  digitalWrite(MOTOR_N1, LOW);
  digitalWrite(MOTOR_N2, HIGH);
  digitalWrite(MOTOR_N3, HIGH);
  digitalWrite(MOTOR_N4, LOW);
}



void Change_Speed_And_Print() {
  control_spd++;
  if((control_spd % 2) == 1){
    speed =180;
    Serial.println("Low speed mode");
    BTSerial.write("Low speed mode");
  }
  else{
    speed = 250;
    Serial.println("High speed mode");
    BTSerial.write("High speed mode");
  }

  
}
void Resetmode(){
  control_spd = 1;
  speed = 150;
}
