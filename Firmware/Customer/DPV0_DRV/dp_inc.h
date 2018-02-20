/**************************  Filename: dp_inc.h  *****************************/
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
/* Description: Extern modules to be included in all DP files.               */
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

    - header include hierarchy for system environment

*/
/*****************************************************************************/
/* reinclude protection */


#ifndef DP_INC_H
#define DP_INC_H


/*****************************************************************************/
/* header include hierarchy for system environment */


/*------------------------------------------------------------------------*/
/* Debug Codes                                                            */
/*------------------------------------------------------------------------*/
typedef enum
{
   START__                          = ((uint8_t)0x01),

   DP_INTERRUPT__                   = ((uint8_t)0x20),
   DP_INTERRUPT_PRM__               = ((uint8_t)0x21),
   DP_INTERRUPT_CFG__               = ((uint8_t)0x22),

   FdlIndicationIsr__               = ((uint8_t)0x30),
   FdlPollEndIsr__                  = ((uint8_t)0x31),

   MSAC_C2_INPUT_QUEUE__            = ((uint8_t)0x40),
   MSAC_C2_PROCESS__                = ((uint8_t)0x41),
   MSAC_C2_PRU_PROVIDE__            = ((uint8_t)0x42),
   MSAC_C2_REALIZE_IND_REC__        = ((uint8_t)0x43),
   MSAC_C2_REALIZE_RESP_SENT__      = ((uint8_t)0x44),
   MSAC_C2_CREATE_DISCONNECT_PDU__  = ((uint8_t)0x45),
   MSAC_C2_SEND_RESPONSE_DATA__     = ((uint8_t)0x46),
   MSAC_C2_HANDLE_USER_RESPONSE__   = ((uint8_t)0x47),
   MSAC_C2_RESP_PDU_PROVIDE__       = ((uint8_t)0x48),

   USER_FORCE_ABORT__               = ((uint8_t)0x49),

   RED_SM__                         = ((uint8_t)0x55),

   RED_RISM_ST_POWER_ON             = ((uint8_t)0x60),
   RED_RISM_ST_S_WAITING            = ((uint8_t)0x61),
   RED_RISM_ST_S_PRIMARY            = ((uint8_t)0x62),
   RED_RISM_ST_C_CONFIGURE          = ((uint8_t)0x63),
   RED_RISM_ST_BACKUP               = ((uint8_t)0x64),
   RED_RISM_ST_BP_PARTNER_ACK       = ((uint8_t)0x65),
   RED_RISM_ST_BP_SWITCHOVER        = ((uint8_t)0x66),
   RED_RISM_ST_BP_PRM_CMD           = ((uint8_t)0x67),
   RED_RISM_ST_BP_DX                = ((uint8_t)0x68),
   RED_RISM_ST_PRIMARY              = ((uint8_t)0x69),
   RED_RISM_ST_PB_PARTNER_ACK       = ((uint8_t)0x6A),
   RED_RISM_ST_PB_SWITCHOVER        = ((uint8_t)0x6B),
   RED_RISM_ST_NIL                  = ((uint8_t)0x6C),

   RED_RISM_ST_DEFAULT              = ((uint8_t)0x80),

   FatalError__                     = ((uint8_t)0xFF)
} DEBUG_CODE;

/*------------------------------------------------------------------------*/
/* Debug Structure                                                        */
/*------------------------------------------------------------------------*/
#define MAX_NR_OF_DEBUG     256

typedef struct
{
   DEBUG_CODE  bDebugCode;
   uint8_t     bDetail1;
   uint8_t     bDetail2;
} sDP_DEBUG_BUFFER_ITEM;
#define psDP_BUFFER_ITEM   sDP_DEBUG_BUFFER_ITEM  PTR_ATTR *

/*------------------------------------------------------------------------*/
/* Debug function, external defines                                       */
/*------------------------------------------------------------------------*/
#ifdef DP_DEBUG_ENABLE
   #define DP_WriteDebugBuffer__( _bDebugCode, _bDetail1, _bDetail2 )\
              DpWriteDebugBuffer( _bDebugCode, _bDetail1, _bDetail2 )

   extern sDP_DEBUG_BUFFER_ITEM    asDebugBuffer[ MAX_NR_OF_DEBUG ];
   extern uint8_t                  bDebugBufferIndex;
   extern uint8_t                  bDebugBufferOverlapped;

   #define DP_ClearDebugBuffer__( void )\
              DpClearDebugBuffer( void )
#else
   #define DP_WriteDebugBuffer__( _bDebugCode, _bDetail1, _bDetail2 )\
                                         /* not used ( DP_WriteDebugBuffer__ ) */
   #define DP_ClearDebugBuffer__( void )\
              /* not used ( DP_ClearDebugBuffer__ ) */
#endif /* #ifdef DP_DEBUG_ENABLE */

/*------------------------------------------------------------------------*/
/* DpAppl.c / main.c                                                      */
/*------------------------------------------------------------------------*/
extern VPC3_STRUC      VPC3_PTR     pVpc3;     /**< Pointer to VPC3+ structure. */
extern VPC3_SYSTEM_STRUC            sDpSystem; /**< User application structure. */

extern void                   DpAppl_SetResetVPC3Channel1          ( void );
extern void                   DpAppl_ClrResetVPC3Channel1          ( void );
extern void                   DpAppl_SetResetVPC3Channel2          ( void );
extern void                   DpAppl_ClrResetVPC3Channel2          ( void );
extern void                   DpAppl_SetOfflineVPC3Channel1        ( void );
extern void                   DpAppl_SetOfflineVPC3Channel2        ( void );
extern void                   DpAppl_EnableInterruptVPC3Channel1   ( void );
extern void                   DpAppl_DisableInterruptVPC3Channel1  ( void );
extern void                   DpAppl_EnableInterruptVPC3Channel2   ( void );
extern void                   DpAppl_DisableInterruptVPC3Channel2  ( void );
extern void                   DpAppl_EnableInterruptVPC3Sync       ( void );
extern void                   DpAppl_DisableInterruptVPC3Sync      ( void );
extern void                   DpAppl_EnableAllInterrupts           ( void );
extern void                   DpAppl_DisableAllInterrupts          ( void );
extern void                   Vpc3Wait_1ms                         ( void );

#if VPC3_SERIAL_MODE
//I have already defined these externs
   extern void                Vpc3Write      ( VPC3_ADR wAddress, uint8_t bData );
   extern uint8_t             Vpc3Read       ( VPC3_ADR wAddress );
   extern void                Vpc3MemSet     ( VPC3_ADR wAddress, uint8_t bValue, uint16_t wLength );
   extern uint8_t             Vpc3MemCmp     ( VPC3_UNSIGNED8_PTR pToVpc3Memory1, VPC3_UNSIGNED8_PTR pToVpc3Memory2, uint16_t wLength );
   extern void                CopyToVpc3     ( VPC3_UNSIGNED8_PTR pToVpc3Memory, MEM_UNSIGNED8_PTR pLocalMemory, uint16_t wLength );
   extern void                CopyFromVpc3   ( MEM_UNSIGNED8_PTR pLocalMemory, VPC3_UNSIGNED8_PTR pToVpc3Memory, uint16_t wLength );

   #define CopyToVpc3_( _pToVpc3Memory, _pLocalMemory, _wLength )\
               CopyToVpc3( _pToVpc3Memory, _pLocalMemory, _wLength )

   #define CopyFromVpc3_( _pLocalMemory, _pToVpc3Memory, _wLength )\
               CopyFromVpc3( _pLocalMemory, _pToVpc3Memory, _wLength )

   #define Vpc3MemSet_( _pToVpc3Memory, _bValue, _wLength )\
               Vpc3MemSet( _pToVpc3Memory, _bValue, _wLength )

   #define Vpc3MemCmp_( _pToVpc3Memory1, _pToVpc3Memory2, _wLength )\
               Vpc3MemCmp( _pToVpc3Memory1, _pToVpc3Memory2, _wLength )

#else

   #define CopyToVpc3_( _pToVpc3Memory, _pLocalMemory, _wLength )\
               memcpy( _pToVpc3Memory, _pLocalMemory, _wLength )

   #define CopyFromVpc3_( _pLocalMemory, _pToVpc3Memory, _wLength )\
               memcpy( _pLocalMemory, _pToVpc3Memory, _wLength )

   #define Vpc3MemSet_( _pToVpc3Memory, _bValue, _wLength )\
               memset( _pToVpc3Memory, _bValue, _wLength )

   #define Vpc3MemCmp_( _pToVpc3Memory1, _pToVpc3Memory2, _wLength )\
               memcmp( _pToVpc3Memory1, _pToVpc3Memory2, _wLength )

 #endif /* #if VPC3_SERIAL_MODE */

#if REDUNDANCY
   extern void                DpAppl_ProfibusInit                 ( uint8_t bMode, uint8_t bSlaveAddress );
#else
   extern void                DpAppl_ProfibusInit                 ( void );
#endif /* #if REDUNDANCY */
extern void                   DpAppl_ProfibusMain                 ( void );
extern void                   DpAppl_FatalError                   ( DP_ERROR_FILE file, uint16_t line, VPC3_ERRCB_PTR errcb_ptr );

extern void                   DpAppl_IsrGoLeaveDataExchange       ( uint8_t bDpState );
extern void                   DpAppl_IsrDxOut                     ( void );
extern void                   DpAppl_IsrNewWdDpTimeout            ( void );
extern void                   DpAppl_IsrBaudrateDetect            ( void );
extern void                   DpAppl_IsrNewGlobalControlCommand   ( uint8_t bGcCommand );
extern void                   DpAppl_IsrNewSetSlaveAddress        ( MEM_STRUC_SSA_BLOCK_PTR psSsa );
extern void                   DpAppl_IsrTimerClock                ( void );

#if DP_TIMESTAMP
   extern void                DpAppl_IsrClockSynchronisation      ( void );
#else
   extern void                DpAppl_MacReset                     ( void );
#endif /* #if DP_TIMESTAMP */

#if DP_SUBSCRIBER
   extern void                DpAppl_IsrDxbOut                    ( void );
   extern void                DpAppl_IsrDxbLinkError              ( void );
#endif /* #if DP_SUBSCRIBER */

#if REDUNDANCY
   extern DP_ERROR_CODE       InitRedundancyChannel               ( uint8_t bAddress );

   #if RS232_REDUNDANCY
      extern void             InitRedundancySerio                 ( uint16_t wBaudrate );
      extern void             ParseRS232                          ( void );
      extern void             StartSerialCommunication            ( void );
   #endif /* #if RS232_REDUNDANCY */
#endif /* #if REDUNDANCY */

/*------------------------------------------------------------------------*/
/* DpPrm.c                                                                */
/*------------------------------------------------------------------------*/
extern void                   DpPrm_Init                          ( void );
extern DP_ERROR_CODE          DpPrm_ChkNewPrmData                 ( MEM_UNSIGNED8_PTR pbPrmData, uint8_t bPrmLength );
extern DP_ERROR_CODE          DpPrm_ChkNewExtPrmData              ( MEM_UNSIGNED8_PTR pbPrmData, uint8_t bPrmLength );

/*------------------------------------------------------------------------*/
/* DpCfg.c                                                                */
/*------------------------------------------------------------------------*/
extern void                   DpCfg_Init                          ( void );
extern E_DP_CFG_ERROR         DpCfg_ChkNewCfgData                 ( MEM_UNSIGNED8_PTR pbCfgData, uint8_t bCfgLength );

/*------------------------------------------------------------------------*/
/* DpDiag.c                                                               */
/*------------------------------------------------------------------------*/
extern void                   DpDiag_Init                         ( void );
extern DP_ERROR_CODE          DpDiag_Alarm                        ( uint8_t bAlarmType, uint8_t bSeqNr, ALARM_STATUS_PDU_PTR psAlarm, uint8_t bCheckDiagFlag );
extern void                   DpDiag_AlarmAckReq                  ( ALARM_STATUS_PDU_PTR psAlarm );
extern ALARM_STATUS_PDU_PTR   DpDiag_AlarmAlloc                   ( void );
extern void                   DpDiag_AlarmInit                    ( void );
extern void                   DpDiag_FreeAlarmBuffer              ( ALARM_STATUS_PDU_PTR alarm_ptr );
extern void                   DpDiag_ResetDiagnosticBuffer        ( void );
extern void                   DpDiag_IsrDiagBufferChanged         ( void );
extern DP_ERROR_CODE          DpDiag_SetPrmOk                     ( DP_ERROR_CODE ePrmError );
extern DP_ERROR_CODE          DpDiag_SetPrmNotOk                  ( DP_ERROR_CODE ePrmError );
extern E_DP_CFG_ERROR         DpDiag_SetCfgOk                     ( E_DP_CFG_ERROR eCfgError );
extern uint8_t                DpDiag_SetCfgNotOk                  ( void );
extern uint8_t                DpDiag_ResetStatDiag                ( void );
extern void                   DpDiag_CheckTimestampAlarms         ( void );
extern uint8_t                DpDiag_SendTimeStampProcessAlarm    ( uint8_t * pbTimestamDiag );
extern void                   DpDiag_SetDefIdentRelDiag           ( void );
extern void                   DpDiag_SetIdentRelDiagEntry         ( uint8_t bModNr );
extern void                   DpDiag_ClrIdentRelDiagEntry         ( uint8_t bModNr );
extern void                   DpDiag_SetDefModuleStatDiag         ( void );
extern void                   DpDiag_SetModulStatusEntry          ( uint8_t bModNr, E_DP_DIAG_MS eMs );
extern void                   DpDiag_CheckDpv0Diagnosis           ( void );
#if REDUNDANCY
   extern void                DpDiag_UpdateRedStateDiag           ( void );
#endif /* #if REDUNDANCY */

/*------------------------------------------------------------------------*/
/* DpV1.c                                                                 */
/*------------------------------------------------------------------------*/
#if DP_FDL
   extern DPV1_RET_VAL        Dpv1_ReadReq                        ( uint8_t bSapNr, MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );
   extern DPV1_RET_VAL        Dpv1_WriteReq                       ( uint8_t bSapNr, MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );

   #if DP_MSAC_C2
      extern DPV1_RET_VAL     Dpv1_Msac2InitiateReq               ( uint8_t bSapNr, INITIATE_REQ_PDU_PTR psInitiateReq, MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );
      extern DPV1_RET_VAL     Dpv1_Msac2DataTransportReq          ( uint8_t bSapNr, MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );
      extern DPV1_RET_VAL     Dpv1_Msac2AbortInd                  ( uint8_t bSapNr );
   #endif /* #if DP_MSAC_C2 */
#endif /* #if DP_FDL */

/*------------------------------------------------------------------------*/
/* DpIm.c                                                                 */
/*------------------------------------------------------------------------*/
#if DP_FDL
   extern uint8_t             DpIm_PowerOn                  ( void );
   extern void                DpIm_ClearImIndex             ( uint8_t bSapNr );
   extern DPV1_RET_VAL        DpIm_WriteImFunction          ( uint8_t bSapNr, MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );
   extern DPV1_RET_VAL        DpIm_ReadImFunction           ( uint8_t bSapNr, MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );
   extern DPV1_RET_VAL        DpIm_WriteImModFunction       ( uint8_t bSapNr, MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );
   extern DPV1_RET_VAL        DpIm_ReadImModFunction        ( uint8_t bSapNr, MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );
#endif /* #if DP_FDL */

/*------------------------------------------------------------------------*/
/* DpFwUpd.c                                                              */
/*------------------------------------------------------------------------*/
#if DP_FDL
   extern DPV1_RET_VAL        DpFwupd_Write                 ( MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );
   extern DPV1_RET_VAL        DpFwupd_Read                  ( MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );
   extern void                DpFwUpd_FirmwareUpdate        ( void );
#endif /* #if DP_FDL */

/*------------------------------------------------------------------------*/
/* DpIso.c                                                                */
/*------------------------------------------------------------------------*/
#if DP_ISOCHRONOUS_MODE
   extern void                DpIso_Activate                ( void );
   extern void                DpIso_Init                    ( void );
   extern DP_ERROR_CODE       DpIso_Prm                     ( uint8_t bDpv1Status3, MEM_STRUC_PRM_BLOCK_ISO_PTR psIso );
   extern void                DpIso_GcClear                 ( void );
   extern void                DpIso_GcOperate               ( void );
   extern void                DpIso_Isr                     ( void );
#endif /* #if DP_ISOCHRONOUS_MODE */

/*------------------------------------------------------------------------*/
/* dp_if.c                                                                */
/*------------------------------------------------------------------------*/
extern void                   GetFirmwareVersion                  ( psDP_VERSION pVersion );
extern DP_ERROR_CODE          VPC3_MemoryTest                     ( void );
extern DP_ERROR_CODE          VPC3_Initialization                 ( uint8_t bSlaveAddress, uint16_t wIdentNumber, psCFG psCfgData );
extern void                   VPC3_Start                          ( void );
extern void                   VPC3_Stop                           ( void );
extern void                   VPC3_ProcessDpv1StateMachine        ( void );
extern DP_ERROR_CODE          VPC3_CalculateInpOutpLength         ( MEM_UNSIGNED8_PTR pToCfgData, uint8_t bCfgLength );
extern void                   VPC3_SetIoDataLength                ( void );
extern VPC3_UNSIGNED8_PTR     VPC3_GetDinBufPtr                   ( void );
extern void                   VPC3_InputDataUpdate                ( MEM_UNSIGNED8_PTR pToInputData );
extern VPC3_UNSIGNED8_PTR     VPC3_GetDoutBufPtr                  ( MEM_UNSIGNED8_PTR pbState );
extern VPC3_UNSIGNED8_PTR     VPC3_GetDiagBufPtr                  ( void );
extern uint8_t                VPC3_GetMasterAddress               ( void );
extern DP_ERROR_CODE          VPC3_SetDiagnosis                   ( MEM_UNSIGNED8_PTR pbToUserDiagData, uint8_t bUserDiagLength, uint8_t bDiagControl, uint8_t bCheckDiagFlag );
extern void                   VPC3_CheckDiagBufferChanged         ( void );
extern uint16_t               VPC3_GetFreeMemory                  ( void );
extern uint8_t                VPC3_SetAlarm                       ( ALARM_STATUS_PDU_PTR psAlarm, uint8_t bCallback );
extern void                   VPC3_GetErrorCounter                ( uint16_t *wFalse, uint16_t *wValid );
extern void                   VPC3_CopyErrorCounter               ( MEM_UNSIGNED8_PTR pbData );
extern DP_ERROR_CODE          VPC3_WaitForGoOffline               ( void );

#if DP_SUBSCRIBER
   extern DP_ERROR_CODE       VPC3_SubscriberToLinkTable          ( MEM_PRM_SUBSCRIBER_TABLE_PTR psSubsTable, uint8_t bNrOfLinks );
   extern VPC3_UNSIGNED8_PTR  VPC3_GetDxbBufPtr                   ( MEM_UNSIGNED8_PTR pbState );
#endif /* #if DP_SUBSCRIBER */

#ifdef DP_DEBUG_ENABLE
   extern void                DpWriteDebugBuffer                  ( DEBUG_CODE bDebugCode, uint8_t bDetail1, uint8_t bDetail2 );
   extern void                DpClearDebugBuffer                  ( void );
#endif /* #ifdef DP_DEBUG_ENABLE */

/*------------------------------------------------------------------------*/
/* dp_isr.c                                                               */
/*------------------------------------------------------------------------*/
extern void                   VPC3_Isr                            ( void );
extern void                   VPC3_Poll                           ( void );
extern void                   VPC3_GetIsrEvent                    ( void );

/*------------------------------------------------------------------------*/
/* dp_fdl.c                                                               */
/*------------------------------------------------------------------------*/
#if DP_FDL
   extern DP_ERROR_CODE       FDL_InitAcyclicServices             ( uint8_t bSlaveAddress, sFdl_Init sFdlInit );
   extern void                FDL_SetPollTimeOutIsr               ( uint16_t wSendTimeOut );
   extern void                FDL_IndicationIsr                   ( void );
   extern void                FDL_PollEndIsr                      ( void );
   extern void                FDL_CyclicService                   ( void );
   extern void                FDL_RespProvide                     ( uint8_t bSapNr, uint8_t bPrimSecBuf );
   extern void                FDL_ResetCntrlRespBit               ( uint8_t bSapNr, uint8_t bPrimSecBuf );
   extern void                FDL_IndDisable                      ( uint8_t bSapNr );
   extern uint8_t             FDL_IndAwait                        ( uint8_t bSapNr );
   extern void                FDL_PerformSapReset                 ( uint8_t bSapNr );
   extern uint8_t             FDL_OpenChannel                     ( void );
   extern void                FDL_CloseChannel                    ( FDL_SAP_TYPE eSapType );
   extern void                FDL_IndDisableSAP50                 ( void );
   extern void                FDL_SetDpv1ResponseLength           ( MSG_HEADER_PTR psMsgHeader, uint8_t bLength );
   extern DPV1_RET_VAL        FDL_SetDpv1ErrorCode                ( MSG_HEADER_PTR psMsgHeader, uint8_t bError1, uint8_t bError2 );

#endif /* #if DP_FDL */

/*------------------------------------------------------------------------*/
/* dp_msac1.c                                                             */
/*------------------------------------------------------------------------*/
#if DP_MSAC_C1
   extern DP_ERROR_CODE       MSAC_C1_Init                  ( void );
   extern uint8_t             MSAC_C1_CheckIndNewPrmData    ( MEM_STRUC_PRM_PTR psPrmData, uint8_t bPrmLength );
   extern void                MSAC_C1_PduReceived           ( uint8_t bSapNr, MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );
   extern void                MSAC_C1_Process               ( void );
   extern void                MSAC_C1_LeaveDx               ( void );
   extern void                MSAC_C1_CheckIndDxOut         ( void );
   extern void                MSAC_C1_DoCfgNotOk            ( void );
   extern void                MSAC_C1_DoCfgOk               ( void );
   extern void                MSAC_C1_IndDisableDone        ( uint8_t bSapNr, uint8_t bRetValue );
   extern uint8_t             MSAC_C1_TransmitDelay         ( uint8_t bSapNr, uint8_t bDpv1Service, DPV1_RET_VAL bDpv1RetValue );

   #if DP_ALARM
      extern void             AL_AlarmProcess               ( void );
      extern uint8_t          AL_AlarmEnabled               ( uint8_t bAlarmType );
      extern uint8_t          AL_SetAlarm                   ( ALARM_STATUS_PDU_PTR psAlarm, uint8_t bCallback );
      extern void             AL_ClearAlarm                 ( uint8_t bAlarmTypeBitField, uint8_t bSeqNr );
      extern uint8_t          AL_CheckTypeStatus            ( uint8_t bAction, uint8_t bAlarmType );
   #endif /* #if DP_ALARM */

   #if REDUNDANCY
      extern void             RedundancyStopMSAC1S          ( void );
      extern void             RedundancyStartMSAC1S         ( void );
      extern void             RedundancyResetMSAC1S         ( void );
   #endif /* #if REDUNDANCY */
#endif /* #if DP_MSAC_C1 */

/*------------------------------------------------------------------------*/
/* dp_msac2.c                                                             */
/*------------------------------------------------------------------------*/
#if DP_MSAC_C2
   extern DP_ERROR_CODE       MSAC_C2_Init                  ( void );
   extern void                MSAC_C2_Process               ( void );
   extern void                MSAC_C2_OpenChannelSap        ( uint8_t bSapNr );
   extern DP_ERROR_CODE       MSAC_C2_OpenChannel           ( void );
   extern uint8_t             MSAC_C2_InitiateReqToRes      ( INITIATE_REQ_PDU_PTR psInitiateReq, MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );
   extern void                MSAC_C2_InputQueue            ( uint8_t bSapNr, uint8_t bRetValue, uint8_t bFdlCode, MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );
   extern void                MSAC_C2_TimerTick10msec       ( void );
   extern uint8_t             MSAC_C2_TransmitDelay         ( uint8_t bSapNr, DPV1_RET_VAL bDpv1RetValue );
   extern void                MSAC_C2_SetTimeoutIsr         ( void );
   extern void                MSAC_C2_ResetStateMachine     ( void );
   extern uint8_t             MSAC_C2_UserForceAbort        ( uint8_t bSapNr, uint8_t bReasonCode, uint8_t bLocation );
#endif /* #if DP_MSAC_C2 */

/*------------------------------------------------------------------------*/
/* time_if.c                                                              */
/*------------------------------------------------------------------------*/
#if DP_TIMESTAMP
   extern void                TimeStamp_Process                      ( void );
   extern void                TimeStamp_Init                         ( uint8_t bInitStopTheProcess );
   extern void                TimeStamp_Timer                        ( void );
   extern void                TimeStamp_Timer10ms                    ( void );
   extern void                TimeStamp_GC                           ( uint8_t bGlobalControl );
   extern void                TimeStamp_GoLeaveDx                    ( uint8_t bDpState );
   extern void                TimeStamp_Prm                          ( DP_ERROR_CODE bPrmRetValue );
   extern DP_ERROR_CODE       TimeStamp_CsPrm                        ( uint8_t bDPV1Alarm, MEM_STRUC_PRM_BLOCK_CS_PTR psCs );
   extern void                TimeStamp_Cfg                          ( E_DP_CFG_ERROR eCfgRetValue );
   extern void                TimeStamp_ApplicationReady             ( void );
   extern void                TimeStamp_ProcessAlarmSend             ( uint8_t bSeqNr );
   extern void                TimeStamp_ProcessAlarmAck              ( uint8_t bSeqNr );
   extern DPV1_RET_VAL        TimeStamp_Dpv1Read                     ( uint8_t bSapNr, MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );
   extern DPV1_RET_VAL        TimeStamp_Dpv1Write                    ( uint8_t bSapNr, MSG_HEADER_PTR psMsgHeader, VPC3_UNSIGNED8_PTR pToDpv1Data );

   extern void                TimeStamp_ApplActivate                 ( void );

   extern void                TimeStamp_CallbackStart                ( void );
   extern void                TimeStamp_AddTimeTriggerDiscrete       ( uint8_t bSlot, uint8_t bBit, uint8_t bBitValue, uint8_t bNewTimeStamp );
   extern uint32_t            TimeStamp_GetUsTicker                  ( void );
   extern void                TimeStamp_ResetUsTicker                ( void );

   extern void                TimeStamp_Switchover                   ( uint8_t bRedState );
   extern void                TimeStamp_SwitchoverDone               ( uint8_t bRedState );
   extern void                TimeStamp_RedDrvSwitchWaitingToBackup  ( uint8_t bChannel );
   extern uint8_t             TimeStamp_CheckProcessAlarm            ( ALARM_STATUS_PDU_PTR psAlarm );
   extern void                TimeStamp_SetChannel                   ( uint8_t bChannel );
   extern void                TimeStamp_InitChannel                  ( uint8_t bChannel );
#endif /* #if DP_TIMESTAMP */

/*------------------------------------------------------------------------*/
/* RedDrv.c                                                               */
/*------------------------------------------------------------------------*/
#if REDUNDANCY
   extern void                RedDrv_Init                   ( void );
   extern void                RedDrv_Timer10ms              ( void );
   extern void                RedDrv_IntSwitchover          ( uint8_t bRedState );
   extern void                RedDrv_CheckChangeToPowerOn   ( void );
   extern void                RedDrv_BaudrateDetect         ( void );
   extern void                RedDrv_GC                     ( uint8_t bCommand );
   extern void                RedDrv_WdTimeout              ( void );
   extern void                RedDrv_GoLeaveDx              ( uint8_t bDpState );
   extern DP_ERROR_CODE       RedDrv_PrmCmd                 ( MEM_STRUC_PRM_BLOCK_PRMCMD_PTR psPrmCmd );
   extern void                RedDrv_GetRedState            ( VPC3_UNSIGNED8_PTR pToData );
   extern void                RedDrv_StateMachine           ( void );
   extern uint8_t             RedDrv_CheckPrmCmd            ( void );

   extern DP_ERROR_CODE       DpAppl_InitRedChannel_1       ( uint8_t bAddress );
   extern DP_ERROR_CODE       DpAppl_InitRedChannel_2       ( uint8_t bAddress );

   extern void                SendDebug                     ( uint8_t bStatus1, uint8_t bStatus2, uint8_t bStatus3 );
#endif /* #if REDUNDANCY */

/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message "The header DP_INC.H is included twice or more !"
#endif


/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2012. Confidential.                         */
/*****************************************************************************/

