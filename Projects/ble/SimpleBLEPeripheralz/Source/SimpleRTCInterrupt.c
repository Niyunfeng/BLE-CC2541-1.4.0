/*
 * SimpleGPIOInterrupt.c
 *
 *  Created on: 2014-1-16
 *      Author: zekezang
 */

#include "hal_lcd.h"
#include "hal_ds1307.h"

#define RTC_Interrupt P1_0

#define RTCPinSelect				P1SEL
#define RTCPinDirectory				P1DIR
#define RTCPinInterruptEable		P1IEN
#define RTCPinInterruptControl		PICTL
#define RTCPinIEN					IEN2
#define RTCPinInterruptStatus		P1IFG
#define RTCPinInterruptFlags		IRCON2

void InitialRTCInterrupt() {
	RTCPinSelect &= ~0x01;
	RTCPinDirectory &= ~0x01; //input mode
	RTCPinInterruptEable |= 0x01;
	RTCPinInterruptControl |= 0x02;
	RTCPinIEN |= 0x10;
	RTCPinInterruptStatus &= ~0x01;
}

uint32 k = 0;
HAL_ISR_FUNCTION( RTCInterruptIsr, P1INT_VECTOR) {
	RTCPinInterruptStatus &= ~0x01;
	k++;
//	HalLcdWriteStringValue("k: ", k, 10, HAL_LCD_LINE_5);
	if (k % 10 == 0) {
		uint8 timeData[7] = { 0 };
		HalDS1307ReadTimeData(timeData);
	}

	RTCPinInterruptFlags &= ~0x08;
}
