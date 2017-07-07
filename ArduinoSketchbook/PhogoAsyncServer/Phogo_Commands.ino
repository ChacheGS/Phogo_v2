#include <Servo.h>
Servo pen_servo;

void pen_move(int deg) {
    pen_servo.attach(PEN_SERVO_PIN);
    pen_servo.write(deg);
    delay(250);
    pen_servo.detach(); // power down the servo after the target position has been reached
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

// Movement
int wheel_bpi = WHEEL_BASE * PI;
float steps_dist = STEPS_PER_REV / (WHEEL_DIAMETER * PI);

void runMotors() {
    while(motors.run()){
        ESP.wdtFeed();
    }
    // right_motor.run();
    // left_motor.run();
}

long steps[2];

int phogo_move_forward(float distance) {
    int s = int(distance * steps_dist);
    steps[0] = -s;
    steps[1] = s;
    DEBUGGING("[PHOGO]\tMoving forward ");
    DEBUGGINGL(distance);
    DEBUGGINGC(" (%d steps)\n", s);
    motors.moveTo(steps);
    // motors.runSpeedToPosition();
    runMotors();
    left_motor.setCurrentPosition(0);
    right_motor.setCurrentPosition(0);

    return 0;
}

int phogo_move_backward(float distance) {
    int s = int(distance * steps_dist);
    steps[0] = s;
    steps[1] = -s;
    DEBUGGING("[PHOGO]\tMoving backward ");
    DEBUGGINGL(distance);
    DEBUGGINGC(" (%d steps)\n", s);
    motors.moveTo(steps);
    runMotors();
    // motors.runSpeedToPosition();
    left_motor.setCurrentPosition(0);
    right_motor.setCurrentPosition(0);

    return 0;
}

int phogo_turn_left(float degrees) {
    float distance = wheel_bpi * (degrees / 360.0);
    int s = int(distance * steps_dist);
    DEBUGGING("[PHOGO]\tTurning left ");
    DEBUGGINGL(degrees);
    DEBUGGINGC(" (%d steps) -> ", s);
    DEBUGGINGL(distance);
    steps[0] = -s;
    steps[1] = -s;
    motors.moveTo(steps);
    runMotors();
    // motors.runSpeedToPosition();
    left_motor.setCurrentPosition(0);
    right_motor.setCurrentPosition(0);

    return 0;
}

int phogo_turn_right(float degrees) {
    float distance = wheel_bpi * (degrees / 360.0);
    int s = int(distance * steps_dist);
    DEBUGGING("[PHOGO]\tTurning right ");
    DEBUGGINGL(degrees);
    DEBUGGINGC(" (%d steps) -> ", s);
    DEBUGGINGL(distance);
    steps[0] = s;
    steps[1] = s;
    motors.moveTo(steps);
    runMotors();
    // motors.runSpeedToPosition();
    left_motor.setCurrentPosition(0);
    right_motor.setCurrentPosition(0);

    return 0;
}
