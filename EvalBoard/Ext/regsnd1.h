/*H**************************************************************************
* NAME:         regsnd1.h         
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      c5132-demo-df-smc-2_0_0      
* REVISION:     1.6     
*----------------------------------------------------------------------------
* PURPOSE:
* This file defines the T8xC51SND1 component:
*   - Register SFRs
*   - Bit SFRs
*****************************************************************************/

#ifndef _REGSND1_H_
#define _REGSND1_H_

/*_____ I N C L U D E S ____________________________________________________*/


/*_____ M A C R O S ________________________________________________________*/


/* C51 CORE */
#define Sfr(x, y)	  	sfr x = y
#define Sbit(x, y, z)   sbit x = y^z
#define Sfr16(x,y)	    sfr16 x = y

Sfr (A       , 0xE0);
Sfr (ACC     , 0xE0);
Sfr (B       , 0xF0);
Sfr (PSW     , 0xD0);
Sfr (SP      , 0x81);
Sfr (DPL     , 0x82);
Sfr (DPH     , 0x83);
Sfr16 (DPTR  , 0x82);

Sbit (CY     , PSW   , 7);
Sbit (AC     , PSW   , 6);
Sbit (F0     , PSW   , 5);
Sbit (RS1    , PSW   , 4);
Sbit (RS0    , PSW   , 3);
Sbit (OV     , PSW   , 2);
Sbit (F1     , PSW   , 1);
Sbit (P      , PSW   , 0);


/* SYSTEM MANAGEMENT */

Sfr (PCON    , 0x87);
Sfr (AUXR    , 0x8E);
Sfr (AUXR1   , 0xA2);
Sfr (NVERS   , 0xFB);


/* PLL & CLOCK */

Sfr (CKCON   , 0x8F);
Sfr (PLLCON  , 0xE9);
Sfr (PLLDIV0 , 0xEE);
Sfr (PLLNDIV , 0xEE);
Sfr (PLLDIV1 , 0xEF);
Sfr (PLLRDIV , 0xEF);


/* INTERRUPT */

Sfr (IEN0    , 0xA8);
Sfr (IPL0    , 0xB8);
Sfr (IPH0    , 0xB7);
Sfr (IEN1    , 0xB1);
Sfr (IPL1    , 0xB2);
Sfr (IPH1    , 0xB3);

Sbit (EA     , IEN0  , 7);
Sbit (EAUD   , IEN0  , 6);
Sbit (EMP3   , IEN0  , 5);
Sbit (ES     , IEN0  , 4);
Sbit (ET1    , IEN0  , 3);
Sbit (EX1    , IEN0  , 2);
Sbit (ET0    , IEN0  , 1);
Sbit (EX0    , IEN0  , 0);

Sbit (IPLAUD , IPL0  , 6);
Sbit (IPLMP3 , IPL0  , 5);
Sbit (IPLS   , IPL0  , 4);
Sbit (IPLT1  , IPL0  , 3);
Sbit (IPLX1  , IPL0  , 2);
Sbit (IPLT0  , IPL0  , 1);
Sbit (IPLX0  , IPL0  , 0);


/* PORTS */

Sfr (P0      , 0x80);
Sfr (P1      , 0x90);
Sfr (P2      , 0xA0);
Sfr (P3      , 0xB0);
Sfr (P4      , 0xC0);
Sfr (P5      , 0xD8);

Sbit (P0_7   , P0    , 7);
Sbit (P0_6   , P0    , 6);
Sbit (P0_5   , P0    , 5);
Sbit (P0_4   , P0    , 4);
Sbit (P0_3   , P0    , 3);
Sbit (P0_2   , P0    , 2);
Sbit (P0_1   , P0    , 1);
Sbit (P0_0   , P0    , 0);

Sbit (P1_7   , P1    , 7);
Sbit (P1_6   , P1    , 6);
Sbit (P1_5   , P1    , 5);
Sbit (P1_4   , P1    , 4);
Sbit (P1_3   , P1    , 3);
Sbit (P1_2   , P1    , 2);
Sbit (P1_1   , P1    , 1);
Sbit (P1_0   , P1    , 0);

Sbit (SDA    , P1    , 7);
Sbit (SCL    , P1    , 6);
Sbit (KIN3   , P1    , 3);
Sbit (KIN2   , P1    , 2);
Sbit (KIN1   , P1    , 1);
Sbit (KIN0   , P1    , 0);

Sbit (P2_7   , P2    , 7);
Sbit (P2_6   , P2    , 6);
Sbit (P2_5   , P2    , 5);
Sbit (P2_4   , P2    , 4);
Sbit (P2_3   , P2    , 3);
Sbit (P2_2   , P2    , 2);
Sbit (P2_1   , P2    , 1);
Sbit (P2_0   , P2    , 0);

Sbit (P3_7   , P3    , 7);
Sbit (P3_6   , P3    , 6);
Sbit (P3_5   , P3    , 5);
Sbit (P3_4   , P3    , 4);
Sbit (P3_3   , P3    , 3);
Sbit (P3_2   , P3    , 2);
Sbit (P3_1   , P3    , 1);
Sbit (P3_0   , P3    , 0);

Sbit (RD     , P3    , 7);
Sbit (WR     , P3    , 6);
Sbit (T1     , P3    , 5);
Sbit (T0     , P3    , 4);
Sbit (INT1   , P3    , 3);
Sbit (INT0   , P3    , 2);
Sbit (TXD    , P3    , 1);
Sbit (RXD    , P3    , 0);

Sbit (P4_7   , P4    , 7);
Sbit (P4_6   , P4    , 6);
Sbit (P4_5   , P4    , 5);
Sbit (P4_4   , P4    , 4);
Sbit (P4_3   , P4    , 3);
Sbit (P4_2   , P4    , 2);
Sbit (P4_1   , P4    , 1);
Sbit (P4_0   , P4    , 0);

Sbit (SS_    , P4    , 3);
Sbit (SCK    , P4    , 2);
Sbit (MOSI   , P4    , 1);
Sbit (MISO   , P4    , 0);

Sbit (P5_3   , P5    , 3);
Sbit (P5_2   , P5    , 2);
Sbit (P5_1   , P5    , 1);
Sbit (P5_0   , P5    , 0);


/* FLASH MEMORY */

Sfr (FCON    , 0xD1);


/* TIMERS */

Sfr (TCON    , 0x88);
Sfr (TMOD    , 0x89);
Sfr (TL0     , 0x8A);
Sfr (TL1     , 0x8B);
Sfr (TH0     , 0x8C);
Sfr (TH1     , 0x8D);

Sbit (TF1    , TCON  , 7);
Sbit (TR1    , TCON  , 6);
Sbit (TF0    , TCON  , 5);
Sbit (TR0    , TCON  , 4);
Sbit (IE1    , TCON  , 3);
Sbit (IT1    , TCON  , 2);
Sbit (IE0    , TCON  , 1);
Sbit (IT0    , TCON  , 0);


/* WATCHDOG */

Sfr (WDTRST  , 0xA6);
Sfr (WDTPRG  , 0xA7);


/* MP3 DECODER */

Sfr (MP3CON  , 0xAA);
Sfr (MP3STA  , 0xC8);
Sfr (MP3STA1 , 0xAF);
Sfr (MP3DAT  , 0xAC);
Sfr (MP3ANC  , 0xAD);
Sfr (MP3VOL  , 0x9E);
Sfr (MP3VOR  , 0x9F);
Sfr (MP3BAS  , 0xB4);
Sfr (MP3MED  , 0xB5);
Sfr (MP3TRE  , 0xB6);
Sfr (MP3CLK  , 0xEB);
Sfr (MP3DBG  , 0xAE); /* hidden register */

Sbit (MPANC  , MP3STA, 7);
Sbit (MPREQ  , MP3STA, 6);
Sbit (ERRLAY , MP3STA, 5);
Sbit (ERRSYN , MP3STA, 4);
Sbit (ERRCRC , MP3STA, 3);
Sbit (MPFS1  , MP3STA, 2);
Sbit (MPFS0  , MP3STA, 1);
Sbit (MPVER  , MP3STA, 0);


/* AUDIO INTERFACE */

Sfr (AUDCON0 , 0x9A);
Sfr (AUDCON1 , 0x9B);
Sfr (AUDSTA  , 0x9C);
Sfr (AUDDAT  , 0x9D);
Sfr (AUDCLK  , 0xEC);


/* USB CONTROLLER */

Sfr (USBCON  , 0xBC);
Sfr (USBADDR , 0xC6);
Sfr (USBINT  , 0xBD);
Sfr (USBIEN  , 0xBE);
Sfr (UEPNUM  , 0xC7);
Sfr (UEPCONX , 0xD4);
Sfr (UEPSTAX , 0xCE);
Sfr (UEPRST  , 0xD5);
Sfr (UEPINT  , 0xF8);
Sfr (UEPIEN  , 0xC2);
Sfr (UEPDATX , 0xCF);
Sfr (UBYCTX  , 0xE2);
Sfr (UBYCTLX , 0xE2);
Sfr (UFNUML  , 0xBA);
Sfr (UFNUMH  , 0xBB);
Sfr (USBCLK  , 0xEA);
Sfr (UDPADDH , 0xD7);
Sfr (UDPADDL , 0xD6);

Sbit (EP3INT , UEPINT, 3);
Sbit (EP2INT , UEPINT, 2);
Sbit (EP1INT , UEPINT, 1);
Sbit (EP0INT , UEPINT, 0);


/* MMC CONTROLLER */

Sfr (MMDAT   , 0xDC);
Sfr (MMCMD   , 0xDD);
Sfr (MMSTA   , 0xDE);
Sfr (MMMSK   , 0xDF);
Sfr (MMCON0  , 0xE4);
Sfr (MMCON1  , 0xE5);
Sfr (MMCON2  , 0xE6);
Sfr (MMINT   , 0xE7);
Sfr (MMCLK   , 0xED);


/* IDE CONTROLLER */

Sfr (DAT16H  , 0xF9);


/* UART */

Sfr (SCON    , 0x98);
Sfr (SBUF    , 0x99);
Sfr (SADDR   , 0xA9);
Sfr (SADEN   , 0xB9);
Sfr (BDRCON  , 0x92);
Sfr (BRL     , 0x91);

Sbit (SM0    , SCON  , 7);
Sbit (FE     , SCON  , 7);
Sbit (SM1    , SCON  , 6);
Sbit (SM2    , SCON  , 5);
Sbit (REN    , SCON  , 4);
Sbit (TB8    , SCON  , 3);
Sbit (RB8    , SCON  , 2);
Sbit (TI     , SCON  , 1);
Sbit (RI     , SCON  , 0);


/* SPI CONTROLLER */

Sfr (SPCON   , 0xC3);
Sfr (SPSTA   , 0xC4);
Sfr (SPDAT   , 0xC5);


/* I2C CONTROLLER */

Sfr (SSCON   , 0x93);
Sfr (SSSTA   , 0x94);
Sfr (SSDAT   , 0x95);
Sfr (SSADR   , 0x96);


/* KEYBOARD */

Sfr (KBCON   , 0xA3);
Sfr (KBSTA   , 0xA4);


/* ADC CONVERTER */

Sfr (ADCON   , 0xF3);
Sfr (ADDL    , 0xF4);
Sfr (ADDH    , 0xF5);
Sfr (ADCLK   , 0xF2);


#endif  /* _REGSND1_H_ */
