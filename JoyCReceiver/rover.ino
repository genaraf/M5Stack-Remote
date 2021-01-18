#include <Servo.h>
Servo servo;

#include <Servo.h>

struct SrvCtl {
  Servo* srv;
  int pin;
  int min;
  int max;
  int def;
  float rate;
  float pos;
  
};

SrvCtl servs[] = {
    { NULL, 25, 0, 180, 91, 0.01, 0 },
    { NULL, 21, 0, 180, 93, 0.01, 0 }
};

void ControlInit() {
    for(int i = 0; i < sizeof(servs)/sizeof(servs[0]); i++) {
      servs[i].srv = new Servo();
      servs[i].srv->attach(servs[i].pin);
      servs[i].srv->write(servs[i].def); 
    }  
}

void SetPosition(int servoId, int pos) {
  if((servoId >= 0) && (servoId < sizeof(servs)/sizeof(servs[0]))) {
    servs[servoId].pos = servs[servoId].def + pos;    
    if(servs[servoId].pos > servs[servoId].max)
      servs[servoId].pos = servs[servoId].max;
    if(servs[servoId].pos < servs[servoId].min)
      servs[servoId].pos = servs[servoId].min;
    servs[servoId].srv->write((int)servs[servoId].pos);  
  }
}

void ProcessingCommand(int lx, int ly, int rx, int ry, uint8_t btn, uint8_t gx, uint8_t gy ) {
      
      if(abs(ly) > 10 || abs(lx) > 0) {
        if(abs(lx) < 10)
          lx = 0;
        if(abs(ly) < 10)
          ly = 0;  
//        Serial.printf("X:%d Y:%d\n", lx, ly);  
        SetPosition(0, -ly - lx);
        SetPosition(1, ly - lx);
      } else {
        SetPosition(0, 0);
        SetPosition(1, 0);
      }
      
}

void ProcessingDisconnect() {
    for(int i = 0; i < sizeof(servs)/sizeof(servs[0]); i++) {
      servs[i].srv->write(servs[i].def);   
    }   
}
