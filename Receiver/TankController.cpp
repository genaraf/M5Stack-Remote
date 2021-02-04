#include "config.h"

#ifdef MODEL_TANK
// Atom PINOUT
// EXT G19 
// EXT G22 
// EXT G23, G34 (ADC1 CH6)
// EXT G33 (ADC1 CH5)

// EXT G21 (SCL) IMU 
// EXT G25 (SDA) IMU (DAC1)
// 
// GROVE G32 
// GROVE G26 
// 
// G27 - Neopixel
// G39 - Button
#include <M5Atom.h>
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


const char* TankController::GetId() {
  return "M5AP_Tank";
}

void TankController::Init() { 
  M5.begin(true, false, true);
  delay(50);
  M5.update();
  Wire.begin(0, 26, 10000);
  M5.dis.drawpix(0, 0x00f000);

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

void TankController::Connected() {
  M5.dis.drawpix(0, 0xf00000); 
  Message(MESSAGE_TYPE_TEXT, MESSAGE_COLOR_GREEN, "Tank Connected");  
  digitalWrite(led, LOW);     
}

void TankController::Disconnected() {
  digitalWrite(led, HIGH);
  M5.dis.drawpix(0, 0x00f000);
}

void TankController::Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy) {
  int lspeed = 0;
  int rspeed = 0;
  if(abs(ly) > 10) {
    lspeed = ly;
    rspeed = ly;    
  }
  if(abs(lx) > 10) {
    lspeed -= lx;
    rspeed += lx;  
  }
  MotorControl(0, lspeed);     
  MotorControl(1, rspeed);     
}

void TankController::Idle() { 
  if(M5.Btn.read() > 0) {
    Message(MESSAGE_TYPE_TEXT, MESSAGE_COLOR_GREEN, "Pressed Btn");      
    while(M5.Btn.read() > 0) {  
      delay(10);
    }
  }
}
#endif /* MODEL_TANK */
