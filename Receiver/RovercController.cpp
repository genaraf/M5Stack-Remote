// 
// https://docs.m5stack.com/en/hat/hat_roverc_pro
// 


#include "config.h"

#ifdef MODEL_ROVERC
#include "RovercController.h"

TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);

uint8_t RovercController::I2CWrite1Byte(uint8_t Addr, uint8_t Data)
{
    Wire.beginTransmission(0x38);
    Wire.write(Addr);
    Wire.write(Data);
    return Wire.endTransmission();
}

uint8_t RovercController::I2CWritebuff(uint8_t Addr, uint8_t *Data, uint16_t Length)
{
    Wire.beginTransmission(0x38);
    Wire.write(Addr);
    for (int i = 0; i < Length; i++)
    {
        Wire.write(Data[i]);
    }
    return Wire.endTransmission();
}

uint8_t RovercController::Setspeed(int16_t Vtx, int16_t Vty, int16_t Wt)
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
    speed_buff[3] = Vty - Vtx + Wt;
    speed_buff[2] = Vty + Vtx - Wt;

    for (int i = 0; i < 4; i++)
    {
        speed_buff[i] = (speed_buff[i] > 100) ? 100 : speed_buff[i];
        speed_buff[i] = (speed_buff[i] < -100) ? -100 : speed_buff[i];
        speed_sendbuff[i] = speed_buff[i];
    }
    return I2CWritebuff(0x00, (uint8_t *)speed_sendbuff, 4);
}

const char* RovercController::GetId() {
  return "M5AP_RoverC";
}

void RovercController::Init() { 
    M5.begin();
    M5.update();
    Wire.begin(0, 26, 10000);
    M5.Lcd.setRotation(1);
    M5.Lcd.setSwapBytes(false);
    Disbuff.createSprite(160, 80);
    Disbuff.setSwapBytes(true);
    Disbuff.fillRect(0, 0, 160, 20, Disbuff.color565(50, 50, 50));
    Disbuff.setTextSize(2);
    Disbuff.setTextColor(WHITE);
    Disbuff.setCursor(15, 35);
    Disbuff.print("RoverC");
    Disbuff.pushSprite(0, 0);
    M5.update();
#if 1
    // Charge
    Wire1.beginTransmission(0x34);
    Wire1.write(0x33);
    Wire1.write(0xC0 | (4 & 0x0f));
    Wire1.endTransmission();
#endif    
}

void RovercController::Connected() {
}

void RovercController::Disconnected() {
  Setspeed(0, 0, 0);
}

void RovercController::Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy) {
  Setspeed(lx, ly, rx);
}

void RovercController::Idle() {
    float volt = M5.Axp.GetBatVoltage();
    Disbuff.fillRect(0, 0, 160, 20, Disbuff.color565(50, 50, 50));
    Disbuff.setTextSize(1);
    if(volt < 3.3) {
      Disbuff.setTextColor(RED);      
    } else {
      Disbuff.setTextColor(GREEN);
    }  
    Disbuff.setCursor(20, 5);
    Disbuff.printf("Bat: %.3fV", volt); 
    Disbuff.pushSprite(0, 0);
    M5.update();
}
#endif /* MODEL_ROVERC */
