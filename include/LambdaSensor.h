#ifndef LAMBDA_SENSOR_H
#define LAMBDA_SENSOR_H
#include "stm32f103c8t6.h"


class LambdaSensor {
    enum LamState {RICH, STOICH, LEAN, HEATING};
    enum Sensor {SINGLE_WIRE, LSU_4_9, HEATED_NARROW_BAND};
private:
    bool mFakeData; //used to simulate a signal of stoich
    PinName mAI; //This is the interface to the hardware (analogue input)
    float mSensorVolts;
    double mLambdaDbl;
    LamState mMixture;
    Sensor mSensor;
        
public:
    LambdaSensor(PinName pin, Sensor sensor=SINGLE_WIRE, bool fake=false);
    void setSensor(Sensor sensor);
    char getFixedPtVolts();
    void setFakeData(bool);
    
private:
    void acquire();
};

#endif