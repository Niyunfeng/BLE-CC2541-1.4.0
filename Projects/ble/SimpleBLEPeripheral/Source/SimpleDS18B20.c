
#include<ioCC2541.h>
#include "SimpleDS18B20.h"
#include "hal_types.h"
#include "hal_defs.h"


#define CL_DQ()     DQ = LOW              //�������
#define SET_DQ()    DQ = HIGH             //��������
#define SET_OUT()   DQ_PORT |=  BV(DQ_PIN);  //����IO����,out����IO����Ϊ���
#define SET_IN()    DQ_PORT &= ~(BV(DQ_PIN));  //����IO����,in�豸IO����Ϊ����

/*
 *    ��ʱ����
 *    ����΢��
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
 *    д�����
 *    ���������  ���DS18B20.H�ж��壩
 *    ���������  ��
 *    ���ز�����  ��
 *
 */
void DS18B20_Write(unsigned char cmd)
{
    unsigned char i;
    SET_OUT();                  //����IOΪ�����2530->DS18B20
    
    /*ÿ��һλ��ѭ��8��*/
    for(i=0; i<8; i++)
    {
        CL_DQ();              //IOΪ��    
        if( cmd & (1<<i) )    //д���ݴӵ�λ��ʼ
        {
          SET_DQ();           //IO����ߵ�ƽ
        }
        else
        {
          CL_DQ();            //IO����͵�ƽ
        }
        delay_nus(40);        //����15~60us
        SET_DQ();             //IO������
    }
    SET_DQ();                 //IO������
}


/*
 *    �����ݺ���
 *    ���������  ��
 *    ���������  ��
 *    ���ز�����  ��ȡ������
 *
 */
unsigned char DS18B20_Read(void)
{
    unsigned char rdData;     //����������
    unsigned char i, dat;     //��ʱ����
    
    rdData = 0;               //���������ݳ�ʼ��Ϊ0     
    
    /* ÿ�ζ�һλ����8�� */
    for(i=0; i<8; i++)
    {
        CL_DQ();            //IO����
        SET_DQ();           //IO����
        SET_IN();           //����IO����Ϊ���� DS18B20->CC2540
        dat = DQ;           //������,�ӵ�λ��ʼ
        
        if(dat)
        {
          rdData |= (1<<i); //�������������λΪ��
        }
        else
        {
          rdData &= ~(1<<i);//�������������λΪ��
        }
        
        delay_nus(70);      //����60~120us
        SET_OUT();          //����IO����Ϊ��� CC2540->DS18B20

    }
    return (rdData);        //���ض���������
}


/*
 *    DS18B20��ʼ��/��λ����
 *    ���������  ��
 *    ���������  ��
 *    ���ز�����  ��
 *
 */
void DS18B20_Init(void)
{
    SET_OUT();
    SET_DQ();         //IO������
    CL_DQ();          //IO������
    delay_nus(550);   //IO���ͺ󱣳�һ��ʱ�� 480-960us
    SET_DQ();         //�ͷ�
    delay_nus(40);    //�ͷ����ߺ�ȴ�15-60us
    SET_IN();         //IO����Ϊ���� DS18B20->CC2540
  
    
    /* �ȴ�DQ��� */
    while(DQ)
    {
        ; 
    }
    delay_nus(240);   //��⵽DQ ��ͺ���ʱ60-240us
    SET_OUT();        //����IO����Ϊ��� CC2540->DS18B20
    SET_DQ();         //IO����
}


/*
 *    DS18B20 ת���¶Ⱥ���
 *    ���������  ��
 *    ���������  ��
 *    ���ز�����  ��
 *
 */
void DS18B20_SendConvert(void)
{
    DS18B20_Init();               //��λ18B20
    DS18B20_Write(SKIP_ROM);      //��������ROMƥ�����
    DS18B20_Write(CONVERT_T);     //�����¶�ת��
}



uint8 DS18B20_GetTem(void)
{
    unsigned char tem_h,tem_l;    //�¶ȸ�λ�ֽڼ���λ�ֽ�
    unsigned char a,b;            //��ʱ����
    unsigned char flag;           //�¶�������ǣ���Ϊ0����Ϊ1
    unsigned char FRACTION_INDEX[16] = {0, 1, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 7, 8, 9, 9 };//С��ֵ��ѯ��
    unsigned char sensor_data_value[2];  //����������
 
    DS18B20_Init();               //DS18B20��λ       
    DS18B20_Write(SKIP_ROM);      //����ROMƥ��
    
    DS18B20_Write(RD_SCRATCHPAD); //д���9�ֽ�����
    tem_l = DS18B20_Read();       //���¶ȵ�λ����һ�ֽ�
    tem_h = DS18B20_Read();       //���¶ȸ�λ���ڶ��ֽ�

    /* �ж�RAM�д洢���¶����� 
       ����ȡ������λ����ʵ������
    */
    if(tem_h & 0x80)
    {
        flag = 1;                 //�¶�Ϊ��
        a = (tem_l>>4);           //ȡ�¶ȵ�4λԭ��
        b = (tem_h<<4)& 0xf0;     //ȡ�¶ȸ�4λԭ��
        /*����-ԭ��ת��
          �����Ĳ���������ԭ��Ļ�����, ����λ����, �����λȡ��, ���+1
        */
        tem_h = ~(a|b) + 1;       //ȡ����������ֵ��������λ
        
        tem_l = ~(a&0x0f) + 1;    //ȡС������ԭֵ����������λ
    }
    else
    {
        flag = 0;                 //Ϊ��
        a = tem_h<<4;
        a += (tem_l&0xf0)>>4;     //�õ���������ֵ 
        b = tem_l&0x0f;           //�ó�С������ֵ
        tem_h = a;                //��������
        tem_l = b&0xff;           //С������
    }
  
    sensor_data_value[0] = FRACTION_INDEX[tem_l]; //����С��ֵ
    sensor_data_value[1] = tem_h| (flag<<7);      //�������֣���������λ
    
    return sensor_data_value[1];
}

/*int ת �ַ�������*/
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
