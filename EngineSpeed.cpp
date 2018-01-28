#include "EngineSpeed.h"
#include "mbed.h"

EngineSpeed::EngineSpeed(PinName pin)
{
    mPin = pin;
}
uint16_t EngineSpeed::getEngineSpeed()
{
    return mRevs; //this will be overriden by the derived class
}
PinName EngineSpeed::getPin()
{
    return mPin;
}

/***********************************************************
 * Derived Class TimedPulse
 * ***************************************************/
TimedPulse::TimedPulse(PinName pin):EngineSpeed(pin),mPrimaryCoil(pin)
{
     // assign the physical primary coil pin to the interrupt
    mPrimaryCoil.rise(callback(this, &TimedPulse::coilFired)); // attach increment function of this counter instance
    mSparkusec.start(); //start the timer
    mWastedSpark=false; //todo put this in NVM flash
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
    if (mSparkusec.read_us()>1000000)
    {
        //we haven't seen a rising edge for a while
        mRevs=0;// set the engine speed to 0 slower than 60rpm
    }
    else 
    {
        int numerator;
        if(mWastedSpark)
        {
            numerator=30000000; // we have to only consider half the pulses genuine firing events
        }
        else{
            numerator=60000000; // every pulse is a firing event
        }
        mRevs = numerator/usecT;
    } 
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
void TimedPulse::setWastedSpark(bool wasted)
{
    // set true for a wasted spark engine
    mWastedSpark = wasted;
}

/***********************************************************
 * Derived Class FrequencyToVoltage
 * ********************************************************/
FrequencyToVoltage::FrequencyToVoltage(PinName pin,float gain,float offset,float vmult):EngineSpeed(pin),mLmPin(pin)
{
    // some initial values to get things going ;)
    mEngSpeedGain=gain;
    mEngSpeedOffset=offset;
    mVoltMult=vmult;
}
//TODO implement support for android calibration of these vars
void FrequencyToVoltage::configureVoltageCal(int gain, int offset, float vmult)
{
    mEngSpeedGain=gain;
    mEngSpeedOffset=offset;
    mVoltMult=vmult;
}
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