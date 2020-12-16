#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <esp_wifi.h>

#include "EEPROM.h"
#include "JoyC.h"

enum {
  L_X_OFFSET = 4,   
  L_Y_OFFSET,   
  R_X_OFFSET,   
  R_Y_OFFSET,   
  BTN_OFFSET,   
  G_X_OFFSET,   
  G_Y_OFFSET   
};

#define EEPROM_SIZE 128
#define EPROM_CALLIBATION_OFFSET 64

TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);
extern const unsigned char connect_on[800];
extern const unsigned char connect_off[800];

#define SYSNUM 		3

JoyC joyc;

uint64_t realTime[4], time_count = 0;
bool k_ready = false;
uint32_t key_count = 0;
bool connected = false;

IPAddress local_IP(192, 168, 4, 100 + SYSNUM );
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

const char *ssid = "M5AP";
const char *password = "77777777";

WiFiUDP Udp;

uint8_t color[3] = {0, 100, 0};


uint8_t SendBuff[] = 	{ 0xAA, 0x55,
                          SYSNUM,
                          0x00,        // packet cnt
                          0x00, 0x00, // Left X,Y
                          0x00, 0x00, // Right X,Y
                          0x00,       // Left Btn * 2 + Right Btn
                          0x00,       // Pitch
                          0x00,       // Roll
                          0xee
                       };

void SendUDP()
{
  if ( WiFi.status() == WL_CONNECTED )
  {
    Udp.beginPacket(IPAddress(192, 168, 4, 1), 1000 + SYSNUM );
    Udp.write(SendBuff, sizeof(SendBuff));
    Udp.endPacket();
  }
}

char APName[20];
String WfifAPBuff[16];
uint32_t count_bn_a = 0, choose = 0;
String ssidname;
uint32_t count = 0;
int16_t lXOffset, lYOffset, rXOffset, rYOffset, gXOffet, gYOffset;

#define CALL_NUMB 5
 
void callibration() {
  int cnt = 0;
  Disbuff.fillRect(0, 20, 80, 140, BLACK);
  Disbuff.setTextColor(RED);
  Disbuff.setCursor(20, 7);
  Disbuff.printf("%s", "Calib");
  Disbuff.pushSprite(0, 0);
  do {
    delay(100);       
  } while(M5.BtnB.read() == 1);
  delay(2000);
  while(cnt++ <= CALL_NUMB) {
    readData();
    lXOffset += 100 - SendBuff[L_X_OFFSET];
    lYOffset += 100 - SendBuff[L_Y_OFFSET];
    rXOffset += 100 - SendBuff[R_X_OFFSET];
    rYOffset += 100 - SendBuff[R_Y_OFFSET];
    gXOffet  += 100 - SendBuff[G_X_OFFSET];
    gYOffset += 100 - SendBuff[G_Y_OFFSET];
    displayData();
    delay(1000);       
    Disbuff.fillRect(0, 0, 10, 20, BLACK);
    Disbuff.setTextColor(RED);
    Disbuff.setCursor(3, 7);
    Disbuff.printf("%d-", cnt);
    Disbuff.pushSprite(0, 0);
  }
  lXOffset /= CALL_NUMB;
  lYOffset /= CALL_NUMB;
  rXOffset /= CALL_NUMB;
  rYOffset /= CALL_NUMB;
  gXOffet  /= CALL_NUMB;
  gYOffset /= CALL_NUMB;
  int8_t calBuf[6];
  
}

void setup() {

  lXOffset = lYOffset = rXOffset = rYOffset = gXOffet = gYOffset = 0;
  M5.begin();
  Wire.begin(0, 26, 400000);
  EEPROM.begin(EEPROM_SIZE);

  M5.Lcd.setRotation(4);
  M5.Lcd.setSwapBytes(false);
  Disbuff.createSprite(80, 160);
  Disbuff.setSwapBytes(true);

  M5.update();
  M5.IMU.Init();
  if(M5.BtnB.read() == 1) {
    callibration();   
  }
  
  Disbuff.fillRect(0, 0, 80, 20, Disbuff.color565(50, 50, 50));
  Disbuff.fillRect(0, 20, 80, 140, BLACK);
  Disbuff.setTextSize(1);
  Disbuff.setTextColor(GREEN);
  Disbuff.setCursor(55, 6);

  Disbuff.pushImage(0, 0, 20, 20, (uint16_t *)connect_off);
  Disbuff.pushSprite(0, 0);

  if (( EEPROM.read(0) != 0x56 ) || ( M5.BtnA.read() == 1 ))
  {
#ifdef LONG_RANGE
    WiFi.begin();
    delay( 500 ); // If not used, somethimes following command fails
    esp_wifi_set_protocol( WIFI_IF_STA, WIFI_PROTOCOL_LR );
#endif
    WiFi.mode(WIFI_STA);
    int n = WiFi.scanNetworks();
    Disbuff.setTextSize(1);
    Disbuff.setTextColor(GREEN);
    Disbuff.fillRect(0, 0, 80, 20, Disbuff.color565(50, 50, 50));

    if (n == 0) {
      Disbuff.setCursor(5, 20);
      Disbuff.printf("no networks");

    }
    else {
      int count = 0;
      for (int i = 0; i < n; ++i) {
        if ( WiFi.SSID(i).indexOf("M5AP") != -1 )
        {
          if ( count == 0 )
          {
            Disbuff.setTextColor(GREEN);
          }
          else
          {
            Disbuff.setTextColor(WHITE);
          }
          Disbuff.setCursor(5, 25 + count * 10 );
          String str = WiFi.SSID(i);
          Disbuff.printf(str.c_str());
          WfifAPBuff[count] = WiFi.SSID(i);
          count++;
        }
      }
      Disbuff.pushSprite(0, 0);
      while ( 1 )
      {
        if ( M5.BtnA.read() == 1 )
        {
          if ( count_bn_a >= 200 )
          {
            count_bn_a = 201;
            EEPROM.writeUChar(0, 0x56);
            EEPROM.writeString(1, WfifAPBuff[choose]);
            EEPROM.commit();
            ssidname = WfifAPBuff[choose];
            break;
          }
          count_bn_a ++;
          Serial.printf("count_bn_a %d \n", count_bn_a );
        }
        else if (( M5.BtnA.isReleased()) && ( count_bn_a != 0 ))
        {
          Serial.printf("count_bn_a %d", count_bn_a);
          if ( count_bn_a > 200 )
          {

          }
          else
          {
            choose ++;
            if ( choose >= count )
            {
              choose = 0;
            }
            Disbuff.fillRect(0, 0, 80, 20, Disbuff.color565(50, 50, 50));
            for (int i = 0; i < count; i++)
            {
              Disbuff.setCursor(5, 25 + i * 10 );
              if ( choose == i )
              {
                Disbuff.setTextColor(GREEN);
              }
              else
              {
                Disbuff.setTextColor(WHITE);
              }
              Disbuff.printf(WfifAPBuff[i].c_str());
            }
            Disbuff.pushSprite(0, 0);
          }
          count_bn_a = 0;
        }
        delay(10);
        M5.update();
      }
     }
  }
  else if ( EEPROM.read(0) == 0x56 )
  {
    ssidname = EEPROM.readString(1);
    EEPROM.readString(1, APName, 16);
  }

  Disbuff.fillRect(0, 20, 80, 140, BLACK);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssidname.c_str(), password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Udp.begin(2000);

  Disbuff.pushImage(0, 0, 20, 20, (uint16_t *)connect_on);
  Disbuff.pushSprite(0, 0);
}

void readData() {
    SendBuff[L_X_OFFSET] = joyc.GetX(0);
    SendBuff[L_Y_OFFSET] = joyc.GetY(0);
    SendBuff[R_X_OFFSET] = joyc.GetX(1);
    SendBuff[R_Y_OFFSET] = joyc.GetY(1);
    SendBuff[BTN_OFFSET] = M5.BtnB.read() * 8 + M5.BtnA.read() * 4 + joyc.GetPress(0) * 2 + joyc.GetPress(1);

    // IMU Calculation
    const double alpha = 0.2;
    float accX = 0;
    float accY = 0;
    float accZ = 0;
    double theta = 0;
    double phi = 0;
    static double last_theta = 0;
    static double last_phi = 0;
    
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    if ((accX < 1) && (accX > -1))
    {
        theta = asin(-accX) * 57.295;
    }
    if (accZ != 0)
    {
        phi = atan(accY / accZ) * 57.295;
    }
    theta = alpha * theta + (1 - alpha) * last_theta;
    phi = alpha * phi + (1 - alpha) * last_phi;
    last_theta = theta;
    last_phi = phi;

    SendBuff[G_X_OFFSET] =  (uint8_t)(100.0 + theta);
    SendBuff[G_Y_OFFSET] = (uint8_t)(100.0 - phi);


}

void displayData() {
  Disbuff.setTextColor(GREEN);
  Disbuff.fillRect( 0, 30, 80, 130, BLACK);
  Disbuff.setCursor(10, 30);
  Disbuff.printf("X0:%03d", SendBuff[L_X_OFFSET]);
  Disbuff.setCursor(10, 45);
  Disbuff.printf("Y0:%03d", SendBuff[L_Y_OFFSET]);
  Disbuff.setCursor(10, 60);
  Disbuff.printf("X1:%03d", SendBuff[R_X_OFFSET]);
  Disbuff.setCursor(10, 75);
  Disbuff.printf("Y1:%03d", SendBuff[R_Y_OFFSET]);
  Disbuff.setCursor(10, 90);
  Disbuff.printf("BTN:%02X", SendBuff[BTN_OFFSET]);

  Disbuff.setCursor(10, 105);
  Disbuff.printf("gX:%03d", SendBuff[G_X_OFFSET]);
  Disbuff.setCursor(10, 120);
  Disbuff.printf("gY:%03d", SendBuff[G_Y_OFFSET]);
}

void displayStatus() {
  Disbuff.setTextColor(GREEN);
  Disbuff.fillRect(20, 0, 80, 20, Disbuff.color565(50, 50, 50));
  Disbuff.setCursor(60, 7);
  if(connected) {
    Disbuff.printf("%03d", WiFi.RSSI());
  } else {
    Disbuff.printf("%03d", 0);      
  }
  Disbuff.setCursor(22, 7);
  if(M5.Axp.GetWarningLevel()) {
    Disbuff.setTextColor(RED);
  }
  Disbuff.printf("%.2fV", M5.Axp.GetBatVoltage());  
}

void loop() {
  delay(10);

  if ( WiFi.status() != WL_CONNECTED )
  {
    if(connected) {
      connected = false;
      Disbuff.pushImage(0, 0, 20, 20, (uint16_t *)connect_off);
      Disbuff.fillRect( 0, 30, 80, 130, BLACK);
      Disbuff.pushSprite(0, 0);
    }
    count ++;
    if ( count > 500 )
    {
      displayStatus();
      Disbuff.pushSprite(0, 0);
      WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
      count = 0;
    }
  }
  else
  {
    if(!connected) {
      connected = true;
      Disbuff.pushImage(0, 0, 20, 20, (uint16_t *)connect_on);
      Disbuff.pushSprite(0, 0);
    }
    SendBuff[3]++;
    readData();
    // add calibration data
    SendBuff[L_X_OFFSET] += lXOffset;
    SendBuff[L_Y_OFFSET] += lYOffset;
    SendBuff[R_X_OFFSET] += rXOffset;
    SendBuff[R_Y_OFFSET] += rYOffset;
    SendBuff[G_X_OFFSET] += gXOffet;
    SendBuff[G_X_OFFSET] += gYOffset;
    SendUDP();
  }

  displayStatus();
  displayData();   
  Disbuff.pushSprite(0, 0);
}
