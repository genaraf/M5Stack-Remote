#if defined(MODEL_BIGROVER)
#ifndef __BigRoverController_h
#define __BigRoverController_h

#include "control_base.h"

#include "M5Atom.h"

class BigRoverController : public ControlBase {
  public:
    virtual const char* GetId();  
    virtual void Init();
    virtual void Connected();
    virtual void Disconnected();
    virtual void Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy);  
    virtual void Idle();
  private:
    uint8_t Setspeed(int16_t Vtx, int16_t Vty, int16_t Wt);
    void    SetServo(int id, int angle);
  private:
    int16_t speed_buff[4];
  
};
#endif  /* __BigRoverController_h */
#endif /* MODEL_BIGROVER */
