#ifndef HAL_SENSOR_H
#define HAL_SENSOR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "hal_types.h"

void Hal_HW_WaitUs(uint16 microSecs);

bool HalSensorReadReg(uint8 addr, uint8 *pBuf, uint8 nBytes);
bool HalSensorWriteReg(uint8 addr, uint8 *pBuf, uint8 nBytes);

bool HalLongAddrSensorReadReg(uint16 addr, uint8 *pBuf, uint8 nBytes);
bool HalLongAddrSensorWriteReg(uint16 addr, uint8 *pBuf, uint8 nBytes);

#ifdef __cplusplus
}
#endif

#endif /* HAL_SENSOR_H */
