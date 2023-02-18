#include "main.h"
using namespace pros;
Motor a1(4); // front left
Motor a2(5); // back left
Motor b1(7); // front right
Motor b2(8); // back right
Motor catapult(18);
ADIDigitalIn loadcat(1);
Motor frontIntake(10);
Controller master(E_CONTROLLER_MASTER);

void on_center_button()
{
    static bool pressed = false;
    pressed = !pressed;
    if (pressed)
    {
        pros::lcd::set_text(2, "I was pressed!");
    }
    else
    {
        pros::lcd::clear_line(2);
    }
}

void initialize()
{
    pros::lcd::initialize();
    pros::lcd::set_text(1, "hello pros user!");
    pros::lcd::register_btn1_cb(on_center_button);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol()
{

    double rightx; // this is actually righty
    double righty; // rightx
    // stick drift calibration
    double rxcalibrate = master.get_analog(ANALOG_RIGHT_Y);
    double rycalibrate = master.get_analog(ANALOG_RIGHT_X);
    bool autoload = false;
    bool loadingcat = false;
    rightx -= rxcalibrate;
    righty -= rycalibrate;

    while (true)
    {

        rightx = master.get_analog(ANALOG_RIGHT_Y);
        righty = master.get_analog(ANALOG_RIGHT_X);
        rightx -= rxcalibrate;
        righty -= rycalibrate;

        a1 = rightx + righty;
        a2 = rightx + righty;
        b1 = -rightx + righty;
        b2 = -rightx + righty;

        if (master.get_digital(DIGITAL_R1) == 1)
        {
            frontIntake = -127;
        }
        else if (master.get_digital(DIGITAL_R2) == 1)
        {
            frontIntake = 127;
        }
        else
        {
            frontIntake = 0;
        }
        
        
        if (master.get_digital(DIGITAL_UP) == 1)
        {
            autoload = true;
        }
        else if (master.get_digital(DIGITAL_UP) == 0 && loadingcat == false)
        {
            autoload = false;
        }
        /*if(master.get_digital(DIGITAL_UP) == 1) {
            rightx = 127;
        }
        else if (master.get_analog(ANALOG_RIGHT_Y) == 0){
            rightx = 0;
        } */
        while (autoload == true)
        {
            if (loadcat.get_value() == 0)
            {
                catapult = -80;
                loadingcat = true;
            }
            else if (loadcat.get_value() == 1)
            {
                catapult = -10;
                loadingcat = false;
            }
        }

        if (master.get_digital(DIGITAL_L1) == 1 && not loadcat.get_value() == 1)
        {
            catapult = -127;
        }
        else if (master.get_digital(DIGITAL_L2) == 1 && not loadcat.get_value() == 1)
        {
            catapult = -60;
        }
        else
        {
            catapult = 0;
        }

        // pros::delay(20); // delay
    }
}