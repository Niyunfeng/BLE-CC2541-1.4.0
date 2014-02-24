#ifndef HAL_AT24C32_H
#define HAL_AT24C32_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "comdef.h"
#include "hal_sensor.h"

void HalAT24C32Read(uint16 addr, uint8 *pBuf, uint8 nBytes,uint8 NumToWrite);
void HalAT24C32Write(uint16 addr, uint8 *pBuf, uint8 nBytes,uint8 NumToRead);

#ifdef __cplusplus
}
#endif

#endif /* HAL_AT24C32_H */
