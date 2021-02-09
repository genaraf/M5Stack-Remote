#ifndef _facejoystick_h_
#define _facejoystick_h_

#include "JoystickBase.h"

class FaceJoystick: public JoystickBase {
public:
  virtual void Init();
  virtual void Read();
  virtual void SetLedColor(uint32_t color);
  virtual uint8_t GetX(uint8_t pos) { return (pos == 0)?x:100; } 
  virtual uint8_t GetY(uint8_t pos) { return (pos == 0)?y:100; }
  virtual uint8_t GetPress(uint8_t pos) { return (pos == 0)?btn:0; }
private:
  uint8_t x;
  uint8_t y;
  uint8_t btn;
};

#endif /* _facejoystick_h_ */
