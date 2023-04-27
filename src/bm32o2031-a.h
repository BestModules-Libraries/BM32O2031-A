/*************************************************
File:       		bm32o2031-a.h
Author:            	BESTMODULES
Description:        BM32O2031-A library for the light efficiency configuration
History：		    -
V1.0.0	 -- initial version；2022-11-23；Arduino IDE : v1.8.13

**************************************************/

#ifndef BM32O2031_A_h
#define BM32O2031_A_h

#include "Arduino.h"
#include "color.h"

#define   BM32O2031_A_BAUD  115200
#define   SUCCESS           0
#define   FAIL              1
typedef enum 
{
    CMD_RESET = 0x00,
    CMD_SYNCACTION = 0x02,
    CMD_GETFWVER = 0x03,
    CMD_SETEID = 0x80,

    FLAG_SYNCACTION = 0x20,

    LE_BICOLOR_BREATH = 0x08,
    LE_BICOLOR_OPEN,
    LE_BICOLOR_BLINK,
    LE_BICOLOR_ROTATE,
    LE_MULTICOLOUR_BREATH,
    LE_MULTICOLOUR_OPEN,
    LE_MULTICOLOUR_INC_DEC,
    LE_MULTICOLOUR_BLINK,
    LE_MULTICOLOUR_ROTATE,
    LE_MODIFY_MULTICOLOUR,
    LE_WRITE_DIY,
    LE_PAUSE,
    LE_OFF,
    LE_RESTART,
    LE_RESET,
}COMMAND_E;

typedef enum 
{
    CLOCKWISE = 0,
    ANTI_CLOCKWISE,
    MIDDLE,
}DIRECTION_E;

class BM32O2031_A
{
public:
    BM32O2031_A(HardwareSerial *theSerial = &Serial1);
    void begin(uint32_t baud = BM32O2031_A_BAUD);
    uint8_t reset(uint8_t theEid, bool radio = FALSE);
    void syncaction();
    uint16_t getFWVer(uint8_t theEid);
    uint8_t setEID(uint8_t theEid);
    uint8_t setBicolorBreath(uint8_t theEid, uint32_t color1, uint32_t color2, uint8_t brightness = 6, uint8_t time = 3, bool loop = TRUE, bool syncaction = FALSE);
    uint8_t setBicolorOpen(uint8_t theEid, uint32_t color1, uint32_t color2, DIRECTION_E direction, uint8_t site, uint8_t num, uint8_t brightness = 255, uint8_t time = 10, bool loop = TRUE, bool syncaction = FALSE);
    uint8_t setBicolorBlink(uint8_t theEid, uint32_t color1, uint32_t color2, uint8_t brightness = 255, uint8_t time = 10, bool loop = TRUE, bool syncaction = FALSE);
    uint8_t setBicolorRotate(uint8_t theEid, uint32_t color1, uint32_t color2, DIRECTION_E direction, uint8_t brightness = 255, uint8_t time = 10, bool loop = TRUE, bool syncaction = FALSE);
    uint8_t setMulticolourBreath(uint8_t theEid, uint8_t num, uint8_t brightness = 6, uint8_t time = 3, bool loop = TRUE, bool syncaction = FALSE);
    uint8_t setMulticolourOpen(uint8_t theEid, uint32_t backcolor, DIRECTION_E direction, uint8_t site, uint8_t num, uint8_t brightness = 255, uint8_t time = 10, bool loop = TRUE, bool syncaction = FALSE);
    uint8_t setMulticolourIncDec(uint8_t theEid, uint32_t backcolor, DIRECTION_E direction, uint8_t site, uint8_t num, bool clear = FALSE, uint8_t brightness = 255, uint8_t time = 10, bool syncaction = FALSE);
    uint8_t setMulticolourBlink(uint8_t theEid, uint8_t num, uint8_t brightness = 255, uint8_t time = 10, bool loop = TRUE, bool syncaction = FALSE);
    uint8_t setMulticolourRotate(uint8_t theEid, DIRECTION_E direction, uint8_t brightness = 255, uint8_t time = 10, bool loop = TRUE, bool syncaction = FALSE);
    uint8_t modifyMulticolourConfiguration(uint8_t theEid, uint32_t color[], uint8_t num);
    uint8_t writeDiyColor(uint8_t theEid, uint32_t color[], uint8_t site, uint8_t num, uint8_t brightness = 255, bool syncaction = FALSE);
    uint8_t pauseLightingEffect(uint8_t theEid, bool syncaction = FALSE);
    uint8_t offLightingEffect(uint8_t theEid, bool syncaction = FALSE);
    uint8_t restartLightingEffect(uint8_t theEid, bool syncaction = FALSE);
    uint8_t resetLightingEffect(uint8_t theEid, bool syncaction = FALSE);

private:
    uint8_t _mid;
    uint8_t _eid;

    HardwareSerial *_serial = NULL;
    void writeBytes(uint16_t writedata[], uint8_t len);
    uint8_t readBytes(uint8_t rxBuf[],uint8_t rlen,uint8_t waitTime = 5);
    void writeCommand(uint8_t cmd, bool radio);
    void setLightingEffect(uint8_t txBuf[], uint8_t len);
    
};

#endif
