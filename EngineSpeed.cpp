#include "EngineSpeed.h"
#include "mbed.h"

/***********************************************************
 * Base Class EngineSpeed
 * ********************************************************/
EngineSpeed::EngineSpeed(PinName pin)
{    
    mHardwarePin = pin;
}

void EngineSpeed::rqstFakeData(bool fake){mFakeData = fake;}

uint16_t EngineSpeed::getEngineSpeed()
{
    /*******************************************
     * This function returns engine speed in RPM
     * returns a U16 value or 1234RPM if you want
     * fake data!
     * ****************************************/
    if(!mFakeData){
        acquire();}
    else{
        mRevs = (uint16_t) 1234;}
    return mRevs;
}


/***********************************************************
 * Derived Class TimedInterrupt
 * ********************************************************/
TimedInterrupt::TimedInterrupt(PinName pin):EngineSpeed(pin)
{
     // assign the physical primary coil pin to the interrupt
    mPrimaryCoil(mHardwarePin);
    mPrimaryCoil.rise(&coilFired); // attach increment function of this counter instance
    mSparkusec.start(); //start the timer
}

void TimedInterrupt::acquire()
{
    //overrides the virtual acquire method set out in the base class
    if(usecT<1000000) //1,000,000usec = 1sec = 60RPM (wasted spark twin cylinder)
    {mRevs = 60000000/usecT; }
    else {mRevs=0;} // set the engine speed to 0 slower than 60rpm
    //TODO deal with timer wrap...
}
void TimedInterrupt::coilFired()
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
FrequencyToVoltage::FrequencyToVoltage(PinName pin):EngineSpeed(pin)
{
    // some initial values to get things going ;)
    engSpeedGain=4087;
    engSpeedOffset=0;
    voltMult=3.3;
    mLmPin(mHardwarePin);
}
void FrequencyToVoltage::setEngSpeedGain(float gain){mEngSpeedGain=gain;}
void FrequencyToVoltage::setEngSpeedOffset(float offset){mEngSpeedOffset=offset;}
void FrequencyToVoltage::setVoltMult(float vmult){mEngSpeedGain=vmult;}

void FrequencyToVoltage::acquire()
{
    float raw=0;
    for(int i =0; i<100; i++)
    {
        raw += lmPin.read();
    }
    raw=raw/100.0;
    if(raw<0.0037){raw=0.0;} //0.0037 = 50 RPM
    mRevs = (uint16_t)((raw * mVoltMult * mEngSpeedGain) + mEngSpeedOffset);
}