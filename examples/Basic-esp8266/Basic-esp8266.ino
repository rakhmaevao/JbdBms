/*
 *  if the esp8266 board has usb to serial converter on the board, the hardware serial wont work, software serial must be used. I used ESP-01S.
 *  BMS may have 5v or 3.3v TTL level. Logic level shifter must install when BMS is 5v.
 *  wiring:
 *  [bms]   [step down to 3.3v]      [ESP-01S]
 *  VCC ----->  VIN+    ------------> 3V3
 *  TX  ----------------------------> RX
 *  RX  ----------------------------> TX
 *  GND  ---->  VIN-    ------------> GND
 * 
 *  Please fill the wifi settings.
 *  Run web browser http://IP
 * 
 */

const char* ssid     = "NAME";  // WIFI SID
const char* password = "MASSWORD"; // WIFI password


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <JbdBms.h>



ESP8266WebServer server(80);
JbdBms myBms(&Serial);
unsigned long SerialLastLoad = 0;
packCellInfoStruct cellInfo;
float BatteryTemp1,BatteryTemp2,BatteryChargePercetage,BatteryCurrent,BatteryVoltage;
int BatterycycleCount;

void setup() {
    WiFi.mode(WIFI_STA);
    WiFi.hostname("BMS");
    WiFi.begin(ssid, password);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      delay(5000);
      ESP.restart();
    }
    server.on("/", HttpTest);
    server.onNotFound(HttpTest);
    server.begin();
}

void loop() {
  server.handleClient();
  if (millis() - SerialLastLoad >= 1000) {
    SerialLastLoad = millis();
    if (myBms.readBmsData() == true)
    {
      BatteryCurrent = myBms.getCurrent()/1000.0f;
      BatteryChargePercetage = myBms.getChargePercentage();
      BatteryTemp1 = myBms.getTemp1();
      BatteryTemp2 = myBms.getTemp2();
      BatterycycleCount = myBms.getCycle();
      BatteryVoltage = myBms.getVoltage();
      if (myBms.readPackData()== true)
      {
        cellInfo = myBms.getPackCellInfo();
      }
    }
  }
}

String GetBatteryVoltageAll() {
  String R = "";
  R.reserve(300);
  for(byte i=0; i < cellInfo.NumOfCells ; i++) {
    R += String( cellInfo.CellVoltage[i] / 1000.0f );
    R += "V,";
  }
  return R;
}

void HttpTest() {
  String R = "";
  R.reserve(300);
  R += "BatteryTemp1:";
  R += String(BatteryTemp1);
  R += "&#8451;<br /> \n";
  R += "BatteryTemp2:";
  R += String(BatteryTemp2);
  R += "&#8451;<br /> \n";
  R += "BatteryChargePercetage:";
  R += String(BatteryChargePercetage,2);
  R += "%<br /> \n";
  R += "BatteryCurrent:";
  R += String(BatteryCurrent);
  R += "A<br /> \n";
  R += "BatterycycleCount:";
  R += String(BatterycycleCount);
  R += "<br /> \n";
  R += "BatterygetVoltage:";
  R += String(BatteryVoltage);
  R += "V<br /> \n";
  R += "Battery list:";
  for(byte i=0; i < cellInfo.NumOfCells ; i++) {
    R += String( cellInfo.CellVoltage[i] / 1000.0f );
    R += "V,";
  }
  R += "<br /> \n";
  R += "Battery diff:";
  R += String(cellInfo.CellDiff);
  R += "mv<br /> \n";
  server.send(200, "text/html", R );
}
