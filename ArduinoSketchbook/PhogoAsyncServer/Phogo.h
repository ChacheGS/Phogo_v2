#ifndef PHOGO_H_
#define PHOGO_H_

#define ULTRASOUND_SAMPLES_PER_MEASURE 5
#define PI 3.1415926536


// Phogo constants
#define WHEEL_DIAMETER 7.5 // cm (increase = spiral out)
#define WHEEL_BASE 10.5 // cm (increase = spiral in) 
#define STEPS_PER_REV 512 // motor specs
#define DELAY_BETWEEN_STEPS 2 // ms

#define ON 0
#define OFF !ON

//---( begin servo pen constants )---

#define PEN_SERVO_PIN 10 // D4
#define SERVO_UP 180 // degrees
#define SERVO_DOWN 60 // degrees

//---( end servo pen constants )---

//---( begin stepper motor constants )---

#define RSM_1 D0 // D0
#define RSM_2 D1 // D1
#define RSM_3 D2 // D2
#define RSM_4 D3 // D3

#define LSM_1 D5 // D5
#define LSM_2 D6 // D6
#define LSM_3 D7 // D7
#define LSM_4 D8 // D8

//---( end stepper motor constants )---


#endif /* PHOGO_H_ */