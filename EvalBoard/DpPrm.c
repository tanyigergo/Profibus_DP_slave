/***************************  Filename: DpPrm.c ******************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0                              */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      Einsteinstraße 6        */
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
 * @brief Handling of PROFIBUS-DP parameter telegram.
 *
 * @author Peter Fredehorst
 * @version $Rev$
 *
 */

/* include hierarchy */

#include "platform.h"

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* function: DpPrm_Init                                                      */
/*---------------------------------------------------------------------------*/
/**
 * @brief Initializes local structures.
 *
 * This function is called during startup and with each received parameter telegram from PROFIBUS-DP master.
 */
void DpPrm_Init( void )
{
   pDpSystem->eDPV1 = DPV0_MODE;
}//void DpPrm_Init( void )

/*---------------------------------------------------------------------------*/
/* function: DpPrm_ChkDpv1StatusBytes                                        */
/*---------------------------------------------------------------------------*/
/**
 * @brief Check DP-V1 status bytes.
 *
 * Some rules are defined in IEC61158-6 ( summery RulesCheckingPrmData.pdf - <a href="RulesCheckingPrmData.pdf">click here ).</a>
 *
 * @attention In DP-V0 mode all bytes are fix to 0x00!
 *
 * @param[in]bDpv1Status1    DP-V1 status byte 1
 * @param[in]bDpv1Status2    DP-V1 status byte 2
 * @param[in]bDpv1Status3    DP-V1 status byte 3
 * @return DP_OK - The check of DP-V1 status bytes is OK.
 * @return DP_PRM_DPV1_STATUS - The check of DP-V1 status bytes isn't OK. @see DP_ERROR_CODE
 */
static DP_ERROR_CODE DpPrm_ChkDpv1StatusBytes( uint8_t bDpv1Status1, uint8_t bDpv1Status2, uint8_t bDpv1Status3 )
{
DP_ERROR_CODE eRetValue;

   eRetValue = DP_OK;

   //DPV0-Mode
   if(    (( bDpv1Status1 & 0xF8 ) != 0x00 )
       || (( bDpv1Status2 & 0xFF ) != 0x00 )
       || (( bDpv1Status3 & 0xFF ) != 0x00 )
     )
   {
      eRetValue = DP_PRM_DPV1_STATUS;
   }//if(    (( bDpv1Status1 & 0xF8 ) != 0x00 ) ...

   return eRetValue;
}//static DP_ERROR_CODE DpPrm_ChkDpv1StatusBytes( uint8_t bDpv1Status1, uint8_t bDpv1Status2, uint8_t bDpv1Status3 )

/*---------------------------------------------------------------------------*/
/* function: DpPrm_ChkNewPrmData                                             */
/*---------------------------------------------------------------------------*/
/**
 * @brief Checking parameter data.
 * The user has to program the function for checking the received parameter data.
 *
 * @param[in] pbPrmData - address of parameter data
 * @param[in] bPrmLength - length of parameter data
 *
 * @return DP_OK - The transferred parameterization is OK.
 * @return DP_NOK - The transferred parameterization isn't OK.
 */

DP_ERROR_CODE DpPrm_ChkNewPrmData( MEM_UNSIGNED8_PTR pbPrmData, uint8_t bPrmLength )
{
MEM_STRUC_PRM_PTR psToPrmData;
DP_ERROR_CODE     eRetValue;

   DpPrm_Init();

   eRetValue = DP_OK;

   if( bPrmLength == PRM_LEN_DPV1 )
   {
      psToPrmData = ( MEM_STRUC_PRM_PTR )pbPrmData;

      eRetValue = DpPrm_ChkDpv1StatusBytes( psToPrmData->bDpv1Status1, psToPrmData->bDpv1Status2, psToPrmData->bDpv1Status3 );
   }//if( bPrmLength == PRM_LEN_DPV1 )
   else
   {
      eRetValue = DP_PRM_LEN_ERROR;
   }//else of if( bPrmLength == PRM_LEN_DPV1 )

   if( ( VPC3_GET_DP_STATE() == DATA_EX ) && ( eRetValue == DP_OK ) )
   {
      //don't send diagnostic here
   }//if( ( VPC3_GET_DP_STATE() == DATA_EX ) && ( eRetValue == DP_OK ) )
   else
   {
      eRetValue = ( eRetValue == DP_OK ) ? DpDiag_SetPrmOk( eRetValue ) : DpDiag_SetPrmNotOk( eRetValue );
   }//else of if( ( VPC3_GET_DP_STATE() == DATA_EX ) && ( eRetValue == DP_OK ) )

   return eRetValue;
}//DP_ERROR_CODE DpPrm_ChkNewPrmData( MEM_UNSIGNED8_PTR pbPrmData, uint8_t bPrmLength )


/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2012. Confidential.                         */
/*****************************************************************************/

