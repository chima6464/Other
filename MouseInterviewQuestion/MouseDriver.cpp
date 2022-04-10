#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include "MouseDriver.h"
#include <iostream>
#include<string.h>
using namespace std; 

MouseDriver::MouseDriver(/* args */)
{
    isLeftButton = false;
    isDeltaX = false;
    b2 = 0;
    b3 = 0;
    b4 = 0;
    b5 = 0;
    b6 = 0;
    timestamp = 0;
}

MouseDriver::~MouseDriver()
{
}

uint16_t MouseDriver::mouseDelta(uint8_t * buffer, bool isDeltaX)
{
    b2 = *(buffer+2);
    b3 = *(buffer+3);
    b4 = *(buffer+4);
    uint16_t delta = 0;

    if (isDeltaX)
    {
        delta = (((b4 & 0xC) << 6) | b2);
    }
    else
    {
        delta = (((b4 & 0x30) << 4) | b3); 
    }

    return delta;  

}

bool MouseDriver::isButtonLeft(uint8_t * buffer, bool isLeftButton)
{
    b4 = *(buffer+4);
    bool rc = false;

    if (isLeftButton){rc = !!(b4 & 0x1);}
    else {rc = !!((b4 & 0x2) >> 1);}

    return rc;
}

uint32_t MouseDriver::timeStamp(uint8_t * buffer)
{
    b4 = *(buffer+4);
    b5 = *(buffer+5);
    b6 = *(buffer+6);
    uint32_t counter = 0;
    

    counter  = (b6 << 10) | (b5 << 2) | ((b4 & 0xC0) >> 6);

    if (counter >= 0x3FFFF || counter < timestamp){timestamp+=counter;}
    else {timestamp = counter;}

    return timestamp;
}

void MouseDriver::handleReport(uint8_t * buffer, uint16_t length)
{
    
    bool leftbutton = false;
    bool rightbutton = false;
    
    //for loop to handle below?
    uint64_t data = (uint64_t)buffer[7] << 56 | (uint64_t)buffer[6] << 48 | \
                    (uint64_t)buffer[5] << 40 | (uint64_t)buffer[4] << 32 | \
                    (uint64_t)buffer[3] << 24 | (uint64_t)buffer[2] << 16 | \
                    (uint64_t)buffer[1] << 8 | (uint64_t)buffer[0];
    
    buffer = (uint8_t*)(&data);
                        
    uint8_t *tmpbuff = (uint8_t*)malloc(length*sizeof(uint8_t)) ;
    memcpy(tmpbuff, buffer, length);

    dX = mouseDelta(tmpbuff, true);
    dY = mouseDelta(tmpbuff, false);

    leftbutton = isButtonLeft(tmpbuff, true);
    rightbutton = isButtonLeft(tmpbuff, false);

    timestamp = timeStamp(tmpbuff);

    std::cout << "Mouse deltas are: DX:" << dX << " DY " << dY <<
        " Button state is left :" << leftbutton << " Right " << rightbutton << " time(ms) " << timestamp
         <<std::endl;
}

/* driver test function */
int main() {
    uint64_t packet = 0xDEADBEEFAB1234;
    uint8_t *buffer = (uint8_t*)(&packet); 
    uint16_t length = 7;

    MouseDriver * md = new MouseDriver();
    //call handleReport to process incoming data
    md->handleReport(buffer, length);

    return 0;
}