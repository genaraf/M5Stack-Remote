#include "M5Atom.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>

#define DEBUG

extern void ProcessingCommand(uint8_t lx, uint8_t ly, uint8_t rx, uint8_t ry, uint8_t btn, uint8_t gx, uint8_t gy);
extern void ProcessingDisconnect();

const char *ssid = "M5AP";
const char *password = "77777777";

WiFiServer server(80);
WiFiUDP Udp1;

uint16_t count = 0;
bool received = false;

void setup()
{
  M5.begin(true, false, true);
  delay(50);
  M5.update();
  Wire.begin(0, 26, 10000);
  M5.dis.drawpix(0, 0x00f000);

  uint64_t chipid = ESP.getEfuseMac();
  String str = ssid + String((uint32_t)(chipid >> 32), HEX);

  Serial.begin(115200);
#ifdef LONG_RANGE  
  WiFi.begin();
  delay( 500 ); // If not used, somethimes following command fails
  ESP_ERROR_CHECK( esp_wifi_set_protocol( WIFI_IF_AP, WIFI_PROTOCOL_LR ) );
  WiFi.mode( WIFI_AP );
#endif /* LONG_RANGE */

  //Set device in STA mode to begin with
  WiFi.softAPConfig(IPAddress(192, 168, 4, 1),
                    IPAddress(192, 168, 4, 1),
                    IPAddress(255, 255, 255, 0));

  WiFi.softAP(str.c_str(), password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Udp1.begin(1003);
  RoverInit();
}

void loop()
{
  count++;
  
  int udplength = Udp1.parsePacket();
  if(udplength == 12) {
    char udodata[udplength];
    Udp1.read( udodata, udplength);
    IPAddress udp_client = Udp1.remoteIP();
    if ((udodata[0] == 0xAA ) && ( udodata[1] == 0x55 ) && ( udodata[11] == 0xee )) {
#ifdef DEBUG
      Serial.printf("#%03d ", udodata[3]);      
      for (int i = 4; i < 11; i++) {
        Serial.printf("%02X ", udodata[i]);
      }
      Serial.println();    
#endif      
      ProcessingCommand(udodata[4], udodata[5], udodata[6], udodata[7], udodata[8], udodata[9], udodata[10]);
      received = true;
    }
  } else {
    delay(10);
  }

  if(count > 100) {
    if(received) {
      M5.dis.drawpix(0, 0xf00000); 
    } else {
      ProcessingDisconnect();
      M5.dis.drawpix(0, 0x00f000);
    }
    count = 0;
    received = false;
  }
}
