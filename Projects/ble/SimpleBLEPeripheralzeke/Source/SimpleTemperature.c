#include "SimpleTemperature.h"
#include<ioCC2541.h>
#include "hal_types.h"


#define DISABLE_ALL_INTERRUPTS() (IEN0 = IEN1 = IEN2 = 0x00)//三个   

/**************************************************************** 
                 温度传感器初始化函数        
****************************************************************/ 
void initTempSensor(void)
{ 
   DISABLE_ALL_INTERRUPTS();            //关闭所有中断 
   //InitClock();                         //设置系统主时钟为 32M 
   TR0=0x01;         //set '1' to connectthe temperature sensorto the SOC_ADC.
   ATEST=0x01;       // Enablesthe temperature sensor
}   

/**************************************************************** 
读取温度传感器 AD 值函数       
****************************************************************/ 
uint8 getTemperature(void){ 
   
   uint16  value;
   float v ;
   ADCCON3  = (0x3E);                  //选择1.25V为参考电压；14位分辨率；对片内温度传感器采样        
   while(!ADCIF);                //等待 AD 转换完成 
   value =  ADCL ;                //ADCL 寄存器低 2 位无效 
   value |= (((uint16)ADCH) << 8);
   value=value>>4;
   
   v=(value-1367.5)/4.5;    //根据 AD 值，计算出实际的温度,芯片手册有错，温度系数应该是4.5 /℃
   return   v;                           //进行温度校正，这里减去5℃（不同芯片根据具体情况校正）

}