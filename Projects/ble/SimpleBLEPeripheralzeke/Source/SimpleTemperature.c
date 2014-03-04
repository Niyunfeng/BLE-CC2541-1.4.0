#include "SimpleTemperature.h"
#include<ioCC2541.h>
#include "hal_types.h"


#define DISABLE_ALL_INTERRUPTS() (IEN0 = IEN1 = IEN2 = 0x00)//����   

/**************************************************************** 
                 �¶ȴ�������ʼ������        
****************************************************************/ 
void initTempSensor(void)
{ 
   DISABLE_ALL_INTERRUPTS();            //�ر������ж� 
   //InitClock();                         //����ϵͳ��ʱ��Ϊ 32M 
   TR0=0X01;         //set '1' to connectthe temperature sensorto the SOC_ADC.
   ATEST=0X01;       // Enablesthe temperature sensor
}   

/**************************************************************** 
��ȡ�¶ȴ����� AD ֵ����       
****************************************************************/ 
uint8 getTemperature(void){ 
   
  uint8 i;
  uint16 adcvalue;
  uint16 value;
  
  adcvalue=0;
  for(i=0;i<4;i++)
  {
    ADCCON3 |=0X0E;//��ͨ��ADת��ԴΪ�¶ȴ�����
    ADCCON3 &=0X3F;//��ͨ��ADת���ο���ѹΪ1.25�ڲ���ѹ
    ADCCON3 |=0X30;//��ͨ��ADת���ֱ���Ϊ512DEC��12��Ч
    ADCCON1 |=0X30;//ADC������ʽѡ��ΪADCCON1.ST=1�¼�
    ADCCON1 |=0X40;//ADC����ת��
        
    while(!ADCCON1&0X80);//�ȴ�ADת�����
    value =  ADCL >> 2; //ADCL�Ĵ�����2λ��Ч
    value |=(((uint16)ADCH)<<6);//����ADCH��ADCL������ֵ��value
    adcvalue +=value;//adcvalue����ֵΪ4��ADֵ֮��
  }
  value=adcvalue>>2;//�ۼӳ���4���õ�ƽ��ֵ
  return ((value) >> 4) - 315;     //����ADֵ�������ʵ�ʵ��¶�
}