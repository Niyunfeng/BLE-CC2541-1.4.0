#ifndef HAL_DS1307_H
#define HAL_DS1307_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "comdef.h"
#include "hal_sensor.h"

bool HalDS1307WriteTimeData(char *time);
bool HalDS1307ReadTimeData(uint8 *timeDate);
bool HalDS1307InitControlRegister();

#ifdef __cplusplus
}
#endif

#endif /* HAL_DS1307_H */
