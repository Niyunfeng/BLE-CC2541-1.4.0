
#include<ioCC2541.h>
#include "SimpleDS18B20.h"
#include "hal_types.h"
#include "hal_defs.h"


#define CL_DQ()     DQ = LOW              //清除数据
#define SET_DQ()    DQ = HIGH             //设置数据
#define SET_OUT()   DQ_PORT |=  BV(DQ_PIN);  //设置IO方向,out设置IO方向为输出
#define SET_IN()    DQ_PORT &= ~(BV(DQ_PIN));  //设置IO方向,in设备IO方向为输入

/*
 *    延时函数
 *    输入微妙
 */
void delay_nus(uint16 timeout)
{
   
    while (timeout--)
    {
        asm("NOP");
        asm("NOP");
        asm("NOP");
    }
}



/*
 *    写命令函数
 *    输入参数：  命令（DS18B20.H中定义）
 *    输出参数：  无
 *    返回参数：  无
 *
 */
void DS18B20_Write(unsigned char cmd)
{
    unsigned char i;
    SET_OUT();                  //设置IO为输出，2530->DS18B20
    
    /*每次一位，循环8次*/
    for(i=0; i<8; i++)
    {
        CL_DQ();              //IO为低    
        if( cmd & (1<<i) )    //写数据从低位开始
        {
          SET_DQ();           //IO输入高电平
        }
        else
        {
          CL_DQ();            //IO输出低电平
        }
        delay_nus(40);        //保持15~60us
        SET_DQ();             //IO口拉高
    }
    SET_DQ();                 //IO口拉高
}


/*
 *    读数据函数
 *    输入参数：  无
 *    输出参数：  无
 *    返回参数：  读取的数据
 *
 */
unsigned char DS18B20_Read(void)
{
    unsigned char rdData;     //读出的数据
    unsigned char i, dat;     //临时变量
    
    rdData = 0;               //读出的数据初始化为0     
    
    /* 每次读一位，读8次 */
    for(i=0; i<8; i++)
    {
        CL_DQ();            //IO拉低
        SET_DQ();           //IO拉高
        SET_IN();           //设置IO方向为输入 DS18B20->CC2540
        dat = DQ;           //读数据,从低位开始
        
        if(dat)
        {
          rdData |= (1<<i); //如果读出的数据位为正
        }
        else
        {
          rdData &= ~(1<<i);//如果读出的数据位为负
        }
        
        delay_nus(70);      //保持60~120us
        SET_OUT();          //设置IO方向为输出 CC2540->DS18B20

    }
    return (rdData);        //返回读出的数据
}


/*
 *    DS18B20初始化/复位函数
 *    输入参数：  无
 *    输出参数：  无
 *    返回参数：  无
 *
 */
void DS18B20_Init(void)
{
    SET_OUT();
    SET_DQ();         //IO口拉高
    CL_DQ();          //IO口拉低
    delay_nus(550);   //IO拉低后保持一段时间 480-960us
    SET_DQ();         //释放
    delay_nus(40);    //释放总线后等待15-60us
    SET_IN();         //IO方向为输入 DS18B20->CC2540
  
    
    /* 等待DQ变低 */
    while(DQ)
    {
        ; 
    }
    delay_nus(240);   //检测到DQ 变低后，延时60-240us
    SET_OUT();        //设置IO方向为输出 CC2540->DS18B20
    SET_DQ();         //IO拉高
}


/*
 *    DS18B20 转换温度函数
 *    输入参数：  无
 *    输出参数：  无
 *    返回参数：  无
 *
 */
void DS18B20_SendConvert(void)
{
    DS18B20_Init();               //复位18B20
    DS18B20_Write(SKIP_ROM);      //发出跳过ROM匹配操作
    DS18B20_Write(CONVERT_T);     //启动温度转换
}



uint8 DS18B20_GetTem(void)
{
    unsigned char tem_h,tem_l;    //温度高位字节及低位字节
    unsigned char a,b;            //临时变量
    unsigned char flag;           //温度正负标记，正为0，负为1
    unsigned char FRACTION_INDEX[16] = {0, 1, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 7, 8, 9, 9 };//小数值查询表
    unsigned char sensor_data_value[2];  //传感器数据
 
    DS18B20_Init();               //DS18B20复位       
    DS18B20_Write(SKIP_ROM);      //跳过ROM匹配
    
    DS18B20_Write(RD_SCRATCHPAD); //写入读9字节命令
    tem_l = DS18B20_Read();       //读温度低位。第一字节
    tem_h = DS18B20_Read();       //读温度高位，第二字节

    /* 判断RAM中存储的温度正负 
       并提取出符号位和真实的数据
    */
    if(tem_h & 0x80)
    {
        flag = 1;                 //温度为负
        a = (tem_l>>4);           //取温度低4位原码
        b = (tem_h<<4)& 0xf0;     //取温度高4位原码
        /*补码-原码转换
          负数的补码是在其原码的基础上, 符号位不变, 其余各位取反, 最后+1
        */
        tem_h = ~(a|b) + 1;       //取整数部分数值，不符号位
        
        tem_l = ~(a&0x0f) + 1;    //取小数部分原值，不含符号位
    }
    else
    {
        flag = 0;                 //为正
        a = tem_h<<4;
        a += (tem_l&0xf0)>>4;     //得到整数部分值 
        b = tem_l&0x0f;           //得出小数部分值
        tem_h = a;                //整数部分
        tem_l = b&0xff;           //小数部分
    }
  
    sensor_data_value[0] = FRACTION_INDEX[tem_l]; //查表得小数值
    sensor_data_value[1] = tem_h| (flag<<7);      //整数部分，包括符号位
    
    return sensor_data_value[1];
}

/*int 转 字符串函数*/
uint8* IntToStr(uint8* buf, int16 m)
{
    int8 tmp[16];
    int32 isNegtive = 0;
    int32 index;

    if(m < 0)
    {
        isNegtive = 1;
        m = - m;
    }

    tmp[15] = '\0';
    index = 14;
    do 
    {
        tmp[index--] = m % 10 + '0';
        m /= 10;
    } while (m > 0);

    if(isNegtive)
        tmp[index--] = '-';
    
    strcpy((char *)buf, (char *)tmp + index + 1);

    return buf;
}
