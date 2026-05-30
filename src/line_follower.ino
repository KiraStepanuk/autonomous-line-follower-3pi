#include <Pololu3pi.h>
#include <PololuQTRSensors.h>
#include <OrangutanMotors.h>
#include <OrangutanPushbuttons.h>
#include <OrangutanAnalog.h>

Pololu3pi robot;
OrangutanMotors motors;
OrangutanPushbuttons buttons;

const int BOOST_SPEED = 5; 
const int KP_INT = 13;
const int KD_INT = 1400 ;
int BASE_SPEED = 230;
const int MAX_STEP_UP = 5;
const int MAX_STEP_DOWN = 11;
int currentLeftSpeed = 0;
int currentRightSpeed = 0;
int lastError = 0;

void setup() {
  robot.init(2000);


  buttons.waitForButton(BUTTON_B);

  for (int i = 0; i < 80; i++) {
    if (i < 20 || i >= 60) motors.setSpeeds(60, -60);
    else motors.setSpeeds(-60, 60);
    robot.calibrateLineSensors(IR_EMITTERS_ON);
    delay(20);
  }
  motors.setSpeeds(0, 0);

  unsigned int sensors[5];
  int pos = robot.readLine(sensors, IR_EMITTERS_ON);
  lastError = pos - 2000;

  delay(1000);
}

void loop() {
  unsigned int sensors[5];
  int position = robot.readLine(sensors, IR_EMITTERS_ON);
  int error = position - 2000;

  int dynamicSpeed = BASE_SPEED - (abs(error) / 9); 
  if (abs(error) < 80) {
      dynamicSpeed += BOOST_SPEED;
  }
  if (abs(error) > 1100) {
      dynamicSpeed = 95;
  } else if (dynamicSpeed < 90) {
      dynamicSpeed = 90;
  }


  long pidOutput = ((long)error * KP_INT + (long)(error - lastError) * KD_INT) >> 8;
  lastError = error;

  int targetLeft = dynamicSpeed + (int)pidOutput;
  int targetRight = dynamicSpeed - (int)pidOutput;


  if (targetLeft < -40) targetLeft = -40; 
  if (targetRight < -40) targetRight = -40;
  if (targetLeft > 255) targetLeft = 255;
  if (targetRight > 255) targetRight = 255;

  int currentStepDown = (abs(error) > 1000) ? 11 : 9; 

  int diffL = targetLeft - currentLeftSpeed;
  if (diffL > MAX_STEP_UP) diffL = MAX_STEP_UP;
  else if (diffL < -currentStepDown) diffL = -currentStepDown;
  currentLeftSpeed += diffL;

  int diffR = targetRight - currentRightSpeed;
  if (diffR > MAX_STEP_UP) diffR = MAX_STEP_UP;
  else if (diffR < -currentStepDown) diffR = -currentStepDown;
  currentRightSpeed += diffR;

  motors.setSpeeds(currentLeftSpeed, currentRightSpeed);

  if (buttons.isPressed(BUTTON_B)) {
    motors.setSpeeds(0, 0);
    while(1);
  }
}
