#include "motorcontroller.h"
#include <Arduino.h>
#include <stdlib.h>

// Motor Pinout
#define MOTOR_A_1 19
#define MOTOR_A_2 21
#define MOTOR_B_1 22
#define MOTOR_B_2 23

#define NUM_MOTORS 4

struct motor_event
{
  int eventtype;
  int power[NUM_MOTORS];
  int ticks[NUM_MOTORS];
};

struct motor_event eventqueue[10] = { 0 };
int waiting_events = 0;
struct motor_event *event;

int motorpower[NUM_MOTORS];
int motorticks[NUM_MOTORS];

void
setupMotors ()
{
  pinMode (MOTOR_A_1, OUTPUT);
  pinMode (MOTOR_A_2, OUTPUT);
  pinMode (MOTOR_B_1, OUTPUT);
  pinMode (MOTOR_B_2, OUTPUT);
}

void
updateMotors ()
{
  if (waiting_events > 0)
    {
      event = &eventqueue[0];
      
      

    }
}

int
convert (int val)
{
  int result = abs ((val / 100) * 255);
  if (result > 255)
    return 255;
  if (result < 0)
    return 0;
  return result;
}

void
forward (int speed)
{
  analogWrite (MOTOR_A_1, convert (speed));
  analogWrite (MOTOR_B_1, convert (speed));
  analogWrite (MOTOR_A_2, 0);
  analogWrite (MOTOR_B_2, 0);
}

void
backward (int speed)
{
  analogWrite (MOTOR_A_1, 0);
  analogWrite (MOTOR_B_1, 0);
  analogWrite (MOTOR_A_2, convert (speed));
  analogWrite (MOTOR_B_2, convert (speed));
}

void
left (int speed)
{
  analogWrite (MOTOR_A_1, 0);
  analogWrite (MOTOR_B_1, convert (speed));
  analogWrite (MOTOR_A_2, convert (speed));
  analogWrite (MOTOR_B_2, 0);
}

void
right (int speed)
{
  analogWrite (MOTOR_A_1, convert (speed));
  analogWrite (MOTOR_B_1, 0);
  analogWrite (MOTOR_A_2, 0);
  analogWrite (MOTOR_B_2, convert (speed));
}

/*
Turn, range from -100 left to 100 right
*/
void
turn (int turnspeed)
{
  if (-100 <= turnspeed < 0)
    {
      left (turnspeed);
    }
  else if (0 <= turnspeed <= 100)
    {
      right (turnspeed);
    }
}

/*
Turn, range from -100 forward to 100 backward
*/
void
moveforward (int speed)
{
  if (-100 <= speed < 0)
    {
      backward (speed);
    }
  else if (0 <= speed <= 100)
    {
      forward (speed);
    }
}

/*
    Allows robot to move like a car
    accel - speed moving forward or back [-100,100]
    turn - turning bias [-100, 100]
*/
void
carDrive (int accel, int turn)
{
  if (-100 <= accel < 0)
    {
      analogWrite (MOTOR_A_1, convert (accel - turn));
      analogWrite (MOTOR_B_1, convert (accel + turn));
      analogWrite (MOTOR_A_2, 0);
      analogWrite (MOTOR_B_2, 0);
    }
  else if (0 <= accel <= 100)
    {
      analogWrite (MOTOR_A_1, 0);
      analogWrite (MOTOR_B_1, 0);
      analogWrite (MOTOR_A_2, convert (accel - turn));
      analogWrite (MOTOR_B_2, convert (accel + turn));
    }
}

void
brake ()
{
  analogWrite (MOTOR_A_1, 0);
  analogWrite (MOTOR_B_1, 0);
  analogWrite (MOTOR_A_2, 0);
  analogWrite (MOTOR_B_2, 0);
}