 /*************************************************
File:       		bm32o2031-a.cpp
Author:            	BESTMODULES
Description:        BM32O2031-A library for the lighting effect configuration
History：		    -
V1.0.0	 -- initial version；2022-11-23；Arduino IDE : v1.8.13

**************************************************/

#include "bm32o2031-a.h"
#include "color.h"

/*************************************************
Description:        Initialize a member variable
Parameters:         *theSerial : point of Serial(default : Serial1)
Return:             -
Others:             -
*************************************************/
BM32O2031_A::BM32O2031_A(HardwareSerial *theSerial) 
{
    _serial = theSerial;
    _mid = 0x11;
    _eid = 0;
}

/*************************************************
Description:        Enable the Serial port of the Module
Parameters:         baud : Baud rate(default : 115200)
Return:             -
Others:             -
*************************************************/
void BM32O2031_A::begin(uint32_t baud)
{
   _serial->begin(baud, SERIAL_9N1);
   _serial->SetTxStatus(DISABLE);
   _serial->SetRxStatus(ENABLE);
}

/*************************************************
Description:        Reset module
Parameters:         theEid : module EID
                    radio : Radio control, TRUE or FALSE(defaul)
Return:             Communication status  0:Success 1:Fail   
Others:             -
*************************************************/
uint8_t BM32O2031_A::reset(uint8_t theEid, bool radio)
{
    uint8_t buf[10];

    _eid = theEid;
    writeCommand(CMD_RESET, radio);
    if(readBytes(buf,4)==SUCCESS)
     {
      if(radio) 
      {
          return SUCCESS;
      }
      else
      {
        if(buf[2] == 0x00)
          {
          return SUCCESS;
          }
       else 
       {
        return FAIL;
       }
      }
     }
     else 
     {
      return FAIL;
     } 
}

/*************************************************
Description:        Sync action module, using with delay command
Parameters:         -
Return:             -
Others:             -
*************************************************/
void BM32O2031_A::syncaction(void)
{
    writeCommand(CMD_SYNCACTION, 1);
}

/**********************************************************
Description:       Get the version information 
Parameters:        theEid : module EID
Return:            Version information  0:Fail  other:Version information(example: 0x0100, Verson:1.00)
Others:            -
**********************************************************/
uint16_t BM32O2031_A::getFWVer(uint8_t theEid)
{
    uint8_t buf[10];
    uint16_t version = 0;
    
    _eid = theEid;
    writeCommand(CMD_GETFWVER, 0);
    delay(5);
   if( readBytes(buf,6)==SUCCESS)
    { 
      if(buf[2] == 0x00)
      {
          version = (buf[4] << 8) | buf[3];
          return version;
      }
      else 
      {
        return 0; 
      }
    }
    else 
   {
    return 0;
   }
}

/*************************************************
Description:        Set up module eid
Parameters:         theEid : module eid
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::setEID(uint8_t theEid)
{
    uint8_t buf[10];
    
    _eid = theEid;
    writeCommand(CMD_SETEID + (theEid & 0x0f), 1);
    delay(5);
    if(readBytes(buf,4)== SUCCESS)
     { 
      if(buf[2] == 0x00)
      {
          return SUCCESS;
      }
      else 
      {
        return FAIL;
      }
     }
     else 
     {
      return FAIL;
     } 
}

/*************************************************
Description:        Two color breathing lighting effect
Parameters:         theEid : module eid
                    color1 : 0~0x00ffffff 
                    color2 : 0~0x00ffffff
                    brightness : 1~8(default:6)
                    time : breathing cycle, 1~255(*1s), (default:3)
                    loop :  loop control, TRUE(default) or FALSE
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail   
Others:             -
*************************************************/
uint8_t BM32O2031_A::setBicolorBreath(uint8_t theEid, uint32_t color1, uint32_t color2, uint8_t brightness, uint8_t time, bool loop, bool syncaction)
{
    uint8_t buf[10];

    buf[0] = LE_BICOLOR_BREATH;
    if(syncaction) 
    {
        buf[0] += FLAG_SYNCACTION;
    }
    if(!brightness) 
    {
        buf[1] = 1;
    }
    else if(brightness>8) 
    {
        buf[1] = 8;
    }
    else 
    {
        buf[1] = brightness;
    }
    if(!time) 
    {
        buf[2] = 1;
    }
    else 
    {
        buf[2] = time;
    }
    buf[3] = color1 >> 16;
    buf[4] = color1 >> 8;
    buf[5] = color1;
    buf[6] = color2 >> 16;
    buf[7] = color2 >> 8;
    buf[8] = color2;
    buf[9] = !loop;
    _eid = theEid;
    setLightingEffect(buf, 10);
    delay(5);
    if(readBytes(buf,4)==SUCCESS)
     { 
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
     }
    else
    {
      return FAIL; 
    }     

}

/*************************************************
Description:        Two color open lighting effect
Parameters:         theEid : module eid
                    color1 : 0~0x00ffffff 
                    color2 : 0~0x00ffffff
                    direction : CLOCKWISE, ANTI_CLOCKWISE and MIDDLE(spread out from the middle to both sides)
                    site : starting position of LED(0~15)
                    num : open LED quantity(0~16); if(direction == MIDDLE), num=(0~9)
                    brightness : 10~255(default:255)
                    time : time to open a LED, 1~255(*5ms),(default:10)
                    loop :  loop control, TRUE(default) or FALSE
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::setBicolorOpen(uint8_t theEid, uint32_t color1, uint32_t color2, DIRECTION_E direction, uint8_t site, uint8_t num, uint8_t brightness, uint8_t time, bool loop, bool syncaction)
{
    uint8_t buf[13];

    buf[0] = LE_BICOLOR_OPEN;
    if(syncaction) 
    {
        buf[0] += FLAG_SYNCACTION;
    }
    if(brightness < 10) 
    {
        buf[1] = 10;
    }
    else 
    {
        buf[1] = brightness;
    }
    if(!time) 
    {
        buf[2] = 1;
    }
    else 
    {
        buf[2] = time;
    }
    buf[3] = color1 >> 16;
    buf[4] = color1 >> 8;
    buf[5] = color1;
    buf[6] = color2 >> 16;
    buf[7] = color2 >> 8;
    buf[8] = color2;
    buf[9] = !loop;
    if(direction > MIDDLE) 
    {
        buf[10] = MIDDLE;
    }
    else
    {
        buf[10] = direction;
    }
    buf[11]=site & 0x0f;
    if(buf[10] < MIDDLE)
    {
        if(num > 16) 
        {
            buf[12] = 16;
        }
        else 
        {
            buf[12] = num;
        }
    }
    else
    {
        if(num > 9) 
        {
            buf[12] = 9;
        }
        else 
        {
            buf[12] = num;
        }
    }
    _eid = theEid;
    setLightingEffect(buf, 13);
    delay(5);
    
    if(readBytes(buf,4)==SUCCESS)
    {
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
    }
    else
    {
      return FAIL; 
    }    
}

/*************************************************
Description:        Two color blink lighting effect
Parameters:         theEid : module eid
                    color1 : 0~0x00ffffff 
                    color2 : 0~0x00ffffff
                    brightness : 10~255(default:255)
                    time : blink cycle, 1~255(*0.1s),(default:10)
                    loop :  loop control, TRUE(default) or FALSE
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::setBicolorBlink(uint8_t theEid, uint32_t color1, uint32_t color2, uint8_t brightness, uint8_t time, bool loop, bool syncaction)
{
    uint8_t buf[10];
    buf[0] = LE_BICOLOR_BLINK;
    if(syncaction) 
    {
        buf[0] += FLAG_SYNCACTION;
    }
    if(brightness < 10) 
    {
        buf[1] = 10;
    }
    else 
    {
        buf[1] = brightness;
    }
    if(!time) 
    {
        buf[2] = 1;
    }
    else 
    {
        buf[2] = time;
    }
    buf[3] = color1 >> 16;
    buf[4] = color1 >> 8;
    buf[5] = color1;
    buf[6] = color2 >> 16;
    buf[7] = color2 >> 8;
    buf[8] = color2;
    buf[9] = !loop;
    _eid = theEid;
    setLightingEffect(buf, 10);
    delay(5);
   if( readBytes(buf,4)==SUCCESS)
   { 
     if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
   }
   else
  {
    return FAIL; 
  }
}

/*************************************************
Description:        Two color rotate lighting effect
Parameters:         theEid : module eid
                    color1 : 0~0x00ffffff 
                    color2 : 0~0x00ffffff
                    direction :  CLOCKWISE or ANTI_CLOCKWISE
                    brightness : 10~255(default:255)
                    time : rotate cycle, 1~255(*0.1s),(default:10)
                    loop :  loop control, TRUE(default) or FALSE
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::setBicolorRotate(uint8_t theEid, uint32_t color1, uint32_t color2, DIRECTION_E direction, uint8_t brightness, uint8_t time, bool loop, bool syncaction)
{
    uint8_t buf[11];

    buf[0] = LE_BICOLOR_ROTATE;
    if(syncaction) 
    {
        buf[0] += FLAG_SYNCACTION;
    }
    if(brightness < 10) 
    {
        buf[1] = 10;
    }
    else 
    {
        buf[1] = brightness;
    }
    if(!time) 
    {
        buf[2] = 1;
    }
    else 
    {
        buf[2] = time;
    }
    buf[3] = color1 >> 16;
    buf[4] = color1 >> 8;
    buf[5] = color1;
    buf[6] = color2 >> 16;
    buf[7] = color2 >> 8;
    buf[8] = color2;
    buf[9] = !loop;
    if(direction > ANTI_CLOCKWISE)
    {
        buf[10] = ANTI_CLOCKWISE;
    }
    else
    {
        buf[10] = direction;
    }
    _eid = theEid;
    setLightingEffect(buf, 11);
    delay(5);
    if( readBytes(buf,4)== SUCCESS)
    {
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
    }
    else
    {
        return FAIL; 
    }
}

/*************************************************
Description:        Colour breathing lighting effect
Parameters:         theEid : module eid
                    num : number of colors(1~16)
                    brightness : 1~8(default:6)
                    time : breathing cycle, 1~255(*1s),(default:3)
                    loop :  loop control, TRUE(default) or FALSE
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::setMulticolourBreath(uint8_t theEid, uint8_t num, uint8_t brightness, uint8_t time, bool loop, bool syncaction)
{
    uint8_t buf[5];

    buf[0] = LE_MULTICOLOUR_BREATH;
    if(syncaction) 
    {
        buf[0] += FLAG_SYNCACTION;
    }
    if(!brightness) 
    {
        buf[1] = 1;
    }
    else if(brightness > 8) 
    {
        buf[1] = 8;
    }
    else buf[1] = brightness;
    if(!time) 
    {
        buf[2] = 1;
    }
    else 
    {
        buf[2] = time;
    }
    buf[3] = !loop;
    if(!num) 
    {
        buf[4] = 1;
    }
    else if(num>16) 
    {
        buf[4] = 16;
    }
    else 
    {
        buf[4] = num;
    }
    _eid = theEid;
    setLightingEffect(buf, 5);
    delay(5);
    
    if(readBytes(buf,4)==SUCCESS)
    {
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
    }
    else
    {
      return FAIL;
    }
}

/*************************************************
Description:        Colour open lighting effect
Parameters:         theEid : module eid
                    backcolor : 0~0x00ffffff
                    direction : CLOCKWISE, ANTI_CLOCKWISE and MIDDLE(spread out from the middle to both sides)
                    site : starting position of LED(0~15)
                    num : open LED quantity(0~16); if(direction == MIDDLE), num=(0~9)
                    brightness : 10~255(default:255)
                    time : time to open a LED, 1~255(*5ms),(default:10)
                    loop :  loop control, TRUE(default) or FALSE
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::setMulticolourOpen(uint8_t theEid, uint32_t backcolor, DIRECTION_E direction, uint8_t site, uint8_t num, uint8_t brightness, uint8_t time, bool loop, bool syncaction)
{
    uint8_t buf[10];

    buf[0] = LE_MULTICOLOUR_OPEN;
    if(syncaction) 
    {
        buf[0] += FLAG_SYNCACTION;
    }
    if(brightness < 10) 
    {
        buf[1] = 10;
    }
    else 
    {
        buf[1] = brightness;
    }
    if(!time) 
    {
        buf[2] = 1;
    }
    else 
    {
        buf[2] = time;
    }
    buf[3] = !loop;
    buf[4] = backcolor >> 16;
    buf[5] = backcolor >> 8;
    buf[6] = backcolor;
    if(direction > MIDDLE) 
    {
        buf[7] = MIDDLE;
    }
    else 
    {
        buf[7] = direction;
    }
    buf[8] = site & 0x0f;
    if(buf[7] < MIDDLE)
    {
        if(num > 16) 
        {
            buf[9] = 16;
        }
        else 
        {
            buf[9] = num;
        }
    }
    else
    {
        if(num > 9) 
        {
            buf[9] = 9;
        }
        else 
        {
            buf[9] = num;
        }
    }
    _eid = theEid;
    setLightingEffect(buf, 10);
    delay(5);
    if(readBytes(buf,4)==SUCCESS)
    {
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
    }
    else
     {
        return FAIL; 
     }
}

/*************************************************
Description:        Colour increase and decrease lighting effect
Parameters:         theEid : module eid
                    backcolor : 0~0x00ffffff
                    direction : CLOCKWISE or ANTI_CLOCKWISE
                    site : starting position of LED(0~15)
                    num : open LED quantity(0~16)
                    clear : Clear previous lighting effect status,the first use command should be cleared. TRUE or FALSE(default)
                    brightness : 10~255(default:255)
                    time : time to open a LED, 1~255(*5ms),(default:10)
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::setMulticolourIncDec(uint8_t theEid, uint32_t backcolor, DIRECTION_E direction, uint8_t site, uint8_t num, bool clear, uint8_t brightness, uint8_t time, bool syncaction)
{
    uint8_t buf[10];

    buf[0] = LE_MULTICOLOUR_INC_DEC;
    if(syncaction) 
    {
        buf[0] += FLAG_SYNCACTION;
    }
    if(brightness < 10) 
    {
        buf[1] = 10;
    }
    else 
    {
        buf[1] = brightness;
    }
    if(!time) 
    {
        buf[2] = 1;
    }
    else 
    {
        buf[2] = time;
    }
    buf[3] = clear;
    buf[4] = backcolor >> 16;
    buf[5] = backcolor >> 8;
    buf[6] = backcolor;
    if(direction > ANTI_CLOCKWISE)
    {
        buf[7] = ANTI_CLOCKWISE;
    }
    else
    {
         buf[7] = direction;
    }
    buf[8] = site & 0x0f;
    if(num > 16) 
    {
        buf[9] = 16;
    }
    else 
    {
        buf[9] = num;
    }
    _eid = theEid;
    setLightingEffect(buf, 10);
    delay(5);
    if(readBytes(buf,4)==SUCCESS)
    {
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
    }
    else 
    {
        return FAIL; 
    } 
}

/*************************************************
Description:        Colour blink lighting effect
Parameters:         theEid : module eid
                    num :  number of colors(1~16)
                    brightness : 10~255(default:255)
                    time : blink cycle, 1~255(*0.1s),(default:10)
                    loop :  loop control, TRUE(default) or FALSE
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::setMulticolourBlink(uint8_t theEid, uint8_t num, uint8_t brightness, uint8_t time, bool loop, bool syncaction)
{
    uint8_t buf[5];

    buf[0] = LE_MULTICOLOUR_BLINK;
    if(syncaction) 
    {
        buf[0] += FLAG_SYNCACTION;
    }
    if(brightness < 10) 
    {
        buf[1] = 10;
    }
    else 
    {
        buf[1] = brightness;
    }
    if(!time) 
    {
        buf[2] = 1;
    }
    else 
    {
        buf[2] = time;
    }
    buf[3] = !loop;
    if(!num) 
    {
        buf[4] = 1;
    }
    else if(num > 16) 
    {
        buf[4] = 16;
    }
    else 
    {
        buf[4] = num;
    }
    _eid = theEid;
    setLightingEffect(buf, 5);
    delay(5);
    if(readBytes(buf,4)==SUCCESS)
    {
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
    }
    else 
   {
      return FAIL; 
   } 
}

/*************************************************
Description:        Colour rotate lighting effect
Parameters:         theEid : module eid
                    direction : CLOCKWISE or ANTI_CLOCKWISE
                    brightness : 10~255(default:255)
                    time : rotate cycle, 1~255(*0.1s),(default:10)        
                    loop :  loop control, TRUE(default) or FALSE
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::setMulticolourRotate(uint8_t theEid, DIRECTION_E direction, uint8_t brightness, uint8_t time, bool loop, bool syncaction)
{
    uint8_t buf[5];

    buf[0] = LE_MULTICOLOUR_ROTATE;
    if(syncaction) 
    {
        buf[0] += FLAG_SYNCACTION;
    }
    if(brightness < 10) 
    {
        buf[1] = 10;
    }
    else 
    {
        buf[1] = brightness;
    }
    if(!time) 
    {
        buf[2] = 1;
    }
    else 
    {
        buf[2] = time;
    }
    buf[3] = !loop;
    if(direction> ANTI_CLOCKWISE)
    {
        buf[4] = ANTI_CLOCKWISE;
    }
    else
    {
        buf[4] = direction;
    }
    _eid = theEid;
    setLightingEffect(buf, 5);
    delay(5);
    if(readBytes(buf,4)==SUCCESS)
    {
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
    }
    else
    {
      return FAIL; 
    }
}

/*************************************************
Description:        Modify the built-in color configuration
Parameters:         theEid : module eid
                    color[] : color data buffer
                    num : number of colors(1~16)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::modifyMulticolourConfiguration(uint8_t theEid, uint32_t color[], uint8_t num)
{
    uint8_t i,len,buf[49],ret;

    setBicolorBreath(theEid, 0, 0, 0, 3, FALSE);
    delay(3);

    buf[0] = LE_MODIFY_MULTICOLOUR;
    if(!num) 
    {
        num = 1;
    }
    else if(num > 16) 
    {
        num = 16;
    }
    for(i = 0; i < num; i++)
    {
        buf[i*3+1] = color[i] >> 16;
        buf[i*3+1+1] = color[i] >> 8;
        buf[i*3+1+2] = color[i];
    }
    len = num * 3 + 1;
    _eid = theEid;
    setLightingEffect(buf, len);
    delay(5);
    if(readBytes(buf,4)==SUCCESS)
    {
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
    }
    else
    {
        return FAIL; 
    }

}

/*************************************************
Description:        Write DIY lighting effect
Parameters:         theEid : module eid
                    color[] : color data buffer
                    site : starting position of LED(0~15)
                    num : number of colors(1~16)
                    brightness : 10~255(default:255)
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::writeDiyColor(uint8_t theEid, uint32_t color[], uint8_t site, uint8_t num, uint8_t brightness, bool syncaction)
{
    uint8_t i,len,buf[52];

    buf[0] = LE_WRITE_DIY;
    if(syncaction) 
    {
        buf[0] += FLAG_SYNCACTION;
    }
    if(!num) 
    {
        num = 1;
    }
    else if(num > 16) 
    {
        num = 16;
    }
    buf[1] = num;
    if(brightness < 10) 
    {
        buf[2] = 10;
    }
    else 
    {
        buf[2] = brightness;
    }
    buf[3] = site & 0x0f;
        for(i = 0; i < num; i++)
    {
        buf[i*3+4] = color[i] >> 16;
        buf[i*3+4+1] = color[i] >> 8;
        buf[i*3+4+2] = color[i];
    }
    len = num * 3 + 4;
    _eid = theEid;
    setLightingEffect(buf, len);
    delay(5);
    if(readBytes(buf,4)==SUCCESS)
    {
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
    }
    else
    {
        return FAIL; 
    }
}

/*************************************************
Description:        lighting effect pause
Parameters:         theEid : module eid
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::pauseLightingEffect(uint8_t theEid, bool syncaction)
{
    uint8_t buf[10];

    _eid = theEid;
    if(syncaction) 
    {
        writeCommand(FLAG_SYNCACTION + LE_PAUSE, 0);
    }
    else 
    {
        writeCommand(LE_PAUSE, 0);
    }
    delay(5);
   if(readBytes(buf,4)==SUCCESS)
  {
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
  }
  else
  {
    return FAIL; 
  }
}

/*************************************************
Description:        lighting effect close
Parameters:         theEid : module eid
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::offLightingEffect(uint8_t theEid, bool syncaction)
{
    uint8_t buf[10];

    _eid = theEid;
    if(syncaction) 
    {
        writeCommand(FLAG_SYNCACTION + LE_OFF, 0);
    }
    else 
    {
        writeCommand(LE_OFF, 0);
    }
    delay(5);
   if(readBytes(buf,4)==SUCCESS)
   {
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
   }
   else
  {
    return FAIL; 
  }
}

/*************************************************
Description:        lighting effect restart
Parameters:         theEid : module eid
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::restartLightingEffect(uint8_t theEid, bool syncaction)
{
    uint8_t buf[10];

    _eid = theEid;
    if(syncaction) 
    {
        writeCommand(FLAG_SYNCACTION + LE_RESTART, 0);
    }
    else 
    {
        writeCommand(LE_RESTART, 0);
    }
    delay(5);
    if(readBytes(buf,4)==SUCCESS)
    {
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
    }
    else
    {
      return FAIL; 
    }
}

/*************************************************
Description:        lighting effect reset
Parameters:         theEid : module eid
                    syncaction : delayed operation control, TRUE or FALSE(default)
Return:             Communication status  0:Success 1:Fail  
Others:             -
*************************************************/
uint8_t BM32O2031_A::resetLightingEffect(uint8_t theEid, bool syncaction)
{
    uint8_t buf[10];

    _eid = theEid;
    if(syncaction) 
    {
        writeCommand(FLAG_SYNCACTION + LE_RESET, 0);
    }
    else 
    {
        writeCommand(LE_RESET, 0);
    }
    delay(5);
    if(readBytes(buf,4)==SUCCESS)
    {
      if(buf[2]==0x00)
      {
        return SUCCESS;   
      }
      else
      {
        return FAIL; 
      }
    }
    else
    {
      return FAIL; 
    }
}

/*************************************************
Description:        Sending system commands
Parameters:         cmd : command
                    radio : Radio control
Return:             -
Others:             -
*************************************************/
void BM32O2031_A::writeCommand(uint8_t cmd, bool radio)
{
    uint16_t mid;
    uint8_t Tlen_eid, instr, check_sum;
    uint16_t senddata[255];

    if(radio) 
    {
        mid = 0x100;
        Tlen_eid = 2<<4;
    }
    else 
    {
        mid = 0x100 + _mid;
        Tlen_eid = (2<<4) + (_eid & 0x0f);
    }
    instr = cmd;
    check_sum = ~(mid + Tlen_eid + instr);
    senddata[0]=mid;
    senddata[1]=Tlen_eid;
    senddata[2]=instr;
    senddata[3]=check_sum;
    writeBytes(senddata,4);
}
/*************************************************
Description:        writeBytes
Parameters:         writedata : Data to be written.
                    len : Data length.
Return:             -
Others:             -
*************************************************/
void BM32O2031_A::writeBytes(uint16_t writedata[], uint8_t len)
{
    while(_serial->available()>0)
    {
      _serial->read();
    }
    _serial->SetRxStatus(DISABLE);
    _serial->SetTxStatus(ENABLE);
    for(uint8_t i =0;i<len; i++)
    {
      _serial->write(writedata[i]); 
    }
    _serial->flush();
    _serial->SetTxStatus(DISABLE);
    _serial->SetRxStatus(ENABLE);
}
/*************************************************
Description:        Configuration of lighting effect
Parameters:         txBuf[] : buffer of lighting effect command 
                    len : Length of lighting effect command
Return:             -
Others:             -
*************************************************/
void BM32O2031_A::setLightingEffect(uint8_t txBuf[], uint8_t len)
{
    uint16_t mid;
    uint8_t i,y =0, Tlen_eid, TLen1, check_sum;
    uint16_t senddata[255];

    mid = 0x100 + _mid;
    if(len > 14)
    {
        Tlen_eid = (_eid & 0x0f);
        TLen1 = len + 1;
    }
    else
    {
        Tlen_eid = ((len + 1)<<4) + (_eid & 0x0f);
        TLen1 = 0;
    }
    check_sum = mid + Tlen_eid + TLen1;
     senddata[y]=mid;
     y++;
     senddata[y]=Tlen_eid;
     y++;
    if(TLen1) 
    {
        senddata[y]=TLen1;
        y++;
    }
    for(i = 0; i < len; i++)
    {
        senddata[y]=txBuf[i];
        y++;
        check_sum += txBuf[i];
    }
    senddata[y]=(uint8_t)~(check_sum);
    writeBytes(senddata,y+1);
}

/**********************************************************
Description:       readBytes.
Parameters:         rxBuf[] : Store data Param 0~N.
                    rlen：Read data length.
                    waitTime:Set timeout.
Output:             0:success 1:fail 
Others:             -
**********************************************************/
 uint8_t BM32O2031_A:: readBytes(uint8_t rBuf[],uint8_t rlen,uint8_t waitTime)
 {
    uint8_t i ,delayCnt =0;
    uint8_t check = 0;
  
    for (i = 0; i < rlen; i++)
    {
      while (_serial->available() == 0)
      {
        delay(1);
        delayCnt++;
        if (delayCnt > waitTime)
        {
          return FAIL; // Timeout error
        }
      }
     rBuf[i] = _serial->read(); 
    }

    /* checkSum  */
  for(i = 0; i < rlen - 1; i++)
    {
        check += rBuf[i];
    }
  if((check) == (uint8_t)(~rBuf[rlen-1]))
   {
     if(((rBuf[0] & 0xff) == _mid)&&((rBuf[1] & 0x0f) == _eid))
      { 
        return SUCCESS;
       }
      else
      {
         return FAIL; 
      }
    }
    else
    {
        return FAIL;
    }
 }
