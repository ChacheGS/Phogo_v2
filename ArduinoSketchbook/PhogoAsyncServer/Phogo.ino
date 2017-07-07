// This source file is part of the Phogo project
// https://github.com/CRM-UAM/Phogo
// Released under the GNU General Public License Version 3
// Club de Robotica-Mecatronica, Universidad Autonoma de Madrid, Spain

#include <Arduino.h>

#include "Phogo.h"

//---( begin misc )---
// map() implementation modified for floating point operation
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//---( end misc )---

//---( begin led )---
#define LED_PIN LED_BUILTIN
#define ON 0
#define OFF 1

void init_led() {
    pinMode(LED_PIN, OUTPUT);
}

void led(bool state) {
    digitalWrite(LED_PIN, state);
}
//---( end led )---

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
        delay(30);
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

#define RSM_1 16 // D0
#define RSM_2 5 // D1
#define RSM_3 4 // D2
#define RSM_4 0 // D3

#define LSM_1 1 // TX
#define LSM_2 3 // RX
#define LSM_3 15 // D8
#define LSM_4 13 // D7

#define MOTORS_MAX_SPEED	1000
#define MOTORS_SPEED		1000
#define MOTORS_ACCEL		300

AccelStepper right_motor(AccelStepper::HALF4WIRE, RSM_1, RSM_3, RSM_2, RSM_4, true);
AccelStepper left_motor(AccelStepper::HALF4WIRE, LSM_1, LSM_3, LSM_2, LSM_4, true);

MultiStepper motors;
//---( end stepper motors )---

//---( begin phogo setup )---
bool _isPhogoSetUp = false;

void phogo_setup() {
    pinMode(ULTRASOUND_TRIGGER_PIN, OUTPUT);
    pinMode(ULTRASOUND_ECHO_PIN, INPUT);

    left_motor.setMaxSpeed(MOTORS_MAX_SPEED);
    left_motor.setAcceleration(MOTORS_ACCEL);
    left_motor.setSpeed(MOTORS_SPEED);
    motors.addStepper(left_motor);

    right_motor.setMaxSpeed(MOTORS_MAX_SPEED);
    right_motor.setAcceleration(MOTORS_ACCEL);
    right_motor.setSpeed(MOTORS_SPEED);
    motors.addStepper(right_motor);

    _isPhogoSetUp = true;
}
//---( end phogo setup )---

//---( begin command interpreter )---
#include <ArduinoJson.h>
unsigned int phogo_controller(const char* request, char* response, size_t size) {

    /*
    {
    	"id": 0,
    	"cmd": {
    		"action": "do stuff"
    		"params": {
    			"param1": "value1",
    			"param2": "value2",
    			// ...
    		}
    	}
    								<- "result": "OK|ERROR"
    }
    */

    if (!_isPhogoSetUp) {
        phogo_setup();
    }

    const size_t bufferSize = 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 120;
    DynamicJsonBuffer jsonBuffer(bufferSize);

    JsonObject& root = jsonBuffer.parseObject(request);

    if (!root.success()) {
        strcpy(response, "{\"result\": \"ERROR: internal error while parsing the command.\"}");
        return 200;
    }

    int id = root["id"]; // command id

    const char* cmd_action = root["cmd"]["action"]; // action

    int cmd_params_int = 0; // integer arg
    const char* cmd_params_param = NULL; // string arg

    int cmd_res = 0;

    unsigned int status_code = 200;

    char str[size];
    char res[50];

    DEBUGGING("[PARS]\tAction requested: %s\n", cmd_action);
    if (0 == strcmp("forward", cmd_action)) {
        cmd_params_int = root["cmd"]["params"]["units"]; // units
        DEBUGGING("[PARS]\tWith parameter %d\n", cmd_params_int);
        cmd_res = phogo_move_forward(cmd_params_int);
        if (cmd_res	== 0) {
            strcpy(str, "OK");
        } else {
            sprintf(str, "ERROR: moving forward %d units", cmd_params_int);
        }
        root["result"] = str;
    } else if (0 == strcmp("backward", cmd_action)) {
        cmd_params_int = root["cmd"]["params"]["units"]; // units
        DEBUGGING("[PARS]\tWith parameter %d\n", cmd_params_int);
        cmd_res = phogo_move_backward(cmd_params_int);
        if (cmd_res	== 0) {
            strcpy(str, "OK");
        } else {
            sprintf(str, "ERROR: moving backward %d units", cmd_params_int);
        }
        root["result"] = str;
    } else if (0 == strcmp("pen_up", cmd_action)) {
        cmd_res = phogo_pen_up();
        if (cmd_res	== 0) {
            strcpy(str, "OK");
        } else {
            sprintf(str, "ERROR: pen up");
        }
        root["result"] = str;
    } else if (0 == strcmp("pen_down", cmd_action)) {
        cmd_res = phogo_pen_down();
        if (cmd_res	== 0) {
            strcpy(str, "OK");
        } else {
            sprintf(str, "ERROR: pen down");
        }
        root["result"] = str;
    } else if (0 == strcmp("distance", cmd_action)) {
        // no arg
#ifndef ULTRASOUND_SAMPLES_PER_MEASURE
#define ULTRASOUND_SAMPLES_PER_MEASURE 3
#endif
        int distance = measure_distance_cm_filtered(ULTRASOUND_SAMPLES_PER_MEASURE);
        sprintf(str, "%d", distance);
        root["result"] = distance;
    } else if (0 == strcmp("left", cmd_action)) {
        cmd_params_int = root["cmd"]["params"]["degrees"]; // degrees
        DEBUGGING("[PARS]\tWith parameter %d\n", cmd_params_int);
        cmd_res = phogo_turn_left(cmd_params_int);
        if (cmd_res	== 0) {
            strcpy(str, "OK");
        } else {
            sprintf(str, "ERROR: turning left %d degrees", cmd_params_int);
        }
        root["result"] = str;
    } else if (0 == strcmp("right", cmd_action)) {
        cmd_params_int = root["cmd"]["params"]["degrees"]; // degrees
        DEBUGGING("[PARS]\tWith parameter %d\n", cmd_params_int);
        cmd_res = phogo_turn_right(cmd_params_int);
        if (cmd_res	== 0) {
            strcpy(str, "OK");
        } else {
            sprintf(str, "ERROR: turning right %d degrees", cmd_params_int);
        }
        root["result"] = str;
    } else {
        //body
        DEBUGGINGC("??\n");
        root["result"] = "ERROR: unrecognized command";
        status_code = 200; //should be something else?
    }

    //run the motors, blocking
    // motors.runSpeedToPosition();

    root.printTo(response, size);

    return status_code;
}
//---( end command interpreter )---
