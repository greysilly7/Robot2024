// drive_control.hpp

#ifndef DRIVE_CONTROL_HPP
#define DRIVE_CONTROL_HPP

#include "main.h"
#include <cmath>

class DriveControl
{
public:
  DriveControl(pros::Motor &frontLeftMotor, pros::Motor &frontRightMotor,
               pros::Motor &backLeftMotor, pros::Motor &backRightMotor);
  void driveWithTrapezoidalProfile(int leftJoystickY, int leftJoystickX, int rightJoystickX);
  void applyTrapezoidalProfile(pros::Motor &motor, int targetVelocity, double maxVelocity, double acceleration);
  int applyAccelerationCurve(int input);

private:
  pros::Motor &frontLeftMotor;
  pros::Motor &frontRightMotor;
  pros::Motor &backLeftMotor;
  pros::Motor &backRightMotor;
};

#endif // DRIVE_CONTROL_HPP
