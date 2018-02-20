/*************************  Filename: serio.h  *******************************/
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
/* Description: defines for serial port                                      */
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

    - external modules
    - global variables

*/
/*****************************************************************************/
/* reinclude protection */


#ifndef DP_SERIO_H
#define DP_SERIO_H

#define Init_serio( _baudrate )     init_serio__(_baudrate)
#define print_hexbyte( _b )         print_hexbyte__(_b)
#define print_hexword( _w )         print_hexword__(_w)
#define print_hexlong( _dw )        print_hexlong__(_dw)
#define print_string( _p )          print_string__(_p)

/*---------------------------------------------------------------------------*/
/* global data definitions                                                   */
/*---------------------------------------------------------------------------*/
extern xdata uint8_t abSndBuffer [] ;        // for RS232 sending
extern xdata uint8_t abRecBuffer [] ;        // for RS232 receiving

extern data  uint8_t bWriteRecPtr;       // Writepointer into receive buffer
extern data  uint8_t bReadRecPtr;        // Readpointer  from receive buffer
extern data  uint8_t bRecCounter;         // Counter of byte in receive buffer
extern data  uint8_t bWriteSndPtr;       // Writepointer into send buffer
extern data  uint8_t bReadSndPtr;        // Readpointer  from send buffer
extern data  uint8_t bSndCounter;         // Counter of byte in send buffer

extern bit   parity;                    // Save parity bit received
extern bit   bThrEmpty;                 // Transmitter reg. empty


/*---------------------------------------------------------------------------*/
/* global macros                                                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* data definitions                                                          */
/*---------------------------------------------------------------------------*/
// defines for bSerialState
#define ON      1
#define OFF     !(ON)

/*---------------------------------------------------------------------------*/
/* global function prototypes                                                */
/*---------------------------------------------------------------------------*/

extern void InitSerio( uint16_t bBaudrate, uint8_t bInterruptEnable );

extern void print_hexbyte__ ( uint8_t b );
extern void print_hexword__ ( uint16_t w );
extern void print_hexlong__ ( uint32_t dw );
extern void init_serio__    ( uint16_t baudrate);
extern void print_string__  ( char *p );
extern void PollSerio       ( void );

/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message "The header serio.h is included twice or more !"
#endif


/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

