/*****************************  Filename: lcd.c ******************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0      Einsteinstraﬂe 6        */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      91074 Herzogenaurach    */
/* 0000  000  0   0 000   0 0     00000 0 0000       Germany                 */
/* 0     00   0   0 0     0 0     0   0 0 0                                  */
/* 0     0 0  0   0 0     0 0   0 0   0 0 0          Tel: ++49-9132-744-200  */
/* 0     0  0  000  0     0  000  0   0 0 0    GmbH  Fax: ++49-9132-744-204  */
/*                                                                           */
/* ========================================================================= */
/*                                                                           */
/* Function:       lcd functions                                             */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Technical support:       Peter FREDEHORST                                 */
/*                          Tel. : ++49-9132-744-2150                        */
/*                          Fax. : ++49-9132-744-29-2150                     */
/*                          eMail: pfredehorst@profichip.com                 */
/*                          eMail: support@profichip.com                     */
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
#include "..\platform.h"

#ifdef LCD

/*---------------------------------------------------------------------------*/
/* local function prototypes                                                 */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global data definitions                                                   */
/*---------------------------------------------------------------------------*/
code uint8_t ascc[16] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46};


/*---------------------------------------------------------------------------*/
/* local data definitions                                                    */
/*---------------------------------------------------------------------------*/
sLCD LCD_PTR   pLCD; // pointer to LCD structure

/*---------------------------------------------------------------------------*/
/* function: LCDDelay                                                        */
/*---------------------------------------------------------------------------*/
void LCDDelay( uint8_t bDelay )
{
uint16_t i,j;

   for( i = 0; i < bDelay; i++ )
   {
      for( j = 0; j < 5; j++ )
      {
         _nop_();
      }//for( j = 0; j < 5; j++ )
   }//for( i = 0; i < bDelay; i++ )
}//void LCDDelay( uint8_t bDelay )

/*---------------------------------------------------------------------------*/
/* function: WaitReady                                                       */
/*---------------------------------------------------------------------------*/
static uint8_t WaitReady( void )
{
uint16_t i;

   LCDDelay(1);

   i = 0xf000;
   while( i-- )
   {
      if( !( pLCD->ctrl.rd.bBusy & LCD_BUSY_FLAG ) )
      {
            return TRUE;
      }//if( !( pLCD->ctrl.rd.bBusy & LCD_BUSY_FLAG ) )
   }//while( i-- )

   return FALSE;
}//static uint8_t WaitReady( void )

/*---------------------------------------------------------------------------*/
/* function: LCDOn                                                        */
/*---------------------------------------------------------------------------*/
void LCDOn( void )
{
   //pLCD->ctrl.wr.bIR = 0x0E;      //Display,Cursor,
   pLCD->ctrl.wr.bIR = 0x0C;     //Display
}//void LCDOn( void )

/*---------------------------------------------------------------------------*/
/* function: LCDOff                                                        */
/*---------------------------------------------------------------------------*/
void LCDOff( void )
{
   pLCD->ctrl.wr.bIR = 0x08;
}//void LCDOff( void )

/*---------------------------------------------------------------------------*/
/* function: LCDClear                                                        */
/*---------------------------------------------------------------------------*/
void LCDClear( void )
{
   pLCD->ctrl.wr.bIR = 0x01;
}//void LCDClear( void )

/*---------------------------------------------------------------------------*/
/* function: LCDReturnHome                                                   */
/*---------------------------------------------------------------------------*/
static uint8_t LCDReturnHome( void )
{
   if( !WaitReady() )
   {
      return FALSE;
   }

   pLCD->ctrl.wr.bIR = 0x02;

   return TRUE;
}//static uint8_t LCDReturnHome( void )

/*---------------------------------------------------------------------------*/
/* function: LCDSetDDRAMAddr                                                 */
/*---------------------------------------------------------------------------*/
static void LCDSetDDRAMAddr( uint8_t bAddr )
{
   WaitReady();

   pLCD->ctrl.wr.bIR = (0x80 | bAddr);
}//static void LCDSetDDRAMAddr( uint8_t bAddr )

/*---------------------------------------------------------------------------*/
/* function: LCDFunctionSet                                                  */
/*---------------------------------------------------------------------------*/
void LCDFunctionSet( void )
{
   pLCD->ctrl.wr.bIR = 0x38;  // Function Set (8 bits, 2 lines, 5x7 dots)
}//void LCDFunctionSet( void )

/*---------------------------------------------------------------------------*/
/* function: LCDModeSet                                                      */
/*---------------------------------------------------------------------------*/
void LCDModeSet( void )
{
   pLCD->ctrl.wr.bIR = 0x06;  // Entry Mode Set ( Moving cursor - increase; No LCD scrolling )
}//void LCDModeSet( void )

/*---------------------------------------------------------------------------*/
/* function: LCDInitDisplay                                                  */
/*---------------------------------------------------------------------------*/
uint8_t LCDInitDisplay( void )
{
   pLCD = (sLCD LCD_PTR)LCD_ADDRESS;

   LCDDelay(255);
   LCDDelay(255);
   LCDDelay(255);
   LCDDelay(255);
   LCDDelay(255);
   LCDDelay(255);
   LCDDelay(255);
   LCDDelay(255);

   LCDFunctionSet();
   LCDDelay(5);
   LCDFunctionSet();
   LCDDelay(5);
   WaitReady();
   LCDOn();
   LCDDelay(5);
   LCDClear();
   LCDDelay(100);
   LCDModeSet();
   LCDDelay(5);

   return TRUE;
}//uint8_t LCDInitDisplay( void )

/*---------------------------------------------------------------------------*/
/* function: LCDUpdateProfichip                                              */
/*---------------------------------------------------------------------------*/
void LCDUpdateProfichip( uint8_t bMainInterface, uint8_t bFunction, uint8_t bBugfix )
{
   LCDDelay(2);
   pLCD->ctrl.wr.bIR = 0x80;
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x50;//P
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x52;//R
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x4F;//O
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x46;//F
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x49;//I
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x43;//C
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x48;//H
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x49;//I
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x50;//P
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x20;
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x47;//G
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x6D;//m
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x62;//b
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x48;//H
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x20;
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x56;//V
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = ascc[(bMainInterface & 0x0F)];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x2E;//.
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = ascc[(bFunction & 0x0F)];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = ascc[(bBugfix & 0x0F)];
   LCDDelay(2);
}//void LCDUpdateProfichip( void )

/*---------------------------------------------------------------------------*/
/* function: LCDUpdate                                                       */
/*---------------------------------------------------------------------------*/
void LCDUpdate( uint8_t *pChar, uint16_t wAsicType )
{
   LCDDelay(2);
   pLCD->ctrl.wr.bIR = 0xC0;

   LCDDelay(2);
   pLCD->ctrl.wr.bDR = pChar[0];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = pChar[1];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = pChar[2];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = pChar[3];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = pChar[4];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = pChar[5];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = pChar[6];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = pChar[7];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = pChar[8];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = pChar[9];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = pChar[10];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = pChar[11];

   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x20;
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x20;

   switch( wAsicType & 0xF000 )
   {
      case DP_COMP_INSTALLED_VPC3_B:
      {
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x56;//V
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x50;//P
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x43;//C
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x33;//3
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x2B;//+
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x42;//B
         LCDDelay(2);
         break;
      }//case DP_COMP_INSTALLED_VPC3_B:

      case DP_COMP_INSTALLED_VPC3_C:
      {
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x56;//V
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x50;//P
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x43;//C
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x33;//3
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x2B;//+
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x43;//C
         LCDDelay(2);
         break;
      }//case DP_COMP_INSTALLED_VPC3_C:

      case DP_COMP_INSTALLED_VPC3_D:
      {
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x20;//M
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x50;//P
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x50;//I
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x43;//1
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x33;//2
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x2B;//x
         LCDDelay(2);
         break;
      }//case DP_COMP_INSTALLED_VPC3_D:

      case DP_COMP_INSTALLED_VPC3_S:
      {
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x56;//V
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x50;//P
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x43;//C
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x33;//3
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x2B;//+
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x53;//S
         LCDDelay(2);
         break;
      }//case DP_COMP_INSTALLED_VPC3_S:

      default:
      {
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x56;//V
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x50;//P
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x43;//C
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x33;//3
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x2B;//+
         LCDDelay(2);
         pLCD->ctrl.wr.bDR = 0x20;
         LCDDelay(2);
      }//default:
   }//switch( wAsicType & 0xF000 )
}//void LCDUpdate( uint8_t *pChar, uint16_t wAsicType )

/*---------------------------------------------------------------------------*/
/* function: LCDUpdateDisplay                                                */
/*---------------------------------------------------------------------------*/
uint8_t LCDUpdateDisplay( void )
{
   LCDDelay(2);
   pLCD->ctrl.wr.bIR = 0x80;
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x50;//P
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x52;//R
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x4F;//O
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x46;//F
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x49;//I
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x43;//C
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x48;//H
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x49;//I
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x50;//P
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x20;
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x47;//G
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x6D;//m
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x62;//b
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x48;//H

   LCDDelay(2);
   pLCD->ctrl.wr.bIR = 0xC0;
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x4D;//M
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x50;//P
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x49;//I
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x31;//1
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x32;//2
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x78;//x
   LCDDelay(2);

   LCDDelay(2);
   pLCD->ctrl.wr.bIR = (0x80 | 0x4B);
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x4D;//M
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x57;//W
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x31;//1
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x30;//0
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x3A;//:
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x30;//0
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x30;//0
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x30;//0
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x30;//0
   LCDDelay(2);

    return TRUE;
}//

void LCDUpdateWord( uint8_t bAddress, uint16_t wValue )
{
   bAddress = bAddress;
   wValue = wValue;
}//void LCDUpdateWord( uint8_t bAddress, uint16_t wValue )

void LCDUpdateTime( uint8_t bAddress, uint8_t bHour, uint8_t bMinute )
{
   LCDDelay(2);
   pLCD->ctrl.wr.bIR = (0x80 | bAddress);
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = ascc[(uint8_t)((bHour >> 4) & 0x0F)];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = ascc[(uint8_t)(bHour & 0x0F)];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = 0x3A;//:
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = ascc[(uint8_t)((bMinute >> 4) & 0x0F)];
   LCDDelay(2);
   pLCD->ctrl.wr.bDR = ascc[(uint8_t)(bMinute & 0x0F)];
   LCDDelay(2);
}//void LCDUpdateWord( uint8_t bAddress, uint16_t wValue )

#endif//#ifdef LCD

/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

