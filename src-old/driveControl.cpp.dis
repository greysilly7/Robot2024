#include "main.h"

DriveControl::DriveControl(pros::Motor &leftMotor, pros::Motor &rightMotor)
    : leftMotor(leftMotor), rightMotor(rightMotor) {}

void DriveControl::driveWithTrapezoidalProfile(int leftJoystickY, int leftJoystickX)
{
  const double maxVelocity = 50.0;  // Adjust as needed
  const double acceleration = 15.0; // Adjust as needed

  // Calculate motor values for arcade drive
  int forwardValue = applyAccelerationCurve(leftJoystickY);
  int turnValue = applyAccelerationCurve(leftJoystickX);

  // Calculate left and right motor values
  int leftMotorValue = forwardValue - turnValue;
  int rightMotorValue = forwardValue + turnValue;

  // Apply trapezoidal profile to the left motor
  applyTrapezoidalProfile(leftMotor, leftMotorValue, maxVelocity, acceleration);

  // Apply trapezoidal profile to the right motor
  applyTrapezoidalProfile(rightMotor, rightMotorValue, maxVelocity, acceleration);
}

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

// Apply an acceleration curve to the input
// This makes the robot easier to control at low speeds
// and makes it easier to drive straight
int DriveControl::applyAccelerationCurve(int input)
{
  // Use absolute value of input so that it works in reverse
  double absoluteInput = std::abs(input / 127.0);

  // Apply the curve
  double curve = std::pow(absoluteInput, 2);

  // Apply the sign of the original input
  // (This can be done in a single line, but is split into 2 for clarity)
  double signedCurve = curve * (input >= 0 ? 1 : -1);

  // Scale back up to the original range of values
  // (This can be done in a single line, but is split into 2 for clarity)
  int scaledCurve = static_cast<int>(signedCurve * 127.0);

  // Return the result
  return scaledCurve;
}
