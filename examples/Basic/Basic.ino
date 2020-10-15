#include "JbdBms.h"

JbdBms myBms(6,7); // RX, TX

void setup()
{
  Serial.begin(9600);
  Serial.println("JBD bms driver");
}

void loop()
{
  if (myBms.readBmsData()== true)
  {
    Serial.print("This capacity: "); Serial.println(myBms.getChargePercentage());
    Serial.print("This current: "); Serial.println(myBms.getCurrent());
    Serial.print("Protection state: ");Serial.println(myBms.getProtectionState());
    Serial.println();
  } else {
    Serial.println("Communication error");
  }
  delay(1000);
}
