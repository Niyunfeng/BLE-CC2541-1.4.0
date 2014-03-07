#include "OSAL.h"
#include "hal_ds1307.h"
#include "hal_i2c.h"
#include "hal_sensor.h"
#include "hal_lcd.h"

static void HalDS1307Select(void);
static char hex2char(uint8 i);
static uint8 char2hex(char c);
static void showDate(uint8 *timeDate);

/**
 * time : (20) 140116144026 -> 2014-01-16 14:40:26 len:14
 */
bool HalDS1307WriteTimeData(char *time) {
	bool success;
	uint8 tmp = 0, k = 0;
	HalDS1307Select();

	for (k = 0; k < 7; k++) {
		tmp = 0;
		if (k < 3) {
			tmp = (char2hex(*(time + 12 - 2 * k)) << 4) | char2hex(*(time + 13 - 2 * k));
			success = HalSensorWriteReg(k, &tmp, 1);
		} else if (k == 3) {
			success = HalSensorWriteReg(k, &tmp, 1);
		} else if (k > 3) {
			tmp = (char2hex(*(time + 14 - 2 * k)) << 4) | char2hex(*(time + 15 - 2 * k));
			success = HalSensorWriteReg(k, &tmp, 1);
		} else {
		}
		if (!success) {
			break;
		}
	}
	return success;
}

uint32 timeTotalCount(uint8 *timeDate) {
	uint32 i = 0;
	i += timeDate[0] && 0x7F; //seconds
	i += (timeDate[1] && 0x7F) * 60; //minutes
	i += (timeDate[1] && 0x7F) * 60; //minutes

	return i;
}

bool HalDS1307ReadTimeData(uint8 *timeDate) {
	bool success;
	HalDS1307Select();
	success = HalSensorReadReg(0x00, timeDate, 7);
	showDate(timeDate);
	return success;
}

bool HalDS1307InitControlRegister() {
	bool success;
	HalDS1307Select();
	uint8 tmp = 0;
	tmp = 0x90; //1001 0000
	success = HalSensorWriteReg(0x07, &tmp, 1);
	return success;
}

bool HalDS1307Write(uint8 addr, uint8 *pBuf, uint8 nBytes) {
	bool success;
	HalDS1307Select();
	success = HalSensorWriteReg(addr, pBuf, nBytes);
	return success;
}

bool HalDS1307Read(uint8 addr, uint8 *pBuf, uint8 nBytes) {
	bool success;
	HalDS1307Select();
	success = HalSensorReadReg(addr, pBuf, nBytes);
	return success;
}

static void HalDS1307Select(void) {
	// Select slave and set clock rate
	HalI2CInit(0x68, i2cClock_533KHZ);
}

static void showDate(uint8 *timeDate) {
	char datestr[30];
	osal_memcpy(datestr, "20", 2);
	char str[3];
	str[2] = '\0';
	str[0] = hex2char((timeDate[6] >> 4) & 0x0F);
	str[1] = hex2char(timeDate[6] & 0x0F);
	osal_memcpy(datestr + 2, str, 2);

	str[0] = hex2char((timeDate[5] >> 4) & 0x01);
	str[1] = hex2char(timeDate[5] & 0x0F);
	osal_memcpy(datestr + 4, str, 2);

	str[0] = hex2char((timeDate[4] >> 4) & 0x03);
	str[1] = hex2char(timeDate[4] & 0x0F);
	osal_memcpy(datestr + 6, str, 2);

	str[0] = hex2char((timeDate[2] >> 4) & 0x01);
	str[1] = hex2char(timeDate[2] & 0x0F);
	osal_memcpy(datestr + 8, str, 2);

	str[0] = hex2char((timeDate[1] >> 4) & 0x07);
	str[1] = hex2char(timeDate[1] & 0x0F);
	osal_memcpy(datestr + 10, str, 2);

	str[0] = hex2char((timeDate[0] >> 4) & 0x07);
	str[1] = hex2char(timeDate[0] & 0x0F);
	osal_memcpy(datestr + 12, str, 2);

	datestr[14] = '\0';
	HalLcdWriteString(datestr, HAL_LCD_LINE_7);
}

static char hex2char(uint8 i) {
	switch (i) {
	case 0:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';
	default:
		return '0';
	}
}

static uint8 char2hex(char c) {
	switch (c) {
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	default:
		return 0;
	}
}
