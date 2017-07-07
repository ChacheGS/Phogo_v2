#ifndef PHOGO_H_
#define PHOGO_H_

#include <Servo.h>

#define ULTRASOUND_SAMPLES_PER_MEASURE 5
#define PI 3.1415926536


// Phogo constants
#define WHEEL_DIAMETER 7.5 //mm (increase = spiral out)
#define WHEEL_BASE 10.2 //mm (increase = spiral in) 
#define STEPS_PER_REV 4076 // motor specs


//---( begin servo pen constants )---

#define PEN_SERVO_PIN 2 // D4
#define SERVO_UP 180 // degrees
#define SERVO_DOWN 140 // degrees

/*void pen_move(int deg) {
    pen_servo.attach(PEN_SERVO_PIN);
    pen_servo.write(deg);
    delay(250);
    pen_servo.detach(); // power down the servo after the target position has been reached
}*/
//---( end servo pen constants )---

#endif /* PHOGO_H_ */