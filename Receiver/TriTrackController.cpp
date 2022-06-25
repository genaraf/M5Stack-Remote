// 
// 3D model on https://www.thingiverse.com/thing:3013262
// Controller  https://m5stack.com/collections/m5-atom/products/atom-lite-esp32-development-kit
// 

#include "config.h"

#ifdef MODEL_TRI_TRACK
// Atom PINOUT
// EXT G19 
// EXT G22 
// EXT G23, G34 (ADC1 CH6)
// EXT G33 (ADC1 CH5)

// EXT G21 (SCL) (IMU MPU6886 Matrix)  
// EXT G25 (SDA) IMU (DAC1)
// 
// GROVE G32 
// GROVE G26 
// 
// G27 - Neopixel
// G39 - Button
#include <M5Atom.h>
#include <ESP32Servo.h>

struct SrvCtl {
  Servo* srv;
  int pin;
  int min;
  int max;
  int def;
  float rate;
  float pos;
  
};

#define SR04_ECHO 26
#define SR04_TRIGGER 32

SrvCtl servs[] = {
    { NULL, 22, 0, 180, 93, 0.01, 0 }, // left
    { NULL, 19, 0, 180, 93, 0.01, 0 }, // right
    { NULL, 23, 0, 180, 160, 0.01, 0 }
};

bool attached = false;


const char* TriTrackController::GetId() {
  return "M5AP_TriTr";
}

void TriTrackController::Init() { 
  M5.begin(true, false, true);
  delay(50);
  M5.update();
  Wire.begin(0, 26, 10000);
  M5.dis.drawpix(0, 0xf00000);

#ifdef BATTERY_CHECK  
  pinMode(GPIO_NUM_34, INPUT);
  gpio_pulldown_dis(GPIO_NUM_23);
  gpio_pullup_dis(GPIO_NUM_23);
#endif /* BATTERY_CHECK */
  
    for(int i = 0; i < sizeof(servs)/sizeof(servs[0]); i++) {
      servs[i].srv = new Servo();
      servs[i].srv->attach(servs[i].pin);
      servs[i].srv->write(servs[i].def);
    }
    delay(200);
#ifdef SR04_ECHO    
    pinMode(SR04_TRIGGER, OUTPUT); // Sets the trigPin as an Output
    pinMode(SR04_ECHO, INPUT); // Sets the echoPin as an Input  
#endif /* SR04_ECHO */    
}

void TriTrackController::Connected() {
  M5.dis.drawpix(0, 0x00f000);
//  Message(MESSAGE_TYPE_TEXT, MESSAGE_COLOR_GREEN, "Tank Connected");  
}

void TriTrackController::Disconnected() {
  M5.dis.drawpix(0, 0xf00000); 
}

int TriTrackController::getDestination() {
#ifdef SR04_ECHO    
  // Clears the triger Pin
  digitalWrite(SR04_TRIGGER, LOW);
  delayMicroseconds(2);

  // Sets the trigger Pin on HIGH state for 10 micro seconds
  digitalWrite(SR04_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(SR04_TRIGGER, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration = pulseIn(SR04_ECHO, HIGH);

  // Calculating the distance
  int distance= duration*0.034/2;
  return distance;
#else
  return -1;  
#endif /* SR04_ECHO */
}

/* Id - 0, 1  sped -100 - 100 */
void TriTrackController::SetPosition(int id, int pos) {
  if((id >= 0) && (id < sizeof(servs)/sizeof(servs[0]))) {
    servs[id].pos = servs[id].def + pos;    
    if(servs[id].pos > servs[id].max)
      servs[id].pos = servs[id].max;
    if(servs[id].pos < servs[id].min)
      servs[id].pos = servs[id].min;
    servs[id].srv->write((int)servs[id].pos);  
  } 
}

void TriTrackController::Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy) {
  int lspeed = 0;
  int rspeed = 0;
  if(abs(ly) > 10) {
    lspeed = +ly;
    rspeed = -ly;    
  }
  if(abs(lx) > 10) {
    lspeed = -lx;
    rspeed = -lx;  
  }
  SetPosition(0, lspeed);     
  SetPosition(1, rspeed); 
  SetPosition(2, 100 - ry * 2);        
  Serial.print("Destination: ");
  Serial.println(getDestination());
}

void TriTrackController::Idle() { 
   if(M5.Btn.read() > 0) {
    Message(MESSAGE_TYPE_TEXT, MESSAGE_COLOR_GREEN, "Pressed Btn");      
    while(M5.Btn.read() > 0) {  
      delay(10);
    }
  }
}
#endif /* MODEL_TRI_TRACK */
