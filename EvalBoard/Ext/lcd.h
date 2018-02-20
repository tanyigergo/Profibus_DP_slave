/*************************  Filename: lcd.h  *********************************/
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
/* Description: defines for lcd                                              */
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

    - external modules
    - global variables

*/
/*****************************************************************************/
/* reinclude protection */


#ifndef LCD_H
#define LCD_H


/*---------------------------------------------------------------------------*/
/* global data definitions                                                   */
/*---------------------------------------------------------------------------*/
#define DISPLAY_NUMBER_OF_CHARACTERS   ((uint8_t)0x14)
#define DISPLAY_NUMBER_OF_LINES        ((uint8_t)0x02)

#define  LCD_BUSY_FLAG                 ((uint8_t)0x80)

typedef struct
{
   union
   {
      struct
      {                       //          [read]
         uint8_t reserved0;     // 000H     reserved
         uint8_t reserved1;     // 001H     reserved
         uint8_t bBusy;         // 002H     Busy Flag
         uint8_t bDR;           // 003H     Data Register
      } rd;

      struct
      {                       //          [read]
         uint8_t bIR;           // 000H     Instruction Register
         uint8_t bDR;           // 001H     Data Register
         uint8_t reserved3;     // 002H     reserved
         uint8_t reserved4;     // 003H     reserved
      } wr;
   }ctrl;
}sLCD;


/*---------------------------------------------------------------------------*/
/* global macros                                                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* data definitions                                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global function prototypes                                                */
/*---------------------------------------------------------------------------*/
extern uint8_t   LCDInitDisplay       ( void );
extern void    LCDUpdateProfichip   ( uint8_t bMainInterface, uint8_t bFunction, uint8_t bBugfix );
extern void    LCDUpdate            ( uint8_t *pChar, uint16_t wAsicType );

/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message "The header LCD.h is included twice or more !"
#endif


/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

