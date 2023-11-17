#ifndef DRIVE_CONTROL_HPP
#define DRIVE_CONTROL_HPP

#include "main.h"

class DriveControl
{
public:
  DriveControl(pros::Motor &leftMotor, pros::Motor &rightMotor);
  void driveWithTrapezoidalProfile(int leftJoystickY, int rightJoystickX);

private:
  pros::Motor &leftMotor;
  pros::Motor &rightMotor;

  void applyTrapezoidalProfile(pros::Motor &motor, int targetVelocity, double maxVelocity, double acceleration);
  int applyAccelerationCurve(int input);
};

#endif // DRIVE_CONTROL_HPP
