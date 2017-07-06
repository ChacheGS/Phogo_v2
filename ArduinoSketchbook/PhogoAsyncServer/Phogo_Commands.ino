int phogo_pen_down() {
  pen_move(SERVO_DOWN);
  DEBUGGING("Pen down\n");
  return 0;
}

int phogo_pen_up() {
  pen_move(SERVO_UP);
  DEBUGGING("Pen up\n");
  return 0;
}

// Movement
float wheel_dia =  67.85;
int wheel_base = 112;
int wheel_bpi = WHEEL_BASE * PI;
float steps_dist = STEPS_PER_REV / (WHEEL_DIAMETER * PI);


long steps[2];

int phogo_move_forward(float distance) {
  int s = int(distance * steps_dist);
  steps[0] = -s;
  steps[1] = s;
  DEBUGGING("Moving forward ");
  DEBUGGINGL(distance);
  DEBUGGINGC(" (%d steps)\n", s);
  motors.moveTo(steps);
  motors.runSpeedToPosition();
  left_motor.setCurrentPosition(0);
  right_motor.setCurrentPosition(0);

  return 0;
}

int phogo_move_backward(float distance) {
  int s = int(distance * steps_dist);
  steps[0] = s;
  steps[1] = -s;
  DEBUGGING("Moving backward ");
  DEBUGGINGL(distance);
  DEBUGGINGC(" (%d steps)\n", s);
  motors.moveTo(steps);
  motors.runSpeedToPosition();
  left_motor.setCurrentPosition(0);
  right_motor.setCurrentPosition(0);

  return 0;
}

int phogo_turn_left(float degrees) {
  float distance = wheel_bpi * (degrees / 360.0);
  int s = int(distance * steps_dist);
  DEBUGGING("Turning left ");
  DEBUGGINGL(degrees);
  DEBUGGINGC(" (%d steps) -> ", s);
  DEBUGGINGL(distance);
  steps[0] = -s;
  steps[1] = -s;
  motors.moveTo(steps);
  motors.runSpeedToPosition();
  left_motor.setCurrentPosition(0);
  right_motor.setCurrentPosition(0);

  return 0;
}

int phogo_turn_right(float degrees) {
  float distance = wheel_bpi * (degrees / 360.0);
  int s = int(distance * steps_dist);
  DEBUGGING("Turning right ");
  DEBUGGINGL(degrees);
  DEBUGGINGC(" (%d steps) -> ", s);
  DEBUGGINGL(distance);
  steps[0] = s;
  steps[1] = s;
  motors.moveTo(steps);
  motors.runSpeedToPosition();
  left_motor.setCurrentPosition(0);
  right_motor.setCurrentPosition(0);

  return 0;
}
