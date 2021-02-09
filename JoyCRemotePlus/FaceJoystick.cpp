#include "config.h"

#ifdef FACE_JOY
#include "FaceJoystick.h"

void FaceJoystick::Init() {
  x = y = btn = 0;  
}

void FaceJoystick::Read() {
  
}

void FaceJoystick::SetLedColor(uint32_t color) {
}

#endif /* FACE_JOY */
