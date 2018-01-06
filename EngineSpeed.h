#ifndef ENGINE_SPEED_H
#define ENGINE_SPEED_H
#include "mbed.h"

class EngineSpeed 
{
    protected:
        bool mFakeData;
        uint16_t mRevs; //Engine Speed [RPM]
        PinName mHardwarePin;

        virtual void acquire();

    public:
        //EngineSpeed(PinName coilPin=PB_15, RPMSensor sensor=STM32_INTERRUPT, PinName lmPin=PB_1, bool fake=false);
        EngineSpeed(PinName);
        void rqstFakeData(bool);
        uint16_t getEngineSpeed();
};

class TimedInterrupt
{
    /************************************************
     * Timed interrupt frequency to calculate engine speed
     * **********************************************/
    public:
        TimedInterrupt(PinName pin);
    protected:
        int usecT; //time between engine spark events
        InterruptIn mPrimaryCoil; // via an optical coupler
        Timer mSparkusec; // timer for spark events

    private:
        void coilFired(); // interrupt routine
};

class FrequencyToVoltage
{
    /************************************************
     * LM2917N Frequency to voltage converter
     * **********************************************/
    public:
        FrequencyToVoltage(PinName pin);
        void setEngSpeedGain(float);
        void setEngSpeedOffset(float);
        void setVoltMult(float);

    protected: 
        float mEngSpeedGain;// = 4087;
        float mEngSpeedOffset;// = 0;
        float mVoltMult;// = 3.3; //8173*3.3=26971
        AnalogIn mLmPin;
};
#endif