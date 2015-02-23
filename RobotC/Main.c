#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTServo,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     MDriveL,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     MSweep,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_1,     MDriveR,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_2,     MLift,         tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C2_1,    SDoor,               tServoNone)
#pragma config(Servo,  srvo_S2_C2_2,    SClamp,               tServoNone)
#pragma config(Servo,  srvo_S2_C2_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////Includes///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include "JoystickDriver.c" //Includes everything needed to use the joystick.



/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////Variables//////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//These are all variables for the various server positions, so if we ever rotate them, we don't have to change every reference to them in the code, only the value of this variable.
//Clamp
int SClampUp = 171;
int SClampDown = 65;

//Door
int SDoorClosed = 45;
int SDoorOpen = 184;
int SDoorTime = 2;

//DeadZone
int ThreshHold = 7;
int LiftHold = 5;

//Auto
int BatteryPower = 100;
int ReferenceMeters = 10;
int Reference Seconds = 3;
int DesiredMeters = 20;
int DesiredSeconds = 9;
int Seconds = 11;
int Time = 12000;
int DesiredRadians = 2
int Reference =



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////Initialize//////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void initializeRobot() //This function is used before the rest of the program starts to initialize parts of the robots, like the servos. If the servos didn't have values set here, then they would pick a random position (wich would be dangerous particularly for the door on the box, it could cause the door to go to an extreme and break either the box or the servo to break.).
{
	servoChangeRate[SClamp] = 10; //The servoChangeRate command tells a particular servo how many positions to move every time it updates (tipically 20 milliseconds). 10 is the default value, and it is quite fast, which is why I chose it for the clamp servo, which needs to be able to clamp quickly before the pentagon moves away from the robot.
	servo[SClamp] = SClampUp; //Sets the clamp to up position
	servoChangeRate[SDoor] = 4; //I set the change rate for the door to a lower value because it has no need to go as fast as the clamp, and we want to avoid damaging the box and or servo.
	servo[SDoor] = SDoorClosed; //Sets the door to the closed position.
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////Control Functions///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//MapRange
def MapRangeXToRangeY(x,y)
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//Drive
void CDrive()
{
	if (abs(joystick.joy1_y1) > ThreshHold) motor[MDriveL] = joystick.joy1_y1; //The first part of this line checks to make sure that the absolute value of the joystick is greater than 7, because if it is anywhere between -7 and 7 the robot might move or strain the motors even when you aren't moving the joystick (it is a deadzone). The next part of the line will set the motor to the appropriate power, but only if it is out of the deadzone.
	else motor[MDriveR] = 0; //This line sets the motor to do nothing if it is inside the deadzone.
	if (abs(joystick.joy1_y2) > ThreshHold) motor[MDriveR] = joystick.joy1_y2;
	else motor[MDriveL] = 0;
}

//Lift
void CLift()
{
	if (abs(joystick.joy2_y2) > ThreshHold) motor[MLift] = joystick.joy2_y2; //It does the exact same as the if loop above, but for the lift instead.
	else motor[MLift] = LiftHold; //Notice that the value used in the deadzone is set to 5 rather than 0, which means that when you aren't pushing on the joystick it still adds a bit of force to the motor, keeping the box and lift at their current height.
}

//Sweep
void CSweep()
{
	if (abs(joystick.joy2_x1) > ThreshHold) motor[MSweep] = joystick.joy2_x1; //The sweep is the exact same as the drive loop, only instead of using the y value, it uses the x value (That should match the direction of the sweeper, left is out, right is in).
	else motor[MSweep] = 0;
}

//Clamp
void CClamp()
{
	if (joystick.joy1_TopHat == 0 || joystick.joy2_TopHat == 0) servo[SClamp] = SClampUp; //When the top hat (the cross) is pushed up, it will set the Clamp to its up position.
	if (joystick.joy1_TopHat == 4 || joystick.joy2_TopHat == 4) servo[SClamp] = SClampDown;//When the top hat (the cross) is pushed down, it will set the Clamp to its down position.
}

//Door
void CDoor()
{
	if (joy2Btn(2)) servo[SDoor] = SDoorOpen;
	else servo[SDoor] = SDoorClosed;
}

//Master
void CMaster()
{
	getJoystickSettings(joystick);//Get all of the values from the joystick.
	CDrive();
	CLift();
	CSweep();
	CClamp();
	CDoor();
}




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////Autonomous//////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//DistanceToMS
void DistanceToMS()
{
	DesiredSeconds = (ReferenceSeconds * DesiredMeters) / ReferenceMeters;
	Time = DesiredSeconds * 1000;
}

//RadiansToMS
void RadiansToMS()
{
	DesiredSeconds = (ReferenceSeconds * DesiredRadians) / ReferenceRadians
}

//DriveS
void ADriveS()
{
	DistanceToMS();
	ClearTimer(T1);
	while (time1[T1]<Time)
	{
		motor[MDriveL] = 70;
		motor[MDriveR] = 70;
	}
	motor[MDriveL] = 0;
	motor[MDriveR] = 0;
}

//TurnRight
void ATurnR()
{
	ClearTimer(T1);
	while (time1[T1]<Time)
}

//LiftUp
void ALiftU()
{
	ClearTimer(T1);
	while (time1[T1] < Time)
	{
		motor[MLift] = 70;
	}
	motor[MLift] = 0;
}

//LiftDown
void ALiftD()
{
	ClearTimer(T1);
	while (time1[T1] < Time)
	{
		motor[MLift] = -10;
	}
	motor[MLift] = 0;
}

//LiftHold
void ALiftH()
{
	ClearTimer(T1);
	while (time1[T1] < Time)
	{
		motor[MLift] = LiftHold;
	}
}

//ClampUp
void AClampU()
{
	servo[SClamp] = SClampUp;
}

//ClampDown
void AClampD()
{
	servo[SClamp] = SClampDown;
}

//DoorClose
void ADoorC()
{
	servo[SDoor] = SDoorClosed;
}

//DoorOpen
void ADoorO()
{
	servo[SDoor] = SDoorClosed;
}

//Master
void AMaster()
{

}



///////////////////////////////////////////////////////////////////////////
////////////////////////////////////Main///////////////////////////////////
///////////////////////////////////////////////////////////////////////////
task main()
{
	initializeRobot(); //Run the initialize function to set the servos to their appropriate starting position.
	waitForStart(); //The program won't continue past this point until told to (you do so by going to Robot ---> Debugger Windows ---> Joystick Controll - Competition.
	while (true) //Infinite Loop
	{
		CMaster(); //Allow joystick controls.
	}
}
