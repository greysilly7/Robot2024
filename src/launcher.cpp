#include "main.h"
#include "launcher.hpp"

Launcher::Launcher(pros::Motor &motor1, pros::Motor &motor2)
    : motor1(motor1), motor2(motor2) {}

void Launcher::launch()
{
  const int launchVelocity = 100; // Adjust as needed
  motor1.move_velocity(launchVelocity);
  motor2.move_velocity(launchVelocity);
}

void Launcher::stop()
{
  motor1.move_velocity(0);
  motor2.move_velocity(0);
}