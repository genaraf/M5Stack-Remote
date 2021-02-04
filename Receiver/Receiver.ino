
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <WiFiUdp.h>

#include "WiFiCarController.h"
WiFiCarController cntrl;

#define DEBUG

const char *password = "77777777";

#define CTL_MESSAGE_LEN 12

WiFiServer server(80);
WiFiUDP Udp1;
IPAddress remoteIP;
uint16_t remotePort;
uint16_t count = 0;
bool received = false;
bool connected = false;
uint8_t txCnt = 0;


void setup()
{
  cntrl.SetMessageCallback(sendMessage);
  Serial.begin(115200);

  //Set device in STA mode to begin with
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1),
                    IPAddress(192, 168, 4, 1),
                    IPAddress(255, 255, 255, 0));
  WiFi.softAP(cntrl.GetId(), password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Udp1.begin(1003);
  cntrl.Init();
}

void sendMessage(unsigned char event_id, unsigned int color, char* text) {
  Serial.printf("msg: %s\n", text);
  Udp1.beginPacket(remoteIP, remotePort);
  Udp1.write(0xAA);
  Udp1.write(0x55);
  Udp1.write(txCnt++);
  Udp1.write((uint8_t)event_id);
  Udp1.write((uint16_t)color & 0xFF);
  Udp1.write(((uint16_t)color >> 8) & 0xFF);  
  Udp1.write((uint8_t*)text, strlen(text));
  Udp1.endPacket();
}

int norm(uint8_t val) {
  int i = val;
  return val - 100;  
}

void loop()
{
  count++;
  if(Udp1.parsePacket() == CTL_MESSAGE_LEN) {
    char rxdata[CTL_MESSAGE_LEN];
    Udp1.read(rxdata, CTL_MESSAGE_LEN);
    remoteIP = Udp1.remoteIP();
    remotePort = Udp1.remotePort();
    if ((rxdata[0] == 0xAA ) && (rxdata[1] == 0x55 ) && (rxdata[11] == 0xee )) {
#ifdef DEBUG
      Serial.printf("#%03d ", rxdata[3]);
      for (int i = 4; i < CTL_MESSAGE_LEN; i++) {
        Serial.printf("%02X ", rxdata[i]);
      }
      Serial.println();    
#endif
      cntrl.Command(norm(rxdata[4]), norm(rxdata[5]), norm(rxdata[6]), norm(rxdata[7]), rxdata[8], norm(rxdata[9]), norm(rxdata[10]));
      received = true;
    }
  } else {
    delay(10);
  }

  if(count > 100) {
    if(received) {
      if(!connected) {
        connected = true;
        cntrl.Connected();
      }
    } else if(connected) {
        connected = false;    
        cntrl.Disconnected();
    }
    count = 0;
    received = false;
  }
  cntrl.Idle();
}
