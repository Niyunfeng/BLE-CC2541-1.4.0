/*
 * SimpleEepromUtils.h
 *
 *  Created on: 2014-1-15
 *      Author: zekezang
 */

#ifndef SIMPLEEEPROMUTILS_H_
#define SIMPLEEEPROMUTILS_H_

#include "bcomdef.h"
#include "OSAL.h"

#define StructSizeIICAddr 0x0A
#define StructDataIICAddr 0x0008

typedef struct InfraredDataStruct {
	uint32 timeSeconds;
	//uint8 data[200];
	char desc[10];
} InfraredDataStruct;


bool StructSizeSave(uint8 size);
void StructSizeRead(uint8 *size);

#endif /* SIMPLEEEPROMUTILS_H_ */
