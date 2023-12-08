#include "main.h"
#include "driveControl.hpp"

DriveControl::DriveControl(pros::Motor &frontLeftMotor, pros::Motor &frontRightMotor,
                           pros::Motor &backLeftMotor, pros::Motor &backRightMotor)
    : frontLeftMotor(frontLeftMotor), frontRightMotor(frontRightMotor),
      backLeftMotor(backLeftMotor), backRightMotor(backRightMotor) {}

void DriveControl::driveWithTrapezoidalProfile(int leftJoystickY, int leftJoystickX, int rightJoystickX)
{
  const double maxVelocity = 75.0;  // Adjust as needed
  const double acceleration = 15.0; // Adjust as needed

  // Calculate motor values for mecanum drive
  int frontLeftMotorValue = applyAccelerationCurve(leftJoystickY + leftJoystickX + rightJoystickX);
  int frontRightMotorValue = applyAccelerationCurve(leftJoystickY - leftJoystickX + rightJoystickX);
  int backLeftMotorValue = applyAccelerationCurve(leftJoystickY - leftJoystickX + rightJoystickX);
  int backRightMotorValue = applyAccelerationCurve(leftJoystickY + leftJoystickX - rightJoystickX);

  // Apply trapezoidal profile to each motor
  applyTrapezoidalProfile(frontLeftMotor, frontLeftMotorValue, maxVelocity, acceleration);
  applyTrapezoidalProfile(frontRightMotor, frontRightMotorValue, maxVelocity, acceleration);
  applyTrapezoidalProfile(backLeftMotor, backLeftMotorValue, maxVelocity, acceleration);
  applyTrapezoidalProfile(backRightMotor, backRightMotorValue, maxVelocity, acceleration);
}

// This function applies a trapezoidal profile to the given motor.
// The trapezoidal profile starts at the current velocity of the motor, and ramps up to the target velocity
// with an acceleration of acceleration. The trapezoidal profile will never exceed maxVelocity.
void DriveControl::applyTrapezoidalProfile(pros::Motor &motor, int targetVelocity, double maxVelocity, double acceleration)
{
  double currentVelocity = motor.get_actual_velocity();
  double error = targetVelocity - currentVelocity;

  // Apply trapezoidal profile
  if (std::abs(error) > maxVelocity)
  {
    // Acceleration phase
    motor.move_velocity(currentVelocity + (error > 0 ? acceleration : -acceleration));
  }
  else
  {
    // Constant speed phase
    motor.move_velocity(targetVelocity);
  }
}

int DriveControl::applyAccelerationCurve(int input)
{
  // Use absolute value of input so that it works in reverse
  double absoluteInput = std::abs(input / 127.0);

  // Apply the curve
  double curve = std::pow(absoluteInput, 2);

  // Apply the sign of the original input
  double signedCurve = curve * (input >= 0 ? 1 : -1);

  // Scale back up to the original range of values
  int scaledCurve = static_cast<int>(signedCurve * 127.0);

  // Return the result
  return scaledCurve;
}
