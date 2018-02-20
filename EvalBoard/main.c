/***************************  Filename: main.c  ******************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0      Einsteinstraﬂe 6        */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      91074 Herzogenaurach    */
/* 0000  000  0   0 000   0 0     00000 0 0000       Germany                 */
/* 0     00   0   0 0     0 0     0   0 0 0                                  */
/* 0     0 0  0   0 0     0 0   0 0   0 0 0          Phone: ++499132744200   */
/* 0     0  0  000  0     0  000  0   0 0 0    GmbH  Fax:   ++4991327442164  */
/*                                                                           */
/* ========================================================================= */
/*                                                                           */
/* Function:       Demo for PROFICHIP Evaluation Board AT89C5132 with VPC3+  */
/*                                                                           */
/* Order Number:   PA 006300                                                 */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* history                                                                   */
/* ========================================================================= */
/* 08.09.2005 [V1.00]  Urversion                                             */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Hardware requirements: ProfiChip Evaluation Board AT89C5132  (PA006101)   */
/*                        ProfiChip Evaluation Board VPC3+/C    (PA006103)   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/* memory:  0000H ... 7FFFH: RAM                                             */
/*          8000H ... 8FFFH: VPC3+                                           */
/*          9000H ... 9FFFH: Reserved                                        */
/*          A000H ... AFFFH: FPGA                                            */
/*          B000H ... BFFFH: RTC                                             */
/*          C000H ... CFFFH: LCD                                             */
/*          D000H ... DFFFH: I/O Port 0                                      */
/*          E000H ... EFFFH: I/O Port 1                                      */
/*          F000H ... FFFFH: I/O Port 2                                      */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Technical support:       eMail: support@profichip.com                     */
/*                          Phone: ++49-9132-744-2150                        */
/*                          Fax. : ++49-9132-744-29-2150                     */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/* contents:

  - function prototypes
  - data structures
  - internal functions

*/
/*****************************************************************************/
/* include hierarchy */

#include <intrins.h>
#include "platform.h"

#if VPC3_SERIAL_MODE
   #if VPC3_I2C
      #include "ext\twi.h"
   #endif//#if VPC3_I2C
#endif//#if VPC3_SERIAL_MODE

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
DP_ERROR_CODE   CheckExternalRam ( void );
void            InitHardware     ( void );
void            PrintHello       ( void );
void            main             ( void );

/*---------------------------------------------------------------------------*/
/* VPC3 reset settings                                                       */
/*---------------------------------------------------------------------------*/
sbit RES_VPC3                    = 0x90;        // hardware port pin 1.0
#define VPC3_RESET__             RES_VPC3 = 1
#define VPC3_CLEAR_RESET__       RES_VPC3 = 0

extern ROMCONST__ uint8_t NAME[];

/*---------------------------------------------------------------------------*/
/* global user data definitions                                              */
/*---------------------------------------------------------------------------*/
#if VPC3_SERIAL_MODE
   static uint8_t bCounter10ms;

   #if VPC3_SPI
      static uint8_t bSpiData;
      static uint8_t bSpiTransmitCompleted;
   #endif//#if VPC3_SPI
#endif//#if VPC3_SERIAL_MODE

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

   VPC3_RESET__;
   _nop_();
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

   VPC3_CLEAR_RESET__;
   _nop_();
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

   IT0 = 1; //falling edge
   EX0 = 1;
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

   EX0 = 0;
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

   IT1 = 1; //falling edge
   EX1 = 1;
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

   EX1 = 0;
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

   EA = 1;
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

   EA = 0;
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
uint8_t i;

   /** @todo Add your own code here! */

   for( i = 0; i < 250; i++ )
   {
      _nop_();
      _nop_();
      _nop_();
      _nop_();
      _nop_();
      _nop_();
      _nop_();
      _nop_();
      _nop_();
      _nop_();
   }//for( i = 0; i < 250; i++ )
}//void Vpc3Wait_1ms( void )

/*---------------------------------------------------------------------------*/
/* function: Vpc3Write                                                       */
/*---------------------------------------------------------------------------*/
#if VPC3_SERIAL_MODE
/**
 * @brief Write a byte to VPC3+.
 *
 * @attention This function is only necessary with VPC3+S in SPI- or IIC-Mode!
 *
 * @param[in]wAddress Address in VPC3+
 * @param[in]bData Data
 */
void Vpc3Write( VPC3_ADR wAddress, uint8_t bData )
{
   /** @todo Add your own code here! */

   #if DP_ISOCHRONOUS_MODE
      DpAppl_DisableInterruptVPC3Sync();
   #endif//#if DP_ISOCHRONOUS_MODE

   #if VPC3_SPI

      DpAppl_DisableInterruptVPC3Channel1();

      SPI_CS = 0;

         bSpiData = 0x00;
         bSpiTransmitCompleted = FALSE;

         SPDAT = 0x12;                          // send command "WRITE_BYTE"
         while( !bSpiTransmitCompleted );       // wait: end of transmission
         bSpiTransmitCompleted = FALSE;         // clear software transfer flag

         SPDAT = (uint8_t)(wAddress >> 8);      // send high byte of address
         while( !bSpiTransmitCompleted );       // wait: end of transmission
         bSpiTransmitCompleted = FALSE;         // clear software transfer flag

         SPDAT = (uint8_t)(wAddress);           // send low byte of address
         while( !bSpiTransmitCompleted );       // wait: end of transmission
         bSpiTransmitCompleted = FALSE;         // clear software transfer flag

         SPDAT = bData;                         // data is send
         while( !bSpiTransmitCompleted );       // wait: end of transmission
         bSpiTransmitCompleted = FALSE;         // clear software transfer flag

      SPI_CS = 1;

      DpAppl_EnableInterruptVPC3Channel1();

   #endif//#if VPC3_SPI

   #if VPC3_I2C

      DpAppl_DisableInterruptVPC3Channel1();

      I2CWriteVpc3Byte( VPC3_I2C_ADDRESS, wAddress, 1, &bData );

      DpAppl_EnableInterruptVPC3Channel1();

   #endif//#if VPC3_I2C

   #if VPC3_PORT_PINS

      uint8_t i;

      DpAppl_DisableAllInterrupts();

      P0 = wAddress;
      P2 = wAddress >> 8;

      for(i=0;i<10;i++)
         _nop_();

      ALE_VPC = 1;
      for(i=0;i<10;i++)
         _nop_();

      ALE_VPC = 0;
      for(i=0;i<10;i++)
         _nop_();

      P0 = bData;
      for(i=0;i<10;i++)
         _nop_();

      WR_VPC = 0;
      for(i=0;i<100;i++)
         _nop_();

      WR_VPC = 1;

      P2 = 0;

      DpAppl_EnableAllInterrupts();

   #endif//#if VPC3_PORT_PINS

   #if DP_ISOCHRONOUS_MODE
      DpAppl_EnableInterruptVPC3Sync();
   #endif//#if DP_ISOCHRONOUS_MODE
}//void Vpc3Write( VPC3_ADR wAddress, uint8_t bData )
#endif//#if VPC3_SERIAL_MODE

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
#if VPC3_SERIAL_MODE
uint8_t Vpc3Read( VPC3_ADR wAddress )
{
   /** @todo Add your own code here! */

   #if VPC3_SPI

      uint8_t bData;

      #if DP_ISOCHRONOUS_MODE
         DpAppl_DisableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE

      DpAppl_DisableInterruptVPC3Channel1();

      SPI_CS = 0;

         bSpiData = 0x00;
         bSpiTransmitCompleted = FALSE;

         SPDAT = 0x13;                          // send command "READ_BYTE"
         while( !bSpiTransmitCompleted );       // wait: end of transmission
         bSpiTransmitCompleted = FALSE;         // clear software transfer flag

         SPDAT = (uint8_t)(wAddress >> 8);      // send high byte of address
         while( !bSpiTransmitCompleted );       // wait: end of transmission
         bSpiTransmitCompleted = FALSE;         // clear software transfer flag

         SPDAT = (uint8_t)(wAddress);           // send low byte of address
         while( !bSpiTransmitCompleted );       // wait: end of transmission
         bSpiTransmitCompleted = FALSE;         // clear software transfer flag

         SPDAT = 0x00;                          // data is send to generate SCK signal
         while( !bSpiTransmitCompleted );       // wait: end of transmission
         bSpiTransmitCompleted = FALSE;         // clear software transfer flag

         bData = bSpiData;

      SPI_CS = 1;

      DpAppl_EnableInterruptVPC3Channel1();

      #if DP_ISOCHRONOUS_MODE
         DpAppl_EnableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE

      return bData;

   #endif//#if VPC3_SPI

   #if VPC3_I2C
      uint8_t bData;

      #if DP_ISOCHRONOUS_MODE
         DpAppl_DisableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE

      DpAppl_DisableInterruptVPC3Channel1();
      I2CReadVpc3Byte( VPC3_I2C_ADDRESS, wAddress, 1, &bData );
      DpAppl_EnableInterruptVPC3Channel1();

      #if DP_ISOCHRONOUS_MODE
         DpAppl_EnableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE

      return bData;
   #endif//#if VPC3_I2C

   #if VPC3_PORT_PINS

      uint8_t bData;
      uint8_t i;

      DpAppl_DisableAllInterrupts();

      P0 = wAddress;
      P2 = wAddress >> 8;  //set CS

      for(i=0;i<10;i++)
         _nop_();

      ALE_VPC = 1;
      for(i=0;i<10;i++)
         _nop_();

      ALE_VPC = 0;

      P0 = 0xFF;

      for(i=0;i<10;i++)
         _nop_();

      RD_VPC = 0;
      for(i=0;i<100;i++)
         _nop_();

      bData = P0;

      RD_VPC = 1;

      P2 = 0;           //clear CS

      DpAppl_EnableAllInterrupts();

      return bData;

   #endif//#if VPC3_PORT_PINS
}//uint8_t Vpc3Read( VPC3_ADR wAddress )
#endif//#if VPC3_SERIAL_MODE

/*---------------------------------------------------------------------------*/
/* function: Vpc3MemSet                                                      */
/*---------------------------------------------------------------------------*/
#if VPC3_SERIAL_MODE
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

   #if VPC3_SPI

      uint16_t i;

      #if DP_ISOCHRONOUS_MODE
         DpAppl_DisableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE

      DpAppl_DisableInterruptVPC3Channel1();

      SPI_CS = 0;

         bSpiData = 0x00;
         bSpiTransmitCompleted = FALSE;

         SPDAT = 0x02;                       // send command "WRITE_ARRAY"
         while( !bSpiTransmitCompleted );    // wait: end of transmission
         bSpiTransmitCompleted = FALSE;      // clear software transfer flag

         SPDAT = (uint8_t)(wAddress >> 8);   // send high byte of address
         while( !bSpiTransmitCompleted );    // wait: end of transmission
         bSpiTransmitCompleted = FALSE;      // clear software transfer flag

         SPDAT = (uint8_t)(wAddress);        // send low byte of address
         while( !bSpiTransmitCompleted );    // wait: end of transmission
         bSpiTransmitCompleted = FALSE;      // clear software transfer flag

         for( i = 0; i < wLength; i++ )
         {
            SPDAT = bValue;                  // data is send
            while( !bSpiTransmitCompleted ); // wait: end of transmission
            bSpiTransmitCompleted = FALSE;   // clear software transfer flag
         }//for( i = 0; i < wLength; i++ )

      SPI_CS = 1;

      DpAppl_EnableInterruptVPC3Channel1();

      #if DP_ISOCHRONOUS_MODE
         DpAppl_EnableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE
   #endif//#if VPC3_SPI

   #if VPC3_I2C

      uint16_t i;

      #if DP_ISOCHRONOUS_MODE
         DpAppl_DisableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE

      DpAppl_DisableInterruptVPC3Channel1();

      for( i = 0; i < wLength; i++ )
      {
         I2CWriteVpc3Byte( VPC3_I2C_ADDRESS, wAddress++, 1, &bValue );
      }//for( i = 0; i < wLength; i++ )

      DpAppl_EnableInterruptVPC3Channel1();

      #if DP_ISOCHRONOUS_MODE
         DpAppl_EnableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE
   #endif//#if VPC3_I2C

   #if VPC3_PORT_PINS

      uint16_t i;

      for( i = 0; i < wLength; i++ )
      {
         Vpc3Write( wAddress++, bValue );
      }//for( i = 0; i < wLength; i++ )

   #endif//#if VPC3_PORT_PINS
}//void Vpc3MemSet( VPC3_ADR wAddress, uint8_t bValue, uint16_t wLength )
#endif//#if VPC3_SERIAL_MODE

/*---------------------------------------------------------------------------*/
/* function: Vpc3MemCmp                                                      */
/*---------------------------------------------------------------------------*/
#if VPC3_SERIAL_MODE
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
#endif//#if VPC3_SERIAL_MODE

/*---------------------------------------------------------------------------*/
/* function: CopyToVpc3                                                      */
/*---------------------------------------------------------------------------*/
#if VPC3_SERIAL_MODE
/**
 * @brief Copy block of memory to VPC3+.
 *
 * @param[in]pToVpc3Memory Pointer to the destination array where the content is to be copied.
 * @param[in]pLocalMemory Pointer to the source of data to be copied.
 * @param[in]wLength Number of bytes to copy.
 */
void CopyToVpc3( VPC3_UNSIGNED8_PTR pToVpc3Memory, MEM_UNSIGNED8_PTR pLocalMemory, uint16_t wLength )
{
   /** @todo Add your own code here! */

   #if VPC3_SPI

      uint16_t wAddress;
      uint16_t i;

      #if DP_ISOCHRONOUS_MODE
         DpAppl_DisableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE

      DpAppl_DisableInterruptVPC3Channel1();

      SPI_CS = 0;

         bSpiData = 0x00;
         bSpiTransmitCompleted = FALSE;

         SPDAT = 0x02;                             // send command "WRITE_ARRAY"
         while( !bSpiTransmitCompleted );          // wait: end of transmission
         bSpiTransmitCompleted = FALSE;            // clear software transfer flag

         wAddress = (VPC3_ADR)pToVpc3Memory;

         SPDAT = (uint8_t)(wAddress >> 8);         // send high byte of address
         while( !bSpiTransmitCompleted );          // wait: end of transmission
         bSpiTransmitCompleted = FALSE;            // clear software transfer flag

         SPDAT = (uint8_t)(wAddress);              // send low byte of address
         while( !bSpiTransmitCompleted );          // wait: end of transmission
         bSpiTransmitCompleted = FALSE;            // clear software transfer flag

         for( i = 0; i < wLength; i++ )
         {
            SPDAT = *pLocalMemory++;               // data is send
            while( !bSpiTransmitCompleted );       // wait: end of transmission
            bSpiTransmitCompleted = FALSE;         // clear software transfer flag
         }//for( i = 0; i < wLength; i++ )

      SPI_CS = 1;

      DpAppl_EnableInterruptVPC3Channel1();

      #if DP_ISOCHRONOUS_MODE
         DpAppl_EnableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE
   #endif//#if VPC3_SPI

   #if VPC3_I2C

      #if DP_ISOCHRONOUS_MODE
         DpAppl_DisableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE

      DpAppl_DisableInterruptVPC3Channel1();

      I2CWriteVpc3Byte( VPC3_I2C_ADDRESS, (VPC3_ADR)pToVpc3Memory, wLength, pLocalMemory );

      DpAppl_EnableInterruptVPC3Channel1();

      #if DP_ISOCHRONOUS_MODE
         DpAppl_EnableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE
   #endif//#if VPC3_I2C

   #if VPC3_PORT_PINS

      uint16_t i;

      for( i = 0; i < wLength; i++ )
      {
         Vpc3Write( (VPC3_ADR)pToVpc3Memory++, *pLocalMemory++ );
      }//for( i = 0; i < wLength; i++ )

   #endif//#if VPC3_PORT_PINS
}//void CopyToVpc3( VPC3_UNSIGNED8_PTR pToVpc3Memory, MEM_UNSIGNED8_PTR pLocalMemory, uint16_t wLength )
#endif//#if VPC3_SERIAL_MODE

/*---------------------------------------------------------------------------*/
/* function: CopyFromVpc3                                                    */
/*---------------------------------------------------------------------------*/
#if VPC3_SERIAL_MODE
/**
 * @brief Copy block of memory from VPC3+.
 *
 * @param[in]pLocalMemory Pointer to the destination array where the content is to be copied.
 * @param[in]pToVpc3Memory Pointer to the source of data to be copied.
 * @param[in]wLength Number of bytes to copy.
 */
void CopyFromVpc3( MEM_UNSIGNED8_PTR pLocalMemory, VPC3_UNSIGNED8_PTR pToVpc3Memory, uint16_t wLength )
{
   /** @todo Add your own code here! */

   #if VPC3_SPI
      uint16_t wAddress;
      uint16_t i;

      #if DP_ISOCHRONOUS_MODE
         DpAppl_DisableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE

      DpAppl_DisableInterruptVPC3Channel1();

      SPI_CS = 0;

         bSpiData = 0x00;
         bSpiTransmitCompleted = FALSE;

         SPDAT = 0x03;                             // send command "READ_ARRAY"
         while( !bSpiTransmitCompleted );          // wait: end of transmission
         bSpiTransmitCompleted = FALSE;            // clear software transfer flag

         wAddress = (VPC3_ADR)pToVpc3Memory;
         SPDAT = (uint8_t)(wAddress >> 8);         // send high byte of address
         while( !bSpiTransmitCompleted );          // wait: end of transmission
         bSpiTransmitCompleted = FALSE;            // clear software transfer flag

         SPDAT = (uint8_t)(wAddress);              // send low byte of address
         while( !bSpiTransmitCompleted );          // wait: end of transmission
         bSpiTransmitCompleted = FALSE;            // clear software transfer flag

         for( i = 0; i < wLength; i ++ )
         {
            SPDAT = 0x00;                          // data is send to generate SCK signal
            while( !bSpiTransmitCompleted );       // wait: end of transmission
            bSpiTransmitCompleted = FALSE;         // clear software transfer flag
         pLocalMemory[ i ] = bSpiData;
      }//for( i = 0; i < wLength; i ++ )

      SPI_CS = 1;

      DpAppl_EnableInterruptVPC3Channel1();

      #if DP_ISOCHRONOUS_MODE
         DpAppl_EnableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE
   #endif//#if VPC3_SPI

   #if VPC3_I2C

      #if DP_ISOCHRONOUS_MODE
         DpAppl_DisableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE

      DpAppl_DisableInterruptVPC3Channel1();

      I2CReadVpc3Byte( VPC3_I2C_ADDRESS, (VPC3_ADR)pToVpc3Memory, wLength, pLocalMemory );

      DpAppl_EnableInterruptVPC3Channel1();

      #if DP_ISOCHRONOUS_MODE
         DpAppl_EnableInterruptVPC3Sync();
      #endif//#if DP_ISOCHRONOUS_MODE
   #endif//#if VPC3_I2C

   #if VPC3_PORT_PINS

      uint16_t i;

      for( i = 0; i < wLength; i++ )
      {
         *pLocalMemory++ = Vpc3Read( (VPC3_ADR)pToVpc3Memory++ );
      }//for( i = 0; i < wLength; i++ )

   #endif//#if VPC3_PORT_PINS
}//void CopyFromVpc3( MEM_UNSIGNED8_PTR pLocalMemory, VPC3_UNSIGNED8_PTR pToVpc3Memory, uint16_t wLength )
#endif//#if VPC3_SERIAL_MODE

/*---------------------------------------------------------------------------*/
/* function: TestVpc3_01                                                     */
/*---------------------------------------------------------------------------*/
/**
 * @brief Hardware test of VPC3+.
 * If you get problems with reading VPC3+, you should read first the status register address 5.
 * The default value is CFhex (VPC3+C) or EFhex (VPC3+S).
 * Check reset signal of VPC3+ (notice: reset is high active).
 *
 */
void TestVpc3_01( void )
{
uint8_t bValue;

   DpAppl_SetResetVPC3Channel1();
   Vpc3Wait_1ms();
   DpAppl_ClrResetVPC3Channel1();

   while(1)
   {
      #if VPC3_SERIAL_MODE
         bValue = Vpc3Read(0x05);
      #else
         bValue = *((unsigned char *)0x28005); //address depends on hardware!!!!!
      #endif//#if VPC3_SERIAL_MODE

      #ifdef RS232_SERIO
         print_hexbyte(bValue);
      #endif//#ifdef RS232_SERIO
   }//while(1)
}//void TestVpc3_01( void )

/*---------------------------------------------------------------------------*/
/* function: main                                                            */
/*---------------------------------------------------------------------------*/
/**
 * @brief Main-function.
 *
 */
void main( void )
{
   #ifdef EvaBoard_AT89C5132
      //necessary for VPC3+/C, because address 10 is input
      CLR_VPC3_ADR10__;
   #endif//#ifdef EvaBoard_AT89C5132

   // init system - adapt it to your special requirements !
   InitHardware();
   PrintHello();

   //initialize VPC3+C/S
   DpAppl_ProfibusInit();

   while(1)
   {
      // call PROFIBUS
      DpAppl_ProfibusMain();

      #ifdef RS232_SERIO
         if( bRecCounter > 0 )
         {
            DpAppl_DisableInterruptVPC3Channel1();
            PrintSerialInputs();
            DpAppl_EnableInterruptVPC3Channel1();
         }//if( bRecCounter > 0 )
      #endif//#ifdef RS232_SERIO
   }//while(1)
}//void main( void )

/*---------------------------------------------------------------------------*/
/* function: PrintHello                                                      */
/*---------------------------------------------------------------------------*/
void PrintHello( void )
{
sDP_VERSION pVersion;

   GetFirmwareVersion( &pVersion );

   #ifdef RS232_SERIO

      print_string( "\r\n\r\nprofichip GmbH VPC3-Demo Version: V6.01.12 02.10.2014\r\n" );
      print_string( "\r\nSoftwareversion: V" );
      print_hexbyte( pVersion.bMainInterface );
      print_string( "." );
      print_hexbyte( pVersion.bFunction      );
      print_hexbyte( pVersion.bBugfix        );
      print_string( "\r\nInstalled Components: " );
      print_hexword( pVersion.wComponentsInstalled );
      PrintHelp();

   #endif//#ifdef RS232_SERIO

   #ifdef EvaBoard_AT89C5132
      #ifdef LCD
         LCDUpdateProfichip( pVersion.bMainInterface, pVersion.bFunction, pVersion.bBugfix );
         LCDUpdate( &NAME[0], pVersion.wComponentsInstalled );
      #endif//#ifdef LCD
   #endif//#ifdef EvaBoard_AT89C5132
}//void PrintHello( void )

/*---------------------------------------------------------------------------*/
/* function: InitHardware                                                    */
/*---------------------------------------------------------------------------*/
void InitHardware( void )
{
   CLR_LED_YLW__;
   CLR_LED_RED__;

   #ifdef EvaBoard_AT89C5132
      #ifdef AT89C5132_MODE_X2
         CKCON |= MSK_X2;
      #endif//#ifdef AT89C5132_MODE_X2

      AUXR = 0x02;    //set ERAM, enable ALE

      // check sram
      if( DP_OK != CheckExternalRam() )
      {
         #ifdef EvaBoard_AT89C5132
            *WRITE_PORT0 = DP_SRAM_ERROR;
         #endif//#ifdef EvaBoard_AT89C5132

         SET_LED_RED__;

         while(1)
         {
         }//while(1)
      }//if( DP_OK != CheckExternalRam() )

      *WRITE_PORT0 = 0x00;
      *WRITE_PORT1 = 0x00;
      *WRITE_PORT2 = 0x00;

   #endif//#ifdef EvaBoard_AT89C5132

   // only used for fibre optic interface
   DISABLE_LWL_INTERFACE_B__;
   SET_DIR_AB__;
   //SET_DIR_BA__;

   // todo: perform VPC3 reset for at least 300ns time
   DpAppl_SetResetVPC3Channel1();          // set reset hardware pin

   #ifdef EvaBoard_AT89C5132
      #ifdef RS232_SERIO
         InitSerio( 38400, VPC3_TRUE );
      #endif//#ifdef RS232_SERIO
   #endif//#ifdef EvaBoard_AT89C5132

   #if VPC3_SERIAL_MODE
      #if VPC3_SPI

         SPI_CS = 1;
         SPCON |= 0x10;                //Master mode
         //SPCON |= 0x82;              //Fclk Periph/128
         //SPCON |= 0x80;//0x02;       //Fclk Periph/8
         SPCON |= 0x01;                //
         SPCON |= 0x20;                //SSA disable
         SPCON &= ~0x08;               //CPOL=0; transmit mode example
         SPCON &= ~0x04;               //CPHA=0; transmit mode example
         //SPCON |= 0x04;              //CPHA=1; transmit mode example
         IEN1 |= 0x04;                 //enable spi interrupt
         SPCON |= 0x40;                //run spi

         //set priority of interrupt
         IPH1 |= MSK_ESPI;
         IPL1 |= MSK_ESPI;

      #endif//#if VPC3_SPI

      #if VPC3_I2C

         I2C_Init();

      #endif//#if VPC3_I2C
   #endif//#if VPC3_SERIAL_MODE

   #ifdef EvaBoard_AT89C5132
      #ifdef LCD
         //init LCD-Display
         LCDInitDisplay();
      #endif//#ifdef LCD
   #endif//#ifdef EvaBoard_AT89C5132

   #if DP_TIMESTAMP
      IPLX0 = 0;
      IPLT0 = 1;

      //THTL = 65535-(T*10MHz/6)
      TH0 = 0xFF;
      TL0 = 0x6C;
      TMOD |= 0x01;        // timer0 is mode1 : 16 bits
      CKCON |= MSK_T0X2;   // Set_timer0_x1_mode();   // timer0 always in x1 mode, even if x2 mode is set for std clk.
      TR0=0;
      ET0=1;
   #else
      #if VPC3_SERIAL_MODE
         IPLX0 = 0;
         IPLT0 = 1;

         //THTL = 65535-(T*10MHz/6)
         TH0 = 0xFF;
         TL0 = 0x6C;
         TMOD |= 0x01;        // timer0 is mode1 : 16 bits
         CKCON |= MSK_T0X2;   // Set_timer0_x1_mode();   // timer0 always in x1 mode, even if x2 mode is set for std clk.
         TR0=1;
         ET0=1;
      #endif//#if VPC3_SERIAL_MODE
   #endif//#if DP_TIMESTAMP

   // enable interrupts
   EA = 1;

   // todo: do other hardware-initialisation
}//void InitHardware( void )

/*---------------------------------------------------------------------------*/
/* function:   CheckExternalRam                                              */
/*---------------------------------------------------------------------------*/
DP_ERROR_CODE CheckExternalRam( void )
{
data DP_ERROR_CODE  bError;
data uint16_t          i;
data uint16_t          j;

   bError = DP_OK;

   j = 5432;
   for( i = 0x4000; i < 0x8000; )
   {
      *((uint16_t xdata*)i)=j;
      i+=2;
      j++;
   }//for( i = 0x4000; i < 0x8000; )

   j = 5432;
   for( i = 0x4000; i < 0x8000; )
   {
      if(*((uint16_t xdata*)i)!=j)
      {
        bError = DP_SRAM_ERROR; // ERROR
      }
      i+=2;
      j++;
   }//for( i = 0x4000; i < 0x8000; )

   j = 0;
   for( i = 0x4000; i < 0x8000; )
   {
      *((uint16_t xdata*)i)=j;
      i+=2;
   }//for( i = 0x4000; i < 0x8000; )

   return bError;
}//DP_ERROR_CODE CheckExternalRam( void )

/*---------------------------------------------------------------------------*/
/* interrupt: external interrupt 0                                           */
/*---------------------------------------------------------------------------*/
#ifdef EvaBoard_AT89C5132
   #if( VPC3_SERIAL_MODE == 0 )

      void ex0_ISR (void) interrupt IRQ_INT0
      {
         VPC3_Isr();
      }//void ex0_ISR (void) interrupt IRQ_INT0

   #endif//#if( VPC3_SERIAL_MODE == 0 )
#endif//#ifdef EvaBoard_AT89C5132

/*---------------------------------------------------------------------------*/
/* function: Vpc3SpiIsr                                                      */
/*---------------------------------------------------------------------------*/
#ifdef EvaBoard_AT89C5132
   #if VPC3_SERIAL_MODE
      #if VPC3_SPI

         void Vpc3SpiIsr( void )
         {
            //read and clear spi status register
            switch( SPSTA )
            {
               case 0x80:
               {
                  bSpiData = SPDAT;             // read receive data
                  bSpiTransmitCompleted = TRUE; // set software flag
                  break;
               }//case 0x80:

               case 0x10:
               {
                  //put here for mode fault tasking
                  #ifdef RS232_SERIO
                     print_string("FAULT");
                  #endif//#ifdef RS232_SERIO
                  break;
               }//case 0x10:

               case 0x40:
                {
                  //put here for overrun tasking
                  #ifdef RS232_SERIO
                     print_string("Overrun");
                  #endif//#ifdef RS232_SERIO
                  break;
               }//case 0x40:

               default:
               {
                  break;
               }//default:
            }//switch( SPSTA )
         }//void Vpc3SpiIsr( void )

      #endif//#if VPC3_SPI
   #endif//#if VPC3_SERIAL_MODE
#endif//#ifdef EvaBoard_AT89C5132

/*---------------------------------------------------------------------------*/
/* interrupt: SPI                                                            */
/*---------------------------------------------------------------------------*/
#ifdef EvaBoard_AT89C5132
   #if VPC3_SERIAL_MODE
      #if VPC3_SPI
         void SPI_ISR(void) interrupt IRQ_SPI
         {
            Vpc3SpiIsr();
         }//void SPI_ISR(void) interrupt IRQ_SPI
      #endif//#if VPC3_SPI
   #endif//#if VPC3_SERIAL_MODE
#endif//#ifdef EvaBoard_AT89C5132

/*---------------------------------------------------------------------------*/
/* interrupt: external interrupt 1                                           */
/*---------------------------------------------------------------------------*/
#ifdef EvaBoard_AT89C5132
   #if DP_ISOCHRONOUS_MODE
      void ex1_ISR (void) interrupt IRQ_INT1
      {
         DpIso_Isr();
      }//void ex1_ISR (void) interrupt IRQ_INT1
   #endif//#if DP_ISOCHRONOUS_MODE
#endif//#ifdef EvaBoard_AT89C5132

#ifdef EvaBoard_AT89C5132
   #if DP_TIMESTAMP
      void Timer0_ISR (void) interrupt IRQ_T0
      {
         TimeStamp_Timer();
      }//void Timer0_ISR (void) interrupt IRQ_T0
   #else
      #if VPC3_SERIAL_MODE
         void Timer0_ISR (void) interrupt IRQ_T0
         {
            TH0 = 0xFF;
            TL0 = 0x6C; //100µs
            if( ++bCounter10ms == 100 )
            {
               DpAppl_IsrTimerClock();
               bCounter10ms = 0;
            }//if( ++bCounter10ms == 100 )
         }//void Timer0_ISR (void) interrupt IRQ_T0
      #endif//#if VPC3_SERIAL_MODE
   #endif//#if DP_TIMESTAMP
#endif//#ifdef EvaBoard_AT89C5132

/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

