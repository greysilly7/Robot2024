#ifndef LAUNCHER_H
#define LAUNCHER_H

class Launcher
{
public:
  Launcher(pros::Motor &motor1, pros::Motor &motor2);

  void launch();
  void stop();

private:
  pros::Motor &motor1;
  pros::Motor &motor2;
};

#endif // LAUNCHER_H