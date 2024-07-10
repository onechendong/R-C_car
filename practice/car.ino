#include<Wire.h>
#include<SoftwareSerial.h>

#define MOTOR_N1 4
#define MOTOR_N2 5
#define MOTOR_N3 6
#define MOTOR_N4 7
#define ENABLE_A 2
#define ENABLE_B 3
#define LED_A 8
#define LED_B  9
#define TIMER 2000

int speed = 130;
int control_spd = 1;      
char val = 0;
SoftwareSerial BTSerial(10, 11);

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
  if(BTSerial.available()){
    char c = BTSerial.read();
    if (0x30 <=c && c < 0x37) {
      val = c;
      switch (val) {
        case '1':
          Forward();
          Serial.println("Go Forward");
          BTSerial.println("Go Forward");
          delay(TIMER);
          Stop();
          break;
        case '3':
          Back();
          Serial.println("Go Back");
          BTSerial.println("Go Back");
          delay(TIMER);
          Stop();
          break;
        case '4':
          Left();
          Serial.println("Turn Left");
          BTSerial.println("Turn Left");
          delay(TIMER);
          Stop();
          break;
        case '2':
          Right();
          Serial.println("Turn Right");
          BTSerial.println("Turn Right");
          delay(TIMER);
          Stop();
          break;
        case '5':
          Change_Speed_And_Print();
          break;
      }
    }c = 0;
  }
  
}

void Forward()  //go forward
{
  analogWrite(ENABLE_A, speed+100);
  analogWrite(ENABLE_B, speed);
  digitalWrite(MOTOR_N1, HIGH);
  digitalWrite(MOTOR_N2, LOW);
  digitalWrite(MOTOR_N3, HIGH);
  digitalWrite(MOTOR_N4, LOW);

} 

void Back()     //go back
{
  analogWrite(ENABLE_A, speed);
  analogWrite(ENABLE_B, speed);
  digitalWrite(MOTOR_N1, LOW);
  digitalWrite(MOTOR_N2, HIGH);
  digitalWrite(MOTOR_N3, LOW);
  digitalWrite(MOTOR_N4, HIGH);
  digitalWrite(LED_B, HIGH);
  digitalWrite(LED_A, HIGH);
} 

void Stop()     //stop
{
  analogWrite(ENABLE_A, speed);
  analogWrite(ENABLE_B, speed);
  digitalWrite(MOTOR_N1, LOW);
  digitalWrite(MOTOR_N2, LOW);
  digitalWrite(MOTOR_N3, LOW);
  digitalWrite(MOTOR_N4, LOW);
  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);
}

void Left()     //turn left
{
  analogWrite(ENABLE_A, speed);
  analogWrite(ENABLE_B, speed);
  digitalWrite(MOTOR_N1, HIGH);
  digitalWrite(MOTOR_N2, LOW);
  digitalWrite(MOTOR_N3, LOW);
  digitalWrite(MOTOR_N4, LOW);
  digitalWrite(LED_B, HIGH);
} 

void Right()    //turn right
{
  analogWrite(ENABLE_A, speed);
  analogWrite(ENABLE_B, speed);
  digitalWrite(MOTOR_N1, LOW);
  digitalWrite(MOTOR_N2, LOW);
  digitalWrite(MOTOR_N3, HIGH);
  digitalWrite(MOTOR_N4, LOW);
  digitalWrite(LED_A, HIGH);
}

void Change_Speed_And_Print() //change speed
{
  //control_spd:odd num   low speed
  //control_spd:even num  high speed
  control_spd++;
  if(control_spd%2 == 1){
    speed = 130;
    Serial.println("Low speed mode");
    BTSerial.write("Low speed mode");
  }
  else{
    speed = 200;
    Serial.println("High speed mode");
    BTSerial.write("High speed mode");
  }
}
