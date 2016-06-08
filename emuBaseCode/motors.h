int LEFT_SPEED = 4;
int RIGHT_SPEED = 5;
int LEFT_DIR = 2;
int RIGHT_DIR = 0;

void motorSetup() {
  pinMode(LEFT_SPEED, OUTPUT);
  pinMode(RIGHT_SPEED, OUTPUT);
  pinMode(LEFT_DIR, OUTPUT);
  pinMode(RIGHT_DIR, OUTPUT);
}

void leftMotor(int motorSpeed, int mult) {
  int absSpeed = motorSpeed*mult;
  int dir;
  if (absSpeed < 0) {
    absSpeed *= -1;
    dir = LOW;
  } else {
    dir = HIGH;
  }

  analogWrite(LEFT_SPEED, absSpeed);
  digitalWrite(LEFT_DIR, dir);
}

void rightMotor(int motorSpeed, int mult) {
  int absSpeed = motorSpeed*mult;
  int dir;
  if (absSpeed < 0) {
    absSpeed *= -1;
    dir = LOW;
  } else {
    dir = HIGH;
  }

  analogWrite(RIGHT_SPEED, absSpeed);
  digitalWrite(RIGHT_DIR, dir);
}
