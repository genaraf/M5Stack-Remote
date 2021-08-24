
#ifdef MODEL_TRI_TRACK
#include "control_base.h"

class TriTrackController : public ControlBase {
  public:
    virtual const char* GetId();  
    virtual void Init();
    virtual void Connected();
    virtual void Disconnected();
    virtual void Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy);  
    virtual void Idle();
  private: 
    void SetPosition(int id, int speed); 
};
#endif /* MODEL_TRI_TRACK */
