#ifndef _facejoystick_h_
#define _facejoystick_h_

#include "JoystickBase.h"

class FaceJoystick: public JoystickBase {
public:
  virtual void Init();
  virtual void Read();
  virtual void SetLedColor(uint32_t color);
  virtual uint8_t GetX(uint8_t pos);
  virtual uint8_t GetY(uint8_t pos);
  virtual uint8_t GetPress(uint8_t pos) { return (pos == jmode)?btn:0; }
private:
  uint8_t jmode;
  int16_t x_data;
  int16_t y_data;
  uint8_t btn;
  void Led(int indexOfLED, int r, int g, int b);
};

#endif /* _facejoystick_h_ */
