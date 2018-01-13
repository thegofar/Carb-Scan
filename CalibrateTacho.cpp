#include "mbed.h"
#include "CarbScan.h"

PwmOut calTachoPin(PB_4); //this pin is labelled B4 on LHS of the STM32

void setupCalibration(int msPeriod)
{
    // 2 ms Period = (1000/2) x 60 / 2 = 15000
    // 5 ms Period = (1000/5) x 60 / 2 = 6000
    // 10 ms Period = (1000/10) x 60 / 2 = 3000
    // 20ms Period = (1000/20) x 60 / 2 = 1500
    // 25 ms Period = (1000/25) x 60 / 2 = 1200
    
    float dc= 0.25;
    calTachoPin.period_ms(msPeriod);
    calTachoPin.write(dc);
}