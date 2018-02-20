/**************************  Filename: DpAppl.c  *****************************/
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
/* Function:      Demo for PROFICHIP Extension Board AT89C5132               */
/*                This example simulates simple profibus device              */
/*                with 2 byte of input and 2 byte of output data.            */
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
#include "lcd16x2.h"
#include <string.h>
#include "platform.h"
#include "DpAppl.h"

/*---------------------------------------------------------------------------*/
/* defines, structures                                                       */
/*---------------------------------------------------------------------------*/
volatile uint8_t masterData[2];
/*---------------------------------------------------------------------------*/
/* global user data definitions                                              */
/*---------------------------------------------------------------------------*/
VPC3_STRUC_PTR             pVpc3;               /**< Pointer to VPC3+ structure. */
VPC3_STRUC_PTR             pVpc3Channel1;       /**< Pointer to VPC3+ structure channel 1. */

VPC3_ADR                   Vpc3AsicAddress;     /**< Global VPC3 address. */

VPC3_SYSTEM_STRUC_PTR      pDpSystem;           /**< Pointer to global profibus system structure. */
VPC3_SYSTEM_STRUC          sDpSystemChannel1;   /**< Global profibus system structure. */

VPC3_STRUC_ERRCB           sVpc3Error;          /**< Error structure. */
DP_APPL_STRUC              sDpAppl;             /**< User application structure. */

#if VPC3_SERIAL_MODE
   VPC3_STRUC              sVpc3OnlyForInit;    /**< Structure is used for VPC3+ initialization in serial mode. */
#endif//#if VPC3_SERIAL_MODE

/*---------------------------------------------------------------------------*/
/* defines, structures and variables for our demo application                */
/*---------------------------------------------------------------------------*/

#ifdef EvaBoard_AT89C5132
   #if VPC3_SERIAL_MODE
      #if VPC3_SPI
         ROMCONST__ uint8_t NAME[12] = { 0x45, 0x41, 0x53, 0x59, 0x34, 0x37, 0x31, 0x31, 0x2D, 0x53, 0x50, 0x49 }; //EASY4711-SPI
      #endif//#if VPC3_SPI

      #if VPC3_I2C
         ROMCONST__ uint8_t NAME[12] = { 0x45, 0x41, 0x53, 0x59, 0x34, 0x37, 0x31, 0x31, 0x2D, 0x49, 0x49, 0x43 }; //EASY4711-IIC
      #endif//#if VPC3_I2C
   #else
         ROMCONST__ uint8_t NAME[12] = { 0x45, 0x41, 0x53, 0x59, 0x34, 0x37, 0x31, 0x31, 0x20, 0x20, 0x20, 0x20 }; //EASY4711
   #endif//#if VPC3_SERIAL_MODE
#endif//#ifdef EvaBoard_AT89C5132

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* function: DpAppl_TestApplEvent                                           */
/*--------------------------------------------------------------------------*/
/*!
  \brief Check if the internal PROFIBUS event is set and clear the event.

  \param[in] eDpApplEv PROFIBUS event @see eDpApplEv_Flags

  \retval VPC3_TRUE Event was set
  \retval VPC3_FALSE Event was not set.
*/
static uint8_t DpAppl_TestApplEvent( eDpApplEv_Flags eDpApplEv )
{
   if( sDpAppl.eDpApplEvent & eDpApplEv )
   {
      sDpAppl.eDpApplEvent &= ~eDpApplEv;
      return VPC3_TRUE;
   }//if( sDpAppl.eDpApplEvent & eDpApplEv )

   return VPC3_FALSE;
}//static uint8_t DpAppl_TestApplEvent( eDpApplEv_Flags eDpApplEv )

/*--------------------------------------------------------------------------*/
/* function: DpAppl_SetApplEvent                                            */
/*--------------------------------------------------------------------------*/
/*!
  \brief Set PROFIBUS event.

  \param[in] eDpApplEv PROFIBUS event @see eDpApplEv_Flags
*/
static void DpAppl_SetApplEvent( eDpApplEv_Flags eDpApplEv )
{
   sDpAppl.eDpApplEvent |= eDpApplEv;
}//static void DpAppl_SetApplEvent( eDpApplEv_Flags eDpApplEv )

/*--------------------------------------------------------------------------*/
/* function: DpAppl_CheckEvIoOut                                            */
/*--------------------------------------------------------------------------*/
/*!
  \brief Handling of the PROFIBUS output data ( master to slave ).

  The VPC3+ has received a DataExchange message and has made the new output data
  available in the N-buffer. In the case of Power_On or Leave_Master, the
  VPC3+ clears the content of the buffer, and generates this event also.
*/
static void DpAppl_CheckEvIoOut( void )
{
VPC3_UNSIGNED8_PTR  pToOutputBuffer;   /**< Pointer to output buffer. */
uint8_t             bOutputState;      /**< State of output data. */

   if( DpAppl_TestApplEvent( eDpApplEv_IoOut ) )
   {
      pToOutputBuffer = VPC3_GetDoutBufPtr( &bOutputState );
      if( pToOutputBuffer )
      {
         CopyFromVpc3_( &sDpAppl.abDpOutputData[0], pToOutputBuffer, pDpSystem->bOutputDataLength );

         /** @todo Write the output data to the output modules. */

         #ifdef EvaBoard_AT89C5132
            *WRITE_PORT1 = sDpAppl.abDpOutputData[0];
            *WRITE_PORT2 = sDpAppl.abDpOutputData[1];
         #endif//#ifdef EvaBoard_AT89C5132
      }//if( pToOutputBuffer )
   }//if( DpAppl_TestApplEvent( eDpApplEv_IoOut ) )
}//static void DpAppl_CheckEvIoOut( void )

/*--------------------------------------------------------------------------*/
/* function: DpAppl_ReadInputData                                           */
/*--------------------------------------------------------------------------*/
/*!
  \brief Handling of the PROFIBUS input data ( slave --> master ).
*/
static void DpAppl_ReadInputData( void )
{
   /** @todo Read cyclically the input module. */

   #ifdef EvaBoard_AT89C5132
      sDpAppl.abDpInputData[0] = *READ_PORT1;
      sDpAppl.abDpInputData[1] = *READ_PORT2;
   #endif//#ifdef EvaBoard_AT89C5132
	
//	sDpAppl.abDpInputData[0] = 0xAA;
//	sDpAppl.abDpInputData[1] = 0xBB;
   VPC3_InputDataUpdate( &sDpAppl.abDpInputData[0] );
}//static void DpAppl_ReadInputData( void )




/*--------------------------------------------------------------------------*/
/* function: DpAppl_ApplicationReady                                        */
/*--------------------------------------------------------------------------*/
/*!
  \brief ApplicatioReady

  This function is called after the PROFIBUS configuration data has been acknowledged
  positive by the user. The slave is now in DataExchange but the static diagnostic bit is set.
  The user can do here own additional initialization and should read here the input data. The
  slave delete now the static diagnostic bit and the master will send DataExchange telegrams.
*/
static void DpAppl_ApplicationReady( void )
{
   #if DPV1_IM_SUPP
      DpIm_ClearImIndex( 0x03 );
   #endif//#if DPV1_IM_SUPP

   /** @todo Make here your own initialization. */

   //read input data
   DpAppl_ReadInputData();

   //reset Diag.Stat
   if( DpDiag_ResetStatDiag() )
   {
      VPC3_ClrDpState( eDpStateCfgOkStatDiag );
      VPC3_SetDpState( eDpStateApplReady );
   }//if( DpDiag_ResetStatDiag() )
}//static void DpAppl_ApplicationReady( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_ProfibusInit                                             */
/*---------------------------------------------------------------------------*/
/*!
  \brief Initializing of PROFIBUS slave communication.
*/
void DpAppl_ProfibusInit( void )
{
DP_ERROR_CODE       bError;

   /*-----------------------------------------------------------------------*/
   /* init application data                                                 */
   /*-----------------------------------------------------------------------*/
   memset( &sDpAppl, 0, sizeof( sDpAppl ) );

   /*-----------------------------------------------------------------------*/
   /* initialize VPC3                                                       */
   /*-----------------------------------------------------------------------*/
   #if VPC3_SERIAL_MODE
      Vpc3AsicAddress = (VPC3_ADR)VPC3_ASIC_ADDRESS;
      pVpc3 = &sVpc3OnlyForInit;
      pDpSystem = &sDpSystemChannel1;

      memset( pVpc3, 0, sizeof( VPC3_STRUC ) );
   #else
      pVpc3Channel1 = (VPC3_STRUC_PTR)VPC3_ASIC_ADDRESS;
      Vpc3AsicAddress = (VPC3_ADR)VPC3_ASIC_ADDRESS;
      pVpc3 = pVpc3Channel1;
      pDpSystem = &sDpSystemChannel1;
   #endif//#if VPC3_SERIAL_MODE

   /*-----------------------------------------------------------------------*/
   /* initialize global system structure                                    */
   /*-----------------------------------------------------------------------*/
   memset( pDpSystem, 0, sizeof( VPC3_SYSTEM_STRUC ));
   pDpSystem->eDpState = eDpStateInit;

   DpPrm_Init();
   DpCfg_Init();
   DpDiag_Init();

   DpAppl_ClrResetVPC3Channel1();

   bError = VPC3_MemoryTest();

   if( DP_OK == bError )
   {
      #ifdef EvaBoard_AT89C5132
         bError = VPC3_Initialization( (*READ_PORT0 & 0x7F), IDENT_NR, (psCFG)&sDpAppl.sCfgData );     // address of slave; PORT0
      #else
         bError = VPC3_Initialization( DP_ADDR, IDENT_NR, (psCFG)&sDpAppl.sCfgData );                  // address of slave
      #endif//#ifdef EvaBoard_AT89C5132

      if( DP_OK == bError )
      {
         DpAppl_EnableInterruptVPC3Channel1();

         //todo: before startup the VPC3+, make here your own initialzations

         VPC3_Start();
      }//if( DP_OK == bError )
      else
      {
         sVpc3Error.bErrorCode = bError;
         DpAppl_FatalError( _DP_USER, __LINE__, &sVpc3Error );
      }//else of if( DP_OK == bError )
   }//if( DP_OK == bError )
   else
   {
      sVpc3Error.bErrorCode = bError;
      DpAppl_FatalError( _DP_USER, __LINE__, &sVpc3Error );
   }//else of if( DP_OK == bError )
}//void DpAppl_ProfibusInit( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_ProfibusMain                                                    */
/*---------------------------------------------------------------------------*/
/*!
  \brief The application program has to call this function cyclically so that the PROFIBUS DP slave services can be processed.
*/
void DpAppl_ProfibusMain( void )
{
#if VPC3_SERIAL_MODE
   uint8_t bStatusRegHigh;
   uint8_t bStatusRegLow;
#endif /* #if VPC3_SERIAL_MODE */
uint8_t bDpState;

   /*-------------------------------------------------------------------*/
   /* trigger watchdogs                                                 */
   /*-------------------------------------------------------------------*/
   // toggle user watchdog
   VPC3_RESET_USER_WD();   // toggle user watchdog

   #if VPC3_SERIAL_MODE
      /*----------------------------------------------------------------*/
      /* Poll PROFIBUS events                                           */
      /*----------------------------------------------------------------*/
      VPC3_Poll();

   #endif//#if VPC3_SERIAL_MODE

   /*-------------------------------------------------------------------*/
   /* internal state machine                                            */
   /*-------------------------------------------------------------------*/
   if( VPC3_GetDpState( eDpStateInit ) )
   {
      // clear data
      memset( &sDpAppl.abDpOutputData , 0, DOUT_BUFSIZE );
      memset( &sDpAppl.abDpInputData,   0, DIN_BUFSIZE );

      #ifdef EvaBoard_AT89C5132
         *WRITE_PORT1 = 0x00;
         *WRITE_PORT2 = 0x00;
      #endif//#ifdef EvaBoard_AT89C5132

      VPC3_ClrDpState( eDpStateInit );
      VPC3_SetDpState( eDpStateRun );
   }//if( VPC3_GetDpState( eDpStateInit ) )

   /*-------------------------------------------------------------------*/
   /* VPC3+ DP-state                                                    */
   /*-------------------------------------------------------------------*/
   #if VPC3_SERIAL_MODE
      bStatusRegHigh = VPC3_GET_STATUS_H();
      bStatusRegLow = VPC3_GET_STATUS_L();

      if( (( bStatusRegLow & VPC3_PASS_IDLE ) == 0x00 ) || (( bStatusRegHigh & AT_MASK ) != AT_VPC3S ) )
      {
         sVpc3Error.bErrorCode = bStatusRegLow;
         sVpc3Error.bCnId = bStatusRegHigh;
         DpAppl_FatalError( _DP_APPL, __LINE__, &sVpc3Error );
      } /* if( (( bStatusRegLow & VPC3_PASS_IDLE ) == 0x00 ) || (( bStatusRegHigh & AT_MASK ) != AT_VPC3S ) ) */
      bDpState = ( bStatusRegLow & MASK_DP_STATE );
   #else
      bDpState = VPC3_GET_DP_STATE();
   #endif /* #if VPC3_SERIAL_MODE */

   switch( bDpState )
   {
      case WAIT_PRM:
      {
				
          //lcdPrint("hu");

         break;
      }//case WAIT_PRM:

      case WAIT_CFG:
      {
        lcdPrint("CFG");

         break;
      }//case WAIT_CFG:

      case DATA_EX:
      {
         //lcdPrint("DATA");

         if(    ( VPC3_GetDpState( eDpStateApplReady ) )
             && ( VPC3_GetDpState( eDpStateRun )  )
           )
         {
            /*---------------------------------------------------------------*/
            /* profibus input ( slave to master )                            */
            /*---------------------------------------------------------------*/
            DpAppl_ReadInputData();
						
					 masterData[0] = sDpAppl.abDpOutputData[0];
					 masterData[1] = sDpAppl.abDpOutputData[1];
					 
					 lcd16x2_gotoxy(0,0);
					 lcdPrintHex(sDpAppl.abDpOutputData[0]);
					 lcd16x2_gotoxy(0,1);
					 lcdPrintHex(sDpAppl.abDpOutputData[1]);
					 clrScr();
					 
					 
					 
         }//if(    ( pDpSystem->bApplicationReady == VPC3_TRUE ) ...

         break;
      }//case DATA_EX:

      case DP_ERROR:
      default:
      {
         sVpc3Error.bErrorCode = VPC3_GET_DP_STATE();
         DpAppl_FatalError( _DP_USER, __LINE__, &sVpc3Error );
         break;
      }//case DP_ERROR:
   }//switch( bDpState )

   /*------------------------------------------------------------------------*/
   /* profibus output ( master to slave )                                    */
   /*------------------------------------------------------------------------*/
   DpAppl_CheckEvIoOut();

   /*------------------------------------------------------------------------*/
   /* handle here profibus interrupt events                                  */
   /*------------------------------------------------------------------------*/
   if( VPC3_GetDpState( eDpStateCfgOkStatDiag ) )
   {
      DpAppl_ApplicationReady();
   }//if( VPC3_GetDpState( eDpStateCfgOkStatDiag ) )
}//void DpAppl_ProfibusMain( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_FatalError                                               */
/*---------------------------------------------------------------------------*/
void DpAppl_FatalError( DP_ERROR_FILE bFile, uint16_t wLine, VPC3_ERRCB_PTR sVpc3Error )
{
   #ifdef EvaBoard_AT89C5132
      uint8_t i,j;

      DP_WriteDebugBuffer__( FatalError__, sVpc3Error->bFunction, sVpc3Error->bErrorCode );

      #ifdef RS232_SERIO
         do
         {
            // wait!
         }
         while( bSndCounter > 80);

         print_string("\r\nFatalError:");
         print_string("\r\nFile: ");
         print_hexbyte( bFile );
         print_string("\r\nLine: ");
         print_hexword( wLine );
         print_string("\r\nFunction: ");
         print_hexbyte( sVpc3Error->bFunction);
         print_string("\r\nError_Code: ");
         print_hexbyte( sVpc3Error->bErrorCode );
         print_string("\r\nDetail: ");
         print_hexbyte( sVpc3Error->bDetail );
         print_string("\r\ncn_id: ");
         print_hexbyte( sVpc3Error->bCnId );
      #endif//#ifdef RS232_SERIO

      *WRITE_PORT0 = sVpc3Error->bErrorCode;
      *WRITE_PORT1 = bFile;
      *WRITE_PORT2 = (uint8_t)wLine;

      SET_LED_YLW__;
      SET_LED_RED__;

      while(1)
      {
         TOGGLE_LED_RED__;
         TOGGLE_LED_YLW__;

         #ifdef RS232_SERIO
            if(bRecCounter > 0)
            {
               PrintSerialInputs();
            }
         #endif//#ifdef RS232_SERIO

         for( i = 0; i < 255; i++ )
         {
            for(j = 0; j < 255; j++);
         }
      }//while(1)

   #else

      while(1)
      {
      }//while(1)

   #endif//#ifdef EvaBoard_AT89C5132
}//void DpAppl_FatalError( DP_ERROR_FILE bFile, uint16_t wLine, VPC3_ERRCB_PTR sVpc3Error )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_MacReset                                                 */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the
   VPC3+ has entered the offline mode (by setting the Go_Offline bit in Moderegister 1).
*/
#if( DP_TIMESTAMP == 0 )
void DpAppl_MacReset( void )
{
   //print_string("\r\nDpAppl_MacReset");
} /* void DpAppl_MacReset( void ) */
#endif /* #if( DP_TIMESTAMP == 0 ) */

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrGoLeaveDataExchange                                   */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the
   DP-Statemachine has entered the DataEx-mode or has exited it.
   With the function VPC3_GET_DP_STATE() you can find out the state of VPC3+.
   \param[in] bDpState - state of PROFIBUS connection (WAIT_PRM,WAIT_CFG,DATA_EX)
*/
void DpAppl_IsrGoLeaveDataExchange( uint8_t bDpState )
{
   if( bDpState != DATA_EX )
   {
      VPC3_ClrDpState( eDpStateApplReady | eDpStateRun );
      VPC3_SetDpState( eDpStateInit );
   }//if( bDpState != DATA_EX )
}//void DpAppl_IsrGoLeaveDataExchange( uint8_t bDpState )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrDxOut                                                 */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the VPC3+
   has received a DataExchange message and has made the new output data
   available in the N-buffer. In the case of Power_On or Leave_Master, the
   VPC3+ clears the content of the buffer, and generates this event also.
*/
void DpAppl_IsrDxOut( void )
{
   DpAppl_SetApplEvent( eDpApplEv_IoOut );
}//void DpAppl_IsrDxOut( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrNewWdDpTimeout                                        */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the
   watchdog timer expired in the WD mode DP_Control.
   The communication between master and slave is time controlled, every time you're
   disconnecting the PROFIBUS master or you're disconnecting the PROFIBUS cable you'll
   get this event.
*/
void DpAppl_IsrNewWdDpTimeout( void )
{
    //not used in our application
}//void DpAppl_IsrNewWdDpTimeout( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrClockSynchronisation                                  */
/*---------------------------------------------------------------------------*/
#if DP_TIMESTAMP
   void DpAppl_IsrClockSynchronisation( void )
   {
      //not used in our application
   }//void DpAppl_IsrClockSynchronisation( void )
#endif//#if DP_TIMESTAMP

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrBaudrateDetect                                        */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the VPC3+
   has exited the Baud_Search mode and has found a baudrate.
   With the macro VPC3_GET_BAUDRATE() you can detect the baudrate.
*/
void DpAppl_IsrBaudrateDetect( void )
{
   //not used in our application
}//void DpAppl_IsrBaudrateDetect( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrNewGlobalControlCommand                               */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the VPC3+
   has received a Global_Control message. The GC_Command_Byte can be read out
   with the macro VPC3_GET_GC_COMMAND().
   \param[in] bGcCommand - global control command @see VPC3_GET_GC_COMMAND()
*/
void DpAppl_IsrNewGlobalControlCommand( uint8_t bGcCommand )
{
   //not used in our application
   bGcCommand = bGcCommand;   //avoid compiler warning
}//void DpAppl_IsrNewGlobalControlCommand( uint8_t bGcCommand )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrNewSetSlaveAddress                                    */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the VPC3+
   has received a Set_Slave_Address message and made the data available in the SSA
   buffer.
  \param[in] psSsa - pointer to set slave address structure
*/
void DpAppl_IsrNewSetSlaveAddress( MEM_STRUC_SSA_BLOCK_PTR psSsa )
{
    //not used in our application
    psSsa = psSsa; //avoid compiler warning
/*
   //store the new address and the bit bNoAddressChanged for the next startup
   print_string("\r\nNewAddr: ");
   print_hexbyte(psSsa->bTsAddr);
   print_hexbyte(psSsa->bNoAddressChanged);
   print_hexbyte(psSsa->bIdentHigh);
   print_hexbyte(psSsa->bIdentLow);
*/
}//void DpAppl_IsrNewSetSlaveAddress( MEM_STRUC_SSA_BLOCK_PTR psSsa )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrTimerClock (10ms)                                     */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() calls this function if the time base
   of the User_Timer_Clock has expired (1/10ms).

   \attention Use this function only interrupt driven!
*/
void DpAppl_IsrTimerClock( void )
{
}//void DpAppl_IsrTimerClock( void )

/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

