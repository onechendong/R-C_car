#include<Wire.h>
#include<SoftwareSerial.h>

#define GREEN 13
#define BLUE  3
#define RED   4

char val = 0;
SoftwareSerial BTSerial(10, 11);

void setup() {
  Serial.begin(9600);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  BTSerial.begin(38400);
}

void loop() {
  
  if(BTSerial.available()>0){
    char c = BTSerial.read();
    if (0x30 <=c && c < 0x80) {
      val = c;
      switch (val) {
        case 'G':
        case 'g':
          Serial.println("Green_LED on");
          BTSerial.println("Green_LED on");
          break;
        case 'B':
        case 'b':
          Serial.println("Blue_LED on");
          BTSerial.println("Blue_LED on");
          break;
        case 'R':
        case 'r':
          Serial.println("Red_LED on");
          BTSerial.println("Red_LED on");
          break;
        case 'A':
        case 'a':
          Serial.println("All LED on");
          BTSerial.println("All LED on");
          break;
        case 'S':
        case 's':
          Serial.println("All LED off");
          BTSerial.println("All LED off");
          break;
      }
    }
  }
    switch (val) {
      case 'G':
      case 'g':
        digitalWrite(GREEN, HIGH);
        break;
      case 'B':
      case 'b':
        digitalWrite(BLUE, HIGH);
        break;
      case 'R':
      case 'r':
        digitalWrite(RED, HIGH);
        break;
      case 'A':
      case 'a':
        digitalWrite(GREEN, HIGH);
        digitalWrite(BLUE, HIGH);
        digitalWrite(RED, HIGH);
        break;
      case 'S':
      case 's':
        break;
    }
    delay(100);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, LOW);
    delay(1000);
}
