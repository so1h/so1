/* ----------------------------------------------------------------------- */
/*                                  mbr.h                                  */
/* ----------------------------------------------------------------------- */
/*          definiciones basicas relativas al Master Boot Record           */
/* ----------------------------------------------------------------------- */

/* El MBR ocupa los 512 bytes del sector 0 del disco. El MBR describe las  */
/* particiones (unidades logicas) en que esta dividido el disco, mediante  */
/* lo que se denomina la tabla de particiones. Ademas contiene codigo      */
/* capaz de buscar en la tabla de particiones la particion activa, leer    */
/* el sector de arranque de esa particion en la dirección 0x7c00 y ceder   */
/* el control a esa direccion.                                             */
/*
/* La tabla de particiones contiene cuatro descritores de partition con la */
/* estructura correspondiente a la declaracion de descParticion_t.         */
/* El campo activa vale 0x80 si la particion es la partition de arranque   */
/* del disco. En otro caso, activa vale 0x00. El campo tipo es un codigo   */
/* estandarizado que indica el tipo de sistema de ficheros contenido en la */
/* particion. Los demas campos del descriptor de particion indican el      */
/* primer sector, el ultimo sector y el numero de sectores que contiene la */
/* partition (se supone que el BIOS conoce la geometria del disco).        */
/*                                                                         */
/* La estructura mbr_t describe en cuales de los 512 bytes del MBR esta su */
/* codigo, la tabla de particiones y la signatura final del MBR cuyo valor */
/* debe ser 0xAA55 (bytes 0x55 y 0xAA).                                    */
/*                                                                         */
/* Algunos BIOS no necesitan el codigo del MBR ya que son capaces de       */
/* identificar la partition activa dentro de la tabla de partitiones, y de */
/* cargar y ceder el control al correspondiente sector de arranque. En ese */
/* tipo de ordenadores es usual ver que todos los bytes de codigo del MBR  */
/* están a 0x00.                                                           */
/*                                                                         */
/* Observese que cuando se ejecuta el codigo del MBR puede que no se       */
/* conozca la geometria del disco. Eso no es problema para leer el primer  */
/* sector de la particion activa, siempre y cuando esa lectura se haga     */
/* utilizando el BIOS (INT 13h) con el esquema CHS o con LBA.              */
/*                                                                         */
/* Geometria del disco: (BPS, SPP, PPC)                                    */
/*                                                                         */
/*      (BytesPorSector, SectsPorPista, PistasPorCilindro, Cilindros)      */
/*                                                                         */
/* Relaciones:                                                             */
/*                                                                         */
/*      (S1-1) + H1*SPP + C1*(SPP*PPC) = LBA                               */
/*      (S2-1) + H2*SPP + C2*(SPP*PPC) = LBA + sectores                    */
/*                                                                         */
/* Salen dos ecuaciones con dos incognitas (SPP y PPC)                     */
/*                                                                         */
/*      SPP*(H1 + PPC*C1] = LBA - S1 + 1                                   */
/*      SPP*(H2 + PPC*C2) = LBA + sectores - S2 + 1                        */
/*                                                                         */
/* Usualmente SPP = 63 (y BytesPosSector = 512).                           */

#ifndef MBR_H
#define MBR_H

#include <so1pub.h/tipos.h>              /* byte_t, word_t, dword_t, MK_FP */

/* entrada de la tabla de particiones */

typedef struct                                          /* descParticion_t */
{
    byte_t  activa ;                                          /*   1 bytes */
    byte_t  cabezaInicial ;                /* H1 */ /* CHS */ /*   1 bytes */
    byte_t  sectorInicial ;                /* S1 */           /*   1 bytes */
    byte_t  cilindroInicial ;              /* C1 */           /*   1 bytes */
    byte_t  tipo ;                                            /*   1 bytes */
    byte_t  cabezaFinal ;                  /* H2 */ /* CHS */ /*   1 bytes */
    byte_t  sectorFinal ;                  /* S2 */           /*   1 bytes */
    byte_t  cilindroFinal ;                /* C2 */           /*   1 bytes */
    dword_t primerSector ;                          /* LBA */ /*   4 bytes */
    dword_t sectores ;                                        /*   4 bytes */
} descParticion_t ;                                    /*  total: 16 bytes */

#define maxParticiones 4  /* numero de entradas en la tabla de particiones */

#define tamMBR       512                                          /* 0x200 */

#define tamCodigoMBR 440                                          /* 0x1B8 */

#define partes_del_MBR \
    byte_t codigo [ tamCodigoMBR ] ;                          /* 440 bytes */\
    dword_t WinDiskSig ;                                      /*   4 bytes */\
    word_t relleno ;                                          /*   2 bytes */\
    descParticion_t descParticion [ maxParticiones ] ;        /*  64 bytes */\
    word_t signatura ;                                        /*   2 bytes */\

typedef struct                                /* master boot record: mbr_t */
{
    partes_del_MBR
} mbr_t ;                                              /* total: 512 bytes */

#if (sizeof(mbr_t) != tamMBR)
#error el tipo mbr_t debe ocupar exactamente 512 bytes (un sector)
#endif

#define signaturaMBR 0xAA55

#define offsetTPart \
  (tamCodigoMBR + sizeof(dword_t) + sizeof(word_t))         /* 446 = 0x1BE */

#define offsetSignaturaMBR \
  (sizeof(mbt_t) - sizeof(word_t))                          /* 510 = 0x1FE */

#define dirLinealCargaMBR 0x7C00
#define segCargaMBR 0x0000                                   /* carga BIOS */
#define offCargaMBR 0x7C00                                   /* carga BIOS */
#define dirCargaMBR MK_FP(segCargaMBR, offCargaMBR)       /* 0x0000:0x7C00 */

#endif
