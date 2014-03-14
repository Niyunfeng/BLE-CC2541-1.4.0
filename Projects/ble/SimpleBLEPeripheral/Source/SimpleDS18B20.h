/*
 * SimpleDS18B20.h
 *
 *  Created on: 2014-3-7
 *      Author: 
 */
#include "hal_types.h"
#ifndef SIMPLEGPDS18B20_H_
#define SIMPLEGPDS18B20_H_




/**************************************************
  ���¶���ΪDS18B20֧�ֵ���������
***************************************************/

#define SEARCH_ROM      0xF0              //����ROM
#define READ_ROM        0x33              //��ROM
#define MATCH_ROM       0x55              //ƥ��ROM(�Ҷ��DS18B20ʱʹ��)
#define SKIP_ROM        0xCC              //����ƥ��ROM(����DS18B20ʱ����)
#define ALARM_SEARCH    0xEC              //��������

#define CONVERT_T       0x44              //��ʼת���¶�
#define WR_SCRATCHPAD   0x4E              //д�ڲ�ram3��4�ֽ�
#define RD_SCRATCHPAD   0xBE              //���ڲ�ram��9�ֽڵ�����
#define CPY_CCTATCHPAD  0x48              //���Ʊ��
#define RECALL_EE       0xB8              //δ����
#define RD_PWR_SUPPLY   0xB4              //����Դ��Ӧ

#define HIGH            1                 //�ߵ�ƽ
#define LOW             0                 //�͵�ƽ


/**************************************************
  �ӿڶ��壬��ֲ�˳���ֻ���޸����к궨�����ʱ����
**************************************************/
#define DQ            P0_7                //DS18B20����IO��
#define DQ_PIN        7                //DS18B20����IO��
#define DQ_PORT       P0DIR


void DS18B20_SendConvert(void);
uint8 DS18B20_GetTem(void);
uint8* IntToStr(uint8* buf, int16 m);
void delay_nus(uint16 timeout);



#endif /* SIMPLEGPDS18B20_H_ */