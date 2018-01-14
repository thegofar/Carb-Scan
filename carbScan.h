#ifndef CARB_SCAN
#define CARB_SCAN

#define LED_ON 0;
#define LED_OFF 1;

#include "EngineSpeed.h"
#include "LambdaSensor.h"
#include "PressureSensor.h"

/*******************************************************************************
 * Carbscan build defs
 * ****************************************************************************/
#define FAKE_DATA_ACQ 0 //set to 1 to generate fake data
#define CAL_TACHO 1 // set to 1 to enable pwm on the pin described in cal tacho
#define DEBUG_PC 0 // set to 1 if you want serial data on the FTDI USB interface also
#define DUMMY_BT_DATA 1

void packData(char btData[]);
void setupCalibration(int msPeriod);
#endif