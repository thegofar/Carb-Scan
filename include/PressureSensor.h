#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H
#include "mbed.h"
#include "CarbScan.h"

class PressureSensor
{
public:
    PressureSensor(PinName,char avgSampleCnt=50);
    void initAutoAcquireIRQ();
    uint16_t getPressure();
    uint16_t getAvgPressure();
    void setMapGain(float gain);
    void setMapOffset(float offset);
    void rqstFakeData(bool fake);
    void setSampleCount(char avgSampleCnt);

protected:
    bool mFakeData;
    uint16_t mPressure;
    double mAvgPressure;
    char mSampleCount;
    float mMapGain;
    float mMapOffset;
    PinName mPinPressureAnalogue; //This is the interface to the hardware (analogue input)
    volatile int dummyAcquire;
    void rollAvg();
    Ticker mTick; //use this to repeatedly call the acquire routine
    void autoAcquire(void);
public:
    
    
};
#endif