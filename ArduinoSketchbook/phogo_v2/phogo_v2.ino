// This source file is part of the Phogo project
// https://github.com/CRM-UAM/Phogo
// Released under the GNU General Public License Version 3
// Club de Robotica-Mecatronica, Universidad Autonoma de Madrid, Spain

//--------------------------
// **** begin MISC *********
//--------------------------
// map() implementation modified for floating point operation
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//--------------------------
// **** begin LED **********
//--------------------------
#define LED_PIN LED_BUILTIN
#define ON 1
#define OFF 0

void init_led() {
    pinMode(LED_PIN, OUTPUT);
}

void led(bool state) {
    digitalWrite(LED_PIN, state);
}

//--------------------------
// **** begin SERVO PEN ****
//--------------------------
#include <Servo.h>

Servo pen_servo;
#define PEN_SERVO_PIN 2 // D4
#define UP 180 // degrees
#define DOWN 140 // degrees

void pen_move(int deg) {
    pen_servo.attach(PEN_SERVO_PIN);
    pen_servo.write(deg);
    delay(500);
    pen_servo.detach(); // power down the servo after the target position has been reached
}

//--------------------------
// **** begin ULTRASOUND ***
//--------------------------
// Based on code by Robi.Wang
#define ULTRASOUND_TRIGGER_PIN 14 // D6
#define ULTRASOUND_ECHO_PIN 12 // D5

void init_ultrasound() {
    pinMode(ULTRASOUND_TRIGGER_PIN, OUTPUT);
    pinMode(ULTRASOUND_ECHO_PIN, INPUT);
    digitalWrite(ULTRASOUND_TRIGGER_PIN, LOW);
}

float measure_distance_cm() {
    // TRIGGER
    digitalWrite(ULTRASOUND_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    // setup of the ECHO pin before finishing the TRIGGER  
    // Measures the duration of the input pulse (it is proportional to the time it takes for the sound to bounce back)
    long microseconds = pulseIn(ULTRASOUND_ECHO_PIN, HIGH, 50000); // 0.05s timeout
    digitalWrite(ULTRASOUND_TRIGGER_PIN, LOW);
    return microseconds / 29. / 2.;
}

int measure_distance_cm_filtered(int nsamples) {
    float res = 0;
    int n = nsamples;
    while (n > 0) {
        float dist = measure_distance_cm();
        if (dist > 0) {
            res += dist;
        } else {
          nsamples--;
        }
        n--;
        delay(10);
    }
    //Serial.println(nsamples);
    if (nsamples < 1) {
      return 0;
    }
    
    return res / nsamples;
}

//--------------------------
// **** begin STEPPER MOTORS
//--------------------------
#include <AccelStepper.h>

#define HALFSTEP 8

#define S1_1 16 // D0
#define S1_2 5 // D1
#define S1_3 4 // D2
#define S1_4 0 // D3

#define S2_1 1 // TX
#define S2_2 3 // Rx
#define S2_3 15 // D8
#define S2_4 13 // D7


AccelStepper stepper1(HALFSTEP, S1_1, S1_3, S1_2, S1_4);
AccelStepper stepper2(HALFSTEP, S2_1, S2_3, S2_2, S2_4);

void setup() {
	stepper1.setMaxSpeed(1000.0);
	stepper1.setAcceleration(100.0);
	stepper1.setSpeed(1000);
	stepper1.moveTo(20000);

	stepper2.setMaxSpeed(1000.0);
	stepper2.setAcceleration(100.0);
	stepper2.setSpeed(1000);
	stepper2.moveTo(20000);

}//--(end setup )---

void runMotors() {
	stepper1.run();
	stepper2.run();
}

void loop() {

  //Change direction when the stepper reaches the target position
	if (stepper1.distanceToGo() == 0) {
		stepper1.moveTo(-stepper1.currentPosition());
	}

	if (stepper2.distanceToGo() == 0) {
		stepper2.moveTo(-stepper2.currentPosition());
	}

	runMotors();
    /*pen_move(0);
    pen_move(10);
    pen_move(20);
    pen_move(30);
    pen_move(90);*/
}

