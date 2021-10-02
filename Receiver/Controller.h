#ifdef MODEL_MODELNAME
#ifndef __controller_h
#define __controller_h

#include "control_base.h"

class Controller : public ControlBase {
  public:
    virtual const char* GetId();  
    virtual void Init();
    virtual void Connected();
    virtual void Disconnected();
    virtual void Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy);  
    virtual void Idle();
  private:
      
};
#endif /* __controller_h */

#endif /* MODEL_MODELNAME */
