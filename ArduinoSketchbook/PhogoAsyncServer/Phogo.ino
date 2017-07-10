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

void init_led() {
    pinMode(LED_PIN, OUTPUT);
}

void led(bool state) {
    digitalWrite(LED_PIN, state);
}
//---( end led )---

#include <NewPing.h>
#define PING_PIN  D4  // Arduino pin tied to both trigger and echo pins on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define PING_TRIGGER PING_PIN  // 14
#define PING_ECHO PING_PIN // 12

NewPing sonar(PING_TRIGGER, PING_ECHO, MAX_DISTANCE); // NewPing setup of pin and maximum distance.
// NewPing sonar(PING_PIN, PING_PIN, MAX_DISTANCE); // NewPing setup of pin and maximum distance.

int measure_distance_cm_filtered(int nsamples) {
    float res = 0;
    int n = nsamples;
    while (n > 0) {
        int dist = sonar.ping_cm();
        DEBUGGING("[PHOGO]\tSample returned %d cm\n", dist);
        if (dist > 0) {
            res += dist;
        } else {
            nsamples--;
        }
        n--;
        busy_delay(30);
    }

    if (nsamples < 1) {
        return 0;
    }

    return res / nsamples;
}

#define ULTRASOUND_TRIGGER_PIN PING_TRIGGER
#define ULTRASOUND_ECHO_PIN PING_ECHO
#define ULTRASOUND_TIMEOUT_SECONDS 0.05 // 0.05s timeout
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
    long microseconds = pulseIn(ULTRASOUND_ECHO_PIN, HIGH, ULTRASOUND_TIMEOUT_SECONDS * 1000000);
    digitalWrite(ULTRASOUND_TRIGGER_PIN, LOW);
    return microseconds / 29. / 2.;
}
//---( end ultrasound )---

//---( begin stepper motors )---

int L_stepper_pins[] = {LSM_4, LSM_2, LSM_1, LSM_3};
int R_stepper_pins[] = {RSM_4, RSM_2, RSM_1, RSM_3};

int fwd_mask[][4] =  {{1, 0, 1, 0},
    {0, 1, 1, 0},
    {0, 1, 0, 1},
    {1, 0, 0, 1}
};

int rev_mask[][4] =  {{1, 0, 0, 1},
    {0, 1, 0, 1},
    {0, 1, 1, 0},
    {1, 0, 1, 0}
};

//---( end stepper motors )---

//---( begin phogo setup )---
bool _isPhogoSetUp = false;
void phogo_setup() {
    // phogo_pen_up();

    // pinMode(ULTRASOUND_TRIGGER_PIN, OUTPUT);
    // pinMode(ULTRASOUND_ECHO_PIN, INPUT);

    for (int pin = 0; pin < 4; pin++) {
        pinMode(L_stepper_pins[pin], OUTPUT);
        digitalWrite(L_stepper_pins[pin], LOW);
        pinMode(R_stepper_pins[pin], OUTPUT);
        digitalWrite(R_stepper_pins[pin], LOW);
    }

    DEBUGGING("[PHOGO]\tSetup complete\n");

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
