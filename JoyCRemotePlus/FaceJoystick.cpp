#include "config.h"

#ifdef FACE_JOY
#include "FaceJoystick.h"

#define FACE_JOY_ADDR 0x5e

void FaceJoystick::Led(int indexOfLED, int r, int g, int b) {
  Wire.beginTransmission(FACE_JOY_ADDR);
  Wire.write(indexOfLED);
  Wire.write(r);
  Wire.write(g);
  Wire.write(b);
  Wire.endTransmission();  
}

void FaceJoystick::Init() {
  btn = 0;
  x_data = y_data = 512;
  jmode = 0;
  for (int i = 0; i < 256; i++)
  {
    Wire.beginTransmission(FACE_JOY_ADDR);
    Wire.write(i % 4);
    Wire.write(random(256) * (256 - i) / 256);
    Wire.write(random(256) * (256 - i) / 256);
    Wire.write(random(256) * (256 - i) / 256);
    Wire.endTransmission();
    delay(2);
  }
  Led(0, 0, 0, 0);
  Led(1, 0, 0, 0);
  Led(2, 0, 0, 0);
  Led(3, 0, 0, 0);  
}

void FaceJoystick::Read() {
  Wire.requestFrom(FACE_JOY_ADDR, 5);
  if (Wire.available()) {
    uint8_t tmp;
    tmp = Wire.read();
    y_data = Wire.read();
    y_data = (y_data << 8) + tmp;
    tmp = Wire.read();
    x_data = Wire.read();
    x_data = (x_data << 8) + tmp;
    
    btn = Wire.read()?0:1;
    
    Serial.printf("x:%d y:%d button:%d\n", (int)x_data, (int)y_data, (int)btn);

    if (x_data > 600){
      Led(2,  0, 0, 50);
      Led(0, 0, 0, 0);
    } else  if (x_data < 400) {
      Led(0,  0, 0, 50);
      Led(2, 0, 0, 0);
    } else {
      Led(0,  0, 0,0);
      Led(2, 0, 0, 0);
    }
    
    if (y_data > 600) {
      Led(3,  0, 0, 50);
      Led(1, 0, 0, 0);
    } else if (y_data < 400)
    {
      Led(1,  0, 0, 50);
      Led(3, 0, 0, 0);
    } else {
      Led(1,  0, 0, 0);
      Led(3, 0, 0, 0);
    }
  }
 
}
uint8_t norm(int16_t v) {
  int16_t out = v / 2 - 160;
  if(out < 0) {
    out = 0;
  } else if(out > 200) {
    out = 200;
  }
  return (uint8_t)out;  
}

uint8_t FaceJoystick::GetX(uint8_t pos) {
  return (pos == jmode)?norm(x_data):100; 
} 

uint8_t FaceJoystick::GetY(uint8_t pos) { 
  return (pos == jmode)?norm(y_data):100; 
}

void FaceJoystick::SetLedColor(uint32_t color) {
  
}

#endif /* FACE_JOY */
