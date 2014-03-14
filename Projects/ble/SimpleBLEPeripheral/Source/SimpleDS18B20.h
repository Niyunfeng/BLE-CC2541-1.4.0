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
  以下定义为DS18B20支持的所有命令
***************************************************/

#define SEARCH_ROM      0xF0              //搜索ROM
#define READ_ROM        0x33              //读ROM
#define MATCH_ROM       0x55              //匹配ROM(挂多个DS18B20时使用)
#define SKIP_ROM        0xCC              //跳过匹配ROM(单个DS18B20时跳过)
#define ALARM_SEARCH    0xEC              //警报搜索

#define CONVERT_T       0x44              //开始转换温度
#define WR_SCRATCHPAD   0x4E              //写内部ram3、4字节
#define RD_SCRATCHPAD   0xBE              //读内部ram中9字节的内容
#define CPY_CCTATCHPAD  0x48              //复制便笺
#define RECALL_EE       0xB8              //未启用
#define RD_PWR_SUPPLY   0xB4              //读电源供应

#define HIGH            1                 //高电平
#define LOW             0                 //低电平


/**************************************************
  接口定义，移植此程序只需修改下列宏定义和延时函数
**************************************************/
#define DQ            P0_7                //DS18B20数据IO口
#define DQ_PIN        7                //DS18B20数据IO口
#define DQ_PORT       P0DIR


void DS18B20_SendConvert(void);
uint8 DS18B20_GetTem(void);
uint8* IntToStr(uint8* buf, int16 m);
void delay_nus(uint16 timeout);



#endif /* SIMPLEGPDS18B20_H_ */