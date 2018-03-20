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
#define DUMMY_BT_DATA 0

#define ONE_KHZ_IN_USEC 1000
#define HUNDRED_HZ_IN_USEC 10000
#define TEN_HZ_IN_USEC 100000
#define ONE_HZ_IN_USEC 1000000

#define RED_LINE_RPM 12500

void packData(char btData[]);
void setupCalibration(int msPeriod);
#endif