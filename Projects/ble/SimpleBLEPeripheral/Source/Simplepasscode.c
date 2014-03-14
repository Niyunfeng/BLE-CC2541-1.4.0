#include "bcomdef.h"
#include "OSAL.h"
#include "OSAL_PwrMgr.h"
#include "osal_snv.h"

#include "OnBoard.h"
#include "hal_adc.h"
#include "hal_led.h"
#include "hal_key.h"
#include "hal_lcd.h"
#include "SimpleBLESerialUart.h"

#include "gatt.h"

#include "hci.h"

#include "gapgattserver.h"
#include "gattservapp.h"
#include "devinfoservice.h"
#include "simpleGATTprofile.h"


#include "peripheral.h"


#include "gapbondmgr.h"

#include "simpleBLEPeripheral.h"
/*********************************************************************
 * PASSKEY
 */
 uint8 passkey_flag;
 uint32 newpasskey;
 uint32 de_passkey=123456;
 uint8 newname[20]={0};
 uint8 code_flag=0;
 uint8 name_flag=0;
 
 // GAP GATT Attributes
static uint8 attDeviceName[GAP_DEVICE_NAME_LEN] = "aico-egg";

	
 void set_de_passkey(void)
{
      GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName); 

      uint32 de_passkey = 123456;
      uint8 pairMode = GAPBOND_PAIRING_MODE_INITIATE;
      uint8 mitm = TRUE;
      uint8 ioCap = GAPBOND_IO_CAP_DISPLAY_ONLY;              
      uint8 bonding = FALSE;
      
      GAPBondMgr_SetParameter(GAPBOND_DEFAULT_PASSCODE, sizeof(uint32), &de_passkey);
      GAPBondMgr_SetParameter(GAPBOND_PAIRING_MODE, sizeof(uint8), &pairMode);
      GAPBondMgr_SetParameter(GAPBOND_MITM_PROTECTION, sizeof(uint8), &mitm);
      GAPBondMgr_SetParameter(GAPBOND_IO_CAPABILITIES, sizeof(uint8), &ioCap);
      GAPBondMgr_SetParameter(GAPBOND_BONDING_ENABLED, sizeof(uint8), &bonding);
      
     // GAPBondMgr_PasscodeRsp( 0, SUCCESS,de_passkey );
      passkey_flag=0xAB;
      osal_snv_write(0xE0,1,&passkey_flag);
             
                
                
	
		
}
void set_new_passkey(uint32 newpasskeyvalue,uint8 newname[])
{
      //uint32 newpasskey = 4567; // passkey "000000"
      uint8 newname_len;
      newname_len=osal_strlen(newname);
      GGS_SetParameter(GGS_DEVICE_NAME_ATT, newname_len, newname);
     
    // HalLcdWriteStringValue("nnl:",newname_len, 10, HAL_LCD_LINE_5);
     
      
      newpasskey=newpasskeyvalue;
      uint8 pairMode = GAPBOND_PAIRING_MODE_INITIATE;
      uint8 mitm = TRUE;
      uint8 ioCap = GAPBOND_IO_CAP_DISPLAY_ONLY;      
      uint8 bonding = FALSE;
      
      GAPBondMgr_SetParameter(GAPBOND_DEFAULT_PASSCODE, sizeof(uint32), &newpasskey);
      GAPBondMgr_SetParameter(GAPBOND_PAIRING_MODE, sizeof(uint8), &pairMode);
      GAPBondMgr_SetParameter(GAPBOND_MITM_PROTECTION, sizeof(uint8), &mitm);
      GAPBondMgr_SetParameter(GAPBOND_IO_CAPABILITIES, sizeof(uint8), &ioCap);
      GAPBondMgr_SetParameter(GAPBOND_BONDING_ENABLED, sizeof(uint8), &bonding);
      //GAPBondMgr_PasscodeRsp( 0, SUCCESS,newpasskey );
      
      passkey_flag=0xAA;
      osal_snv_write(0xE0,1,&passkey_flag);
      osal_snv_write(0xE1,6,&newpasskey);
      osal_snv_write(0xE2,newname_len,newname);
                
}
 
 void set_passkey(void)
 {
    //uint32 newpasskey;
      uint8 newname_len;
   
      osal_snv_read(0xE0,1,&passkey_flag);
      osal_snv_read(0xE1,6,&newpasskey);
      osal_snv_read(0xE2,20,newname);
       
      newname_len=osal_strlen(newname);
     // HalLcdWriteStringValue("nnl:",newname_len, 10, HAL_LCD_LINE_4);
      //HalLcdWriteString(newname,  HAL_LCD_LINE_5);
      
      if(passkey_flag==0xAA)
      {
         set_new_passkey(newpasskey,newname);
      }
      else 
      {
         set_de_passkey();
      }
 }
  /*********************************************************************
 *********************************************************************/

/*将字符串s转换成相应的整数*/
uint32 atoi(uint8 s[])
{
      uint8 i;
      uint32 n = 0;
      for (i = 0;i<6;++i)
      {
        if(s[i] >= '0' && s[i] <= '9')	
          n = 10 * n + (s[i] - '0');
      }
      return n;
} 
 
 void set_code_name(uint8 valuechar3[])
 {
      uint32 newpasskeyvalue;
      uint8 valuechar3_1[6]={0};
              
      if(valuechar3[0]==0x61)
       {
         osal_memset(valuechar3_1, 0, osal_strlen((char*)valuechar3)-1);
          osal_memcpy(valuechar3_1, valuechar3+1, osal_strlen((char*)valuechar3));
         newpasskeyvalue=atoi(valuechar3_1);        
         //HalLcdWriteString("newpasskey",  HAL_LCD_LINE_5 );
         HalLcdWriteString(valuechar3_1,  HAL_LCD_LINE_8 );
         code_flag=1;
       }
       else if(valuechar3[0]==0x62)
       {
      
         osal_memset(newname, 0, osal_strlen((char*)valuechar3)-1);
         osal_memcpy(newname, valuechar3+1, osal_strlen((char*)valuechar3));
         name_flag=1;
         HalLcdWriteString(newname,  HAL_LCD_LINE_8 );
         
       }
       
         if(code_flag==1&&name_flag==1)
         {
           
           set_new_passkey(newpasskeyvalue,newname);
         }
               
 }

