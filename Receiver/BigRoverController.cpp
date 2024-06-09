// 
// 
#include "config.h"

#if defined(MODEL_BIGROVER)
#include "BigRoverController.h"
#include "UNIT_HBRIDGE.h"
#include "UsonicI2c.h"

UNIT_HBRIDGE drv[4];
UsomicI2C usonic;

void BigRoverController::SetServo(int id, int degree) {
  if((id < 0) || (id > 1)) {
    return;
  }
  degree = min(90, int(degree));
  degree = max(0, int(degree));
}

uint8_t BigRoverController::Setspeed(int16_t Vtx, int16_t Vty, int16_t Wt)
{
    Wt = (Wt > 100) ? 100 : Wt;
    Wt = (Wt < -100) ? -100 : Wt;

    Vtx = (Vtx > 100) ? 100 : Vtx;
    Vtx = (Vtx < -100) ? -100 : Vtx;
    Vty = (Vty > 100) ? 100 : Vty;
    Vty = (Vty < -100) ? -100 : Vty;

    Vtx = (Wt != 0) ? Vtx * (100 - abs(Wt)) / 100 : Vtx;
    Vty = (Wt != 0) ? Vty * (100 - abs(Wt)) / 100 : Vty;

    speed_buff[0] = Vty - Vtx - Wt;
    speed_buff[1] = Vty + Vtx + Wt;
    speed_buff[2] = Vty + Vtx - Wt;
    speed_buff[3] = Vty - Vtx + Wt;
    
    Serial.printf("Speed: %d %d %d %d\r\n", speed_buff[0],speed_buff[1],speed_buff[2],speed_buff[3]);
    for (int i = 0; i < 4; i++)
    {
        speed_buff[i] = (speed_buff[i] > 100) ? 100 : speed_buff[i];
        speed_buff[i] = (speed_buff[i] < -100) ? -100 : speed_buff[i];
        if(speed_buff[i] == 0) {
          drv[i].setDriverSpeed8Bits(0);
          delay(5);
          drv[i].setDriverDirection(DIR_STOP);
          delay(5);
        } else {
          drv[i].setDriverSpeed8Bits(abs(speed_buff[i]));
          delay(5);
          drv[i].setDriverDirection((speed_buff[i] > 0)?DIR_FORWARD:DIR_REVERSE);
          delay(5);
        }        
    }
    return 0;
}

const char* BigRoverController::GetId() {  
  return "M5AP_BigRover";
}

void BigRoverController::Init() { 
    M5.begin(true, false, true);
    M5.dis.drawpix(0, 0x00);
    delay(50);                    // delay 50ms.
    Wire.begin(26, 32, 100000L);
    M5.update();

    drv[0].begin(&Wire, HBRIDGE_ADDR + 1);
    drv[1].begin(&Wire, HBRIDGE_ADDR + 2);
    drv[2].begin(&Wire, HBRIDGE_ADDR + 3);
    drv[3].begin(&Wire, HBRIDGE_ADDR + 4);
    for(int i = 0; i < 4; i++) {
      drv[i].setDriverPWMFreq(5000);
      delay(10);
    }
    usonic.begin(&Wire); 
    usonic.setAutoTrigger(150);   
    M5.dis.drawpix(0, 0x0000f0);
    delay(50);                    // delay 50ms.
    M5.update();
}

void BigRoverController::Connected() {
  M5.dis.drawpix(0, 0x00ff00); 
  delay(50);   
  M5.update();
  Serial.println("Connected");
}

void BigRoverController::Disconnected() {
  Setspeed(0, 0, 0);
  SetServo(0, 0);
  M5.dis.drawpix(0, 0xff0000);
  delay(50);   
  M5.update();
  Serial.println("Disconnected");
}

void BigRoverController::Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy) {
  Setspeed(rx, ly, lx);
  SetServo(0, ry);
}

void BigRoverController::Idle() {
    M5.update();
    delay(2);
    auto dist = usonic.getDistance();
    if(dist > 0) {
      Serial.printf("Distance:%f\n", dist);
    } 
//    canvas.printf("%.2fV", drv[0]].getAnalogInput(_12bit) / 4095.0f * 3.3f / 0.09f);
}
#endif /* MODEL_BIGROVER */
