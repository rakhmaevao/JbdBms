# JbdBms library for Arduino

This is a library for working with battery manager system JBD.
See [my project in hack a day](https://hackaday.io/project/162806-jbd-bms-protocol).

## Hardware

Connect UART pins to an Arduino board DIGITAL pins:
* RX
* TX
* GND (Ground)

Attention! UART GND circuit is electrically isolated from the power GND BMS.

## Usage

### Initialization

```c++
#include "JbdBms.hpp"

JbdBms myBms(6,7); // RX, TX

void setup()
{
  myBms.begin();
}
```
### Reading data from BMS

```c++
if (myBms.readBmsData()== true)
{
  // use the get-functions
}
```
After calling `readBmsData ()` request is sent to the BMS.
In case of a successful read response, the method returns `true`.

You can work with the data obtained using the get-functions.

### Get-functions
### Charge percentage
```c++
myBms.getChargePercentage();
```
This method return the float value.

### Consumption current
```c++
myBms.getCurrent();
```
This method return the float value.

### Protection state
```c++
myBms.getProtectionState();
```
This method return the uint16_t value.

If this value is not 0, then BMS detected some errors. You can check this value on the mask by using the following error values:
```c++
#define BMS_STATUS_OK				0
#define BMS_STATUS_CELL_OVP			1
#define BMS_STATUS_CELL_UVP			2		///< Power off
#define BMS_STATUS_PACK_OVP			4
#define BMS_STATUS_PACK_UVP			8		///< Power off
#define BMS_STATUS_CHG_OTP			16
#define BMS_STATUS_CHG_UTP			32
#define BMS_STATUS_DSG_OTP			64		///< Power off
#define BMS_STATUS_DSG_UTP			128		///< Power off
#define BMS_STATUS_CHG_OCP			256
#define BMS_STATUS_DSG_OCP			512		///< Power off
#define BMS_STATUS_SHORT_CIRCOUT	1024	///< Power off
#define BMS_STATUS_AFE_ERROR		2048
#define BMS_STATUS_SOFT_LOCK		4096
#define BMS_STATUS_CHGOVERTIME		8192
#define BMS_STATUS_DSGOVERTIME		16384	///< Power off
#define BMS_POWER_OFF_ERRORS		0x46CA
```
Some errors lead to power down (labeled "Power off"). If you want to read them a device that is powered from the battery, it will not work.
