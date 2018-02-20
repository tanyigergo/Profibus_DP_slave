#ifndef __SPI_VPC3_H 
#define __SPI_VPC3_H 


#include "platform.h"
#include "DpAppl.h"
//#include "platform.h"

/*****************************DEFINÍCIÓK*******************************************/
#define READ_BYTE 			(0X13)
#define READ_ARRAY			(0X03)
#define WRITE_BYTE      (0X12)
#define WRITE_ARRAY     (0X02)



void       Vpc3Write    ( VPC3_ADR wAddress, uint8_t bData );
uint8_t    Vpc3Read     ( VPC3_ADR wAddress );
void       Vpc3MemSet   ( VPC3_ADR wAddress, uint8_t bValue, uint16_t wLength );
uint8_t    Vpc3MemCmp   ( VPC3_UNSIGNED8_PTR pToVpc3Memory1, VPC3_UNSIGNED8_PTR pToVpc3Memory2, uint16_t wLength );
void       CopyToVpc3   ( VPC3_UNSIGNED8_PTR pToVpc3Memory, MEM_UNSIGNED8_PTR pLocalMemory, uint16_t wLength );
void       CopyFromVpc3 ( MEM_UNSIGNED8_PTR pLocalMemory, VPC3_UNSIGNED8_PTR pToVpc3Memory, uint16_t wLength );




void DpAppl_SetResetVPC3Channel1( void );

void DpAppl_ClrResetVPC3Channel1( void );

void DpAppl_EnableInterruptVPC3Channel1( void );

void DpAppl_DisableInterruptVPC3Channel1( void );

void DpAppl_EnableInterruptVPC3Sync( void );

void DpAppl_DisableInterruptVPC3Sync( void );

void DpAppl_EnableAllInterrupts( void );

void DpAppl_DisableAllInterrupts( void );

void Vpc3Wait_1ms( void );



#endif
