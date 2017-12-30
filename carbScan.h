#ifndef CARB_SCAN
#define CARB_SCAN

#define LED_ON 0;
#define LED_OFF 1;

/*******************************************************************************
 * Carbscan build defs
 * ****************************************************************************/
#define FAKE_DATA_ACQ 0 //set to 1 to generate fake data
#define CAL_TACHO 1 // set to 1 to enable pwm on the pin described in cal tacho
#define DEBUG_PC 0 // set to 1 if you want serial data on the FTDI USB interface also
#define WIDE_BAND_OXY 0 //set to 0 when using narrowband lambda
#define DUMMY_BT_DATA 0


void packData(char btData[]);
unsigned int getEngineSpeed(int fakeData);
unsigned int getManifoldPressure(int fakeData,int sampleCount);
void setupCalibration(int msPeriod);
double rollAvg(double &avg, double newSample, unsigned int N);

#endif