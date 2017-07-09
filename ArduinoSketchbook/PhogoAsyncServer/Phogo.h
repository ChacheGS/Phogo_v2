#ifndef PHOGO_H_
#define PHOGO_H_

#define ULTRASOUND_SAMPLES_PER_MEASURE 5
#define PI 3.1415926536


// Phogo constants
#define WHEEL_DIAMETER 75 //mm (increase = spiral out)
#define WHEEL_BASE 102 //mm (increase = spiral in) 
#define STEPS_PER_REV 512 // motor specs
#define DELAY_BETWEEN_STEPS 2 // ms

#define ON 0
#define OFF !ON

//---( begin servo pen constants )---

#define PEN_SERVO_PIN 10 // D4
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