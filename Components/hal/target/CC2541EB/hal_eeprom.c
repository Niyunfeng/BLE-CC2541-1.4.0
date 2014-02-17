/**************************************************************************************************
 Filename:       hal_Eeprom.c
 Revised:        $Date: 2013-04-05 07:25:57 -0700 (Fri, 05 Apr 2013) $
 Revision:       $Revision: 33773 $

 Description:    Driver for the TI TMP06 Eepromrared thermophile sensor.


 Copyright 2012-2013 Texas Instruments Incorporated. All rights reserved.

 IMPORTANT: Your use of this Software is limited to those specific rights
 granted under the terms of a software license agreement between the user
 who downloaded the software, his/her employer (which must be your employer)
 and Texas Instruments Incorporated (the "License").  You may not use this
 Software unless you agree to abide by the terms of the License. The License
 limits your use, and you acknowledge, that the Software may not be modified,
 copied or distributed unless embedded on a Texas Instruments microcontroller
 or used solely and exclusively in conjunction with a Texas Instruments radio
 frequency transceiver, which is integrated into your product.  Other than for
 the foregoing purpose, you may not use, reproduce, copy, prepare derivative
 works of, modify, distribute, perform, display or sell this Software and/or
 its documentation for any purpose.

 YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
 PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
 NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
 TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
 NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
 LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
 INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
 OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
 OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
 (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

 Should you have any questions regarding your right to use this Software,
 contact Texas Instruments Incorporated at www.TI.com.
 **************************************************************************************************/

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include "hal_eeprom.h"
#include "hal_i2c.h"
#include "hal_sensor.h"

/* ------------------------------------------------------------------------------------------------
 *                                           Local Functions
 * ------------------------------------------------------------------------------------------------
 */
static void HalEepromSelect(void);


/**************************************************************************************************
 * @fn          HalINFTurnOn
 *
 * @brief       Turn the sensor on
 *
 * @return      none
 **************************************************************************************************/
void HalEepromTurnOn(void) {
	HalEepromSelect();
}

/**************************************************************************************************
 * @fn          HalEepromTurnOff
 *
 * @brief       Turn the sensor off
 *
 * @return      none
 **************************************************************************************************/
void HalEepromTurnOff(void) {
	HalEepromSelect();
}

/**************************************************************************************************
 * @fn          HalEepromWrite
 *
 * @brief       Read the sensor voltage and sensor temperature registers
 *
 * @param       Voltage and temperature in raw format (2 + 2 bytes)
 *
 * @return      TRUE if valid data
 **************************************************************************************************/
bool HalEepromWrite(uint8 addr, uint8 *pBuf, uint8 nBytes) {
	bool success;
	HalEepromTurnOn();
	success = HalSensorWriteReg(addr, pBuf, nBytes);
	return success;
}

/**************************************************************************************************
 * @fn          HalEepromRead
 *
 * @brief       Read the sensor voltage and sensor temperature registers
 *
 * @param       Voltage and temperature in raw format (2 + 2 bytes)
 *
 * @return      TRUE if valid data
 **************************************************************************************************/
bool HalEepromRead(uint8 addr, uint8 *pBuf, uint8 nBytes) {
	bool success;
	HalEepromTurnOn();
	// Read the sensor registers
	success = HalSensorReadReg(addr, pBuf, nBytes);
	return success;
}

/* ------------------------------------------------------------------------------------------------
 *                                           Private functions
 * -------------------------------------------------------------------------------------------------
 */

/**************************************************************************************************
 * @fn          HalEepromSelect
 *
 * @brief       Select the TMP006 slave and set the I2C bus speed
 *
 * @return      none
 **************************************************************************************************/
static void HalEepromSelect(void) {
	// Select slave and set clock rate
	HalI2CInit(0x50, i2cClock_533KHZ);
}
