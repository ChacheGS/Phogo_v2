#include <Servo.h>
Servo pen_servo;

void pen_move(int deg) {
    // pen_servo.attach(PEN_SERVO_PIN);
    // pen_servo.write(deg);
    delay(250);
    // power down the servo after the target position has been reached
    // pen_servo.detach();
}

int phogo_pen_down() {
    pen_move(SERVO_DOWN);
    DEBUGGING("[PHOGO]\tPen down\n");
    return 0;
}

int phogo_pen_up() {
    pen_move(SERVO_UP);
    DEBUGGING("[PHOGO]\tPen up\n");
    return 0;
}

int step(float distance) {
    int steps = distance * STEPS_PER_REV / (WHEEL_DIAMETER * PI);
    return steps;
}

int phogo_move_forward(float distance) {
    int steps = step(distance);
    /*for (int step = 0; step < steps; step++) {
        for (int mask = 0; mask < 4; mask++) {
            for (int pin = 0; pin < 4; pin++) {
                digitalWrite(L_stepper_pins[pin], rev_mask[mask][pin]);
                digitalWrite(R_stepper_pins[pin], fwd_mask[mask][pin]);
            }
            delay(DELAY_BETWEEN_STEPS);
        }
    }*/
    DEBUGGING("[PHOGO]\tMoving forward ");
    DEBUGGINGL(distance);
    DEBUGGINGC(" (%d steps)\n", steps);

    return 0;
}

int phogo_move_backward(float distance) {
    int steps = step(distance);
    /*for (int step = 0; step < steps; step++) {
        for (int mask = 0; mask < 4; mask++) {
            for (int pin = 0; pin < 4; pin++) {
                digitalWrite(L_stepper_pins[pin], fwd_mask[mask][pin]);
                digitalWrite(R_stepper_pins[pin], rev_mask[mask][pin]);
            }
            delay(DELAY_BETWEEN_STEPS);
        }
    }*/
    DEBUGGING("[PHOGO]\tMoving backward ");
    DEBUGGINGL(distance);
    DEBUGGINGC(" (%d steps)\n", steps);

    return 0;
}

int phogo_turn_left(float degrees) {
    float rotation = degrees / 360.0;
    float distance = WHEEL_BASE * PI * rotation;
    int steps = step(distance);
    /*for (int step = 0; step < steps; step++) {
        for (int mask = 0; mask < 4; mask++) {
            for (int pin = 0; pin < 4; pin++) {
                digitalWrite(R_stepper_pins[pin], fwd_mask[mask][pin]);
                digitalWrite(L_stepper_pins[pin], fwd_mask[mask][pin]);
            }
            delay(DELAY_BETWEEN_STEPS);
        }
    }*/
    DEBUGGING("[PHOGO]\tTurning left ");
    DEBUGGINGL(degrees);
    DEBUGGINGC(" (%d steps) -> ", steps);
    DEBUGGINGL(distance);

    return 0;
}

int phogo_turn_right(float degrees) {
    float rotation = degrees / 360.0;
    float distance = WHEEL_BASE * PI * rotation;
    int steps = step(distance);
    /*for (int step = 0; step < steps; step++) {
        for (int mask = 0; mask < 4; mask++) {
            for (int pin = 0; pin < 4; pin++) {
                digitalWrite(R_stepper_pins[pin], rev_mask[mask][pin]);
                digitalWrite(L_stepper_pins[pin], rev_mask[mask][pin]);
            }
            delay(DELAY_BETWEEN_STEPS);
        }
    }*/
    DEBUGGING("[PHOGO]\tTurning right ");
    DEBUGGINGL(degrees);
    DEBUGGINGC(" (%d steps) -> ", steps);
    DEBUGGINGL(distance);

    return 0;
}

void done() { // unlock stepper to save battery
   /* for (int mask = 0; mask < 4; mask++) {
        for (int pin = 0; pin < 4; pin++) {
            digitalWrite(R_stepper_pins[pin], LOW);
            digitalWrite(L_stepper_pins[pin], LOW);
        }
        delay(DELAY_BETWEEN_STEPS);
    }*/
}
