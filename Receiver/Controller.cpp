#include "Controller.h"

const char* Controller::GetId() {
  return "M5AP_WiFiCar";
}

void Controller::Init() { 
}

void Controller::Connected() {
  Message(MESSAGE_TYPE_TEXT, MESSAGE_COLOR_GREEN, "WiFICar Connected");  
}

void Controller::Disconnected() {
  
}

void Controller::Command(int lx, int ly, int rx, int ry, unsigned char btn, int gx, int gy) {
  
}

void Controller::Idle() { 

}
