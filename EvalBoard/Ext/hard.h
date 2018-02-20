/*H**************************************************************************
* NAME:         hard.h
*----------------------------------------------------------------------------
* Copyright (c) 2003 Atmel.
*----------------------------------------------------------------------------
* RELEASE:      c5132-demo-nf-mmc-2_0_0      
* REVISION:     1.22     
*----------------------------------------------------------------------------
* PURPOSE:
* This file contains the low-level memory definition
*****************************************************************************/

#ifndef _HARD_H_
#define _HARD_H_

/*_____ I N C L U D E S ____________________________________________________*/

#ifndef MEM_CHIP_TYPE
  #error MEM_CHIP_TYPE must be defined in config.h
  #define MEM_CHIP_TYPE  CHIP_NONE
#endif
#ifndef MEM_CARD_TYPE
  #error MEM_CARD_TYPE must be defined in config.h
  #define MEM_CARD_TYPE  CARD_NONE
#endif

/*_____ M A C R O S ________________________________________________________*/

/*----- Memory type -----*/
#define MEM_CHIP        ((bit)0)            /* on-board memory */
#define MEM_CARD        ((bit)1)            /* memory card */

#define MEM_DFC          ((bit)0)            /* data flash card*/
#define MEM_SD          ((bit)1)             /* sd */

#define CHIP_NONE       0
#define CHIP_NF         1
#define CHIP_DF         2
#define CHIP_HDD        3

#define CARD_NONE       0
#define CARD_MMC        1
#define CARD_SD         2
#define CARD_CF         3
#define CARD_SMC        4
#define CARD_CDR        5
#define CARD_DF         6
#define CARD_SD_DFC     7

#define NF_1X_512B      0
#define NF_2X_512B      1
#define NF_1X_2KB       2
#define NF_2X_2KB       3
#define NF_1X           4

#ifdef NotUsed

#if (MEM_CARD_TYPE == CARD_NONE) && (MEM_CHIP_TYPE == CHIP_NONE)
  #error No card and chip type defined in config.h
#endif

#if MEM_CHIP_TYPE == CHIP_NF
  #ifndef NF_CONFIG
    #error NF_CONFIG must be defined in config.h
  #endif
  #if NF_CONFIG == NF_1X_512B
    #include "lib_mem\nf_1x_512b\nf.h"      /* single NF/512B definition */
  #elif NF_CONFIG == NF_2X_512B
    #include "lib_mem\nf_2x_512b\nf.h"      /* double NF/512B definition */
  #elif NF_CONFIG == NF_1X_2KB
    #include "lib_mem\nf_1x_2kb\nf.h"       /* single NF/2KB definition */
  #elif NF_CONFIG == NF_2X_2KB
    #include "lib_mem\nf_2x_2kb\nf.h"       /* Double NF/2KB definition */
  #elif NF_CONFIG == NF_1X
    #include "lib_mem\nf_1x\nf.h"           /* Single NF/512B or 2KB definition */
  #else
    #error NF_CONFIG baddly defined in config.h
  #endif
  #define Chip_init()                       nf_init()
  #define Chip_install()                    nf_read_spare_byte()                  
  #define Chip_uninstall()                  nf_uninstall()
  #define Chip_read_byte()                  nf_read_byte()
  #define Chip_read_open(pos)               nf_read_open(pos)
  #define Chip_read_close()                 nf_read_close()
  #define Chip_write_byte(b)                nf_write_byte(b)
  #define Chip_write_open(pos)              nf_write_open(pos)
  #define Chip_write_close()                nf_write_close()
  #define Chip_format()                     nf_format()
#elif MEM_CHIP_TYPE == CHIP_DF
  #include "lib_mem\df\df.h"                /* dataflash definition */
  #define Chip_init()                       df_init()
  #define Chip_install()                    df_init_mem()
  #define Chip_uninstall()                  df_uninstall()
  #define Chip_read_byte()                  df_read_byte()
  #define Chip_read_open(pos)               df_read_open(pos)
  #define Chip_read_close()                 df_read_close()
  #define Chip_write_byte(b)                df_write_byte(b)
  #define Chip_write_open(pos)              df_write_open(pos)
  #define Chip_write_close()                df_write_close()
  #define Chip_format()                     df_format()
#elif MEM_CHIP_TYPE == CHIP_HDD
  #include "lib_mem\ide\hdd.h"              /* hard disk definition */
  #define Chip_init()                       OK
  #define Chip_install()                    ata_install(0)
  #define Chip_uninstall()                  hdd_uninstall()
  #define Chip_read_byte()                  hdd_read_byte()
  #define Chip_read_open(pos)               hdd_read_open(pos, 1)
  #define Chip_read_close()                 hdd_read_close()
  #define Chip_write_byte(b)                hdd_write_byte(b)
  #define Chip_write_open(pos)              hdd_write_open(pos, 1)
  #define Chip_write_close()                hdd_write_close()
  #define Chip_format()                     hdd_format()
  #define Chip_read_long_big_endian()       hdd_read_long_big_endian()
  #define Chip_read_one_sector()            hdd_read_one_sector()
  #define Chip_write_one_sector()           hdd_write_one_sector()
  #define Chip_get_capacity()               (Uint32)(hdd_mem_size)
#else 
  #define Chip_install()                    KO
  #define Chip_uninstall()
#endif

#endif//#ifdef NotUsed

  #define Chip_read_open(pos)               memory_read_open(pos)
  #define Chip_read_byte()                  memory_read_byte()
  #define Chip_read_close()                 memory_read_close()

  #define Chip_write_byte(b)                memory_write_byte(b)
  #define Chip_write_open(pos)              memory_write_open(pos)
  #define Chip_write_close()                memory_write_close()

  #define Chip_format()                     memory_format()

  #include "mmc.h"              /* multimediacard definition */
  #define Card_init()                       OK
  #define Card_install()                    mmc_install()
  #define Card_uninstall()                  mmc_uninstall()
  #define Card_read_byte()                  mmc_read_byte()
  #define Card_read_open(pos)               mmc_read_open(pos)
  #define Card_read_close()                 mmc_read_close()
  #define Card_write_byte(b)                mmc_write_byte(b)
  #define Card_write_open(pos)              mmc_write_open(pos)
  #define Card_write_close()                mmc_write_close()
  #define Card_format()                     mmc_format()
  #define Card_select_next()                mmc_change()
  #define Card_check_presence()             mmc_check_presence()
  #define Card_get_socket()                 mmc_get_selected_card()
  #define CARD_SOCKET_NB                    2

/*_____ D E F I N I T I O N ________________________________________________*/

extern  bdata bit     gl_memory;            /* selected memory */
extern        bit     gl_sd_dfc;            /* select df / mmc card*/

/*_____ D E C L A R A T I O N ______________________________________________*/

#define Hard_chip_init()                Chip_init()
#define Hard_chip_install()             Chip_install()
#define Hard_chip_uninstall()           Chip_uninstall()
#define Hard_card_init()                Card_init()
#define Hard_card_install()             Card_install()
#define Hard_card_uninstall()           Card_uninstall()

#if MEM_CARD_TYPE == CARD_NONE
  #define Hard_read_byte()              Chip_read_byte()
  #define Hard_read_open(pos)           Chip_read_open(pos)
  #define Hard_read_close()             Chip_read_close()
  #define Hard_write_byte(b)            Chip_write_byte(b)
  #define Hard_write_open(pos)          Chip_write_open(pos)
  #define Hard_write_close()            Chip_write_close()
  #define Hard_format()                 Chip_format()
  #define Hard_read_long_big_endian()   Chip_read_long_big_endian()
  #define Hard_load_sector()            Chip_read_one_sector()
  #define Hard_download_sector()        Chip_write_one_sector()
  #define Hard_get_capacity()           Chip_get_capacity()
#endif

#if MEM_CHIP_TYPE == CHIP_NONE
  #define Hard_read_byte()              Card_read_byte()
  #define Hard_read_open(pos)           Card_read_open(pos)
  #define Hard_read_close()             Card_read_close()
  #define Hard_write_byte(b)            Card_write_byte(b)
  #define Hard_write_open(pos)          Card_write_open(pos)
  #define Hard_write_close()            Card_write_close()
  #define Hard_format()                 Card_format()
  #define Hard_read_long_big_endian()   Card_read_long_big_endian()
  #define Hard_load_sector()            Card_read_one_sector()
  #define Hard_download_sector()        Card_write_one_sector()
  #define Hard_iso_read_byte()          Card_read_byte()
  #define Hard_iso_read_word()          Card_read_word()
  #define Hard_iso_read_open(pos)       Card_read_open(pos)
  #define Hard_iso_read_close()         Card_read_close()
  #define Hard_iso_read_toc()           Card_read_toc()
  #define Hard_iso_ready()              Card_ready()
#endif

#if ((MEM_CHIP_TYPE != CHIP_NONE) && (MEM_CARD_TYPE != CARD_NONE))
  #if ((MEM_CHIP_TYPE == CHIP_HDD) && (MEM_CARD_TYPE == CARD_CDR))
    #define Hard_read_byte()              Chip_read_byte()
    #define Hard_read_open(pos)           Chip_read_open(pos)
    #define Hard_read_close()             Chip_read_close()
    #define Hard_write_byte(b)            Chip_write_byte(b)
    #define Hard_write_open(pos)          Chip_write_open(pos)
    #define Hard_write_close()            Chip_write_close()
    #define Hard_format()                 Chip_format()
    #define Hard_read_long_big_endian()   Chip_read_long_big_endian()
    #define Hard_load_sector()            Chip_read_one_sector()
    #define Hard_download_sector()        Chip_write_one_sector()
    #define Hard_get_capacity()           Chip_get_capacity()
    #define Hard_iso_read_byte()          Card_read_byte()
    #define Hard_iso_read_open(pos)       Card_read_open(pos)
    #define Hard_iso_read_close()         Card_read_close()
    #define Hard_iso_read_toc()           Card_read_toc()
    #define Hard_iso_read_word()          Card_read_word()
    #define Hard_iso_ready()              Card_ready()
  #else
  #define Hard_read_byte()              ((gl_memory == MEM_CHIP) ? Chip_read_byte() : Card_read_byte())
  #define Hard_read_open(pos)           ((gl_memory == MEM_CHIP) ? Chip_read_open(pos) : Card_read_open(pos))
  #define Hard_read_close()             ((gl_memory == MEM_CHIP) ? Chip_read_close() : Card_read_close())
  #define Hard_write_byte(b)            ((gl_memory == MEM_CHIP) ? Chip_write_byte(b) : Card_write_byte(b))
  #define Hard_write_open(pos)          ((gl_memory == MEM_CHIP) ? Chip_write_open(pos) : Card_write_open(pos))
  #define Hard_write_close()            ((gl_memory == MEM_CHIP) ? Chip_write_close() : Card_write_close())
  #define Hard_format()                 ((gl_memory == MEM_CHIP) ? Chip_format() : Card_format())
  #define Hard_read_long_big_endian()   ((gl_memory == MEM_CHIP) ? Chip_read_long_big_endian() : Card_read_long_big_endian())
  #define Hard_load_sector()            ((gl_memory == MEM_CHIP) ? Chip_read_one_sector() : Card_read_one_sector())
  #define Hard_download_sector()        ((gl_memory == MEM_CHIP) ? Chip_write_one_sector() : Card_write_one_sector())
#endif
#endif

#endif  /* _HARD_H_ */

