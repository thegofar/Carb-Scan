#include "PressureSensor.h"
 
PressureSensor::PressureSensor(PinName p, char samplecount) 
{
    /* -- calibration points -- */
    mPinPressureAnalogue=p;
    mMapGain = 3297.6; //3146.7*(1027/980);
    mMapOffset = -21.836;
    mSampleCount=samplecount;
    //initAutoAcquireIRQ(); //do not do this, call this after construction is complete (from main)
}

void PressureSensor::initAutoAcquireIRQ()
{
    mTick.attach_us(Callback<void()>(this,&PressureSensor::autoAcquire),ONE_KHZ_IN_USEC);
}

uint16_t PressureSensor::getPressure()
{
    //acquisition is handled by the IRQ timer mTick
    return mPressure;
}
uint16_t PressureSensor::getAvgPressure()
{
    // the roll avg routine is auto called by the IRQ timer mTick
    return (uint16_t)mAvgPressure;
}

void PressureSensor::setMapGain(float gain){mMapGain = gain;}
void PressureSensor::setMapOffset(float off){mMapOffset=off;}
void PressureSensor::rqstFakeData(bool fake){mFakeData = fake;}
void PressureSensor::setSampleCount(char cnt){mSampleCount=cnt;}

void PressureSensor::autoAcquire()
{
    //dummyAcquire++;
    //this is called by the 1khz ticker
    AnalogIn aIn(mPinPressureAnalogue);
    mPressure = ((aIn.read() *  mMapGain) + mMapOffset); // mbar
    rollAvg();
}

void PressureSensor::rollAvg() 
{
    /*-----------------------------------------------------------
    TODO: This really needs to be replaced with a ring buffer
    avg as any large anomaly will persist forever
    ------------------------------------------------------------*/
    mAvgPressure -= mAvgPressure /mSampleCount;
    mAvgPressure += mPressure/ mSampleCount;  
}