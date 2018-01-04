#ifndef ENGINE_SPEED_H
#define ENGINE_SPEED_H

class EngineSpeed {
    enum RPMSensor {LM2917N, STM32_INTERRUPT};

private:
    bool mFakeData;
    uint16_t mRevs;
    RPMSensor mSensor;
    InterruptIn mPrimaryCoil; // via an optical coupler

    void acquire();
    void coilFired();
public:
    EngineSpeed(PinName coilPin=PB_15, PinName lmPin= RPMSensor sensor=STM32_INTERRUPT, bool fake=false)
    uint16_t getEngineSpeed();
};
#endif