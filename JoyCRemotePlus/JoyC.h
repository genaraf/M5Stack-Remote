/*
 * @Author: Sorzn
 * @Date: 2019-11-22 14:48:31
 * @LastEditTime: 2019-11-22 15:22:45
 * @Description: M5Stack HAT JOYC project
 * @FilePath: /M5StickC/examples/Hat/JoyC/JoyC.h
 */

#ifndef _HAT_JOYC_H_
#define _HAT_JOYC_H_

#include "JoystickBase.h"

class JoyC: public JoystickBase {
    public:
        virtual void Init() {};
        virtual void Read() {};
        virtual void SetLedColor(uint32_t color);
        virtual uint8_t GetX(uint8_t pos);
        virtual uint8_t GetY(uint8_t pos);
        virtual uint8_t GetPress(uint8_t pos);
};

#endif
