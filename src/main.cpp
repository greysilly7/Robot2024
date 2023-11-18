#include "main.h"

// Whether to turn left or right in autonomous
bool leftOrRight = false;

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button()
{
	static bool pressed = false;
	pressed = !pressed;
	if (pressed)
	{
		leftOrRight = true;
	}
	else
	{
		leftOrRight = false;
	}
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
	pros::lcd::set_text(1, "Please select an autonomous mode");
	pros::lcd::set_text(2, "Press Center button if on right of goal when looking towards opposite goal");

	pros::lcd::register_btn1_cb(on_center_button);
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
void competition_initialize() {}

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
					.withMotors(1, -2)
					// Green gearset, 4 in wheel diam, 11.5 in wheel track
					// TODO: UPDATE this
					.withDimensions(AbstractMotor::gearset::green, {{4_in, 11.5_in}, imev5GreenTPR})
					.build();

	intake.moveAbsolute(-190, 50);

	// Drive forward 52 inches
	driveController->moveDistance(52_in);
	// Turn left or right 90 degrees
	if (leftOrRight)
		driveController->turnAngle(90_deg);
	else
		driveController->turnAngle(-90_deg);
	intake.moveAbsolute(-25, 50);
	// Drive forward 13 inches
	driveController->moveDistance(13.5_in);
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
		// Read joystick values
		int leftJoystickY = master.get_analog(ANALOG_LEFT_Y);
		int rightJoystickX = -master.get_analog(ANALOG_RIGHT_X);

		driveControl.driveWithTrapezoidalProfile(leftJoystickY, rightJoystickX);
		if (master.get_digital(DIGITAL_L1))
			intake.moveAbsolute(0, 50);
		else if (master.get_digital(DIGITAL_L2))
			intake.moveAbsolute(-190, 50);
		pros::delay(20);
	}
}
