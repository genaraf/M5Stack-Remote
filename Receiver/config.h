#ifndef __config_h
#define __config_h

#define MODEL_WIFICAR // Board: NodeMCU 0.9
//#define MODEL_TANK    // Board: M5Stack Atom

#if defined(MODEL_WIFICAR)

#include "WiFiCarController.h"
#define CONTROLLER WiFiCarController

#elif defined(MODEL_TANK)

#include "TankController.h"
#define CONTROLLER TankController

#endif
#endif // __config_h
