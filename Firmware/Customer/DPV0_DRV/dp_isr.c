/***********************  Filename: dp_isr.c  ********************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0                              */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      Einsteinstraﬂe 6        */
/* 0000  000  0   0 000   0 0     00000 0 0000       91074 Herzogenaurach    */
/* 0     00   0   0 0     0 0     0   0 0 0                                  */
/* 0     0 0  0   0 0     0 0   0 0   0 0 0          Phone: ++499132744200   */
/* 0     0  0  000  0     0  000  0   0 0 0    GmbH  Fax:   ++4991327442164  */
/*                                                                           */
/* ========================================================================= */
/*                                                                           */
/* Function:       interrupt service routine for VPC3+                       */
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


#if VPC3_SERIAL_MODE
   #define MakeWord( Hi, Lo ) ( (uint16_t)( ( ( (uint8_t)( Hi ) ) << 8 ) | ( (uint8_t)( Lo ) ) ) )
#endif /* #if VPC3_SERIAL_MODE */

/*---------------------------------------------------------------------------*/
/* function: VPC3_Isr ( interrupt service routine )                          */
/*---------------------------------------------------------------------------*/
#if( VPC3_SERIAL_MODE == 0 )
/*!
  \brief Interrupt service routine of VPC3+.

  This function has to be chained to the external interrupt handler.
*/
void VPC3_Isr( void )
{
volatile uint8_t bResult;

   #if DP_INTERRUPT_MASK_8BIT == 0

      #if VPC3_SERIAL_MODE

         pDpSystem->wInterruptEvent = MakeWord( Vpc3Read( bVpc3RoIntReg_H ), Vpc3Read( bVpc3RoIntReg_L ) );

      #else

         CopyFromVpc3_( (MEM_UNSIGNED8_PTR)&pDpSystem->wInterruptEvent, ((uint8_t *)(VPC3_ADR)( Vpc3AsicAddress + 0x02 )), 2 );

         #if BIG_ENDIAN
            Swap16( &pDpSystem->wInterruptEvent );
         #endif /* #if BIG_ENDIAN */

      #endif /* #if VPC3_SERIAL_MODE */

   #endif /* #if DP_INTERRUPT_MASK_8BIT == 0 */

   #if( DP_TIMESTAMP == 0 )

      /*---------------------------------------------------------------------*/
      /* IND_MAC_RESET                                                       */
      /*---------------------------------------------------------------------*/
      if( VPC3_GET_IND_MAC_RESET() )
      {
         DpAppl_MacReset();

         VPC3_CON_IND_MAC_RESET();
      } /* if( VPC3_GET_IND_MAC_RESET() ) */

   #endif /* #if( DP_TIMESTAMP == 0 ) */

   /*------------------------------------------------------------------------*/
   /* IND_DIAG_BUF_CHANGED                                                   */
   /*------------------------------------------------------------------------*/
   #if( ISR_ENABLE_VPC3_INT_DIAG_BUF_CHANGED == 1 )
      if( VPC3_GET_IND_DIAG_BUFFER_CHANGED() )
      {
         DpDiag_IsrDiagBufferChanged();
					
         VPC3_CON_IND_DIAG_BUFFER_CHANGED();
      } /* if( VPC3_GET_IND_DIAG_BUFFER_CHANGED() ) */
   #endif /* #if( ISR_ENABLE_VPC3_INT_DIAG_BUF_CHANGED == 1 ) */

   /*------------------------------------------------------------------------*/
   /* IND_NEW_PRM_DATA                                                       */
   /*------------------------------------------------------------------------*/
   #if( ISR_ENABLE_VPC3_INT_NEW_PRM_DATA == 1 )
      if( VPC3_GET_IND_NEW_PRM_DATA() )
      {
         uint8_t bPrmLength;

         bResult = VPC3_PRM_FINISHED;

         #if DP_INTERRUPT_MASK_8BIT == 0
            VPC3_CON_IND_NEW_PRM_DATA();
         #endif /* #if DP_INTERRUPT_MASK_8BIT == 0 */

         do
         {
            bPrmLength = VPC3_GET_PRM_LEN();

            CopyFromVpc3_( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], VPC3_GET_PRM_BUF_PTR(), bPrmLength );

            if( DpPrm_ChkNewPrmData( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bPrmLength ) == DP_OK )
            {
               #if REDUNDANCY
                  #if DP_MSAC_C1
                     if( VPC3_GET_PRM_LEN() != PRM_CMD_LENGTH )
                     {
                        MSAC_C1_CheckIndNewPrmData( (MEM_STRUC_PRM_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bPrmLength );
                     } /* if( VPC3_GET_PRM_LEN() != PRM_CMD_LENGTH ) */
                  #endif /* #if DP_MSAC_C1 */
               #else
                  #if DP_MSAC_C1
                     MSAC_C1_CheckIndNewPrmData( (MEM_STRUC_PRM_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bPrmLength );
                  #endif /* #if DP_MSAC_C1 */
               #endif /* #if REDUNDANCY */

               bResult = VPC3_SET_PRM_DATA_OK();
            } /* if( DpPrm_ChkNewPrmData( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bPrmLength ) == DP_OK ) */
            else
            {
               bResult = VPC3_SET_PRM_DATA_NOT_OK();
            } /* else of if( DpPrm_ChkNewPrmData( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bPrmLength ) == DP_OK ) */
         }
         while( bResult == VPC3_PRM_CONFLICT );
       } /* if( VPC3_GET_IND_NEW_PRM_DATA() ) */
   #endif /* #if( ISR_ENABLE_VPC3_INT_NEW_PRM_DATA == 1 ) */

   /*------------------------------------------------------------------------*/
   /* IND_NEW_EXT_PRM_DATA                                                   */
   /*------------------------------------------------------------------------*/
   #if( ISR_ENABLE_VPC3_INT_NEW_EXT_PRM_DATA == 1 )
      if( VPC3_GET_IND_NEW_EXT_PRM_DATA() )
      {
         bResult = VPC3_PRM_FINISHED;

         do
         {
            bPrmLength = VPC3_GET_PRM_LEN();

            CopyFromVpc3_( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], VPC3_GET_PRM_BUF_PTR(), bPrmLength );

            if( UserChkNewExtPrmData( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bPrmLength ) == DP_OK )
            {
               bResult = VPC3_SET_PRM_DATA_OK();
            } /* if( UserChkNewExtPrmData( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bPrmLength ) == DP_OK */
            else
            {
               bResult = VPC3_SET_PRM_DATA_NOT_OK();
            } /* else of if( UserChkNewExtPrmData( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bPrmLength ) == DP_OK */
         }
         while( bResult == VPC3_PRM_CONFLICT );
      } /* if( VPC3_GET_IND_NEW_EXT_PRM_DATA() ) */
   #endif /* #if( ISR_ENABLE_VPC3_INT_NEW_EXT_PRM_DATA == 1 ) */

   /*------------------------------------------------------------------------*/
   /* check config data , application specific!                              */
   /*------------------------------------------------------------------------*/
   #if( ISR_ENABLE_VPC3_INT_NEW_CFG_DATA == 1 )
      if( VPC3_GET_IND_NEW_CFG_DATA() )
      {
         uint8_t bCfgLength;

         bResult = VPC3_CFG_FINISHED;

         #if DP_INTERRUPT_MASK_8BIT == 0
            VPC3_CON_IND_NEW_CFG_DATA();
         #endif /* #if DP_INTERRUPT_MASK_8BIT == 0 */

         do
         {
            bCfgLength = VPC3_GET_CFG_LEN();
            CopyFromVpc3_( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], VPC3_GET_CFG_BUF_PTR(), bCfgLength );

            switch( DpCfg_ChkNewCfgData( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bCfgLength ) )
            {
               case DP_CFG_OK:
               {
                  #if DP_MSAC_C1
                     MSAC_C1_DoCfgOk();
                  #endif /* #if DP_MSAC_C1 */

                  bResult = VPC3_SET_CFG_DATA_OK();
                  break;
               } /* case DP_CFG_OK: */

               case DP_CFG_FAULT:
               {
                  #if DP_MSAC_C1
                     MSAC_C1_DoCfgNotOk();
                  #endif /* #if DP_MSAC_C1 */

                  bResult = VPC3_SET_CFG_DATA_NOT_OK();
                  break;
               } /* case DP_CFG_FAULT: */

               case DP_CFG_UPDATE:
               {
                  /* Calculate the length of the input and output using the configuration bytes */
                  if( DP_OK != VPC3_CalculateInpOutpLength( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bCfgLength ) )
                  {
                     #if DP_MSAC_C1
                        MSAC_C1_DoCfgNotOk();
                     #endif /* #if DP_MSAC_C1 */

                     bResult = VPC3_SET_CFG_DATA_NOT_OK();
                  } /* if( DP_OK != VPC3_CalculateInpOutpLength( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bCfgLength ) ) */
                  else
                  {
                     /* set IO-Length */
                     VPC3_SetIoDataLength();

                     #if DP_MSAC_C1
                        MSAC_C1_DoCfgOk();
                     #endif /* #if DP_MSAC_C1 */

                     VPC3_SET_READ_CFG_LEN( bCfgLength );
                     VPC3_UPDATE_CFG_BUFFER();

                     bResult = VPC3_SET_CFG_DATA_OK();
                  } /* else of if( DP_OK != VPC3_CalculateInpOutpLength( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bCfgLength ) ) */

                  break;
               } /* case DP_CFG_UPDATE: */

               default:
               {
                  break;
               } /* default: */
            } /* switch( DpCfg_ChkNewCfgData( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bCfgLength ) ) */
         }
         while( bResult == VPC3_CFG_CONFLICT );
      } /* if( VPC3_GET_IND_NEW_CFG_DATA() ) */
   #endif /* #if( ISR_ENABLE_VPC3_INT_NEW_CFG_DATA == 1 ) */

   /*------------------------------------------------------------------------*/
   /* IND_GO_LEAVE_DATA_EX                                                   */
   /*------------------------------------------------------------------------*/
   #if( ISR_ENABLE_VPC3_INT_GO_LEAVE_DATA_EX == 1 )
      if( VPC3_GET_IND_GO_LEAVE_DATA_EX() )
      {
         #if DP_MSAC_C1
            MSAC_C1_LeaveDx();
         #endif /* #if DP_MSAC_C1 */

         DpAppl_IsrGoLeaveDataExchange( VPC3_GET_DP_STATE() );

         VPC3_CON_IND_GO_LEAVE_DATA_EX();
      } /* if( VPC3_GET_IND_GO_LEAVE_DATA_EX() ) */
   #endif /* #if( ISR_ENABLE_VPC3_INT_GO_LEAVE_DATA_EX == 1 ) */

   /*------------------------------------------------------------------------*/
   /* IND_DX_OUT                                                             */
   /*------------------------------------------------------------------------*/
   #if( ISR_ENABLE_VPC3_INT_DX_OUT == 1 )
      if( VPC3_GET_IND_DX_OUT() )
      {
         #if DP_MSAC_C1
            MSAC_C1_CheckIndDxOut();
         #endif /* #if DP_MSAC_C1 */

         DpAppl_IsrDxOut();

         VPC3_CON_IND_DX_OUT();
      } /* if( VPC3_GET_IND_DX_OUT() ) */
   #endif /* #if( ISR_ENABLE_VPC3_INT_DX_OUT == 1 ) */

   /*------------------------------------------------------------------------*/
   /* IND_WD_DP_TIMEOUT                                                      */
   /*------------------------------------------------------------------------*/
   #if( ISR_ENABLE_VPC3_INT_WD_DP_TIMEOUT == 1 )
      if( VPC3_GET_IND_WD_DP_MODE_TIMEOUT() )
      {
         DpAppl_IsrNewWdDpTimeout();

         VPC3_CON_IND_WD_DP_MODE_TIMEOUT();
      } /* if( VPC3_GET_IND_WD_DP_MODE_TIMEOUT() ) */
   #endif /* #if( ISR_ENABLE_VPC3_INT_WD_DP_MODE_TIMEOUT == 1 ) */

   /*------------------------------------------------------------------------*/
   /* IND_MAC_RESET                                                          */
   /*------------------------------------------------------------------------*/
   #if( ISR_ENABLE_VPC3_INT_CLOCK_SYNC == 1 )
      if( VPC3_GET_IND_CLOCK_SYNC() )
      {
         #if DP_TIMESTAMP
            /* TimeStampProcess(); */
            /* DpAppl_IsrClockSynchronisation(); */
         #endif /* #if DP_TIMESTAMP */

         VPC3_CON_IND_CLOCK_SYNC();
      } /* if( VPC3_GET_IND_CLOCK_SYNC() ) */
   #endif /* #if( ISR_ENABLE_VPC3_INT_CLOCK_SYNC == 1 ) */

   /*------------------------------------------------------------------------*/
   /* IND_BAUDRATE_DETECT                                                    */
   /*------------------------------------------------------------------------*/
   #if( ISR_ENABLE_VPC3_INT_BAUDRATE_DETECT == 1 )
      if( VPC3_GET_IND_BAUDRATE_DETECT() )
      {
         #if DP_MSAC_C2
            MSAC_C2_SetTimeoutIsr();
         #endif /* #if DP_MSAC_C2 */

         DpAppl_IsrBaudrateDetect();

         VPC3_CON_IND_BAUDRATE_DETECT();
      } /* if( VPC3_GET_IND_BAUDRATE_DETECT() ) */
   #endif /* #if( ISR_ENABLE_VPC3_INT_BAUDRATE_DETECT == 1 ) */

   /*------------------------------------------------------------------------*/
   /* IND_NEW_GC_COMMAND                                                     */
   /*------------------------------------------------------------------------*/
   #if( ISR_ENABLE_VPC3_INT_NEW_GC_COMMAND == 1 )
      if( VPC3_GET_IND_NEW_GC_COMMAND() )
      {
         DpAppl_IsrNewGlobalControlCommand( VPC3_GET_GC_COMMAND() );

         VPC3_CON_IND_NEW_GC_COMMAND();
      } /* if( VPC3_GET_IND_NEW_GC_COMMAND() ) */
   #endif /* #if( ISR_ENABLE_VPC3_INT_NEW_GC_COMMAND == 1 ) */

   /*------------------------------------------------------------------------*/
   /* IND_NEW_SSA_DATA                                                       */
   /*------------------------------------------------------------------------*/
   #if( ISR_ENABLE_VPC3_INT_NEW_SSA_DATA == 1 )
      if( VPC3_GET_IND_NEW_SSA_DATA() )
      {
         CopyFromVpc3_( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], VPC3_GET_SSA_BUF_PTR(), 4 );
         DpAppl_IsrNewSetSlaveAddress( (MEM_STRUC_SSA_BLOCK_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0] );

         bResult = VPC3_FREE_SSA_BUF();

         VPC3_CON_IND_NEW_SSA_DATA();
      } /* if( VPC3_GET_IND_NEW_SSA_DATA() ) */
   #endif /* #if( ISR_ENABLE_VPC3_INT_NEW_SSA_DATA == 1 ) */

   /*------------------------------------------------------------------------*/
   /* IND_USER_TIMER_CLOCK                                                   */
   /*------------------------------------------------------------------------*/
   #if( ISR_ENABLE_VPC3_INT_USER_TIMER_CLOCK == 1 )
      if( VPC3_GET_IND_USER_TIMER_CLOCK() )
      {
         DpAppl_IsrTimerClock();

         #if DP_MSAC_C2
            MSAC_C2_TimerTick10msec();
         #endif /* #if DP_MSAC_C2 */

         VPC3_CON_IND_USER_TIMER_CLOCK();
      } /* if( VPC3_GET_IND_USER_TIMER_CLOCK() ) */
   #endif /* #if( ISR_ENABLE_VPC3_INT_USER_TIMER_CLOCK == 1 ) */

   #ifdef DP_SUBSCRIBER

      /*---------------------------------------------------------------------*/
      /* IND_DXB_OUT                                                         */
      /*---------------------------------------------------------------------*/
      #if( ISR_ENABLE_VPC3_INT_DXB_OUT == 1 )
         if( VPC3_GET_IND_DXB_OUT() )
         {
            DpAppl_IsrDxbOut();

            VPC3_CON_IND_DXB_OUT();
         } /* if( VPC3_GET_IND_DXB_OUT() ) */
      #endif /* #if( ISR_ENABLE_VPC3_INT_DXB_OUT == 1 ) */

      /*---------------------------------------------------------------------*/
      /* IND_DXB_LINK_ERROR                                                  */
      /*---------------------------------------------------------------------*/
      #if( ISR_ENABLE_VPC3_INT_DXB_LINK_ERROR == 1 )
         if( VPC3_GET_IND_DXB_LINK_ERROR() )
         {
            DpAppl_IsrDxbLinkError();

            VPC3_CON_IND_DXB_LINK_ERROR();
         } /* if( VPC3_GET_IND_DXB_LINK_ERROR() ) */
      #endif /* #if( ISR_ENABLE_VPC3_INT_DXB_LINK_ERROR == 1 ) */

   #endif /* #ifdef DP_SUBSCRIBER */

   #if DP_FDL

      /*---------------------------------------------------------------------*/
      /* IND_POLL_END                                                        */
      /*---------------------------------------------------------------------*/
      #if( ISR_ENABLE_VPC3_INT_POLL_END_IND == 1 )
         if( VPC3_GET_IND_POLL_END_IND() )
         {
            VPC3_CON_IND_POLL_END_IND();
            FDL_PollEndIsr();
         } /* if( VPC3_GET_IND_POLL_END_IND() ) */
      #endif /* #if( ISR_ENABLE_VPC3_INT_POLL_END_IND == 1 ) */

      /*---------------------------------------------------------------------*/
      /* IND_REQ_PDU                                                         */
      /*---------------------------------------------------------------------*/
      #if( ISR_ENABLE_VPC3_INT_FDL_IND == 1 )
         if( VPC3_GET_IND_FDL_IND() )
         {
            VPC3_CON_IND_FDL_IND();
            FDL_IndicationIsr();
         } /* if( VPC3_GET_IND_FDL_IND() ) */
      #endif /* #if( ISR_ENABLE_VPC3_INT_FDL_IND == 1 ) */

   #endif /* #if DP_FDL */

   #if DP_INTERRUPT_MASK_8BIT == 0

      #if VPC3_SERIAL_MODE

         Vpc3Write( bVpc3WoIntAck_L, (uint8_t)(pDpSystem->wInterruptEvent & 0xFF) );
         Vpc3Write( bVpc3WoIntAck_H, (uint8_t)(pDpSystem->wInterruptEvent >> 8) );

      #else

         #if BIG_ENDIAN
            Swap16( &pDpSystem->wInterruptEvent );
         #endif /* #if BIG_ENDIAN */

         CopyToVpc3_( ((uint8_t *)(VPC3_ADR)( Vpc3AsicAddress + 0x02 )), (MEM_UNSIGNED8_PTR)&pDpSystem->wInterruptEvent, 2 );

      #endif /* #if VPC3_SERIAL_MODE */

      pDpSystem->wInterruptEvent = 0;

   #endif /* #if DP_INTERRUPT_MASK_8BIT == 0 */

   /*------------------------------------------------------------------------*/
   /* reset interrupt line                                                   */
   /*------------------------------------------------------------------------*/
   VPC3_SET_EOI();

} /* void VPC3_Isr( void ) */
#endif /* #if( VPC3_SERIAL_MODE == 0 ) */

/*---------------------------------------------------------------------------*/
/* function: VPC3_Poll                                                       */
/*---------------------------------------------------------------------------*/
#if VPC3_SERIAL_MODE
void VPC3_Poll( void )
{
	//GPIOC->ODR ^= GPIO_Pin_13;
	
volatile uint8_t bResult;

   #if DP_INTERRUPT_MASK_8BIT == 0

      #if VPC3_SERIAL_MODE

         pDpSystem->wPollInterruptEvent = MakeWord( Vpc3Read( bVpc3RwIntReqReg_H ), Vpc3Read( bVpc3RwIntReqReg_L ) );

      #else

         CopyFromVpc3_( (MEM_UNSIGNED8_PTR)&pDpSystem->wPollInterruptEvent, ((uint8_t *)(VPC3_ADR)( Vpc3AsicAddress )), 2 );

         #if BIG_ENDIAN
            Swap16( &pDpSystem->wPollInterruptEvent );
         #endif /* #if BIG_ENDIAN */

      #endif /* #if VPC3_SERIAL_MODE */

      pDpSystem->wPollInterruptEvent &= pDpSystem->wPollInterruptMask;

      if( pDpSystem->wPollInterruptEvent > 0 )
      {

   #endif /* #if DP_INTERRUPT_MASK_8BIT == 0 */

         #if( DP_TIMESTAMP == 0 )
            /*---------------------------------------------------------------*/
            /* IND_MAC_RESET                                                 */
            /*---------------------------------------------------------------*/
            if( VPC3_POLL_IND_MAC_RESET() )
            {
							
               DpAppl_MacReset();

               VPC3_CON_IND_MAC_RESET();
            } /* if( VPC3_POLL_IND_MAC_RESET() ) */
         #endif /* #if( DP_TIMESTAMP == 0 ) */

         /*------------------------------------------------------------------*/
         /* IND_DIAG_BUF_CHANGED                                             */
         /*------------------------------------------------------------------*/
         if( VPC3_POLL_IND_DIAG_BUFFER_CHANGED() )
         {
            DpDiag_IsrDiagBufferChanged();

            VPC3_POLL_CON_IND_DIAG_BUFFER_CHANGED();
         } /* if( VPC3_POLL_IND_DIAG_BUFFER_CHANGED() ) */

         /*------------------------------------------------------------------*/
         /* IND_NEW_PRM_DATA                                                 */
         /*------------------------------------------------------------------*/
         if( VPC3_POLL_IND_NEW_PRM_DATA() )
         {
					 
            uint8_t bPrmLength;

            #if DP_INTERRUPT_MASK_8BIT == 0
               VPC3_POLL_CON_IND_NEW_PRM_DATA();
            #endif /* #if DP_INTERRUPT_MASK_8BIT == 0 */

            bResult = VPC3_PRM_FINISHED;

            do
            {
               bPrmLength = VPC3_GET_PRM_LEN();

               CopyFromVpc3_( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], VPC3_GET_PRM_BUF_PTR(), bPrmLength );

               if( DpPrm_ChkNewPrmData( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bPrmLength ) == DP_OK )
               {
                  #if REDUNDANCY
                     #if DP_MSAC_C1
                        if( VPC3_GET_PRM_LEN() != PRM_CMD_LENGTH )
                        {
                           MSAC_C1_CheckIndNewPrmData( (MEM_STRUC_PRM_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bPrmLength );
                        } /* if( VPC3_GET_PRM_LEN() != PRM_CMD_LENGTH ) */
                     #endif /* #if DP_MSAC_C1 */
                  #else
                     #if DP_MSAC_C1
                        MSAC_C1_CheckIndNewPrmData( (MEM_STRUC_PRM_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bPrmLength );
                     #endif /* #if DP_MSAC_C1 */
                  #endif /* #if REDUNDANCY */

                  bResult = VPC3_SET_PRM_DATA_OK();
               } /* if( DpPrm_ChkNewPrmData( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bPrmLength ) == DP_OK ) */
               else
               {
                  bResult = VPC3_SET_PRM_DATA_NOT_OK();
               } /* else of if( DpPrm_ChkNewPrmData( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bPrmLength ) == DP_OK ) */
            }
            while( bResult == VPC3_PRM_CONFLICT );
         } /* if( VPC3_POLL_IND_NEW_PRM_DATA() ) */

         /*------------------------------------------------------------------*/
         /* check config data , application specific!                        */
         /*------------------------------------------------------------------*/
         if( VPC3_POLL_IND_NEW_CFG_DATA() )
         {
            uint8_t bCfgLength;

            #if DP_INTERRUPT_MASK_8BIT == 0
               VPC3_POLL_CON_IND_NEW_CFG_DATA();
            #endif /* #if DP_INTERRUPT_MASK_8BIT == 0 */

            bResult = VPC3_CFG_FINISHED;

            do
            {
               bCfgLength = VPC3_GET_CFG_LEN();
               CopyFromVpc3_( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], VPC3_GET_CFG_BUF_PTR(), bCfgLength );

               switch( DpCfg_ChkNewCfgData( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bCfgLength ) )
               {
                  case DP_CFG_OK:
                  {
                     #if DP_MSAC_C1
                        MSAC_C1_DoCfgOk();
                     #endif /* #if DP_MSAC_C1 */

                     bResult = VPC3_SET_CFG_DATA_OK();
                     break;
                  } /* case DP_CFG_OK: */

                  case DP_CFG_FAULT:
                  {
                     #if DP_MSAC_C1
                        MSAC_C1_DoCfgNotOk();
                     #endif /* #if DP_MSAC_C1 */

                     bResult = VPC3_SET_CFG_DATA_NOT_OK();
                     break;
                  } /* case DP_CFG_FAULT: */

                  case DP_CFG_UPDATE:
                  {
                     /* Calculate the length of the input and output using the configuration bytes */
                     if( DP_OK != VPC3_CalculateInpOutpLength( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bCfgLength ) )
                     {
                        #if DP_MSAC_C1
                           MSAC_C1_DoCfgNotOk();
                        #endif /* #if DP_MSAC_C1 */

                        bResult = VPC3_SET_CFG_DATA_NOT_OK();
                     } /* if( DP_OK != VPC3_CalculateInpOutpLength( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bCfgLength ) ) */
                     else
                     {
                        /* set IO-Length */
                        VPC3_SetIoDataLength();

                        #if DP_MSAC_C1
                           MSAC_C1_DoCfgOk();
                        #endif /* #if DP_MSAC_C1 */

                        VPC3_SET_READ_CFG_LEN( bCfgLength );
                        VPC3_UPDATE_CFG_BUFFER();

                        bResult = VPC3_SET_CFG_DATA_OK();
                     } /* else of if( DP_OK != VPC3_CalculateInpOutpLength( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], bCfgLength ) ) */
                     break;
                  } /* case DP_CFG_UPDATE: */

                  default:
                  {
                     break;
                  } /* default: */
               } /* switch( DpCfg_ChkNewCfgData( pCfgData, bCfgLength ) ) */
            }
            while( bResult == VPC3_CFG_CONFLICT );
         } /* if( VPC3_POLL_IND_NEW_CFG_DATA() ) */

         /*------------------------------------------------------------------*/
         /* IND_WD_DP_TIMEOUT                                                */
         /*------------------------------------------------------------------*/
         if( VPC3_POLL_IND_WD_DP_MODE_TIMEOUT() )
         {
					 	
					 
            DpAppl_IsrNewWdDpTimeout();

            VPC3_CON_IND_WD_DP_MODE_TIMEOUT();
         } /* if( VPC3_POLL_IND_WD_DP_MODE_TIMEOUT() ) */

         /*------------------------------------------------------------------*/
         /* IND_GO_LEAVE_DATA_EX                                             */
         /*------------------------------------------------------------------*/
         if( VPC3_POLL_IND_GO_LEAVE_DATA_EX() )
         {
					 
            #if DP_MSAC_C1
               MSAC_C1_LeaveDx();
            #endif /* #if DP_MSAC_C1 */

            DpAppl_IsrGoLeaveDataExchange( VPC3_GET_DP_STATE() );

            VPC3_CON_IND_GO_LEAVE_DATA_EX();
         } /* if( VPC3_POLL_IND_GO_LEAVE_DATA_EX() ) */

         /*------------------------------------------------------------------*/
         /* IND_DX_OUT                                                       */
         /*------------------------------------------------------------------*/
         if( VPC3_POLL_IND_DX_OUT() )
         {
            #if DP_MSAC_C1
               MSAC_C1_CheckIndDxOut();
            #endif /* #if DP_MSAC_C1 */

            DpAppl_IsrDxOut();

            VPC3_CON_IND_DX_OUT();
         } /* if( VPC3_POLL_IND_DX_OUT() ) */

         /*------------------------------------------------------------------*/
         /* IND_NEW_GC_COMMAND                                               */
         /*------------------------------------------------------------------*/
         if( VPC3_POLL_IND_NEW_GC_COMMAND() )
         {
            DpAppl_IsrNewGlobalControlCommand( VPC3_GET_GC_COMMAND() );

            VPC3_CON_IND_NEW_GC_COMMAND();
         } /* if( VPC3_POLL_IND_NEW_GC_COMMAND() ) */

         /*------------------------------------------------------------------*/
         /* IND_NEW_SSA_DATA                                                 */
         /*------------------------------------------------------------------*/
         if( VPC3_POLL_IND_NEW_SSA_DATA() )
         {
            CopyFromVpc3_( (MEM_UNSIGNED8_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0], VPC3_GET_SSA_BUF_PTR(), 4 );
            DpAppl_IsrNewSetSlaveAddress( (MEM_STRUC_SSA_BLOCK_PTR)&pDpSystem->abPrmCfgSsaHelpBuffer[0] );

            bResult = VPC3_FREE_SSA_BUF();

            VPC3_CON_IND_NEW_SSA_DATA();
         } /* if( VPC3_POLL_IND_NEW_SSA_DATA() ) */

         /*------------------------------------------------------------------*/
         /* IND_BAUDRATE_DETECT                                              */
         /*------------------------------------------------------------------*/
         if( VPC3_POLL_IND_BAUDRATE_DETECT() )
         {
            #if DP_MSAC_C2
               MSAC_C2_SetTimeoutIsr();
            #endif /* #if DP_MSAC_C2 */

            DpAppl_IsrBaudrateDetect();

            VPC3_CON_IND_BAUDRATE_DETECT();
         } /* if( VPC3_POLL_IND_BAUDRATE_DETECT() ) */

         #if DP_SUBSCRIBER

            /*---------------------------------------------------------------*/
            /* IND_DXB_OUT                                                   */
            /*---------------------------------------------------------------*/
            if( VPC3_POLL_IND_DXB_OUT() )
            {
               DpAppl_IsrDxbOut();

               VPC3_CON_IND_DXB_OUT();
            } /* if( VPC3_POLL_IND_DXB_OUT() ) */

            /*---------------------------------------------------------------*/
            /* IND_DXB_LINK_ERROR                                            */
            /*---------------------------------------------------------------*/
            if( VPC3_POLL_IND_DXB_LINK_ERROR() )
            {
               DpAppl_IsrDxbLinkError();

               VPC3_CON_IND_DXB_LINK_ERROR();
            } /* if( VPC3_POLL_IND_DXB_LINK_ERROR() ) */

         #endif /* #if DP_SUBSCRIBER */

         #if DP_FDL

            /*---------------------------------------------------------------*/
            /* IND_POLL_END                                                  */
            /*---------------------------------------------------------------*/
            if( VPC3_POLL_IND_POLL_END_IND() )
            {
               VPC3_CON_IND_POLL_END_IND();

               FDL_PollEndIsr();
            } /* if( VPC3_POLL_IND_POLL_END_IND() ) */

            /*---------------------------------------------------------------*/
            /* IND_REQ_PDU                                                   */
            /*---------------------------------------------------------------*/
            if( VPC3_POLL_IND_FDL_IND() )
            {
               VPC3_CON_IND_FDL_IND();
               FDL_IndicationIsr();
            } /* if( VPC3_POLL_IND_FDL_IND() ) */

         #endif /* #if DP_FDL */

         #if DP_INTERRUPT_MASK_8BIT == 0

            #if VPC3_SERIAL_MODE

               Vpc3Write( bVpc3WoIntAck_L, (uint8_t)(pDpSystem->wPollInterruptEvent & 0xFF) );
               Vpc3Write( bVpc3WoIntAck_H, (uint8_t)(pDpSystem->wPollInterruptEvent >> 8) );

            #else

               #if BIG_ENDIAN
                  Swap16( &pDpSystem->wPollInterruptEvent );
               #endif /* #if BIG_ENDIAN */

               CopyToVpc3_( ((uint8_t *)(VPC3_ADR)( Vpc3AsicAddress + 0x02 )), (MEM_UNSIGNED8_PTR)&pDpSystem->wPollInterruptEvent, 2 );

            #endif /* #if VPC3_SERIAL_MODE */

            pDpSystem->wPollInterruptEvent = 0;

         #endif /* #if DP_INTERRUPT_MASK_8BIT == 0 */

   #if DP_INTERRUPT_MASK_8BIT == 0

      } /* if( pDpSystem->wPollInterruptEvent > 0 ) */

   #endif /* #if DP_INTERRUPT_MASK_8BIT == 0 */
} /* void VPC3_Poll( void ) */
#endif /* #if VPC3_SERIAL_MODE */

/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

