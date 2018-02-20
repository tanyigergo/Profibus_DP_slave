#include "test.h"
#include "lcd16x2.h"
#include <string.h>
#include <stdio.h>
void test(uint16_t bValue){


	lcdPrintHex((uint8_t)(bValue >> 8));
	lcdPrintHex((uint8_t)bValue);
	
}


void ledTest(){
	
	int i = 0;
	while(i<20){
	GPIOC->ODR ^=0X2000;
	DelayMs(50);
	i++;
	}
	GPIOC->ODR |= 0X2000;
}


