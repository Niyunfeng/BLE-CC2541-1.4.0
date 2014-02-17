#include "hal_at24c32.h"
#include "hal_i2c.h"
#include "hal_sensor.h"

static void HalAT24C32Select(void);

bool HalAT24C32Write(uint16 addr, uint8 *pBuf, uint8 nBytes) {
	bool success;
	HalAT24C32Select();
	success = HalLongAddrSensorWriteReg(addr, pBuf, nBytes);
	return success;
}

bool HalAT24C32Read(uint16 addr, uint8 *pBuf, uint8 nBytes) {
	bool success;
	HalAT24C32Select();
	// Read the sensor registers
	success = HalLongAddrSensorReadReg(addr, pBuf, nBytes);
	return success;
}

static void HalAT24C32Select(void) {
	// Select slave and set clock rate
	HalI2CInit(0x50, i2cClock_267KHZ);
}
