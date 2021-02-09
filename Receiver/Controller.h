#ifdef MODEL_MODELNAME

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

#endif /* MODEL_MODELNAME */
