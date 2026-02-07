/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       brianpech                                                 */
/*    Created:      21/1/2026, 4:53:05 p.m.                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

// Controlador
controller Controller1 = controller(primary);

// Motores para X-Drive (cartuchos verdes = ratio18_1)
// Configuración de motores para base X-Drive
motor FrontLeft = motor(PORT1, ratio18_1, false);
motor FrontRight = motor(PORT2, ratio18_1, true); // Invertido
motor BackLeft = motor(PORT3, ratio18_1, false);
motor BackRight = motor(PORT4, ratio18_1, true); // Invertido

// Motor de intake
motor Intake = motor(PORT5, ratio18_1, false);

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void)
{

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...

  // Configurar modo de frenado para los motores
  FrontLeft.setStopping(brake);
  FrontRight.setStopping(brake);
  BackLeft.setStopping(brake);
  BackRight.setStopping(brake);
  Intake.setStopping(brake);

  // Resetear posiciones de los encoders
  FrontLeft.resetPosition();
  FrontRight.resetPosition();
  BackLeft.resetPosition();
  BackRight.resetPosition();
  Intake.resetPosition();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void)
{
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void)
{
  // User control code here, inside the loop
  while (1)
  {
    // Leer valores de los joysticks
    // Axis3 = Joystick izquierdo vertical (avanzar/retroceder)
    // Axis4 = Joystick izquierdo horizontal (moverse lateralmente - strafe)
    // Axis1 = Joystick derecho horizontal (girar izquierda/derecha)
    int throttle = Controller1.Axis3.position(); // -100 a 100 (avance/retroceso)
    int strafe = Controller1.Axis4.position();   // -100 a 100 (movimiento lateral)
    int turn = Controller1.Axis1.position();     // -100 a 100 (giro)

    // Deadband para evitar deriva cuando el joystick está cerca de 0
    auto applyDeadband = [](int v, int db)
    {
      return (v > db || v < -db) ? v : 0;
    };
    throttle = applyDeadband(throttle, 5);
    strafe = applyDeadband(strafe, 5);
    turn = applyDeadband(turn, 5);

    // Calcular velocidades para cada motor del X-Drive (cinemática omnidireccional)
    // Para X-Drive con ruedas a 45°: cada motor contribuye a avance, lateral y giro
    int frontLeftSpeed = throttle + strafe + turn;
    int frontRightSpeed = throttle - strafe - turn;
    int backLeftSpeed = throttle - strafe + turn;
    int backRightSpeed = throttle + strafe - turn;

    // Normalización para evitar saturación (mantener el vector de movimiento)
    // Si alguna velocidad excede 100, escalar proporcionalmente todas
    int maxSpeed = abs(frontLeftSpeed);
    if (abs(frontRightSpeed) > maxSpeed)
      maxSpeed = abs(frontRightSpeed);
    if (abs(backLeftSpeed) > maxSpeed)
      maxSpeed = abs(backLeftSpeed);
    if (abs(backRightSpeed) > maxSpeed)
      maxSpeed = abs(backRightSpeed);

    if (maxSpeed > 100)
    {
      frontLeftSpeed = (frontLeftSpeed * 100) / maxSpeed;
      frontRightSpeed = (frontRightSpeed * 100) / maxSpeed;
      backLeftSpeed = (backLeftSpeed * 100) / maxSpeed;
      backRightSpeed = (backRightSpeed * 100) / maxSpeed;
    }

    // Establecer velocidades de los motores
    FrontLeft.spin(forward, frontLeftSpeed, velocityUnits::pct);
    FrontRight.spin(forward, frontRightSpeed, velocityUnits::pct);
    BackLeft.spin(forward, backLeftSpeed, velocityUnits::pct);
    BackRight.spin(forward, backRightSpeed, velocityUnits::pct);

    // Intake: L2 mete (gira hacia atras), R2 saca (gira hacia adelante)
    if (Controller1.ButtonL2.pressing())
    {
      Intake.spin(reverse, 100, velocityUnits::pct);
    }
    else if (Controller1.ButtonR2.pressing())
    {
      Intake.spin(forward, 100, velocityUnits::pct);
    }
    else
    {
      Intake.stop();
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main()
{
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true)
  {
    wait(100, msec);
  }
}
