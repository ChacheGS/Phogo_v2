/* the actual phogo */
// This source file is part of the Phogo project
// https://github.com/CRM-UAM/Phogo
// Released under the GNU General Public License Version 3
// Club de Robotica-Mecatronica, Universidad Autonoma de Madrid, Spain

#include "Phogo.h"

//---( begin misc )---
// map() implementation modified for floating point operation
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//---( end misc )---

//---( begin led )---
#define LED_PIN LED_BUILTIN
#define ON 1
#define OFF 0

void init_led() {
	pinMode(LED_PIN, OUTPUT);
}

void led(bool state) {
	digitalWrite(LED_PIN, state);
}
//---( end led )---

//---( begin servo pen )---
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
//---( end servo pen )---

//---( begin ultrasound )---
#include <NewPing.h>
#define PING_PIN  12  // Arduino pin tied to both trigger and echo pins on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define PING_TRIGGER 14
#define PING_ECHO 12

NewPing sonar(PING_TRIGGER, PING_ECHO, MAX_DISTANCE); // NewPing setup of pin and maximum distance.
// NewPing sonar(PING_PIN, PING_PIN, MAX_DISTANCE); // NewPing setup of pin and maximum distance.

int measure_distance_cm_filtered(int nsamples) {
    float res = 0;
    int n = nsamples;
    while (n > 0) {
        int dist = sonar.ping_cm();
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

#define ULTRASOUND_TRIGGER_PIN PING_TRIGGER
#define ULTRASOUND_ECHO_PIN PING_ECHO
float measure_distance_cm() {
    // Set to LOW to make a clean pulse
    digitalWrite(ULTRASOUND_TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    // TRIGGER
	digitalWrite(ULTRASOUND_TRIGGER_PIN, HIGH);
    // pulse of *at least* 10us
	delayMicroseconds(10);
    // setup of the ECHO pin before finishing the TRIGGER
    // Measures the duration of the input pulse (it is proportional to the time it takes for the sound to bounce back)
    long microseconds = pulseIn(ULTRASOUND_ECHO_PIN, HIGH, 50000); // 0.05s timeout
    digitalWrite(ULTRASOUND_TRIGGER_PIN, LOW);
    return microseconds / 29. / 2.;
}
//---( end ultrasound )---

//---( begin stepper motors )---
#include <AccelStepper.h>
#include <MultiStepper.h>

#define HALFSTEP 8

#define S1_1 16 // D0
#define S1_2 5 // D1
#define S1_3 4 // D2
#define S1_4 0 // D3

#define S2_1 1 // TX
#define S2_2 3 // Rx
#define S2_3 15 // D8
#define S2_4 13 // D7

#define MOTORS_MAX_SPEED	4000.0
#define MOTORS_SPEED		4000.0
#define MOTORS_ACCEL		200.0

AccelStepper right_motor(HALFSTEP, S1_1, S1_3, S1_2, S1_4);
AccelStepper left_motor(HALFSTEP, S2_1, S2_3, S2_2, S2_4);

MultiStepper motors;
//---( end stepper motors )---

//---( begin phogo setup )---
bool _isPhogoSetUp = false;

void phogo_setup() {
	right_motor.setMaxSpeed(MOTORS_MAX_SPEED);
	right_motor.setAcceleration(MOTORS_ACCEL);
	right_motor.setSpeed(MOTORS_SPEED);

	left_motor.setMaxSpeed(MOTORS_MAX_SPEED);
	left_motor.setAcceleration(MOTORS_ACCEL);
	left_motor.setSpeed(MOTORS_SPEED);

	motors.addStepper(right_motor);
	motors.addStepper(left_motor);

	_isPhogoSetUp = true;
}
//---( end phogo setup )---

void runMotors() {
	right_motor.run();
	left_motor.run();
}

//---( begin command interpreter )---
#include <ArduinoJson.h>
unsigned int phogo_controller(String request, char* response, size_t size) {

	if (!_isPhogoSetUp) {
		phogo_setup();
	}

	const char* json;
	unsigned int status_code = 200;
	int i = 0;
	int arg = 0;

	char str[200];
	char res[200];

	StaticJsonBuffer<200> buff;
	JsonObject& req = buff.parseObject(request);

	int cmd_id = req["cmd_id"];

	const char* cmd = req["cmd"]["action"];
	arg = req["cmd"]["arg"];

	DEBUGGING("%d|%s|", cmd_id, cmd);

	JsonObject& obj = buff.createObject();
	obj["cmd_id"] = cmd_id;
	if (0 == strcmp("move", cmd)) {
        //body
		DEBUGGINGC("%d\n", arg);
		strcpy(str, "OK");
		obj["result"] = str;
	} else if (0 == strcmp("turn", cmd)) {
        //body
		DEBUGGINGC("%d\n", arg);
		strcpy(str, "OK");
		obj["result"] = str;
	} else if (0 == strcmp("pen", cmd)) {
        //body
		DEBUGGINGC("%d\n", arg);
		strcpy(str, "OK");
		obj["result"] = str;
	} else if (0 == strcmp("distance", cmd)) {
        // no arg
#ifndef ULTRASOUND_SAMPLES_PER_MEASURE
#define ULTRASOUND_SAMPLES_PER_MEASURE 3
#endif
		int distance = measure_distance_cm_filtered(ULTRASOUND_SAMPLES_PER_MEASURE);
		DEBUGGINGC("%d\n", distance);
		// strcpy(str, "OK");
		// sprintf(str, "%d", distance);
		obj["result"] = distance;
	} else if (0 == strcmp("speak", cmd)) {
        //body
		DEBUGGINGC("%d\n", arg);
		strcpy(str, "OK");
		obj["result"] = str;
	} else {
        //body
		DEBUGGINGC("??\n");
		obj["result"] = "unrecognized command";
        status_code = 200; //should be something else?
    }

    //run the motors, blocking
    motors.runSpeedToPosition();

#ifdef DEBUG
    obj.prettyPrintTo(response, size);
#else
    obj.printTo(response, size);
#endif

    return status_code;
}
//---( end command interpreter )---
