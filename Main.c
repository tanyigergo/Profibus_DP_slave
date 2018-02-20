/*
 *A simple est application with two switches connected to the stm32
 *Switch_1(ON/OFF) starts/stops a counter, whose data is constantly being sent over profibus to plc
 *Switch_2(ON/OFF) simply toggles a bit in a variable, which is also sent to plc
 */
 
#include "stm32f10x.h"
#include "delay.h"
#include "lcd16x2.h"
#include "periph_init.h"
#include "vpc3.h"
#include "test.h"
#include "DpAppl.h"


int main(void)
{
/******************************Init peripherals******************************************************/
	myRCC_Init();
	myGPIO_Init();
	mySPI_Init();
	DelayInit();
	lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
	volatile uint8_t masterData[2];
	
	SPI_Cmd(SPI1, ENABLE);
	
/********************************Application*********************************************/
	

	uint8_t kapcs;

	DpAppl_SetResetVPC3Channel1();
	DpAppl_ProfibusInit();
	

	
	while(1)
	{
	
	DpAppl_ProfibusMain();
			kapcs = GPIOB->IDR;
			
		if(kapcs & 64){
			
			sDpAppl.abDpInputData[0]++;
		}
					
		if(kapcs & 128){
			
			sDpAppl.abDpInputData[1]=1;
		}
		
		else sDpAppl.abDpInputData[1]=0;
			
	}//while(1)
}

/******************************end main****************************************/











