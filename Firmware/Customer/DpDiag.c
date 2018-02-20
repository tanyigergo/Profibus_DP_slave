/***************************  Filename: DpDiag.c *****************************/
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


*/
/*****************************************************************************/
/** @file
 * Copyright (C) by profichip GmbH   All Rights Reserved. Confidential
 *
 * @brief Handling of PROFIBUS-DP diagnostics.
 *
 * @author Peter Fredehorst
 * @version $Rev$
 */

/* include hierarchy */
#include <string.h>
#include "platform.h"

/*---------------------------------------------------------------------------*/
/* defines, structures                                                       */
/*---------------------------------------------------------------------------*/
#define MAX_DIAG_RETRY 10

// -- defines for user alarm state
#define USER_AL_STATE_CLOSED        ((uint8_t)0x00)
#define USER_AL_STATE_OPEN          ((uint8_t)0x01)
// -- defines for diagnostics
#define USER_TYPE_RESET_DIAG        ((uint8_t)0xF8)
#define USER_TYPE_DPV0              ((uint8_t)0xFA)
#define USER_TYPE_PRM_OK            ((uint8_t)0xFB)
#define USER_TYPE_PRM_NOK           ((uint8_t)0xFC)
#define USER_TYPE_CFG_OK            ((uint8_t)0xFD)
#define USER_TYPE_CFG_NOK           ((uint8_t)0xFE)
#define USER_TYPE_APPL_RDY          ((uint8_t)0xFF)

/*---------------------------------------------------------------------------*/
/* local user data definitions                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* defines, structures and variables for our demo application                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* function: DpDiag_AlarmInit                                               */
/*--------------------------------------------------------------------------*/
/**
 * @brief Initializes user alarm structure.
 *
 * Set all values to zero.
 */
void DpDiag_AlarmInit( void )
{
   //nothing todo in this example.
}//void DpDiag_AlarmInit( void )

/*---------------------------------------------------------------------------*/
/* function: DpDiag_Init                                                     */
/*---------------------------------------------------------------------------*/
/**
 * @brief Initializes local structures.
 *
 * This function is called during startup and with each received parameter telegram from PROFIBUS-DP master.
 */
void DpDiag_Init( void )
{
   //nothing todo in this example.
}//void DpDiag_Init( void )

/*---------------------------------------------------------------------------*/
/* function: DpDiag_Alarm ( is also called from alarm state machine !!!! )   */
/*---------------------------------------------------------------------------*/
/**
 * @brief Reset static diagnostic bit.
 *
 * @param[in]bAlarmType
 * @param[in]bSeqNr
 * @param[in]psAlarm
 * @param[in]bCheckDiagFlag
 *
 * @return VPC3_TRUE - the static diagnostic is resettet
 * @return VPC3_FALSE - the static diagnostic is not resettet yet, try again
 */
DP_ERROR_CODE DpDiag_Alarm( uint8_t bAlarmType, uint8_t bSeqNr, ALARM_STATUS_PDU_PTR psAlarm, uint8_t bCheckDiagFlag )
{
MEM_UNSIGNED8_PTR pbToDiagArray;
DP_ERROR_CODE     bRetValue;
uint8_t           bExtDiagFlag;
uint8_t           bDiagLength;
uint16_t          wDiagEvent;

   bRetValue = DP_NOK;

   psAlarm = psAlarm; //avoid warning
   wDiagEvent = ( (uint16_t)(bAlarmType << 8) | ((uint16_t)bSeqNr) );

   VPC3_CheckDiagBufferChanged();

   //don't send diagnostic twice!
   if( wDiagEvent != pDpSystem->wOldDiag )
   {
      if( !( VPC3_GetDpState( eDpStateDiagActive ) ))
      {
      	memset( &pDpSystem->abUserDiagnostic[0], 0x00, sizeof( pDpSystem->abUserDiagnostic ) );
      	pbToDiagArray = pDpSystem->abUserDiagnostic;
      	
      	bExtDiagFlag = 0x00;
      	bDiagLength = 0x00;
      	
      	switch( bAlarmType )
      	{
      	   case USER_TYPE_CFG_OK:
      	   {
      	      bExtDiagFlag = STAT_DIAG_SET;
      	      break;
      	   }//case USER_TYPE_CFG_OK:
      	
      	   case USER_TYPE_APPL_RDY:
      	   case USER_TYPE_RESET_DIAG:
      	   case USER_TYPE_PRM_NOK:
      	   case USER_TYPE_PRM_OK:
      	   default:
      	   {
      	      bExtDiagFlag = 0x00;
      	      bDiagLength = 0x00;
      	      break;
      	   }//default:
      	}//switch( bAlarmType )
      	
      	VPC3_SetDpState( eDpStateDiagActive );
      	
      	bRetValue = VPC3_SetDiagnosis( pDpSystem->abUserDiagnostic, bDiagLength, bExtDiagFlag, bCheckDiagFlag );
      	
      	if( bRetValue == DP_OK )
      	{
      	   pDpSystem->wOldDiag = wDiagEvent;
      	}//if( bRetValue == DP_OK )
      	else
      	{
      	   VPC3_ClrDpState( eDpStateDiagActive );
      	}//else of if( bRetValue == DP_OK )
   	}//if( !( VPC3_GetDpState( eDpStateDiagActive ) ))
      else
      {
         bRetValue = DP_DIAG_ACTIVE_DIAG;
      }//else of if( !( VPC3_GetDpState( eDpStateDiagActive ) ))
   }//if( wDiagEvent != pDpSystem->wOldDiag )
   else
   {
      bRetValue = DP_DIAG_SAME_DIAG;
   }//else if( wDiagEvent != pDpSystem->wOldDiag )

   return bRetValue;
}//DP_ERROR_CODE DpDiag_Alarm( uint8_t bAlarmType, uint8_t bSeqNr, ALARM_STATUS_PDU_PTR psAlarm, uint8_t bCheckDiagFlag )

/*--------------------------------------------------------------------------*/
/* function: DpDiag_SetPrmOk                                                */
/*--------------------------------------------------------------------------*/
/**
 * @brief Set User specific diagnostic, if SetPrm is OK.
 * In this demo, the last user specific diagnostic will be cleared!
 *
 * @return DP_OK - diagnostic is set
 * @return DP_DIAG_SAME_DIAG - no change necessary
 * @return other - diagnostic is not resettet yet, try again
 */
DP_ERROR_CODE DpDiag_SetPrmOk( DP_ERROR_CODE ePrmError )
{
DP_ERROR_CODE bRetValue;
DP_ERROR_CODE bDiagRetValue;
uint8_t bLoop;

   bLoop = 0;
   bRetValue = DP_PRM_RETRY_ERROR;
   while( bLoop++ < MAX_DIAG_RETRY )
   {
      bDiagRetValue = DpDiag_Alarm( USER_TYPE_PRM_OK, 0, (ALARM_STATUS_PDU_PTR) VPC3_NULL_PTR, VPC3_FALSE );
      if( ( bDiagRetValue == DP_OK ) || ( bDiagRetValue == DP_DIAG_SAME_DIAG ) )
      {
         bRetValue = ePrmError;
         break;
      }
   }

   return bRetValue;
}//DP_ERROR_CODE DpDiag_SetPrmOk( DP_ERROR_CODE ePrmError )

/*--------------------------------------------------------------------------*/
/* function: DpDiag_SetPrmNotOk                                             */
/*--------------------------------------------------------------------------*/
/**
 * @brief Set User specific diagnostic, if SetPrm is not OK.
 * In this demo, the last user specific diagnostic will be cleared!
 *
 * @return VPC3_TRUE - diagnostic is set
 * @return VPC3_FALSE - diagnostic is not resettet yet, try again
 */
DP_ERROR_CODE DpDiag_SetPrmNotOk( DP_ERROR_CODE ePrmError )
{
DP_ERROR_CODE bRetValue;

uint8_t bLoop;

   bLoop = 0;
   bRetValue = DP_PRM_RETRY_ERROR;
   while( bLoop++ < MAX_DIAG_RETRY )
   {
      if( DpDiag_Alarm( USER_TYPE_PRM_NOK, 0, (ALARM_STATUS_PDU_PTR) VPC3_NULL_PTR, VPC3_FALSE ) == DP_OK )
      {
         bRetValue = ePrmError;
         break;
      }
   }

   return bRetValue;
}//DP_ERROR_CODE DpDiag_SetPrmNotOk( DP_ERROR_CODE ePrmError )

/*--------------------------------------------------------------------------*/
/* function: DpDiag_SetCfgOk                                                */
/*--------------------------------------------------------------------------*/
/**
 * @brief Reset static diagnostic bit.
 *
 * @return VPC3_TRUE - the static diagnostic is resettet
 * @return VPC3_FALSE - the static diagnostic is not resettet yet, try again
 */
E_DP_CFG_ERROR DpDiag_SetCfgOk( E_DP_CFG_ERROR eCfgError )
{
E_DP_CFG_ERROR eRetValue;
uint8_t bLoop;

   bLoop = 0;
   eRetValue = DP_CFG_FAULT;
   while( bLoop++ < MAX_DIAG_RETRY )
   {
      if( DpDiag_Alarm( USER_TYPE_CFG_OK, 0x22, (ALARM_STATUS_PDU_PTR) VPC3_NULL_PTR, VPC3_FALSE ) == DP_OK )
      {
         eRetValue = eCfgError;
         break;
      }
   }

   return eRetValue;
}//E_DP_CFG_ERROR DpDiag_SetCfgOk( E_DP_CFG_ERROR eCfgError )

/*--------------------------------------------------------------------------*/
/* function: DpDiag_ResetStatDiag                                           */
/*--------------------------------------------------------------------------*/
/**
 * @brief Reset static diagnostic bit.
 *
 * @return VPC3_TRUE - the static diagnostic bit is resettet
 * @return VPC3_FALSE - the static diagnostic bit is not resettet yet, try again
 */
uint8_t DpDiag_ResetStatDiag( void )
{
   if( DpDiag_Alarm( USER_TYPE_APPL_RDY, 0, (ALARM_STATUS_PDU_PTR) VPC3_NULL_PTR, VPC3_FALSE ) == DP_OK )
   {
      return VPC3_TRUE;
   }
   return VPC3_FALSE;
}//uint8_t DpDiag_ResetStatDiag( void )

/*---------------------------------------------------------------------------*/
/* function: UserResetDiagnosticBuffer                                       */
/*---------------------------------------------------------------------------*/
/**
 * @brief Reset diagnostic buffer.
 */
void DpDiag_ResetDiagnosticBuffer( void )
{
   do
   {
      //fetch new diagnosis buffer
      pDpSystem->pDiagBuffer = VPC3_GetDiagBufPtr();
   }
   while( pDpSystem->pDiagBuffer == VPC3_NULL_PTR );

   //clear diagnostic buffer
   DpDiag_Alarm( USER_TYPE_RESET_DIAG, USER_TYPE_RESET_DIAG, (ALARM_STATUS_PDU_PTR) VPC3_NULL_PTR, VPC3_FALSE );
}//void DpDiag_ResetDiagnosticBuffer( void )

/*---------------------------------------------------------------------------*/
/* function: DpDiag_IsrDiagBufferChanged                                     */
/*---------------------------------------------------------------------------*/
/**
 * @brief The function VPC3_Isr() or VPC3_Poll() calls this function if the VPC3+ has
 * exchanged the diagnostic buffers, and made the old buffer available again to the user.
 */
void DpDiag_IsrDiagBufferChanged( void )
{
   // diagnosis buffer has been changed
   VPC3_ClrDpState( eDpStateDiagActive );
   // Fetch new diagnosis buffer
   pDpSystem->pDiagBuffer = VPC3_GetDiagBufPtr();
}//void DpDiag_IsrDiagBufferChanged( void )

/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

