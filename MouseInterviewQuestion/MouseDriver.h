#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

class MouseDriver
{
private:
    /* data */
    bool isDeltaX;
    bool isLeftButton;
    uint8_t b2;
    uint8_t b3;
    uint8_t b4;
    uint8_t b5;
    uint8_t b6;
    uint16_t dX;
    uint16_t dY;
    uint32_t timestamp;
    
public:
    MouseDriver();
    virtual ~MouseDriver();
    void handleReport(uint8_t * buffer, uint16_t length);
    bool isButtonLeft(uint8_t * buffer, bool isLeftButton);
    uint32_t timeStamp(uint8_t * buffer);
    uint16_t mouseDelta(uint8_t * buffer, bool isDeltaX);

};