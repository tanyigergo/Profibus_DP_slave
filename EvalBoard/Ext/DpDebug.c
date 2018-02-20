/**********************  Filename: DpDebug.c  ********************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0      Einsteinstraße 6        */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      91074 Herzogenaurach    */
/* 0000  000  0   0 000   0 0     00000 0 0000       Germany                 */
/* 0     00   0   0 0     0 0     0   0 0 0                                  */
/* 0     0 0  0   0 0     0 0   0 0   0 0 0          Phone: ++499132744200   */
/* 0     0  0  000  0     0  000  0   0 0 0    GmbH  Fax:   ++4991327442164  */
/*                                                                           */
/* ========================================================================= */
/*                                                                           */
/* Description: Debug fuctions for VPC3+C/S.                                 */
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

#include "..\platform.h"

#ifdef RS232_SERIO

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* function: Wait                                                            */
/*---------------------------------------------------------------------------*/
void Wait( void )
{
   do
   {
      // wait!
      // VPC3_RESET_USER_WD();   // toggle user watchdog
   }
   while( bSndCounter > 50);
}//void Wait( void )

/*---------------------------------------------------------------------------*/
/* function: PrintDebugBuffer                                                */
/*---------------------------------------------------------------------------*/
#ifdef DP_DEBUG_ENABLE
void PrintDebugBuffer( void )
{
uint16_t wIndex;
uint16_t i;

   print_string("\r\nDebug_Buffer: ");
   print_hexbyte( bDebugBufferIndex );
   print_hexbyte( bDebugBufferOverlapped );

   Wait();

   wIndex = 0;
   if( bDebugBufferOverlapped == TRUE )
   {
      for( i = bDebugBufferIndex; i < (MAX_NR_OF_DEBUG-1); i++ )
      {
         if( (wIndex%4) == 0 )
         {
            Wait();

            print_string("\r\n");
            print_hexbyte(wIndex);
            print_string("  ");
         }//if( (wIndex%4) == 0 )
         print_hexbyte( asDebugBuffer[i].bDebugCode );
         print_hexbyte( asDebugBuffer[i].bDetail1   );
         print_hexbyte( asDebugBuffer[i].bDetail2   );
         print_string(" - ");
         wIndex++;
      }//for( i = 0; i < bDebugBufferIndex; i++ )
   }//if( bDebugBufferOverlapped == TRUE )

   for( i = 0; i < bDebugBufferIndex; i++ )
   {
      if( (wIndex%4) == 0 )
      {
         Wait();

         print_string("\r\n");
         print_hexbyte(wIndex);
         print_string("  ");
      }//if( (wIndex%4) == 0 )
      print_hexbyte( asDebugBuffer[i].bDebugCode );
      print_hexbyte( asDebugBuffer[i].bDetail1   );
      print_hexbyte( asDebugBuffer[i].bDetail2   );
      print_string(" - ");
      wIndex++;
   }//for( i = 0; i < bDebugBufferIndex; i++ )
}//void PrintDebugBuffer( void )
#endif//#ifdef DP_DEBUG_ENABLE

/*---------------------------------------------------------------------------*/
/* function: PrintHelp                                                      */
/*---------------------------------------------------------------------------*/
void PrintHelp( void )
{
   print_string("\r\nHelp: ");
   print_string("\r\na,A : Statusregister");
   print_string("\r\nb,B : Moderegister");
   print_string("\r\nc,C : Organizational Parameter");
   Wait();
   print_string("\r\ne,E : Diagnostic Data");
   print_string("\r\nf,F : Parameter Data");
   print_string("\r\ng,G : Check Config Data");
   print_string("\r\nj,J : VPC3 Free Memory");
   print_string("\r\n");
}//void PrintHelp( void )

/*---------------------------------------------------------------------------*/
/* function: ShowBuffer                                                     */
/*---------------------------------------------------------------------------*/
void ShowBuffer( char * pcMsg, uint8_t * pbPtr, uint16_t wLength )
{
uint16_t i;

   print_string("\r\n");
   print_string( pcMsg );

   print_string("\r\nAdr: ");
   print_hexword( pbPtr );

   print_string("  length: ");
   print_hexword( wLength );

   print_string("\r\nAdr:   00-01-02-03-04-05-06-07-08-09-0A-0B-0C-0D-0E-0F");
   for( i = 0; i < wLength; i++ )
   {
      if( i%16 == 0 )
      {
         Wait();

         print_string("\r\n");
         print_hexword(i);
         print_string("  ");
      }//if( i%16 == 0 )

      print_string(" ");

      #if VPC3_SERIAL_MODE
         print_hexbyte( Vpc3Read( (VPC3_ADR)(pbPtr+i) ) );
      #else
         print_hexbyte( *(pbPtr+i) );
      #endif//#if VPC3_SERIAL_MODE
   }//for( i = 0; i < length; i++ )

   print_string("\r\n");
}//void ShowBuffer( char * pcMsg, uint8_t * pbPtr, uint16_t wLength )

/*---------------------------------------------------------------------------*/
/* function: PrintOrganizationalParameter                                    */
/*---------------------------------------------------------------------------*/
void PrintOrganizationalParameter( void )
{
   #if VPC3_SERIAL_MODE
      ShowBuffer("Organizational Parameters:", (uint8_t *)bVpc3RwTsAddr, 42 );
   #else
      ShowBuffer("Organizational Parameters:", &pVpc3->bTsAddr, 42 );
   #endif//#if VPC3_SERIAL_MODE
}//void PrintOrganizationalParameter( void )

/*---------------------------------------------------------------------------*/
/* function: PrintDiagBuffer                                                 */
/*---------------------------------------------------------------------------*/
void PrintDiagBuffer( void )
{
   print_string("\r\nDiag_SM:");
   ShowBuffer("Diag Buffer1:", VPC3_GET_DIAG1_PTR(), VPC3_GET_DIAG1_LENGTH() );
   ShowBuffer("Diag Buffer2:", VPC3_GET_DIAG2_PTR(), VPC3_GET_DIAG2_LENGTH() );
}//void PrintDiagBuffer( void )

/*---------------------------------------------------------------------------*/
/* function: PrintPrmData                                                    */
/*---------------------------------------------------------------------------*/
void PrintPrmData( void )
{
   ShowBuffer("NEW_PRM_DATA:", VPC3_GET_PRM_BUF_PTR(), VPC3_GET_PRM_LEN() );
}//void PrintPrmData( void )

/*---------------------------------------------------------------------------*/
/* function: print cfg_data                                                  */
/*---------------------------------------------------------------------------*/
void PrintCfgData( void )
{
   ShowBuffer("Read  CFG Buffer:", VPC3_GET_READ_CFG_BUF_PTR(), VPC3_GET_READ_CFG_LEN() );
   ShowBuffer("Check CFG Buffer:", VPC3_GET_CFG_BUF_PTR(), VPC3_GET_CFG_LEN() );
}//void PrintCfgData( void )

/*---------------------------------------------------------------------------*/
/* function: PrintSerialInputs                                               */
/*---------------------------------------------------------------------------*/
void PrintSerialInputs( void )
{
   if( bRecCounter > 0 )
   {
      switch( abRecBuffer[bReadRecPtr] )
      {
         case 0x3F: // Key '?':
         {
             PrintHelp();
             bRecCounter--;
             break;
         }//case 0x3F:

         case 0x61: // Key 'a'
         case 0x41: // Key 'A'
         {
            print_string("\r\nStatusreg: ");
            print_hexbyte( VPC3_GET_STATUS_H() );
            print_string(" ");
            print_hexbyte( VPC3_GET_STATUS_L() );
            bRecCounter--;
            break;
         }//case 0x61:

         // SPC3: mode register only writable
         case 0x62: // Key 'b'
         case 0x42: // Key 'B'
         {
             print_string("\r\nModereg1: ");
             print_hexbyte( VPC3_GET_MODE_REG_1() );
             print_hexbyte( VPC3_GET_HW_MODE_HIGH());
             print_hexbyte( VPC3_GET_HW_MODE_LOW());
             bRecCounter--;
             break;
         }//case 0x62:

         case 0x63: // Key 'c'
         case 0x43: // Key 'C'
         {
             PrintOrganizationalParameter();
             bRecCounter--;
             break;
         }//case 0x63:

         case 0x65: // Key 'e'
         case 0x45: // Key 'E'
         {
             PrintDiagBuffer();
             bRecCounter--;
             break;
         }//case 0x65:

         case 0x66: // Key 'f'
         case 0x46: // Key 'F'
         {
             PrintPrmData();
             bRecCounter--;
             break;
         }//case 0x66:

         case 0x67: // Key 'g'
         case 0x47: // Key 'G'
         {
             PrintCfgData();
             bRecCounter--;
             break;
         }//case 0x67:

         case 0x6A: // Key 'j'
         case 0x4A: // Key 'J'
         {
             print_string("\r\nVPC3+ free memory: 0x");
             print_hexword( VPC3_GetFreeMemory() );
             bRecCounter--;
             break;
         }//case 0x6A:

         #if REDUNDANCY
            case 0x6B: // Key 'k'
            case 0x4B: // Key 'K'
            {
               DP_ClearDebugBuffer__();
               sRedCom.bSwitchOverChannel1 = TRUE;
               print_string("\r\nPrmCmd_1");
               //print_hexbyte( sRedCom.bSwitchOverChannel1 );
               bRecCounter--;
               break;
            }//case 0x6B:
            
            case 0x6C: // Key 'l'
            case 0x4C: // Key 'L'
            {
               DP_ClearDebugBuffer__();
               sRedCom.bSwitchOverChannel2 = TRUE;
               print_string("\r\nPrmCmd_2");
               //print_hexbyte( sRedCom.bSwitchOverChannel2 );
               bRecCounter--;
               break;
            }//case 0x6C:
         #endif /* #if REDUNDANCY */

         #ifdef DP_DEBUG_ENABLE
             case 0x79: // Taste 'y'
             case 0x59: // Taste 'Y'
             {
                 DpClearDebugBuffer();
                 bRecCounter--;
                 break;
             }//case 0x79:

             case 0x7A: // Taste 'z'
             case 0x5A: // Taste 'Z'
             {
                 PrintDebugBuffer();
                 bRecCounter--;
                 break;
             }//case 0x7A:
         #endif//#ifdef DP_DEBUG_ENABLE

         default:
         {
             bRecCounter--;
             break;
         }//default:
      }//switch (abRecBuffer[bReadRecPtr])

      bReadRecPtr++;
   }//if( bRecCounter > 0 )
}//void PrintSerialInputs( void )

#endif//#ifdef RS232_SERIO

/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

