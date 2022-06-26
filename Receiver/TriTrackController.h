
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
    typedef enum { MANUAL_MODE, AUTOMATIC1_MODE, AUTOMATIC2_MODE, MAX_MODE } ROBOT_MODE; 
    void SetPosition(int id, int speed);
    int getDestination();
    ROBOT_MODE mode;
    unsigned char btn_last;
};
#endif /* MODEL_TRI_TRACK */
