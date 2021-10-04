#ifdef MODEL_ROVERC
#ifndef __RovercController_h
#define __RovercController_h

#include "control_base.h"

#include <M5StickC.h>

class RovercController : public ControlBase {
  public:
    virtual const char* GetId();  
    virtual void Init();
    virtual void Connected();
    virtual void Disconnected();
    virtual void Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy);  
    virtual void Idle();
  private:
    uint8_t I2CWrite1Byte(uint8_t Addr, uint8_t Data);
    uint8_t I2CWritebuff(uint8_t Addr, uint8_t *Data, uint16_t Length);
    uint8_t Setspeed(int16_t Vtx, int16_t Vty, int16_t Wt);
    void    SetServo(int id, int angle);
  private:
    int16_t speed_buff[4];
    int8_t  speed_sendbuff[4];
  
};
#endif  /* __RovercController_h */
#endif /* MODEL_ROVERC */
