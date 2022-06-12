#include "config.h"

#ifdef MODEL_QUICKYBOT
#include <ESP8266WiFi.h>

#include "QuickyBotController.h"

#define LEFT_MOTOR_PIN  D6
#define RIGHT_MOTOR_PIN D5

#define BUZZER_PIN D1 // Piezo effects (sounds) 100om resistor on buzer plus
#define VBAT_PIN A0   // 150K 
#define RANGE_TRIG_PIN D2
#define RANGE_ECHO_PIN D7
#define RGB_LED_PIN D3 // 470om resistor
#define CHARGING_DETECT_PIN D8

#define LOW_BATTERY 644 // ~3.3V
#define MOTORS_STOP_INACTIVITY_TIMEOUT 10000 // 10sec
#define INACTIVITY_SOUND_TIMEOUT 100000      // 100sec
#define AUTOMATIC_PAUSE 2000


Servo leftMotor;
Servo rightMotor;

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
    { NULL, D6, 0, 180, 101, 0.01, 0 },
    { NULL, D5, 0, 180, 90, 0.01, 0 }
};


PiezoEffects mySounds( BUZZER_PIN ); //PiezoEffects object

#define BRIGHTNESS 50
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, RGB_LED_PIN, NEO_GRBW + NEO_KHZ800);


const char* QuickyBotController::GetId() {
  return "M5AP_QuickyBot";
}

void QuickyBotController::motorsAttach() {
  if(!motorsOn) {
    for(int i = 0; i < sizeof(servs)/sizeof(servs[0]); i++) {
      servs[i].srv->attach(servs[i].pin);
      servs[i].srv->write(servs[i].def);
    }
    motorsOn = true;  
  }
  
}

void QuickyBotController::motorsDetach() {
  if(motorsOn) {
    for(int i = 0; i < sizeof(servs)/sizeof(servs[0]); i++) {
      servs[i].srv->write(servs[i].def);
//      servs[i].srv->detach(servs[i].pin);
    }
    motorsOn = false;
  }
}

void QuickyBotController::setMode(QuickyBotController::ROBOT_MODE mode) {
  switch(mode) {
    case MANUAL_MODE:
      Message(MESSAGE_TYPE_TEXT, MESSAGE_COLOR_GREEN, "Mode: manual");
      motorsAttach();
      strip.setPixelColor(0, strip.Color(0,150,0));
      strip.show();  
      lastActivity = millis();
      robotMode = MANUAL_MODE;
      break;
    
    case AUTOMATIC1_MODE:
      Message(MESSAGE_TYPE_TEXT, MESSAGE_COLOR_GREEN, "Mode: automatic 1");
      randomSeed(millis());
      strip.setPixelColor(0, strip.Color(0,120,120, 50 ) );  
      strip.show();  
      lastActivity = millis() - AUTOMATIC_PAUSE;
      robotMode = AUTOMATIC1_MODE;
      motorsAttach();
      break;
    
    case AUTOMATIC2_MODE:
      Message(MESSAGE_TYPE_TEXT, MESSAGE_COLOR_GREEN, "Mode: automatic 2");
      randomSeed(millis());
      strip.setPixelColor(0, strip.Color(0,0,250, 50 ) );  
      strip.show();  
      lastActivity = millis() - AUTOMATIC_PAUSE;
      robotMode = AUTOMATIC2_MODE;
      motorsAttach();
      break;

    case GUARD_MODE:
      Message(MESSAGE_TYPE_TEXT, MESSAGE_COLOR_GREEN, "Mode: guard");
      robotMode = GUARD_MODE;
      motorsDetach();
      strip.clear();  
      break;
   
  }
}

int16_t QuickyBotController::getDistanceCentim() {
  digitalWrite(RANGE_TRIG_PIN, LOW);  
  delayMicroseconds(2); 
  digitalWrite(RANGE_TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(RANGE_TRIG_PIN, LOW);  
  return pulseIn(RANGE_ECHO_PIN, HIGH, 5000) / 58.2;
}

void QuickyBotController::chargingMode() {
  Serial.println("Start Charging mode");
  WiFi.mode(WIFI_OFF);

  motorsDetach();
  mySounds.play(soundLaugh);
  
  while(digitalRead(CHARGING_DETECT_PIN) == HIGH)
  {
      strip.setPixelColor(0, strip.Color(0,255,0, 50 ) );
      strip.show();
      delay(200);    
      strip.setPixelColor(0, strip.Color(0, 0 ,0, 0 ) );
      strip.show();    
      delay(1000);    
  }
  while(1) {
    mySounds.play(soundCuddly);
    delay(10000);
  }
  Serial.println("End Charging mode");
}

void QuickyBotController::lowBatteryMode()
{
  Message(MESSAGE_TYPE_TEXT, MESSAGE_COLOR_GREEN, "Low battery");
  WiFi.mode(WIFI_OFF);
  delay(500); 
  strip.clear();
  strip.show(); // ?? 
  motorsDetach();
  while(1) {
    mySounds.play( soundBeeps );
    delay(200); 
    mySounds.play( soundBeeps );
    delay(200); 
    mySounds.play( soundBeeps );
    delay(10000); 
  }
}

void QuickyBotController::manualMode() {
  unsigned long timeOut = millis() - lastActivity; 
  if(timeOut > MOTORS_STOP_INACTIVITY_TIMEOUT) {
      if(motorsOn) {
        motorsDetach();
      } else {
        if(timeOut % INACTIVITY_SOUND_TIMEOUT)
          mySounds.play(soundSleeping);  
      }      
  }
}

void QuickyBotController::automatic1Mode() {
  unsigned long timeOut = millis() - lastActivity;
  if(timeOut < AUTOMATIC_PAUSE)
    return;   
}
  
void QuickyBotController::automatic2Mode() {
    
}

void QuickyBotController::guardMode() {
  distance = getDistanceCentim();    
}

bool QuickyBotController::isLowBattary() {
  return false;
// return analogRead(VBAT_PIN) < LOW_BATTERY;
}

void QuickyBotController::Init() { 
    distance = 0;
    voltage = 0;
    motorsOn = false;
    lastActivity = 0;

    robotMode = MANUAL_MODE;
    pinMode(CHARGING_DETECT_PIN, INPUT); 
    pinMode(RANGE_TRIG_PIN, OUTPUT);
    pinMode(RANGE_ECHO_PIN, INPUT);
    strip.setBrightness(BRIGHTNESS);
    strip.begin();
    strip.setPixelColor(0, strip.Color(255,0,0, 255 ) );
    strip.show();
    if(digitalRead(CHARGING_DETECT_PIN) == HIGH)   {
        chargingMode();
    } 
    if(isLowBattary())   {
        lowBatteryMode();
    }
    for(int i = 0; i < sizeof(servs)/sizeof(servs[0]); i++) {
      servs[i].srv = new Servo();
    }
    motorsAttach();
    delay(200); 
}

void QuickyBotController::Connected() {
    Message(MESSAGE_TYPE_TEXT, MESSAGE_COLOR_GREEN, "QuickyBot Connected");
    mySounds.play( soundSuperHappy );
    strip.setPixelColor(0, strip.Color(0,150,0));
    strip.show(); 
}

void QuickyBotController::Disconnected() {
    mySounds.play( soundOhOoh );
    strip.setPixelColor(0, strip.Color(150,0,0));
    strip.show(); 
}

/* Id - 0, 1  sped -100 - 100 */
void QuickyBotController::setPosition(int id, int pos) {
  if((id >= 0) && (id < sizeof(servs)/sizeof(servs[0]))) {
    servs[id].pos = servs[id].def + pos;    
    if(servs[id].pos > servs[id].max)
      servs[id].pos = servs[id].max;
    if(servs[id].pos < servs[id].min)
      servs[id].pos = servs[id].min;
    servs[id].srv->write((int)servs[id].pos);  
  } 
}

void QuickyBotController::Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy) {
  if(robotMode == MANUAL_MODE) {
    int lspeed = 0;
    int rspeed = 0;
    if(abs(ly) > 10) {
      lspeed = -ly;
      rspeed = ly;
    
    }
    if(abs(lx) > 10) {
      lspeed += lx;
      rspeed += lx;  
    }
    if((lspeed != 0) || (rspeed != 0)) {
      lastActivity = millis();
    }
    Serial.print("lspeed: ");
    Serial.println(lspeed);
    setPosition(0, lspeed);     
    setPosition(1, rspeed);
    if(!motorsOn) {
      motorsAttach();  
    }
           
  }
}

void QuickyBotController::Idle() { 
    if(digitalRead(CHARGING_DETECT_PIN) == HIGH)   {
        chargingMode();
    } 
    if(isLowBattary())   {
        lowBatteryMode();
    } 

    switch(robotMode) {
        case MANUAL_MODE:
            manualMode();   
            break;

        case AUTOMATIC1_MODE:
            automatic1Mode();
            break;

        case AUTOMATIC2_MODE:
            automatic2Mode();
            break;

        case GUARD_MODE:
            guardMode();
            break;
    }

}

#endif /* MODEL_QUICKYBOT */
