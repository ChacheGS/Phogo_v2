//--------------------------
// **** begin SERVO PEN ****
//--------------------------
#include <Servo.h>

Servo pen_servo;
#define PEN_SERVO_PIN 2
#define UP 0 // degrees
#define DOWN 180 // degrees

void pen_move(int deg) {
    pen_servo.attach(PEN_SERVO_PIN);
    pen_servo.write(deg);
    delay(1500);
    pen_servo.detach(); // power down the servo after the target position has been reached
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("0");
  pen_move(0);
  Serial.println("80");
  pen_move(80);
  Serial.println("180");
  pen_move(180);
}

