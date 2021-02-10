#ifdef MODEL_QUICKYBOT

#include <Servo.h>
#include <PiezoEffects.h>
#include <Adafruit_NeoPixel.h>

#include "control_base.h"

class QuickyBotController : public ControlBase {
  public:
    virtual const char* GetId();  
    virtual void Init();
    virtual void Connected();
    virtual void Disconnected();
    virtual void Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy);  
    virtual void Idle();
  private:
    void motorsAttach();
    void motorsDetach();
    int16_t getDistanceCentim();
    void manualMode();
    void automatic1Mode();
    void automatic2Mode();
    void guardMode();
    void chargingMode();
    // variables
    int ledtMotorOffset;
    int rightMotorOffset;
    int distance;
    int voltage;
    bool motorsOn;
    unsigned long lastActivity;
};

#endif /* MODEL_QUICKYBOT */
