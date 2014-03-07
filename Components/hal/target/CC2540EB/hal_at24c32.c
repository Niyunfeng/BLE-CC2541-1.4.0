#include "hal_at24c32.h"
#include "hal_i2c.h"
#include "hal_sensor.h"
#include "hal_lcd.h"

static void HalAT24C32Select(void);

void HalAT24C32Write(uint16 addr, uint8 *pBuf, uint8 nBytes,uint8 NumToWrite) {
	//bool success;
	HalAT24C32Select();
	//success = HalWriteReg( addr,  pBuf, nBytes, NumToWrite);//HalSensorWriteReg( addr, pBuf, nBytes,);//HalLongAddrSensorWriteReg(addr, pBuf, nBytes);// 
        HalWriteReg( addr,  pBuf, nBytes, NumToWrite);
	//return success;
}

void HalAT24C32Read(uint16 addr, uint8 *pBuf, uint8 nBytes,uint8 NumToRead) {
	//bool success;
	HalAT24C32Select();
	// Read the sensor registers
	//success = HalReadReg(addr,  pBuf, nBytes, NumToRead);// HalSensorReadReg( addr, pBuf, nBytes,);//HalLongAddrSensorReadReg(addr, pBuf, nBytes);
         HalReadReg(addr,  pBuf, nBytes, NumToRead);
	//return success;
}

static void HalAT24C32Select(void) {
	// Select slave and set clock rate
	HalI2CInit(0x50, i2cClock_267KHZ);
       
}
