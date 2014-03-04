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
   TR0=0X01;         //set '1' to connectthe temperature sensorto the SOC_ADC.
   ATEST=0X01;       // Enablesthe temperature sensor
}   

/**************************************************************** 
读取温度传感器 AD 值函数       
****************************************************************/ 
uint8 getTemperature(void){ 
   
  uint8 i;
  uint16 adcvalue;
  uint16 value;
  
  adcvalue=0;
  for(i=0;i<4;i++)
  {
    ADCCON3 |=0X0E;//单通道AD转换源为温度传感器
    ADCCON3 &=0X3F;//单通道AD转换参考电压为1.25内部电压
    ADCCON3 |=0X30;//单通道AD转换分辨率为512DEC，12有效
    ADCCON1 |=0X30;//ADC启动方式选择为ADCCON1.ST=1事件
    ADCCON1 |=0X40;//ADC启动转换
        
    while(!ADCCON1&0X80);//等待AD转换完成
    value =  ADCL >> 2; //ADCL寄存器低2位无效
    value |=(((uint16)ADCH)<<6);//连接ADCH和ADCL，并赋值给value
    adcvalue +=value;//adcvalue被赋值为4次AD值之和
  }
  value=adcvalue>>2;//累加除以4，得到平均值
  return ((value) >> 4) - 315;     //根据AD值，计算出实际的温度
}