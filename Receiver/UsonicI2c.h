#ifndef _UsonicI2C_H_
#define _UsonicI2C_H_

#include "Arduino.h"
#include "Wire.h"
#include "pins_arduino.h"

class UsomicI2C {
   private:
    uint8_t _addr;
    TwoWire* _wire;
    uint8_t _scl;
    uint8_t _sda;
    uint8_t _speed;
    unsigned long _time;
   public:
    void begin(TwoWire* wire = &Wire, uint8_t addr = 0x57, uint8_t sda = SDA,
               uint8_t scl = SCL, uint32_t speed = 200000L);
    void trigger();           
    float getDistance();
};
#endif // _UsonicI2C_