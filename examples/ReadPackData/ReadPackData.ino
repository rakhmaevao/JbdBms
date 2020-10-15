#include <SoftwareSerial.h>

#include "JbdBms.h"

// Initialization examples
// JbdBms myBms(&Serial2);
// SoftwareSerial mySerial(10, 11);
// JbdBms myBms(&mySerial); //RX, TX SoftwareSerial
JbdBms myBms(10, 11);

void setup()
{
  Serial.begin(115200);
  Serial.println("JBD bms driver");
}

void loop()
{
if (myBms.readBmsData()== true)
    {
      Serial.print("This capacity: "); Serial.println(myBms.getChargePercentage());
      Serial.print("Current: "); Serial.println(myBms.getCurrent());
      Serial.print("Voltage: "); Serial.println(myBms.getVoltage());
      Serial.print("Protection state: "); Serial.println(myBms.getProtectionState());
      Serial.print("Cycle: "); Serial.println(myBms.getCycle());
      Serial.print("Temp1: "); Serial.println(myBms.getTemp1());
      Serial.print("Temp2: "); Serial.println(myBms.getTemp2());

      Serial.println();
    } else {
      Serial.println("Communication error");
    }

    if (myBms.readPackData()== true)
    {
      packCellInfoStruct packInfo = myBms.getPackCellInfo();

      Serial.print("Number Of Cell: "); Serial.println(packInfo.NumOfCells);
      Serial.print("Low: "); Serial.println(packInfo.CellLow);
      Serial.print("High: "); Serial.println(packInfo.CellHigh);
      Serial.print("Diff: "); Serial.println(packInfo.CellDiff);
      Serial.print("Avg: "); Serial.println(packInfo.CellAvg);


      //go trough individual cells
      for (byte i = 0; i < packInfo.NumOfCells; i++)
      {
        Serial.print("Cell"); Serial.print(i+1); Serial.print(": "); Serial.println(packInfo.CellVoltage[i]);
      }
      Serial.println();
    } else {
      Serial.println("Communication error");
    }
  delay(1000);
}
