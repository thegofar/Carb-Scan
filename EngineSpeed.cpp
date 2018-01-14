#include "EngineSpeed.h"
#include "mbed.h"

/***********************************************************
 * Derived Class TimedPulse
 * ***************************************************/
TimedPulse::TimedPulse(PinName tpPin):mPrimaryCoil(tpPin)
{
     // assign the physical primary coil pin to the interrupt
    mPrimaryCoil.rise(callback(this, &TimedPulse::coilFired)); // attach increment function of this counter instance
    mSparkusec.start(); //start the timer
}
void TimedPulse::setFakeData(bool fake){mFakeData=fake;}

uint16_t TimedPulse::getEngineSpeed()
{
    if(!mFakeData)
    {
        acquire();
        return mRevs;
    }
    else
    {
        return (uint16_t) 1234;
    }
}


void TimedPulse::acquire()
{
    //overrides the virtual acquire method set out in the base class
    if(usecT<1000000) //1,000,000usec = 1sec = 60RPM (wasted spark twin cylinder)
    {mRevs = 30000000/usecT; }
    else {mRevs=0;} // set the engine speed to 0 slower than 60rpm
    //TODO deal with timer wrap...
}
void TimedPulse::coilFired()
{
    // this is called on the rising edge of the primary coil firing
    mSparkusec.stop();
    usecT = mSparkusec.read_us(); //1,000,000 [uSec/rev]
    mSparkusec.reset();
    mSparkusec.start();
}

/***********************************************************
 * Derived Class FrequencyToVoltage
 * ********************************************************/
FrequencyToVoltage::FrequencyToVoltage(PinName pin):mLmPin(pin)
{
    // some initial values to get things going ;)
    mEngSpeedGain=4087;
    mEngSpeedOffset=0;
}
//TODO implement support for android calibration of these vars
void FrequencyToVoltage::setEngSpeedGain(float gain){mEngSpeedGain=gain;}
void FrequencyToVoltage::setEngSpeedOffset(float offset){mEngSpeedOffset=offset;}
void FrequencyToVoltage::setVoltMult(float vmult){mEngSpeedGain=vmult;}
void FrequencyToVoltage::setFakeData(bool fake){mFakeData=fake;}

void FrequencyToVoltage::acquire()
{
    float raw=0;
    for(int i =0; i<100; i++)
    {
        raw += mLmPin.read();
    }
    raw=raw/100.0;
    if(raw<0.0037){raw=0.0;} //0.0037 = 50 RPM
    mRevs = (uint16_t)((raw * mVoltMult * mEngSpeedGain) + mEngSpeedOffset);
}

uint16_t FrequencyToVoltage::getEngineSpeed()
{
    if(!mFakeData)
    {
        acquire();
        return mRevs;
    }
    else
    {
        return (uint16_t) 1234;
    }
}