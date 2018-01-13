#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H
#include "mbed.h"

class PressureSensor
{
public:
    PressureSensor(PinName p,char avgSampleCnt=50);
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
    AnalogIn mPressureAnalogue;
    Ticker mTick; //use this to repeatedly call the acquire routine

    void acquire();
    void autoAcquire();
    void rollAvg();
};
#endif