#ifndef __config_h
#define __config_h

//#define M5STICK
//#define M5STICKCPLUS
#define M5STACK

#if defined(M5STICKCPLUS)
#include <M5StickCPlus.h>
#define JOYC
#elif defined(M5STICK)  
#include <M5StickC.h>
#define JOYC
#elif defined(M5STACK)
#define M5STACK_MPU6886
#include <M5Stack.h>
#define FACE_JOY
#endif /* M5STICKCPLUS */

#ifdef JOYC 
#include "JoyC.h"
#define JOYSTICK JoyC
#elif defined(FACE_JOY)
#include "FaceJoystick.h" 
#define JOYSTICK FaceJoystick
#endif

#endif /* __config_h */
