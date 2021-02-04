#include <Arduino.h>
#include "WiFiCarController.h"

static const uint8_t pwm_A = 5 ;
static const uint8_t pwm_B = 4;
static const uint8_t dir_A = 0;
static const uint8_t dir_B = 2;
static const uint8_t led   = 16;

// Motor speed = [0-1024]
int motor_speed = 1024;

const char* WiFiCarController::GetId() {
  return "M5AP_WiFiCar";
}

void WiFiCarController::Init() { 
  pinMode(pwm_A, OUTPUT); // PMW A
  pinMode(pwm_B, OUTPUT); // PMW B
  pinMode(dir_A, OUTPUT); // DIR A
  pinMode(dir_B, OUTPUT); // DIR B
  pinMode(led, OUTPUT); // LED
  digitalWrite(led, HIGH);
  MotorControl(0, 0);
}

void WiFiCarController::Connected() {
  Message(MESSAGE_TYPE_TEXT, MESSAGE_COLOR_GREEN, "WiFICar Connected");  
  digitalWrite(led, LOW);     
}

void WiFiCarController::Disconnected() {
  digitalWrite(led, HIGH);
  MotorControl(0, 0);     
  MotorControl(1, 0);     
}

void WiFiCarController::Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy) {
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

void WiFiCarController::Idle() { 

}

/* Id - 0, 1  sped -100 - 100 */
void WiFiCarController::MotorControl(int id, int speed) {
  int pwm = abs(speed) * 10;
  if(pwm > 1024) pwm = 1024;
  
  if(id == 0) {
    analogWrite(pwm_A, pwm);
    if(speed > 0) {
      digitalWrite(dir_A, HIGH);   
    } else {
      digitalWrite(dir_A, LOW);   
    }
  } else {
    analogWrite(pwm_B, pwm);
    if(speed > 0) {
      digitalWrite(dir_B, LOW);   
    } else {
      digitalWrite(dir_B, HIGH);   
    }
  }
}
