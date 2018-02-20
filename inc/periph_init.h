

#ifndef __PERIPH_INIT_H   		
#define __PERIPH_INIT_H      


/****************************************DEFINICIÓK***************************************/
#define SET_NSS_HIGH 					(GPIOA->ODR |= 0X10)
#define SET_NSS_LOW						(GPIOA->ODR &= ~(0X10))
#define SET_LED								(GPIOC->ODR |= 0X2000)
#define RESET_LED							(GPIOC->ODR &= ~(0X2000))
#define SET_RESET_PIN					(GPIOA->ODR |= 0X2)
#define RESET_RESET_PIN				(GPIOA->ODR &= ~(0X2))

#define SPI1_NSS_PIN		(0x10)
#define SPI1_SCK_PIN		(0x20)
#define SPI1_MISO_PIN		(0x40)
#define SPI1_MOSI_PIN		(0x80)



void myRCC_Init(void);
void myGPIO_Init(void);
void mySPI_Init(void);
void myTimer_Init(void);
void myTIM_Init(void);



#endif

