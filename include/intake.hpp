#ifndef INTAKE_HPP
#define INTAKE_HPP

#include "main.h"

class Intake
{
public:
  Intake();
  void actuateIntake(double input);
  void moveAbsolute(double position, double velocity);

private:
  pros::Motor intakeActuatorMotorLeft;
  pros::Motor intakeActuatorMotorRight;
};

#endif // INTAKE_HPP
