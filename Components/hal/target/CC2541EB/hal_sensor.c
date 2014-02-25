#include "hal_sensor.h"
#include "hal_i2c.h"
#include "hal_lcd.h"

static uint8 buffer[24];
static uint8 addrMSB = 0;
static uint8 addrLSB = 0;
static uint8 addrBuf[2] = { 0 };

/**************************************************************************************************
 * @fn          HalSensorReadReg
 *
 * @brief       This function implements the I2C protocol to read from a sensor. The sensor must
 *              be selected before this routine is called.
 *
 * @param       addr - which register to read
 * @param       pBuf - pointer to buffer to place data
 * @param       nBytes - numbver of bytes to read
 *
 * @return      TRUE if the required number of bytes are reveived
 **************************************************************************************************/
bool HalSensorReadReg(uint8 addr, uint8 *pBuf, uint8 nBytes) {
	uint8 i = 0;

	/* Send address we're reading from */
	if (HalI2CWrite(1, &addr) == 1) {
		/* Now read data */
		i = HalI2CRead(nBytes, pBuf);
	}

	return i == nBytes;
}

/**************************************************************************************************
 * @fn          HalSensorWriteReg
 * @brief       This function implements the I2C protocol to write to a sensor. he sensor must
 *              be selected before this routine is called.
 *
 * @param       addr - which register to write
 * @param       pBuf - pointer to buffer containing data to be written
 * @param       nBytes - number of bytes to write
 *
 * @return      TRUE if successful write
 */
bool HalSensorWriteReg(uint8 addr, uint8 *pBuf, uint8 nBytes) {
	uint8 i;
	uint8 *p = buffer;

	/* Copy address and data to local buffer for burst write */
	*p++ = addr;
	for (i = 0; i < nBytes; i++) {
		*p++ = *pBuf++;
	}
	nBytes++;

	/* Send address and data */
	i = HalI2CWrite(nBytes, buffer);
	if (i != nBytes) {
	}
	//HAL_TOGGLE_LED2();

	return (i == nBytes);       
        
}

/**************************************************************************************************
 * @fn          HalLongAddrSensorReadReg
 *
 * @brief       This function implements the I2C protocol to read from a sensor. The sensor must
 *              be selected before this routine is called.
 *
 * @param       uint16 addr - which register to read
 * @param       pBuf - pointer to buffer to place data
 * @param       nBytes - numbver of bytes to read
 *
 * @return      TRUE if the required number of bytes are reveived
 **************************************************************************************************/
bool HalLongAddrSensorReadReg(uint16 addr, uint8 *pBuf, uint8 nBytes) {
	uint8 i = 0;
	addrBuf[0] = addr >> 8;
	addrBuf[1] = addr & 0xFF;
	/* Send address we're reading from */
	if (HalI2CWrite(2, addrBuf) == 2) {
		/* Now read data */
		i = HalI2CRead(nBytes, pBuf);
	}

	return i == nBytes;
        
        
       
}

/**************************************************************************************************
 * @fn          HalLongAddrSensorWriteReg
 * @brief       This function implements the I2C protocol to write to a sensor. he sensor must
 *              be selected before this routine is called.
 *
 * @param       uint16 addr - which register to write
 * @param       pBuf - pointer to buffer containing data to be written
 * @param       nBytes - number of bytes to write
 *
 * @return      TRUE if successful write
 */
bool HalLongAddrSensorWriteReg(uint16 addr, uint8 *pBuf, uint8 nBytes) {
	uint8 i;
	uint8 *p = buffer;
        addrMSB = addr >> 8;
	addrLSB = addr & 0xFF;

	/*Copy address and data to local buffer for burst write*/ 
	*p++ = addrMSB;
	*p++ = addrLSB;
	for (i = 0; i < nBytes; i++) {
		*p++ = *pBuf++;
	}
	nBytes++;
	nBytes++;

	/* Send address and data */
	i = HalI2CWrite(nBytes, buffer);
	if (i != nBytes) {
	}
	//HAL_TOGGLE_LED2();

	return (i == nBytes);
       
}

void Hal_HW_WaitUs(uint16 microSecs) {
	while (microSecs--) {
		/* 32 NOPs == 1 usecs */
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
	}
}


/**************************************************************************************************
 * @fn          HalSensorWriteReg
 * @brief       This function implements the I2C protocol to write to a sensor. he sensor must
 *              be selected before this routine is called.
 *
 * @param       addr - which register to write
 * @param       pBuf - pointer to buffer containing data to be written
 * @param       nBytes - number of bytes to write
 *
 * @return      TRUE if successful write
 */
void HalWriteReg(uint8 addr, uint8 *pBuf, uint8 nBytes,uint8 NumToWrite) {
  
        
  uint8 *p;
  
  while(NumToWrite--)
	{
		*p=*pBuf++;
                HalSensorWriteReg(addr,p,nBytes);
		addr++;
		
	}
	  
        
}


/**************************************************************************************************
 * @fn          HalSensorReadReg
 *
 * @brief       This function implements the I2C protocol to read from a sensor. The sensor must
 *              be selected before this routine is called.
 *
 * @param       addr - which register to read
 * @param       pBuf - pointer to buffer to place data
 * @param       nBytes - numbver of bytes to read
 *
 * @return      TRUE if the required number of bytes are reveived
 **************************************************************************************************/
void HalReadReg(uint8 addr, uint8 *pBuf, uint8 nBytes,uint8 NumToRead) {
	
  	while(NumToRead)
	{
		
                HalSensorReadReg(addr, pBuf, nBytes);
                HalLcdWriteString((uint8*)pBuf, HAL_LCD_LINE_5);
                Hal_HW_WaitUs(5000000);
                //while(1);
                addr++;
		NumToRead--;       
	}
  
}








