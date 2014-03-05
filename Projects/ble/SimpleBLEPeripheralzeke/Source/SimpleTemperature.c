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
   TR0=0x01;         //set '1' to connectthe temperature sensorto the SOC_ADC.
   ATEST=0x01;       // Enablesthe temperature sensor
}   

/**************************************************************** 
��ȡ�¶ȴ����� AD ֵ����       
****************************************************************/ 
uint8 getTemperature(void){ 
   
   uint16  value;
   float v ;
   ADCCON3  = (0x3E);                  //ѡ��1.25VΪ�ο���ѹ��14λ�ֱ��ʣ���Ƭ���¶ȴ���������        
   while(!ADCIF);                //�ȴ� AD ת����� 
   value =  ADCL ;                //ADCL �Ĵ����� 2 λ��Ч 
   value |= (((uint16)ADCH) << 8);
   value=value>>4;
   
   v=(value-1367.5)/4.5;    //���� AD ֵ�������ʵ�ʵ��¶�,оƬ�ֲ��д��¶�ϵ��Ӧ����4.5 /��
   return   v;                           //�����¶�У���������ȥ5�棨��ͬоƬ���ݾ������У����

}