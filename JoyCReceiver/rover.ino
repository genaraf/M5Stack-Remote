#include <Servo.h>
Servo servo;

#include <Servo.h>


static const int servoPin = 25;

Servo servo1;

void RoverInit() {
    servo1.attach(servoPin);
}



static int16_t speed_buff[4] = {0};
static uint8_t Setspeed( int16_t Vtx, int16_t Vty, int16_t Wt)
{
  Wt = ( Wt > 100 )  ? 100 :  Wt;
  Wt = ( Wt < -100 ) ? -100 : Wt;

  Vtx = ( Vtx > 100 )  ? 100 :  Vtx;
  Vtx = ( Vtx < -100 ) ? -100 : Vtx;
  Vty = ( Vty > 100 )  ? 100 :  Vty;
  Vty = ( Vty < -100 ) ? -100 : Vty;

  Vtx = ( Wt != 0 ) ? Vtx *  (100 - abs(Wt)) / 100 : Vtx;
  Vty = ( Wt != 0 ) ? Vty *  (100 - abs(Wt)) / 100 : Vty;

  speed_buff[0] = Vty - Vtx - Wt ;
  speed_buff[1] = Vty + Vtx + Wt ;
  speed_buff[3] = Vty - Vtx + Wt ;
  speed_buff[2] = Vty + Vtx - Wt ;

  for (int i = 0; i < 4; i++)
  {
    speed_buff[i] = ( speed_buff[i] > 100 )  ? 100 :  speed_buff[i];
    speed_buff[i] = ( speed_buff[i] < -100 ) ? -100 : speed_buff[i];
  }
  return 1;
  //return I2CWritebuff(0x00, (uint8_t*)speed_sendbuff, 4);
}

void ProcessingCommand(uint8_t lx, uint8_t ly, uint8_t rx, uint8_t ry, uint8_t btn, uint8_t gx, uint8_t gy ) {
      servo1.write(lx);
/*
      if ( udodata[6] == 0x01 )
      {
        IIC_ReState = Setspeed( udodata[3] - 100  , udodata[4] - 100 , udodata[5] - 100 );
      }
      else
      {
        IIC_ReState = Setspeed( 0  , 0 , 0 );
      }
*/   
}

void ProcessingDisconnect() {
  
}
