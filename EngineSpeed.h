#ifndef ENGINE_SPEED_H
#define ENGINE_SPEED_H
#include "mbed.h"

class TimedPulse
{
    /************************************************
     * Timed interrupt frequency to calculate engine speed
     * **********************************************/
    public:
        TimedPulse(PinName);
        uint16_t getEngineSpeed();
        void setFakeData(bool);

        // // via an optical coupler
        

    private:
        void coilFired(); // interrupt routine
        void acquire();

        InterruptIn mPrimaryCoil;
        Timer mSparkusec; // timer for spark events
        int usecT; //time between engine spark events
        uint16_t mRevs;
        bool mFakeData;
};

class FrequencyToVoltage
{
    /************************************************
     * LM2917N Frequency to voltage converter
     * **********************************************/
    public:
        FrequencyToVoltage(PinName);
        void setEngSpeedGain(float);
        void setEngSpeedOffset(float);
        void setVoltMult(float);
        uint16_t getEngineSpeed();
        void setFakeData(bool);

    private: 
        void acquire();
    
        float mEngSpeedGain;// = 4087;
        float mEngSpeedOffset;// = 0;
        float mVoltMult;// = 3.3; //8173*3.3=26971
        AnalogIn mLmPin;
        uint16_t mRevs;
        bool mFakeData;
};
#endif