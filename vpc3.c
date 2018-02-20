#include "stm32f10x.h"                  // Device header
#include "vpc3.h"

#include "delay.h"
#include "periph_init.h"

#include "test.h"


/*---------------------------------------------------------------------------*/
/* function: Vpc3Write                                                       */
/*---------------------------------------------------------------------------*/
/**
 * @brief Write a byte to VPC3+.
 *
 * @attention This function is only necessary with VPC3+S in SPI- or IIC-Mode!
 *
 * @param[in]wAddress Address in VPC3+
 * @param[in]bData Data
 */

void Vpc3Write(VPC3_ADR wAddress, uint8_t bData )
{
	
	
   //16 bit address into 2 bytes
	uint8_t upperByte = (uint8_t)(wAddress >> 8);
	uint8_t lowerByte = (uint8_t)wAddress;
	//Variable to return 
	uint8_t spiRxBuffer;
	
	DpAppl_DisableInterruptVPC3Channel1();
	
	/*NSS pin = LOW*/
	SET_NSS_LOW;
	DelayUs(10);
	
	
	/*Send slave the read instruction, 0001 0011*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = WRITE_BYTE;
	
	
	/*Send 16 bit address of slave memory cell*/	
			//1. Upper byte address
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = upperByte;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	spiRxBuffer = SPI1->DR;
	
			//2. Lower byte address
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = lowerByte;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	spiRxBuffer = SPI1->DR;
	
	/*Send data byte*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = bData;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	spiRxBuffer = SPI1->DR;
	
	
	/*Wait while the SPI interface is busy*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);		
	
	spiRxBuffer = SPI1->DR;
	
	DelayUs(10);
	
	/*NSS pin high*/
	SET_NSS_HIGH;
	
	DelayUs(10);
	DpAppl_EnableInterruptVPC3Channel1();
	
}//void Vpc3Write( VPC3_ADR wAddress, uint8_t bData )



/*---------------------------------------------------------------------------*/
/* function: Vpc3Read                                                        */
/*---------------------------------------------------------------------------*/
/**
 * @brief Read one byte from VPC3+.
 *
 * @attention This function is only necessary with VPC3+S in SPI- or IIC-Mode!
 *
 * @param[in]wAddress Address in VPC3+
 * @return value of wAddress
 */

uint8_t Vpc3Read( VPC3_ADR wAddress )
{
	
	
	
   //16 bit address into 2 bytes
	uint8_t upperByte = (uint8_t)(wAddress >> 8);
	uint8_t lowerByte = (uint8_t)wAddress;
	
	//Variable to return 
	uint8_t spiRxBuffer;
		
	DpAppl_DisableInterruptVPC3Channel1();
	
	/*NSS pin = LOW*/
	SET_NSS_LOW;
	
	
	
	/*Send slave the read instruction, 0001 0011*/
	SPI1->DR = READ_BYTE;
	
	
	/*Send 16 bit address of slave memory cell*/	
		//1. Upper byte address
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = upperByte;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	spiRxBuffer = SPI1->DR;
		
		//2. Lower byte address
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = lowerByte;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	spiRxBuffer = SPI1->DR;
	
	/*Send dummy byte*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = 0x00;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	spiRxBuffer = SPI1->DR;
	
//	/*Send dummy byte*/
//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//	SPI1->DR = 0x00;
//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//	spiRxBuffer = SPI1->DR;
	
	/*Wait while the SPI interface is busy*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);	

/*Read the received data*/
	spiRxBuffer = SPI1->DR;


	
/*NSS pin high*/
	SET_NSS_HIGH;
	
	DpAppl_EnableInterruptVPC3Channel1();
	
	return spiRxBuffer;
  
	
	
}//uint8_t Vpc3Read( VPC3_ADR wAddress )




/*---------------------------------------------------------------------------*/
/* function: Vpc3MemSet                                                      */
/*---------------------------------------------------------------------------*/
/**
 * @brief Fill block of VPC3+ memory.
 *
 * @param[in]wAddress Address of the block of memory to fill.
 * @param[in]bValue  Value to be set.
 * @param[in]wLength Number of bytes to be set to the value.
 */

void Vpc3MemSet( VPC3_ADR wAddress, uint8_t bValue, uint16_t wLength )
{
   /** @todo Add your own code here! */
	uint8_t upperByte = (uint8_t)(wAddress >> 8);
	uint8_t lowerByte = (uint8_t)wAddress;
	uint16_t i;
	uint8_t spiRxBuffer;
	
	DpAppl_DisableInterruptVPC3Channel1();
	
	/*NSS pin = LOW*/
	SET_NSS_LOW;
	
		
	
	/*Send slave the read instruction, 0001 0011*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = WRITE_ARRAY;
	
	
	/*Send 16 bit address of slave memory cell*/	
			//1. Upper byte address
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = upperByte;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	spiRxBuffer = SPI1->DR;
	
			//2. Lower byte address
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = lowerByte;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	spiRxBuffer = SPI1->DR;
	
	/*Send data byte*/
	for(i = 0; i <wLength; i++){
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR = bValue;
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	spiRxBuffer = SPI1->DR;
	}
	
	
	/*Wait while the SPI interface is busy*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);		
	spiRxBuffer = SPI1->DR;
	
	
	/*NSS pin high*/
	SET_NSS_HIGH;
	
	DpAppl_EnableInterruptVPC3Channel1();
}//void Vpc3MemSet( VPC3_ADR wAddress, uint8_t bValue, uint16_t wLength )


/*---------------------------------------------------------------------------*/
/* function: Vpc3MemCmp                                                      */
/*---------------------------------------------------------------------------*/
/**
 * @brief Compare two blocks of VPC3+ memory.
 *
 * @param[in]pToVpc3Memory1 Pointer to block of memory.
 * @param[in]pToVpc3Memory2 Pointer to block of memory.
 * @param[in]wLength Number of bytes to compare.
 * @return 0 Indicates that the contents of both memory blocks are equal.
 * @return 1 Indicates that the contents of both memory blocks are not equal.
 */

uint8_t Vpc3MemCmp( VPC3_UNSIGNED8_PTR pToVpc3Memory1, VPC3_UNSIGNED8_PTR pToVpc3Memory2, uint16_t wLength )
{
   /** @todo Add your own code here! */

uint8_t bRetValue;
uint16_t i;

   bRetValue = 0;
   for( i = 0; i < wLength; i++ )
   {
      if( Vpc3Read( (VPC3_ADR)pToVpc3Memory1++ ) != Vpc3Read( (VPC3_ADR)pToVpc3Memory2++ ) )
      {
         bRetValue = 1;
         break;
      }//if( Vpc3Read( (VPC3_ADR)pToVpc3Memory1++ ) != Vpc3Read( (VPC3_ADR)pToVpc3Memory2++ ) )
   }//for( i = 0; i < wLength; i++ )

   return bRetValue;
}//uint8_t Vpc3MemCmp( VPC3_UNSIGNED8_PTR pToVpc3Memory1, VPC3_UNSIGNED8_PTR pToVpc3Memory2, uint16_t wLength )






/*---------------------------------------------------------------------------*/
/* function: CopyFromVpc3                                                    */
/*---------------------------------------------------------------------------*/

/**
 * @brief Copy block of memory from VPC3+.
 *
 * @param[in]pLocalMemory Pointer to the destination array where the content is to be copied.
 * @param[in]pToVpc3Memory Pointer to the source of data to be copied.
 * @param[in]wLength Number of bytes to copy.
 */
void CopyFromVpc3( MEM_UNSIGNED8_PTR pLocalMemory, VPC3_UNSIGNED8_PTR pToVpc3Memory, uint16_t wLength)
{
	uint16_t wAddress = (VPC3_ADR)pToVpc3Memory;
	uint8_t upperByte = (uint8_t)(wAddress >> 8);
	uint8_t lowerByte = (uint8_t)wAddress;
	
	DpAppl_DisableInterruptVPC3Channel1();
	int i;
	
	
	
	SET_NSS_LOW;
	
	/*Send slave the read instruction, 0001 0011*/
	SPI1->DR = READ_ARRAY;
	
	
	/*Send 16 bit address of slave memory cell*/	
		//1. Upper byte address
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = upperByte;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	pLocalMemory[0]  = SPI1->DR;
		
	//2. Lower byte address
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = lowerByte;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	pLocalMemory[0]  = SPI1->DR;
	
	/*Send dummy byte*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = 0x00;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	pLocalMemory[0]  = SPI1->DR;
	
	for(i = 0; i < wLength-1; i++){
		
	/*Send dummy byte*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = 0x00;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	pLocalMemory[i]  = SPI1->DR;
		
	}
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	pLocalMemory[wLength-1]  = SPI1->DR;

	/*NSS pin high*/
	SET_NSS_HIGH;
	
	
	
	
	
	
  
	/** @todo Add your own code here! */
}//void CopyFromVpc3( MEM_UNSIGNED8_PTR pLocalMemory, VPC3_UNSIGNED8_PTR pToVpc3Memory, uint16_t wLength )






/*---------------------------------------------------------------------------*/
/* function: CopyToVpc3                                                      */
/*---------------------------------------------------------------------------*/
/**
 * @brief Copy block of memory to VPC3+.
 *
 * @param[in]pToVpc3Memory Pointer to the destination array where the content is to be copied.
 * @param[in]pLocalMemory Pointer to the source of data to be copied.
 * @param[in]wLength Number of bytes to copy.
 */

void CopyToVpc3(VPC3_UNSIGNED8_PTR pToVpc3Memory, MEM_UNSIGNED8_PTR pLocalMemory, uint16_t wLength)
{
   /** @todo Add your own code here! */
	
	
	uint16_t wAddress = (VPC3_ADR)pToVpc3Memory;
	uint8_t upperByte = (uint8_t)(wAddress >> 8);
	uint8_t lowerByte = (uint8_t)wAddress;
	uint16_t i;
	uint8_t spiRxBuffer;
	
	DpAppl_DisableInterruptVPC3Channel1();
	
	/*NSS pin = LOW*/
	SET_NSS_LOW;
	
	
	/*Send slave the read instruction, 0001 0011*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = WRITE_ARRAY;
	
	
	/*Send 16 bit address of slave memory cell*/	
			//1. Upper byte address
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = upperByte;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	spiRxBuffer = SPI1->DR;
	
			//2. Lower byte address
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = lowerByte;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	spiRxBuffer = SPI1->DR;
	
	/*Send data byte*/
	for(i = 0; i <wLength; i++){
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		SPI1->DR = pLocalMemory[i];
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		spiRxBuffer = SPI1->DR;
	}
	
	
	/*Wait while the SPI interface is busy*/
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);		
	spiRxBuffer = SPI1->DR;
	
	
	/*NSS pin high*/
	SET_NSS_HIGH;
	
	DpAppl_EnableInterruptVPC3Channel1();
	

}//void CopyToVpc3( VPC3_UNSIGNED8_PTR pToVpc3Memory, MEM_UNSIGNED8_PTR pLocalMemory, uint16_t wLength )







/*---------------------------------------------------------------------------*/
/* function: DpAppl_SetResetVPC3Channel1                                     */
/*---------------------------------------------------------------------------*/
/**
 * @brief Set VPC3+ reset.
 *
 * @attention The VPC3+ reset is high-active!
 *
 */
void DpAppl_SetResetVPC3Channel1( void )
{
   /** @todo Add your own code here! */
	
	
	SET_RESET_PIN;
	
	
}//void DpAppl_SetResetVPC3Channel1( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_ClrResetVPC3Channel1                                     */
/*---------------------------------------------------------------------------*/
/**
 * @brief Clear VPC3+ reset.
 *
 * @attention The VPC3+ reset is high-active!
 *
 */
void DpAppl_ClrResetVPC3Channel1( void )
{
   /** @todo Add your own code here! */
	RESET_RESET_PIN;
}//void DpAppl_ClrResetVPC3Channel1( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_EnableInterruptVPC3Channel1                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief Enable VPC3+ interrupt.
 */
void DpAppl_EnableInterruptVPC3Channel1( void )
{
   /** @todo Add your own code here! */
}//void DpAppl_EnableInterruptVPC3Channel1( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_DisableInterruptVPC3Channel1                             */
/*---------------------------------------------------------------------------*/
/**
 * @brief Disable VPC3+ interrupt.
 *
 */
void DpAppl_DisableInterruptVPC3Channel1( void )
{
   /** @todo Add your own code here! */
}//void DpAppl_DisableInterruptVPC3Channel1( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_EnableInterruptVPC3Sync                                  */
/*---------------------------------------------------------------------------*/
/**
 * @brief Enable VPC3+ isochronous interrupt.
 *
 * @attention Is only supported from VPC3+S!
 *
 */
void DpAppl_EnableInterruptVPC3Sync( void )
{
   /** @todo Add your own code here! */
}//void DpAppl_EnableInterruptVPC3Sync( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_DisableInterruptVPC3Sync                                 */
/*---------------------------------------------------------------------------*/
/**
 * @brief Disable VPC3+ isochronous interrupt.
 *
 * @attention Is only supported from VPC3+S!
 *
 */
void DpAppl_DisableInterruptVPC3Sync( void )
{
   /** @todo Add your own code here! */
}//void DpAppl_DisableInterruptVPC3Sync( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_EnableAllInterrupts                                      */
/*---------------------------------------------------------------------------*/
/**
 * @brief Enable all microcontroller interrupts.
 *
 */
void DpAppl_EnableAllInterrupts( void )
{
   /** @todo Add your own code here! */
}//void DpAppl_EnableAllInterrupts( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_DisableAllInterrupts                                     */
/*---------------------------------------------------------------------------*/
/**
 * @brief Disable all microcontroller interrupts.
 *
 */
void DpAppl_DisableAllInterrupts( void )
{
   /** @todo Add your own code here! */
}//void DpAppl_DisableAllInterrupts( void )

/*---------------------------------------------------------------------------*/
/* function: Vpc3Wait_1ms                                                    */
/*---------------------------------------------------------------------------*/
/**
 * @brief Wait some time.
 *
 */
void Vpc3Wait_1ms( void )
{
   /** @todo Add your own code here! */
	DelayMs(1);
}//void Vpc3Wait_1ms( void )

