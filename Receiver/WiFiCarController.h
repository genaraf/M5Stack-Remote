
#ifdef MODEL_WIFICAR
#include "control_base.h"

class WiFiCarController : public ControlBase {
  public:
    virtual const char* GetId();  
    virtual void Init();
    virtual void Connected();
    virtual void Disconnected();
    virtual void Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy);  
    virtual void Idle();
  private:   
    void MotorControl(int id, int speed);
};
#endif /* MODEL_WIFICAR */
