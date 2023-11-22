#include "main.h"

bool mirrorAuton = false;
bool useAuton = false;

void button0Callback()
{
	mirrorAuton = true;
}

void button2Callback()
{
	useAuton = true;
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
	pros::lcd::initialize();
	pros::lcd::register_btn0_cb(button0Callback);
	pros::lcd::register_btn2_cb(button2Callback);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize()
{
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous()
{
	Intake intake;

	std::shared_ptr<ChassisController> driveController =
			ChassisControllerBuilder()
					.withMotors(1, 2)
					// Green gearset, 4 in wheel diam, 11.5 in wheel track
					.withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
					.withMaxVelocity(150)
					.build();

	if (mirrorAuton)
		driveController->setTurnsMirrored(true);

	intake.moveAbsolute(-190, 50);

	// Drive forward 52 inches
	driveController->moveDistance(52_in);

	driveController->turnAngle(-78_deg);

	intake.moveAbsolute(5, 100);
	pros::delay(1000);
	// Drive forward 13 inches
	driveController->moveDistance(13.5_in);
	driveController->moveDistance(-4_in);
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol()
{
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor left_mtr(1);
	pros::Motor right_mtr(2, true);

	Intake intake;
	DriveControl driveControl(left_mtr, right_mtr);

	while (true)
	{
		if (useAuton)
		{
			autonomous();
			return;
		}
		// Read joystick values
		int leftJoystickY = master.get_analog(ANALOG_LEFT_Y);
		int rightJoystickX = -master.get_analog(ANALOG_RIGHT_X);

		driveControl.driveWithTrapezoidalProfile(leftJoystickY, rightJoystickX);
		if (master.get_digital(DIGITAL_L1))
			intake.moveAbsolute(0, 50);
		else if (master.get_digital(DIGITAL_L2))
			intake.moveAbsolute(-210, 50);
		pros::delay(20);
	}
}
