// Atom PINOUT
// EXT G19 
// EXT G22 
// EXT G23, G34 (ADC1 CH6)
// EXT G33 (ADC1 CH5)

// EXT G21 (SCL) IMU 
// EXT G25 (SDA) IMU (DAC1)
// 
// GROVE G32 
// GROVE G26 
// 
// G27 - Neopixel
// G39 - Button

#include "M5Atom.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>

//#define DEBUG

extern void ControlInit();
extern void ProcessingCommand(int lx, int ly, int rx, int ry, uint8_t btn, uint8_t gx, uint8_t gy);
extern void ProcessingDisconnect();

const char *ssid = "M5AP";
const char *password = "77777777";

#define CTL_MESSAGE_LEN 12
#define GREEN 0x2589

#define BATTERY_CHECK 

WiFiServer server(80);
WiFiUDP Udp1;
IPAddress remoteIP;
uint16_t remotePort;
uint16_t count = 0;
bool received = false;
uint8_t txCnt = 0;

void setup()
{
  M5.begin(true, false, true);

  delay(50);
  M5.update();
  Wire.begin(0, 26, 10000);
  M5.dis.drawpix(0, 0x00f000);

  uint64_t chipId = ESP.getEfuseMac();
  String str = ssid + String((uint32_t)(chipId >> 32), HEX);

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
  ControlInit();
}

void sendMessage(uint8_t event_id, uint16_t color, char* text) {
  Serial.printf("msg: %s\n", text);
  Udp1.beginPacket(remoteIP, remotePort);
  Udp1.write(0xAA);
  Udp1.write(0x55);
  Udp1.write(txCnt++);
  Udp1.write(event_id);
  Udp1.write(color & 0xFF);
  Udp1.write((color >> 8) & 0xFF);  
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
      ProcessingCommand(norm(rxdata[4]), norm(rxdata[5]), norm(rxdata[6]), norm((int)rxdata[7]), rxdata[8], rxdata[9], rxdata[10]);
      received = true;
    }
  } else {
    delay(10);
  }
  
  if(M5.Btn.read() > 0) {
    sendMessage(1, GREEN, "button pressed");  
    while(M5.Btn.read() > 0) {
      delay(10);
    }
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
