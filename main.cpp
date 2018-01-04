#include "stm32f103c8t6.h"
#include "CarbScan.h"
#include "LambdaSensor.h"
#include "mbed.h"
#include <string>

Ticker ms_tick;
Ticker hundredMs_tick;
Ticker sec_tick;

bool getAnalogueRPM=false;
bool getInstMap=false;
bool sendOverBT=false;

char MAP_AVG_N_SAMPLES = 50; //TODO make this calibratable over BT
unsigned int timeStamp = 0;
DigitalOut  myled(LED1);

unsigned int instMap;
unsigned int rpmVal;
double mapVal=1000;
LambdaSensor narrowLambda1(PA_0); //this value is acquired in the pack data routine!
Serial pc(PA_2, PA_3, 9600); //Out to FTDI
Serial bt(PA_9, PA_10, 9600); //Out to BT module

void configureBTModule(Serial &pc, Serial &bt)
{
    if(CAL_TACHO){setupCalibration((int) 5);}
    //DigitalOut  myled(LED1);
    bt.baud(9600);
    /* configure the CC41-A bluetooth module*/
    //bt.printf("AT\r\n");
    //bt.printf("AT+NAMEGPZ Logger\r\n");
}
void sendBTData(Serial &bt, Serial &pc)
{
    char sendOverBt[21]; // we are limited to 20 bytes for a BLE transaction (21 when you include the \n)
    packData(sendOverBt); // pack the data efficiently into this array to be sent over BT
    for(int i = 0; i<20; i++)
    {
        bt.printf("%c" ,sendOverBt[i]);   
    }    
    if (bt.readable() && DEBUG_PC)
    {
        // put a byte out to the FTDI chip from a received byte from the BT module
        pc.putc(bt.getc());
    }        
}
void milliSecTask(void)
{
/*********************************************************************
* This function is called every millisecond, keep all computation in 
* this IRQ context minimal to avoid blocking the user context 'main'
**********************************************************************/
    timeStamp++;
    getInstMap=true;
}
void hundredMilliSecTask(void)
{
/*********************************************************************
* This function is called every millisecond, keep all computation in 
* this IRQ context minimal to avoid blocking the user context 'main'
**********************************************************************/
    getAnalogueRPM = true;
    sendOverBT = true;
}
void secTask(void)
{
    //put stuff in here to do every second
}

int main() 
{
    confSysClock();     //Configure system clock (72MHz HSE clock, 48MHz USB clock)    
    configureBTModule(pc,bt);
    myled=LED_OFF;

    /******************************************
     * Configure the various tasks
     * ***************************************/
    ms_tick.attach_us(milliSecTask,1000);
    hundredMs_tick.attach(hundredMilliSecTask,0.1);
    sec_tick.attach(secTask,1);

    while(1) 
    {   
        //wait((float)0.0005); // allow time to service interupts
        if(getAnalogueRPM)
        {
            rpmVal = getEngineSpeed(FAKE_DATA_ACQ);
            getAnalogueRPM = true; // set this flag false and wait for interrupt
        }
        if(getInstMap)
        {
            //this gets called every millisecond
            instMap=getManifoldPressure(FAKE_DATA_ACQ,10);
            rollAvg(mapVal,(double)instMap, MAP_AVG_N_SAMPLES);
            getInstMap=false; // set this flag back to false and wait for the interrupt
        }
        if(sendOverBT)
        {
            sendBTData(bt,pc);
            myled= !myled;
        }  
    }  
}

void packData(char btData[])
{
    uint16_t rpm;
    uint16_t map;
    uint16_t mapI;
    uint32_t tS;
    char lam;
        
    if(DUMMY_BT_DATA)
    {
        rpm  = 0xf07f; //61567
        map = 0x0100; //256
        mapI = 0x007f; //127
        tS = 0xFEA15478; //4271985784  
        lam = 0x72; // perfect stoich voltage
    }
    else
    {
        rpm = (uint16_t)rpmVal;
        map = (uint16_t)mapVal;
        mapI = (uint16_t)instMap;
        tS = timeStamp;
        lam = narrowLambda1.getFixedPtVolts();
    }
    
    btData[0] = 0x2; //<STX>
    
    btData[2] = (rpm >> 8) & 0xFF;
    btData[1] = (rpm)      & 0xFF;
    
    btData[4] = (map >> 8) & 0xFF;
    btData[3] = (map)      & 0xFF;
    
    btData[8] = (tS >> 24) & 0xFF;
    btData[7] = (tS >> 16) & 0xFF;
    btData[6] = (tS >>  8) & 0xFF;
    btData[5] = (tS)       & 0xFF;
    
    btData[10] = (mapI >> 8) & 0xFF;
    btData[9]  = (mapI)      & 0xFF;
    
    btData[11] = (lam)& 0xFF;
    
    btData[19] = 0x3; //<ETX>
    btData[20] = 0xA; //(Line feed does not count as part of our alloted BLE bytes)
}


