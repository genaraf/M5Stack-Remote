#include "UsonicI2c.h"

/*! @brief Initialize the Sonic. */
void UsomicI2C::begin(TwoWire* wire, uint8_t addr, uint8_t sda, uint8_t scl,
                      uint32_t speed) {
    _wire  = wire;
    _addr  = addr;
    _sda   = sda;
    _scl   = scl;
    _speed = speed;
    _autoTrigger = 0;
    _triggerOn=false;
    _time = millis();
    _wire->begin((int)_sda, (int)_scl, (uint32_t)_speed);
}

void UsomicI2C::setAutoTrigger(uint16_t msec) {
  if((msec > 0) && (msec < 150))
    msec = 150;
  _autoTrigger = msec;  
}

void UsomicI2C::trigger() {
    _wire->beginTransmission(_addr);  // Transfer data to 0x57. 
    _wire->write(0x01);
    _wire->endTransmission();  // Stop data transmission with the Ultrasonic
    _time = millis();
    _triggerOn = true;
}

/*! @brief Get distance data. */
float UsomicI2C::getDistance() {
    uint32_t data;
    unsigned long new_time = millis();
    if(_autoTrigger > 0 && (new_time > (_time+_autoTrigger))) {
      trigger();
      return -1;  
    }
    if(!_triggerOn || (new_time < _time + 120)) {
      return -1;
    }
    _wire->requestFrom(_addr, (uint8_t)3);  // Request 3 bytes from Ultrasonic Unit.
    data = _wire->read();
    data <<= 8;
    data |= _wire->read();
    data <<= 8;
    data |= _wire->read();
    float Distance = float(data) / 1000;
    _triggerOn = false;
    if (Distance > 4500.00) {
        return 4500.00;
    } else {
        return Distance;
    }
}
