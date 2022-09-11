
#include "encoder.h"
#include <Arduino.h>

#define ENCODER1_PIN 15
#define ENCODER2_PIN 2
#define ENCODER3_PIN 4
#define ENCODER4_PIN 16

#define NUM_ENCODERS 4
#define NUM_BITS_IN_INT 16

#define DISTANCE_PER_TICK 1.055

#define SPEED_RECORD_SIZE 5

int t;
int total_ticks[NUM_ENCODERS] = { 0 };
int ticktime[NUM_ENCODERS] = { 0 };
int readings[NUM_ENCODERS] = { 0 };
double speedrecord[NUM_ENCODERS][SPEED_RECORD_SIZE] = { 0 };
double speed[NUM_ENCODERS] = { 0 };
double speedtemp;
int encoder_pins[NUM_ENCODERS]
    = { ENCODER1_PIN, ENCODER2_PIN, ENCODER3_PIN, ENCODER4_PIN };
void
setupEncoders ()
{
  pinMode (ENCODER1_PIN, INPUT);
  pinMode (ENCODER2_PIN, INPUT);
  pinMode (ENCODER3_PIN, INPUT);
  pinMode (ENCODER4_PIN, INPUT);
}

double
min (double a, double b)
{
  if (a < b)
    return a;
  else
    return b;
}

void
updateEncoders ()
{
  t = millis ();
  for (int i = 0; i < NUM_ENCODERS; i++)
    {
      // Read and Store Sensor Value
      readings[i] = readings[i] << 1 | digitalRead (encoder_pins[i]);

      // Calculate Ticks
      if (((readings[i] & 0b11) == 0b10) || ((readings[i] & 0b11) == 0b01))
        {
          total_ticks[i]++;
          speedtemp
              = DISTANCE_PER_TICK
                / ((t - ticktime[i]) / 1000.0); // distance / seconds per tick
          //   Serial.printf ("%2lf \n", speedtemp);
          pushArray (speedtemp, speedrecord[i], SPEED_RECORD_SIZE);
          //   printArray (speedrecord[i], SPEED_RECORD_SIZE);
          ticktime[i] = t;
        }

      // If no ticks for 1 second set speed to 0.
      if ((t - ticktime[i]) > 100)
        {
          for (int j = 0; j < SPEED_RECORD_SIZE; j++)
            {
              speedrecord[i][j] = 0;
            }
        }

      // Calculate the averaged speed
      speed[i] = 0;
      for (int j = 0; j < SPEED_RECORD_SIZE; j++)
        {
          speed[i] += speedrecord[i][j];
        }

      speed[i] = speed[i] / SPEED_RECORD_SIZE;
    }

  //   Serial.printf ("%d %d %d %d", total_ticks[0], total_ticks[1],
  //   total_ticks[2],
  //                  total_ticks[3]);
  // Serial.printf ("%2lf %2lf %2lf %2lf", speed[0], speed[1], speed[2],
  //               speed[3]);

  Serial.printf ("%2lf", (speed[0] + speed[1] + speed[2] + speed[3]) / 4);
  Serial.print ("\n");
}

void
pushArray (double val, double *array, int size)
{
  for (int i = 0; i < size - 1; i++)
    {
      array[i] = array[i + 1];
    }
  array[size - 1] = val;
}

void
printArray (double *array, int size)
{
  for (int i = 0; i < size; i++)
    {
      Serial.printf ("%2lf ", array[i]);
    }
  Serial.printf ("\n");
}

double
getSpeed (int encoder)
{
  return speed[encoder];
}

int
getTicks (int encoder)
{
  return total_ticks[encoder];
}

int
getTicksDiff (int start, int encoder)
{
  return start - total_ticks[encoder];
}
