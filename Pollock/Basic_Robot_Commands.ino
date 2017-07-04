// Pollock - Autonomous Drawing Robot
// See http://interactdigitalarts.uk/pollock
// This uses some snippets of code found on the Internet as well as fresh code. Thanks Internet people.

// Basic Robot Commands

// - pendown()
// - penup()
// - forward(float distance)
// - backward(float distance)
// - left(float degrees)
// - right(float degrees)
// - done()

void pendown() {
  penServo.write(90);	
  delay(250);
}

void penup() {
  penServo.write(180);
  delay(250);
}

void forward(float distance) {
  int s = int(distance*steps_dist);
  steps[0] = -s;
  steps[1] = s;
  Serial.print("f: ");
  Serial.println(distance);
  motors.moveTo(steps);
  motors.runSpeedToPosition();
  leftMotor.setCurrentPosition(0);
  rightMotor.setCurrentPosition(0);
}

void backward(float distance) {
  int s = int(distance*steps_dist);
  steps[0] = s;
  steps[1] = -s;
  Serial.print("b: ");	Serial.println(distance);
  motors.moveTo(steps);
  motors.runSpeedToPosition();
  leftMotor.setCurrentPosition(0);
  rightMotor.setCurrentPosition(0);
}

void left(float degrees) {
  float distance = wheel_bpi * (degrees/360.0);
  Serial.print("l: ");
  Serial.println(distance);
  int s = int(distance*steps_dist);
  steps[0] = -s;
  steps[1] = -s;
  motors.moveTo(steps);
  motors.runSpeedToPosition();
  leftMotor.setCurrentPosition(0);
  rightMotor.setCurrentPosition(0);
}

void right(float degrees) {
  float distance = wheel_bpi * (degrees/360.0);
  Serial.print("r: ");
  Serial.println(distance);
  int s = int(distance*steps_dist);
  steps[0] = s;
  steps[1] = s;
  motors.moveTo(steps);
  motors.runSpeedToPosition();
  leftMotor.setCurrentPosition(0);
  rightMotor.setCurrentPosition(0);
}

void done() {
  penup();
  for (int i=2; i<10; i++) {
    digitalWrite(i, LOW);
  }
}
