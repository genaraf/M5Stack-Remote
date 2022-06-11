#ifdef MODEL_QUICKYBOT
#ifndef __QuickyBotController_h__
  #define __QuickyBotController_h__

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
      typedef enum { MANUAL_MODE, AUTOMATIC1_MODE, AUTOMATIC2_MODE, GUARD_MODE, MAX_MODE } ROBOT_MODE;
      void motorsAttach();
      void motorsDetach();
      int16_t getDistanceCentim();
      void manualMode();
      void automatic1Mode();
      void automatic2Mode();
      void guardMode();
      void chargingMode();
      void lowBatteryMode();
      void setMode(QuickyBotController::ROBOT_MODE mode);
      // variables
      QuickyBotController::ROBOT_MODE robotMode;
      int ledtMotorOffset;
      int rightMotorOffset;
      int distance;
      int voltage;
      bool motorsOn;
      unsigned long lastActivity;
  };
#endif /* __QuickyBotController_h__ */
#endif /* MODEL_QUICKYBOT */
