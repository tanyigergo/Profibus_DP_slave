/*****************************  Filename: serio.c ****************************/
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
/* Function:       serial function                                           */
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
#include <string.h>
#include "..\platform.h"

#ifdef RS232_SERIO

/*---------------------------------------------------------------------------*/
/* local function prototypes                                                 */
/*---------------------------------------------------------------------------*/
void StartSerialCommunication ( void );
void set_baud                 ( uint16_t baudrate );

/*---------------------------------------------------------------------------*/
/* global data definitions                                                   */
/*---------------------------------------------------------------------------*/
uint8_t       abSndBuffer [256]; // for RS232 sending
uint8_t       abRecBuffer [256]; // for RS232 receiving

uint8_t data  bSerInterrupt;
uint8_t data  bWriteRecPtr;  // Writepointer into receive buffer
uint8_t data  bReadRecPtr;   // Readpointer  from receive buffer
uint8_t data  bRecCounter;   // Counter of byte in receive buffer
uint8_t data  bWriteSndPtr;  // Writepointer into send buffer
uint8_t data  bReadSndPtr;   // Readpointer  from send buffer
uint8_t data  bSndCounter;   // Counter of byte in send buffer
uint8_t       bSerialState;  // shows state of serial line
bit           bThrEmpty;     // Transmitter reg. empty

/*---------------------------------------------------------------------------*/
/* local data definitions                                                    */
/*---------------------------------------------------------------------------*/
//static const uint8_t asccii[16] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46};
uint8_t asccii[16];
bit     parity;         // Save parity bit received

sbit CTS    = 0x93;
sbit RTS    = 0x94;


/*---------------------------------------------------------------------------*/
/* functions                                                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*   void print_string__(uint8_t *p)                                           */
/*                                                                           */
/*   function:     print string                                              */
/*                                                                           */
/*---------------------------------------------------------------------------*/
void print_string__( uint8_t *p )
{
   while ( *p )
   {
       abSndBuffer [bWriteSndPtr++] = *p++;
       bSndCounter++;
   }
   StartSerialCommunication();
}//void print_string__( uint8_t *p )


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*   uint8_t print_hexbyte__(uint8_t b)                                          */
/*                                                                           */
/*   function:     konverts integer to asccii                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
void print_hexbyte__(uint8_t b)
{
   abSndBuffer [bWriteSndPtr++] = asccii[(b >> 4)];
   bSndCounter++;
   abSndBuffer [bWriteSndPtr++] = asccii[(b & 0x0F)];
   bSndCounter++;
   StartSerialCommunication();
}//void print_hexbyte__(uint8_t b)


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*   uint8_t print_hexword__(uint16_t w)                                          */
/*                                                                           */
/*   function:     konverts integer to asccii                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
void print_hexword__(uint16_t w)
{
   abSndBuffer [bWriteSndPtr++] = asccii[(uint8_t)((w >> 12) & 0x000F)];
   bSndCounter++;
   abSndBuffer [bWriteSndPtr++] = asccii[(uint8_t)((w >> 8) & 0x000F)];
   bSndCounter++;
   abSndBuffer [bWriteSndPtr++] = asccii[(uint8_t)((w >> 4) & 0x000F)];
   bSndCounter++;
   abSndBuffer [bWriteSndPtr++] = asccii[(uint8_t)(w & 0x000F)];
   bSndCounter++;
   StartSerialCommunication();
}//void print_hexword__(uint16_t w)


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*   uint8_t print_hexlong__(uint32_t dw)                                          */
/*                                                                           */
/*   function:     konverts integer to asccii                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
void print_hexlong__(uint32_t dw)
{
   abSndBuffer [bWriteSndPtr++] = asccii[(uint8_t)((dw >> 28) & 0x000F)];
   bSndCounter++;
   abSndBuffer [bWriteSndPtr++] = asccii[(uint8_t)((dw >> 24) & 0x000F)];
   bSndCounter++;
   abSndBuffer [bWriteSndPtr++] = asccii[(uint8_t)((dw >> 20) & 0x000F)];
   bSndCounter++;
   abSndBuffer [bWriteSndPtr++] = asccii[(uint8_t)((dw >> 16) & 0x000F)];
   bSndCounter++;
   abSndBuffer [bWriteSndPtr++] = asccii[(uint8_t)((dw >> 12) & 0x000F)];
   bSndCounter++;
   abSndBuffer [bWriteSndPtr++] = asccii[(uint8_t)((dw >> 8) & 0x000F)];
   bSndCounter++;
   abSndBuffer [bWriteSndPtr++] = asccii[(uint8_t)((dw >> 4) & 0x000F)];
   bSndCounter++;
   abSndBuffer [bWriteSndPtr++] = asccii[(uint8_t)(dw & 0x000F)];
   bSndCounter++;
   StartSerialCommunication();
}//void print_hexlong__(uint32_t dw)


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*   void StartSerialCommunication(void)                                   */
/*                                                                           */
/*   function:     starts serial communication                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
void StartSerialCommunication( void )
{
   if( bSerInterrupt == TRUE )
   {
      if( bSndCounter > 0 )
      {
         // disable interrupts
         DpAppl_DisableInterruptVPC3Channel1();

         if( bThrEmpty )
         {
            bThrEmpty = FALSE;
            // set RTS handshake
            //RTS = 0;
            // transmit trigger via softw. interrupt
            TI = 1;
         }//if( bThrEmpty )

         // enable interrupts
         DpAppl_EnableInterruptVPC3Channel1();
      }//if( bSndCounter > 0 )
   }//if( bSerInterrupt == TRUE )
}//void StartSerialCommunication( void )


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*   void init_serio__(uint16_t baudrate)                                        */
/*                                                                           */
/*   function:     initialize serial communication                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/
#ifdef EvaBoard_AT89C5132

   #define Set_SMOD1()              (PCON |= MSK_SMOD1)

void InitSerio( uint16_t bBaudrate, uint8_t bInterruptEnable )
{
   memset(&abSndBuffer[0], 0, sizeof(abSndBuffer));
   memset(&abRecBuffer[0], 0, sizeof(abSndBuffer));

   bWriteRecPtr = 0;      // Writepointer into receive buffer
   bReadRecPtr  = 0;      // Readpointer  from receive buffer
   bRecCounter   = 0;      // Counter of byte in receive buffer
   bWriteSndPtr = 0;      // Writepointer into send buffer
   bReadSndPtr  = 0;      // Readpointer  from send buffer
   bSndCounter   = 0;      // Counter of byte in send buffer

   asccii[0] = 0x30;
   asccii[1] = 0x31;
   asccii[2] = 0x32;
   asccii[3] = 0x33;
   asccii[4] = 0x34;
   asccii[5] = 0x35;
   asccii[6] = 0x36;
   asccii[7] = 0x37;
   asccii[8] = 0x38;
   asccii[9] = 0x39;
   asccii[10] = 0x41;
   asccii[11] = 0x42;
   asccii[12] = 0x43;
   asccii[13] = 0x44;
   asccii[14] = 0x45;
   asccii[15] = 0x46;

   bSerialState  = ON;     // shows state of serial line
   bThrEmpty     = TRUE;   // Transmitter reg. empty

   TXD = 1;
   RXD = 1;
   SCON = 0x50;               //uart mode: mode 1, 8bit, receive enable
   BDRCON &=0xEC;                //BRR=0; SRC=0;
   BDRCON |=0x0E;                //TBCK=1; RBCK=1; SPD=1

   #ifdef AT89C5132_MODE_X2
      switch( bBaudrate )
      {
         case 115200:
         {
            Set_SMOD1();      //SMOD1 = 1
            BRL = 245;
            break;
         }//case 115200:

         case 57600:
         {
            Set_SMOD1();      //SMOD1 = 1
            BRL = 234;
            break;
         }//case 57600:

         case 19200:
         {
            Set_SMOD1();      //SMOD1 = 1
            BRL = 191;
            break;
         }//case 19200:

         case 9600:
         {
            Set_SMOD1();      //SMOD1 = 1
            BRL = 126;
            break;
         }//case 9600:

         case 4800:
         {
            BRL = 126;
            break;
         }//case 4800:

         case 38400:
         default:
         {
            Set_SMOD1();      //SMOD1 = 1
            BRL = 223;
            break;
         }//default:
      }//switch( bBaudrate )
   #else

      Set_SMOD1();            //SMOD1 = 1
      switch( bBaudrate )
      {
         case 57600:
         {
            BRL = 245;
            break;
         }//case 57600:

         case 19200:
         {
            BRL = 223;
            break;
         }//case 19200:

         case 9600:
         {
            BRL = 191;
            break;
         }//case 9600:

         case 4800:
         {
            BRL = 126;
            break;
         }//case 4800:

         case 38400:
         default:
         {
            BRL = 240;
            break;
         }//default:
      }//switch( bBaudrate )
   #endif//#ifdef AT89C5132_MODE_X2

   REN=1;
   // clear interrupt flags
   TI = RI = 0;

   bSerInterrupt = bInterruptEnable;
   if( bInterruptEnable == TRUE )
   {
      IPH0 |= MSK_ES;
      // IPL0 |= MSK_ES;
      ES = 1;                 //Enable serial interrupt
      BDRCON |= 0x10;         //Baud rate generator run
   }//if( bInterruptEnable == TRUE )
}//void init_serio__ (uint16_t baudrate)
#endif//#ifdef EvaBoard_AT89C5132

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*   void ser_isr ( void ) interrupt                                         */
/*                                                                           */
/*   function:     interrupt routine                                         */
/*                                                                           */
/*---------------------------------------------------------------------------*/
void ser_isr ( void ) interrupt IRQ_UART using 1          // Keil Compiler
{
   //disable interrupts
   DpAppl_DisableInterruptVPC3Channel1();

   //send fifo empty
   if( _testbit_( TI ) )
   {
      if( bSndCounter == 0 )
      {
         // Nothing to send, set flag
         bThrEmpty = 1;
         // RTS handshake
         RTS = 1;
      }//if( bSndCounter == 0 )
      else
      {
         // something to send
         // Byte moved through the ACCU, to get the correct parity set
         ACC = abSndBuffer [bReadSndPtr];
         // even parity
         //TB8 = P;
         // start send
         SBUF = ACC;
         // wraps automatically, since Byte
         bReadSndPtr++;
         bSndCounter--;
      }//else of if( bSndCounter == 0 )
   }//if( _testbit_( TI ) )

   // receive fifo full
   if( _testbit_( RI ) )
   {
      // read it in!
      abRecBuffer [bWriteRecPtr] = SBUF;
      // Byte moved through the ACCU automatically, so
      // is the correct parity set
      /*
      parity = P;
      // here, check EVEN parity
      if( parity != RB8 )
      {

      }
      else
      {
          // clear error
      }
      */
      if( bSerialState == ON )
      {
          // wraps automatically since byte
          bWriteRecPtr++;
          bRecCounter++;
      }//if( bSerialState == ON )
      else
      {
          // clear serial buffer, serial communication is closed
          bWriteRecPtr = bReadRecPtr;
          bRecCounter = 0;
      }//else of if( bSerialState == ON )
   }//if( _testbit_( RI ) )

   // enable interrupts
   DpAppl_EnableInterruptVPC3Channel1();
}//void ser_isr ( void ) interrupt IRQ_UART using 1


/*---------------------------------------------------------------------------*/
/* function: ProfibusMain                                                    */
/*---------------------------------------------------------------------------*/
void PollSerio( void )
{
   if( bSerInterrupt == FALSE )
   {
      //send fifo empty
      //if( _testbit_( TI ) )
      {
         if( bSndCounter == 0 )
         {
            // Nothing to send, set flag
            bThrEmpty = 1;
            // RTS handshake
            RTS = 1;
         }//if( bSndCounter == 0 )
         else
         {
            // something to send
            // Byte moved through the ACCU, to get the correct parity set
            ACC = abSndBuffer [bReadSndPtr];
            // even parity
            //TB8 = P;
            // start send
            SBUF = ACC;
            // wraps automatically, since Byte
            bReadSndPtr++;
            bSndCounter--;
         }//else of if( bSndCounter == 0 )
      }//if( _testbit_( TI ) )

      // receive fifo full
      if( _testbit_( RI ) )
      {
         // read it in!
         abRecBuffer [bWriteRecPtr] = SBUF;
         // Byte moved through the ACCU automatically, so
         // is the correct parity set
         /*
         parity = P;
         // here, check EVEN parity
         if( parity != RB8 )
         {

         }
         else
         {
             // clear error
         }
         */
         if( bSerialState == ON )
         {
             // wraps automatically since byte
             bWriteRecPtr++;
             bRecCounter++;
         }//if( bSerialState == ON )
         else
         {
             // clear serial buffer, serial communication is closed
             bWriteRecPtr = bReadRecPtr;
             bRecCounter = 0;
         }//else of if( bSerialState == ON )
      }//if( _testbit_( RI ) )
   }//if( bSerInterrupt == FALSE )
}//void PollSerio( void )

#endif//#ifdef RS232_SERIO

/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

