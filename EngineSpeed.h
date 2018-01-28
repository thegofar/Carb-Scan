#ifndef ENGINE_SPEED_H
#define ENGINE_SPEED_H
#include "mbed.h"

class EngineSpeed
{
private: //class members
    PinName mPin;

protected: // allow derived classes to access this
    uint16_t mRevs;

private:
// private methods
    virtual void acquire();

public:
// public methods
    EngineSpeed(PinName);
    virtual uint16_t getEngineSpeed();
    PinName getPin();
};

class TimedPulse: public EngineSpeed
{
    /************************************************
     * Timed interrupt frequency to calculate engine speed
     * **********************************************/
    private: //class members
        InterruptIn mPrimaryCoil;
        Timer mSparkusec; // timer for spark events
        int usecT; //time between engine spark events
        bool mWastedSpark; //wasted spark fires the plug on exhaust stroke as well!
        bool mFakeData;
    
    private: //class methods
        void coilFired(); // interrupt routine
        void acquire();
    
    public: //public class methods
        TimedPulse(PinName);
        uint16_t getEngineSpeed();
        void setFakeData(bool);
};

class FrequencyToVoltage: public EngineSpeed
{
    /************************************************
     * LM2917N Frequency to voltage converter
     * **********************************************/
    private: //class members
        int mEngSpeedGain;// = 4087;
        int mEngSpeedOffset;// = 0;
        float mVoltMult;// = 3.3; //8173*3.3=26971
        AnalogIn mLmPin;
        bool mFakeData;

    private: //class methods
        void acquire();

    public: //class methods
        FrequencyToVoltage(PinName,float,float, float vmult=3.3);
        void configureVoltageCal(int,int,float);
        uint16_t getEngineSpeed();
        void setFakeData(bool);
};
#endif