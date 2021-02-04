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
    { NULL, 22, 0, 180, 91, 0.01, 0 },
    { NULL, 19, 0, 180, 93, 0.01, 0 },
    { NULL, 23, 0, 180, 160, 0.01, 0 }
};

bool attached = false;

void ControlInit() {

#ifdef BATTERY_CHECK  
  pinMode(GPIO_NUM_34, INPUT);
  gpio_pulldown_dis(GPIO_NUM_23);
  gpio_pullup_dis(GPIO_NUM_23);
#endif /* BATTERY_CHECK */
  
    for(int i = 0; i < sizeof(servs)/sizeof(servs[0]); i++) {
      servs[i].srv = new Servo();
    }
    SetDefault(2);
    delay(200);
}

void SetDefault(int servoId) {
    if((servoId >= 0) && (servoId < sizeof(servs)/sizeof(servs[0]))) {
      SetPosition(servoId, servs[servoId].def);
    }
}

void SetPosition(int servoId, int pos) {
  if((servoId >= 0) && (servoId < sizeof(servs)/sizeof(servs[0]))) {
    if(!servs[servoId].srv->attached()) {
      servs[servoId].srv->attach(servs[servoId].pin);  
      delay(50);
    }
    servs[servoId].pos = servs[servoId].def + pos;    
    if(servs[servoId].pos > servs[servoId].max)
      servs[servoId].pos = servs[servoId].max;
    if(servs[servoId].pos < servs[servoId].min)
      servs[servoId].pos = servs[servoId].min;
    servs[servoId].srv->write((int)servs[servoId].pos);  
  } 
}

void Detach(int servoId){
  if((servoId >= 0) && (servoId < sizeof(servs)/sizeof(servs[0]))) {
    if(servs[servoId].srv->attached()) {
      servs[servoId].srv->detach();
    }
  }  
}
void ProcessingCommand(int lx, int ly, int rx, int ry, uint8_t btn, uint8_t gx, uint8_t gy ) {  
      static int inactivityCnt = 0; 
      if(abs(ly) > 10 || abs(lx) > 0) {
        if(abs(lx) < 10)
          lx = 0;
        if(abs(ly) < 10)
          ly = 0;  
        SetPosition(0, -ly - lx);
        SetPosition(1, ly - lx);
        inactivityCnt = 0;
      } else {
        if(++inactivityCnt > 10) {
          Detach(0);
          Detach(1);
        } else {
          SetPosition(0, 0);
          SetPosition(1, 0);
         
        }
      }
      SetPosition(2, 100 - ry * 2);       
}

void ProcessingDisconnect() {
    for(int i = 0; i < sizeof(servs)/sizeof(servs[0]); i++) {
      servs[i].srv->write(servs[i].def);   
    }
    delay(200);   
    Detach(0);
    Detach(1);
    Detach(2);            
}
