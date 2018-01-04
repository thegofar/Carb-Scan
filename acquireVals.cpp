#include "carbScan.h"
#include "mbed.h"
//#include "PinNames.h"

AnalogIn engSpeedAI(PB_1);
AnalogIn mapSensor(PB_0);

/* -- calibration points -- */
const float engSpeedGain = 4087;
const float engSpeedOffset = 0;
const float voltMult = 3.3; //8173*3.3=26971

const float mapGain = 3297.6; //3146.7*(1027/980);
const float mapOffset = -21.836;

unsigned int getEngineSpeed(int fakeData)
{
    unsigned int engSpeedValue;
    if(fakeData)
    {
        engSpeedValue = 1200;
    }
    else
    {
        // we want to read from the analogue pin AD9:PB1
        float raw=0;
        for(int i =0; i<100; i++)
        {
            raw += engSpeedAI.read();
        }
        raw=raw/100.0;
        if(raw<0.0037){raw=0.0;} //0.0037 = 50 RPM
        engSpeedValue = (unsigned int)((raw * voltMult * engSpeedGain) + engSpeedOffset);
    }  
    return engSpeedValue;
}
             
unsigned int getManifoldPressure(int fakeData, int sampleCount)
{
    // raw*vref = mapSensor_AIN;
    float raw=0;
    for(int i=0; i<sampleCount; i++)
    {
        raw +=mapSensor.read();
    }
    raw=raw/(float)sampleCount;
    unsigned int manifoldPressure = ((raw *  mapGain) + mapOffset); // mbar
    return manifoldPressure;
}

double rollAvg(double &avg, double newSample, char N) 
{
    /*-----------------------------------------------------------
    TODO: This really needs to be replaced with a ring buffer
    avg as any large anomaly will persist forever
    ------------------------------------------------------------*/
    avg -= avg / N;
    avg += newSample / N;  

    return avg;
}