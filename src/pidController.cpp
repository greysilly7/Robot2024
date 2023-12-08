#include "PIDController.hpp"

PIDController::PIDController(double kP, double kI, double kD)
    : kP(kP), kI(kI), kD(kD), integral(0), previousError(0) {}

double PIDController::calculate(double setpoint, double actual)
{
  double error = setpoint - actual;
  integral += error;
  double derivative = error - previousError;
  previousError = error;
  return kP * error + kI * integral + kD * derivative;
}

void PIDController::setP(double kP) { this->kP = kP; }
void PIDController::setI(double kI) { this->kI = kI; }
void PIDController::setD(double kD) { this->kD = kD; }

double PIDController::getP() const { return kP; }
double PIDController::getI() const { return kI; }
double PIDController::getD() const { return kD; }