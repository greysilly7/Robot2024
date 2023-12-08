#include "main.h"
#include "driveControl.hpp"
#include "launcher.hpp"

// Initize some variables for Auton and testing it
// This lets use go on either side of the field, default is turning left
bool mirrorAuton = false;
// This lets us test auton in teleop, tied to a button
bool testAuton = false;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	// Register a call back for button 0 and 2 changing to toggle mirrorAuton and testAuton
	pros::lcd::register_btn1_cb([]()
															{ mirrorAuton = !mirrorAuton; });
	pros::lcd::register_btn2_cb([]()
															{ testAuton = !testAuton; });
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
void autonomous() {}

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
	pros::Motor frontLeftMotor(-2);
	pros::Motor frontRightMotor(4);
	pros::Motor backLeftMotor(-1);
	pros::Motor backRightMotor(3);

	pros::Motor launcherTop(5);
	pros::Motor luancherButtom(6);

	DriveControl driveControl(frontLeftMotor, frontRightMotor, backLeftMotor, backRightMotor);
	Launcher launcher(launcherTop, luancherButtom);

	// Keep track of launcher state
	bool isLauncherOn = false;
	// Debounce delay in milliseconds
	const int debounceDelay = 200;

	while (true)
	{
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
										 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
										 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0); // Prints status of the emulated screen LCDs

		// Read joystick values
		int leftJoystickY = master.get_analog(ANALOG_LEFT_Y);		 // Fowards and vackwards
		int leftJoystickX = master.get_analog(ANALOG_LEFT_X);		 // Strafing
		int rightJoystickX = -master.get_analog(ANALOG_RIGHT_X); // Rotation

		// Drive the robot using the joystick values
		driveControl.driveWithTrapezoidalProfile(leftJoystickY, leftJoystickX, rightJoystickX);

		// If L1 is pressed and the launcher is off, turn it on
		if (master.get_digital(DIGITAL_L1) && !isLauncherOn)
		{
			launcher.launch();
			isLauncherOn = true;

			// Debounce delay
			pros::delay(debounceDelay);
		}
		// If L2 is pressed and the launcher is on, turn it off
		else if (master.get_digital(DIGITAL_L2) && isLauncherOn)
		{
			launcher.stop();
			isLauncherOn = false;

			// Debounce delay
			pros::delay(debounceDelay);
		}

		// Delay for a short time before the next update
		pros::delay(20);
	}
}