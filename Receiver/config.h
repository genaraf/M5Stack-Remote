#ifndef __config_h
#define __config_h

//#define MODEL_WIFICAR     // Board: NodeMCU 0.9
//#define MODEL_TINY_TRAK     // Board: M5Stack Atom
//#define MODEL_QUICKYBOT   // Board: WeMos D1 mini
//#define MODEL_TRI_TRACK   // Board: M5Stack Atom
#define MODEL_ROVERC        // Board: M5Stick

#if defined(MODEL_WIFICAR)
    #include "WiFiCarController.h"
    #define CONTROLLER WiFiCarController
#elif defined(MODEL_TINY_TRAK)
    #include "TinyTrakController.h"
    #define CONTROLLER TinyTrakController
#elif defined(MODEL_QUICKYBOT)
    #include "QuickyBotController.h"
    #define CONTROLLER QuickyBotController
#elif defined(MODEL_TRI_TRACK)
    #include "TriTrackController.h"
    #define CONTROLLER TriTrackController
#elif defined(MODEL_ROVERC)
    #include "RovercController.h"
    #define CONTROLLER RovercController
#endif


#endif // __config_h
