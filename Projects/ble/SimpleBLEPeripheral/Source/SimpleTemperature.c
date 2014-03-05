#include "SimpleTemperature.h"
#include<ioCC2541.h>
#include "hal_types.h"
#include "hal_adc.h"

#define HAL_ADC_DEC_064     0x00    /* Decimate by 64 : 8-bit resolution */
#define HAL_ADC_DEC_128     0x10    /* Decimate by 128 : 10-bit resolution */
#define HAL_ADC_DEC_256     0x20    /* Decimate by 256 : 12-bit resolution */
#define HAL_ADC_DEC_512     0x30    /* Decimate by 512 : 14-bit resolution */
#define HAL_ADC_DEC_BITS    0x30    /* Bits [5:4] */
#define DISABLE_ALL_INTERRUPTS() (IEN0 = IEN1 = IEN2 = 0x00)//三个   

/**************************************************************** 
                 温度传感器初始化函数        
****************************************************************/ 
void initTempSensor(void)
{ 
   DISABLE_ALL_INTERRUPTS();            //关闭所有中断 
   //InitClock();                         //设置系统主时钟为 32M 
   TR0=0X01;         //set '1' to connectthe temperature sensorto the SOC_ADC.
   ATEST=0X01;       // Enablesthe temperature sensor
}   

/**************************************************************** 
读取温度传感器 AD 值函数       
****************************************************************/ 
uint8 getTemperature(void){ 
   
  uint16 value;

    ADCIF = 0;
    ADCCON3 = (HAL_ADC_REF_125V | HAL_ADC_DEC_128 | HAL_ADC_CHN_TEMP);
    while ( !ADCIF );
    value = ADCL;
    value |= ((uint16) ADCH) << 8;
    value = value >> 6;
    

    return ( (uint8)value ); 

}