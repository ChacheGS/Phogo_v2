// Pollock - Autonomous Drawing Robot
// See http://interactdigitalarts.uk/pollock
// This uses some snippets of code found on the Internet as well as fresh code. Thanks Internet people.

#include <NewPing.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>

// Ultrasonic Sensor
#define TRIGGER_PIN 12                // Arduino pin tied to trigger pin on the ultrasonic sensor
#define ECHO_PIN 11                   // Arduino pin tied to echo pin on the ultrasonic sensor
#define MAX_DISTANCE 200              // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
int sonarDistance;

// Stepper Motor
#define MAX_SPEED 520                 // You may need to experument with this to best suit your stepper motors
AccelStepper rightMotor(AccelStepper::HALF4WIRE, 2,4,3,5, true);
AccelStepper leftMotor(AccelStepper::HALF4WIRE, 6,8,7,9, true);
MultiStepper motors;
long steps[2];

// Servo
#define SERVO_PIN 10
Servo penServo;

// Movement Calculations
float pi = 3.141592;
float wheel_dia =  67.85;             // 66.25; // mm (increase = spiral out)
int wheel_base = 112;                 // mm (increase = spiral in) 
int wheel_bpi = wheel_base*pi;
int steps_rev = 4076;
float steps_dist = steps_rev / (wheel_dia*pi);

void setup() {
  Serial.begin(9600);
  penServo.attach(SERVO_PIN);
  penup();
  pendown();
	
  leftMotor.setMaxSpeed(MAX_SPEED);
  rightMotor.setMaxSpeed(MAX_SPEED);
  motors.addStepper(leftMotor);
  motors.addStepper(rightMotor);
}

void loop() {
  forward(5);
  sonarDistance = sonar.ping_cm();    // Send ping, get distance in cm (0 = outside set distance range)

  Serial.print("p: ");
  Serial.print(sonarDistance);
  Serial.println("cm");

  // Settings for vertical (front facing) ultrasonic sensor
  if ((sonarDistance > 0) && (sonarDistance < 20)) {
    backward(10);
    left(90);
  }
  // Settings for horizontal (down facing) ultrasonic sensor
  //if (sonarDistance > 6) {
  //  backward(10);
  //  left(90);
  //}

  // done();
}
