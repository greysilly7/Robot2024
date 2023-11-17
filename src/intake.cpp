#include "main.h"

Intake::Intake() : intakeActuatorMotorLeft(3, true), intakeActuatorMotorRight(4)
{
  intakeActuatorMotorLeft.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  intakeActuatorMotorRight.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  intakeActuatorMotorLeft.tare_position();
  intakeActuatorMotorRight.tare_position();
}
void Intake::actuateIntake(double input)
{
  // Scale input from +-127 to +-145
  double scaledInput = input * 145.0 / 127.0;

  // Apply deadzone
  if (std::abs(scaledInput) <= 10)
  {
    return;
  }

  // Apply scaling for motor velocity based on the percentage of joystick value
  double velocityScale = std::abs(scaledInput) / 145.0;   // Normalize to 0-1
  double velocity = static_cast<int>(velocityScale * 50); // Scale to motor velocity

  // Limit the maximum velocity to prevent saturation
  velocity = std::min(static_cast<int>(velocity), 50);
  // Update both intake motors with the target position and scaled velocity
  moveAbsolute(scaledInput, velocity);
}

void Intake::moveAbsolute(double position, double velocity)
{
  // Not needed since we moved off of analog
  // position = std::max(-107.0, std::min(45.0, position));

  // Update both intake motors with the target position and specified velocity
  intakeActuatorMotorLeft.move_absolute(position, velocity);
  intakeActuatorMotorRight.move_absolute(position, velocity);
}
