/************************** Filename: dp_user.h ******************************/
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
/* Function:                                                                 */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Technical support:       eMail: support@profichip.com                     */
/*                          Phone: ++49-9132-744-2150                        */
/*                          Fax. : ++49-9132-744-29-2150                     */
/*                                                                           */
/*****************************************************************************/

/*! \file
     \brief Header file for user application.

*/

/*****************************************************************************/
/* contents:
*/
/*****************************************************************************/
/* reinclude protection */


#ifndef DP_APPL_H
#define DP_APPL_H

/*---------------------------------------------------------------------------*/
/* defines, structures                                                       */
/*---------------------------------------------------------------------------*/
/** \brief Events for PROFIBUS Application */
typedef enum
{
   eDpApplEv_Init              = 0x0000,   /**< Init value. */
   eDpApplEv_ResetBasp         = 0x0001,   /**< Enable BASP signal. */
   eDpApplEv_SetBasp           = 0x0002,   /**< Disable BASP signal. */
   eDpApplEv_IoIn              = 0x0010,   /**< Read input data. */
   eDpApplEv_IoOut             = 0x0020,   /**< Write output data. */
   eDpApplEv_Freeze            = 0x0100,   /**< Write freeze command. */
   eDpApplEv_UnFreeze          = 0x0200,   /**< Write unfreeze command. */
   eDpApplEv_Sync              = 0x0400,   /**< Write sync command. */
   eDpApplEv_UnSync            = 0x0800,   /**< Write unsync command. */
   eDpApplEv_FwUpd             = 0x1000,   /**< Firmware update command. */
   eDpApplEv_PollEepromRW      = 0x2000,   /**< Poll EEPROM Read command. */
   eDpApplEv_PollServChnRW     = 0x4000    /**< Poll service channel. */
} eDpApplEv_Flags;

typedef struct
{
   eDpApplEv_Flags   eDpApplEvent;                 /**< User application structure. */

   CFG_STRUCT        sCfgData;                     /**< Default PROFIBUS configuration data. */

   uint8_t           abDpOutputData[DOUT_BUFSIZE]; /**< Buffer for PROFIBUS output data ( master --> slave ). */
   uint8_t           abDpInputData[DIN_BUFSIZE];   /**< Buffer for PROFIBUS input data ( slave to master ). */
}DP_APPL_STRUC;

extern DP_APPL_STRUC  sDpAppl;   /**< User application structure. */

/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message "The header DPAPPL.H is included twice or more !"
#endif


/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2012. Confidential.                         */
/*****************************************************************************/

