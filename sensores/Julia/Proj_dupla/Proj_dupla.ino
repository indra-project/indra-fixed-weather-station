#include <Servo.h>
int servoPin = 9;
Servo myservo;

void setup() {
  // put your setup code here, to run once:
myservo.attach(servoPin);
}

void loop() {
  // put your main code here, to run repeatedly:
myservo.write(0);
delay(1000);
myservo.write(150);
delay(2000);
}
