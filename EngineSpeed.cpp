#include "EngineSpeed.h"
#include "mbed.h"

EngineSpeed :: EngineSpeed(PinName coilPin, PinName lmPin, RPMSensor sensor, bool fake)
{
    // Engine Speed constructor
    mSensor = sensor;
    mFakeData = fake;
    
    // assign the physical primary coil pin to the interrupt
    mPrimaryCoil(coilPin);
    mPrimaryCoil.rise(&coilFired); // attach increment function of this counter instance
}

void EngineSpeed :: acquire()
{
    switch(mSensor)
    {
        case(LM2917N):
        // read analogue volts
            mRevs =(uint16_t) 1800; //TODO fix this to work with the LM2917N interface
            break;
        case(STM32_INTERRUPT):
        //todo put some logic here
            break;
            default:
            break;
    }
}
void coilFired()
{
    // this is called on the rising edge of the primary coil firing
    
}

uint16_t EngineSpeed :: getEngineSpeed()
{
    /*******************************************
     * This function returns engine speed in RPM
     * returns a U16 value or 1234RPM if you want
     * fake data!
     * ****************************************/
    if(!mFakeData)
    {
        acquire();
    }
    else
    {
        mRevs = (uint16_t) 1234;
    }
    
    return mRevs;
}