/*
 * SimpleEepromUtils.c
 *
 *  Created on: 2014-1-15
 *      Author: zekezang
 */

#include "SimpleEepromUtils.h"
#include "hal_eeprom.h"

bool StructSizeSave(uint8 size) {
	Hal_HW_WaitUs(1000);
	return HalEepromWrite(StructSizeIICAddr, &size, 1);;
}

void StructSizeRead(uint8 *size){
	Hal_HW_WaitUs(1000);
	HalEepromRead(StructSizeIICAddr, size, 1);
}
