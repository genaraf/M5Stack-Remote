#ifndef __joystick_base_
#define __joystick_base_

#include "Arduino.h"

class JoystickBase
{
    public:
        /**
         * @description: Init joystick
         * @param:
         * @return: 
         */        
        virtual void Init() = 0;
        /**
         * @description: Read values
         * @param:
         * @return: 
         */        
        virtual void Read() = 0;
        /**
         * @description: 
         * @param color: (r << 16) | (g << 8) | b 
         * @return: 
         */        
        virtual void SetLedColor(uint32_t color) = 0;

        /**
         * @description: 
         * @param pos: 0: left, 1: right 
         * @return: x value: 0 ~ 200
         */        
        virtual uint8_t GetX(uint8_t pos);
        
        /**
         * @description: 
         * @param pos: 0: left, 1: right 
         * @return: y value: 0 ~ 200
         */       
        virtual uint8_t GetY(uint8_t pos);
        
        /**
         * @description: 
         * @param pos: 0: left, 1: right 
         * @return: 0 or 1
         */        
        virtual uint8_t GetPress(uint8_t pos);
};

#endif /* __joystick_base_ */
