#include "stm32f103c8t6.h"
#include "CarbScan.h"
#include "mbed.h"
#include <string>

#define DEBUG 1

Ticker ms_tick;
Ticker hundredMs_tick;

bool sendOverBT=false;

unsigned int timeStamp = 0;
DigitalOut  onBoardLED(LED1);

//these values are currently acquired in the pack data routine!
LambdaSensor narrowLambda1(PA_0); 
TimedPulse timedPulse(PB_15);
FrequencyToVoltage lmChip(PB_1);
//PressureSensor mapSensor(PB_0);

Serial pc(PA_2, PA_3, 9600); //Out to FTDI
Serial bt(PA_9, PA_10, 9600); //Out to BT module

void configureBTModule(Serial &pc, Serial &bt)
{
    if(CAL_TACHO){setupCalibration((int) 5);}
    //DigitalOut  onBoardLED(LED1);
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
}
void hundredMilliSecTask(void)
{
/*********************************************************************
* This function is called every millisecond, keep all computation in 
* this IRQ context minimal to avoid blocking the user context 'main'
**********************************************************************/
    sendOverBT = true;
}

int main() 
{
    confSysClock();     //Configure system clock (72MHz HSE clock, 48MHz USB clock)    
    configureBTModule(pc,bt);
    onBoardLED=LED_OFF;

    /******************************************
     * Configure the various tasks
     * ***************************************/
    ms_tick.attach_us(milliSecTask,1000); //handles the timestamp for now
    hundredMs_tick.attach(hundredMilliSecTask,0.1);

    while(1) 
    {   
        if(sendOverBT)
        {
            sendOverBT=false;
            sendBTData(bt,pc);
            onBoardLED= !onBoardLED;
        }  
    }  
}

void packData(char btData[])
{
    uint16_t rpmTim;
    uint16_t rpmLm;
    uint16_t map;
    uint16_t mapI;
    uint32_t tS;
    char lam;
        
    if(DUMMY_BT_DATA)
    {
        rpmTim  = 0xf07f; //61567
        map = 0x0100; //256
        mapI = 0x007f; //127
        tS = 0xFEA15478; //4271985784  
        lam = 0x72; // perfect stoich voltage
        rpmLm=0xf07f; //61567
    }
    else
    {
        rpmTim = timedPulse.getEngineSpeed();
        //map = mapSensor.getAvgPressure();
        //mapI = mapSensor.getPressure();
        tS = timeStamp;
        lam = narrowLambda1.getFixedPtVolts();
        rpmLm=lmChip.getEngineSpeed();
    }
    
    btData[0] = 0x2; //<STX>
    
    btData[2] = (rpmTim >> 8) & 0xFF;
    btData[1] = (rpmTim)      & 0xFF;
    
    btData[4] = (map >> 8) & 0xFF;
    btData[3] = (map)      & 0xFF;
    
    btData[8] = (tS >> 24) & 0xFF;
    btData[7] = (tS >> 16) & 0xFF;
    btData[6] = (tS >>  8) & 0xFF;
    btData[5] = (tS)       & 0xFF;
    
    btData[10] = (mapI >> 8) & 0xFF;
    btData[9]  = (mapI)      & 0xFF;
    
    btData[11] = (lam)& 0xFF;

    btData[13] = (rpmLm >> 8) & 0xFF;
    btData[12] = (rpmLm)      & 0xFF;
    
    btData[19] = 0x3; //<ETX>
    btData[20] = 0xA; //(Line feed does not count as part of our alloted BLE bytes)
}


