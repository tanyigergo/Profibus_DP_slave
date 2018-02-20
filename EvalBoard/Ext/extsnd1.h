/*H**************************************************************************
* NAME:         extsnd1.h         
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      c5132-demo-df-smc-2_0_0      
* REVISION:     1.7     
*----------------------------------------------------------------------------
* PURPOSE:
* This file is an extension to the regsnd1.h file.
* It defines mask for registers.
*****************************************************************************/

#ifndef _EXTSND1_H_
#define _EXTSND1_H_

/*_____ I N C L U D E S ____________________________________________________*/


/*_____ M A C R O S ________________________________________________________*/

/* INTERNAL RESSOURCES */

#define ERAM_SIZE     0x0800
#define DRAM_SIZE     0x0080
#define IRAM_SIZE     0x0100
#define CODE_SIZE     0x10000
#define BOOT_ADDRESS  0xF000
#define BOOT_SIZE     0x1000


/* INTERRUPT NUMBER */

#define IRQ_INT0      0
#define IRQ_T0        1
#define IRQ_INT1      2
#define IRQ_T1        3
#define IRQ_UART      4
#define IRQ_MP3       5
#define IRQ_AUD       6
#define IRQ_MMC       7
#define IRQ_I2C       8
#define IRQ_SPI       9
#define IRQ_ADC       10
#define IRQ_KBD       11
#define IRQ_USB       13


/* SYSTEM MANAGEMENT */

#define MSK_SMOD1     0x80    /* PCON */
#define MSK_SMOD0     0x40
#define MSK_GF1       0x08
#define MSK_GF0       0x04
#define MSK_PD        0x02
#define MSK_IDL       0x01

#define MSK_EXT16     0x40    /* AUXR */
#define MSK_M0        0x20
#define MSK_DPHDIS    0x10
#define MSK_XRS       0x0C
#define MSK_EXTRAM    0x02
#define MSK_AO        0x01
#define ERAM_256      0x00
#define ERAM_512      0x04
#define ERAM_1024     0x08
#define ERAM_2048     0x0C

#define MSK_ENBOOT    0x20    /* AUXR1 */
#define MSK_GF3       0x08
#define MSK_DPS       0x01


/* PLL & CLOCK */

#define MSK_X2        0x01    /* CKCON */
#define MSK_T0X2      0x02
#define MSK_T1X2      0x04
#define MSK_WDX2      0x40

#define MSK_PLOCK     0x01    /* PLLCON */
#define MSK_PLLEN     0x02
#define MSK_PLLRES    0x08


/* INTERRUPT */

#define MSK_EAUD      0x40    /* IEN0 */
#define MSK_EMP3      0x20
#define MSK_ES        0x10
#define MSK_ET1       0x08
#define MSK_EX1       0x04
#define MSK_ET0       0x02
#define MSK_EX0       0x01

#define MSK_EUSB      0x40    /* IEN1 */
#define MSK_EKB       0x10
#define MSK_EADC      0x08
#define MSK_ESPI      0x04
#define MSK_EI2C      0x02
#define MSK_EMMC      0x01


/* TIMERS */

#define MSK_GATE1     0x80    /* TMOD */
#define MSK_C_T1      0x40
#define MSK_MO1       0x30
#define MSK_GATE0     0x08
#define MSK_C_T0      0x04
#define MSK_MO0       0x03


/* WATCHDOG */

#define MSK_WTO       0x07    /* WDTPRG*/


/* MP3 DECODER */

#define MSK_MPANC     0x80    /* MP3STA */
#define MSK_MPREQ     0x40
#define MSK_ERRLAY    0x20
#define MSK_ERRSYN    0x10
#define MSK_ERRCRC    0x08
#define MSK_MPFS1     0x04
#define MSK_MPFS0     0x02
#define MSK_MPVER     0x01

#define MSK_MPFREQ    0x10    /* MP3STA1 */
#define MSK_MPBREQ    0x08

#define MSK_MPEN      0x80    /* MP3CON */
#define MSK_MPBBST    0x40
#define MSK_CRCEN     0x20
#define MSK_MSKANC    0x10
#define MSK_MSKREQ    0x08
#define MSK_MSKLAY    0x04
#define MSK_MSKSYN    0x02
#define MSK_MSKCRC    0x01

#define MSK_MPFULL    0x08    /* MP3DBG */


/* AUDIO INTERFACE */

#define MSK_JUST      0xF8    /* AUDCON0 */
#define MSK_POL       0x04
#define MSK_DSIZ      0x02
#define MSK_HLR       0x01

#define MSK_SRC       0x80    /* AUDCON1 */
#define MSK_DRQEN     0x40
#define MSK_MSREQ     0x20
#define MSK_MUDRN     0x10
#define MSK_DUP       0x06
#define MSK_AUDEN     0x01
#define MSK_DUP0      0x00    /* 0 duplication */
#define MSK_DUP1      0x02    /* 1 duplication */
#define MSK_DUP2      0x04    /* 3 duplication */
#define MSK_DUP3      0x06    /* 4 duplication */

#define MSK_SREQ      0x80    /* AUDSTA */
#define MSK_UDRN      0x40
#define MSK_AUBUSY    0x20


/* MMC CONTROLLER */

#define MSK_CBUSY     0x20    /* MMSTA */
#define MSK_CRC16S    0x10
#define MSK_DATFS     0x08
#define MSK_CRC7S     0x04
#define MSK_RESPFS    0x02
#define MSK_CFLCK     0x01

#define MSK_DRPTR     0x80    /* MMCON0 */
#define MSK_DTPTR     0x40
#define MSK_CRPTR     0x20
#define MSK_CTPTR     0x10
#define MSK_MBLOCK    0x08
#define MSK_DFMT      0x04
#define MSK_RFMT      0x02
#define MSK_CRCDIS    0x01

#define MSK_BLEN      0xF0    /* MMCON1 */
#define MSK_DATDIR    0x08
#define MSK_DATEN     0x04
#define MSK_RESPEN    0x02
#define MSK_CMDEN     0x01

#define MSK_MMCEN     0x80    /* MMCON2 */
#define MSK_DCR       0x40
#define MSK_CCR       0x20
#define MSK_DATD      0x06
#define MSK_FLOWC     0x01

#define MSK_MCBI      0x80    /* MMINT */
#define MSK_EORI      0x40
#define MSK_EOCI      0x20
#define MSK_EOFI      0x10
#define MSK_F2FI      0x08
#define MSK_F1FI      0x04
#define MSK_F2EI      0x02
#define MSK_F1EI      0x01

#define MSK_MCBM      0x80    /* MMMSK */
#define MSK_EORM      0x40
#define MSK_EOCM      0x20
#define MSK_EOFM      0x10
#define MSK_F2FM      0x08
#define MSK_F1FM      0x04
#define MSK_F2EM      0x02
#define MSK_F1EM      0x01


/* SPI CONTROLLER */

#define MSK_SPR       0x83    /* SPCON */
#define MSK_SPEN      0x40
#define MSK_SSDIS     0x20
#define MSK_MSTR      0x10
#define MSK_MODE      0x0C
#define MSK_CPOL      0x08
#define MSK_CPHA      0x04

#define MSK_SPIF      0x80    /* SPSTA */
#define MSK_WCOL      0x40
#define MSK_MODF      0x10


/* TWI CONTROLLER */

#define MSK_SSCR      0x83    /* SSCON */
#define MSK_SSPE      0x40
#define MSK_SSSTA     0x20
#define MSK_SSSTO     0x10
#define MSK_SSSI      0x08
#define MSK_SSAA      0x04


/* KEYBOARD */

#define MSK_KINL      0xF0    /* KBCON */
#define MSK_KINM      0x0F

#define MSK_KPDE      0x80    /* KBSTA */
#define MSK_KINF      0x0F


/* ADC CONVERTER */

#define MSK_ADIDL     0x40    /* ADCON */
#define MSK_ADEN      0x20
#define MSK_ADEOC     0x10
#define MSK_ADSST     0x08
#define MSK_ADCS      0x01

#define MSK_ADATL     0x03    /* ADDL */

#define F_CONV_MAX  700     /* 700 KHz max frequency */

#define AIN0        0x01    /* Analog Inputs */
#define AIN1        0x00


/* USB */

#define MSK_TXCMPL    0x01    /* UEPSTAX */
#define MSK_RXOUTB0   0x02
#define MSK_RXOUT     0x02
#define MSK_RXSETUP   0x04
#define MSK_STALLED   0x08
#define MSK_TXRDY     0x10
#define MSK_STALLRQ   0x20
#define MSK_RXOUTB1   0x40
#define MSK_DIR       0x80

#define MSK_RXOUTB0B1 0x42    /* Ping-Pong abilities */

#define MSK_SPINT     0x01    /* USBINT */
#define MSK_SOFINT    0x08
#define MSK_EORINT    0x10
#define MSK_WUPCPU    0x20

#define MSK_SUSPCLK   0x40

//ADDED

#define MSK_ESPINT    0x01
#define MSK_EEORINT   0x10
#define MSK_EWUPCPU   0x20

#define MSK_USBE      0x80    /* USBCON */
#define MSK_CONFG     0x02
#define MSK_FADDEN    0x01
#define MSK_UPRSM     0x08
#define MSK_SDRMWUP   0x20
#define MSK_RMWUPE    0x04

#endif  /* _EXTSND1_H_ */
