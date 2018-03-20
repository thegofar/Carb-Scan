#include "LambdaSensor.h"
#include "stm32f103c8t6.h"
#include "mbed.h"

const static float vref = 3.3;

LambdaSensor :: LambdaSensor(PinName pin, Sensor sensor,  bool fake)
{
    mSensor = sensor;
    mFakeData = fake;
    mAI = pin;
    mSensorVolts=0.0;
}

void LambdaSensor::setFakeData(bool fake)
{
    mFakeData=fake;
}

void LambdaSensor :: acquire()
{
    /*****************************************
    * This should be called by all public 
    * functions to get the latest sensor value
    *****************************************/
    // read the analogue value from the mAI pin
    AnalogIn a(mAI);
    switch (mSensor)
    {
        case(SINGLE_WIRE):
        case(HEATED_NARROW_BAND):
        /* **************************
         * Narrowband Sensor Logic 
         * 0.1v = lambda 1.1
         * 0.45v = lambda 1
         * 0.8v ~ lambda 0.9
         * 0.9v ~ lambda 0.8
         ***************************/
        if(mFakeData)
        {
            if(mSensorVolts >=0.3)
            {
                mSensorVolts=0;
            }
            else{mSensorVolts=mSensorVolts+0.001;}
        }
        else
        { 
            mSensorVolts = a.read() * vref;
        }
        
        if (mSensorVolts > 0.8)
        {
            mMixture = RICH;
        }
        else if((mSensorVolts <= 0.8) && (mSensorVolts> 0.2))
        {
            mMixture = STOICH;
        }
        else if(mSensorVolts <=0.2)
        {
            mMixture = LEAN;
        }
        break;
        default:
            mMixture = HEATING;
            mSensorVolts = 2.0;
        break;           
    }
}

char LambdaSensor :: getFixedPtVolts()
{
    /*****************************************************************
    * The Android client code is expecting an unsigned 1 byte value
    * Raw Value: 0-255
    * Range:     0-1v
    * Offset:    0
    * Scaling:   0.003921
    ******************************************************************/
    acquire(); //gets the latest values from the sensor
    if(mSensorVolts > 1) {mSensorVolts=1;} // constrain to a maximum of 1 volt
    return mSensorVolts*0xFF; //Apply the fixed point scaling
}
void LambdaSensor :: setSensor(Sensor sensor)
{
    mSensor = sensor;
}

