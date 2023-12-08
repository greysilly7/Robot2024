#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H

class PIDController
{
public:
  // Constructor
  PIDController(double kP, double kI, double kD);

  // Calculate the PID output
  double calculate(double setpoint, double actual);

  // Setters for the PID gains
  void setP(double kP);
  void setI(double kI);
  void setD(double kD);

  // Getters for the PID gains
  double getP() const;
  double getI() const;
  double getD() const;

private:
  double kP, kI, kD;
  double integral;
  double previousError;
};

#endif // PIDCONTROLLER_H