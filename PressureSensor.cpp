#include "PressureSensor.h"
 
PressureSensor::PressureSensor(PinName pin, char samplecount) : mPressureAnalogue(pin)
{
    /* -- calibration points -- */
    mMapGain = 3297.6; //3146.7*(1027/980);
    mMapOffset = -21.836;
    mSampleCount=samplecount;
    mTick.attach_us(callback(this,&PressureSensor::autoAcquire),100);  //100 is 1khz
}

uint16_t PressureSensor::getPressure()
{
    acquire();
    rollAvg(); // do the averaging every time we get the pressure
    return mPressure;
}
uint16_t PressureSensor::getAvgPressure()
{
    //this will only be meaningful provided we have been calling 'getPressure' periodically
    return (uint16_t)mAvgPressure;
}


void PressureSensor::setMapGain(float gain){mMapGain = gain;}
void PressureSensor::setMapOffset(float off){mMapOffset=off;}
void PressureSensor::rqstFakeData(bool fake){mFakeData = fake;}
void PressureSensor::setSampleCount(char cnt){mSampleCount=cnt;}

void PressureSensor::autoAcquire()
{
    //this is called by the 1khz ticker
    mPressure = ((mPressureAnalogue.read() *  mMapGain) + mMapOffset); // mbar
}

void PressureSensor::acquire()
{
    int samples = 10;
    float raw=0;
    for(char i=0; i<samples; i++)
    {
        raw +=mPressureAnalogue.read();
    }
    raw=raw/samples;
    mPressure = ((raw *  mMapGain) + mMapOffset); // mbar
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
