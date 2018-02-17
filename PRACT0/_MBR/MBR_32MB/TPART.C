/* ----------------------------------------------------------------------- */
/*                                  tpart.c                                */
/* ----------------------------------------------------------------------- */
/*   Tabla de particiones conteniendo una particion activa de tipo FAT16   */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>              /* byte_t, word_t, dword_t, FALSE */
#include <_mbr\mbr_32mb\mbr.h>                          /* descParticion_t */

#define descParticion startBin                     /* truco para el linker */

#if (FALSE)                 /* repeticion de la declaracion hecha en mbr.h */
typedef struct                                          /* descParticion_t */
{
    byte_t  activa ;                                          /*   1 bytes */
    byte_t  cabezaInicial ;                                   /*   1 bytes */
    byte_t  sectorInicial ;                                   /*   1 bytes */
    byte_t  cilindroInicial ;                                 /*   1 bytes */
    byte_t  tipo ;                                            /*   1 bytes */
    byte_t  cabezaFinal ;                                     /*   1 bytes */
    byte_t  sectorFinal ;                                     /*   1 bytes */
    byte_t  cilindroFinal ;                                   /*   1 bytes */
    dword_t primerSector ;                                    /*   4 bytes */
    dword_t sectores ;                                        /*   4 bytes */
} descParticion_t ;                                    /*  total: 16 bytes */
#endif

/*    0x80 => Activa, 0x00 no activa */            /* tabla de particiones */
/*      |   H, S, C iniciales de la particion         ==================== */
/*      |   |  |  |  0x04 tipo de la particion (FAT16), 0x00 p. vacia      */
/*      |   |  |  |  |      H,  S,  C finales de la particion              */
/*      |   |  |  |  |      |   |   |  63 primer sector logico de la p.    */
/*      |   |  |  |  |      |   |   |   |   65457 num de sectores de la p. */

descParticion_t descParticion [ maxParticiones ] =
{
    { 0x80, 1, 1, 0, 0x04, 15, 63, 64, 63L, 65457L },         /*  16 bytes */
    { 0x00, 0, 0, 0, 0x00,  0,  0,  0,  0L,     0L },         /*  16 bytes */
    { 0x00, 0, 0, 0, 0x00,  0,  0,  0,  0L,     0L },         /*  16 bytes */
    { 0x00, 0, 0, 0, 0x00,  0,  0,  0,  0L,     0L }          /*  16 bytes */
} ;                                                           /*  64 bytes */

/* Particion mas grande que cabe en un disco de 32 MB con geometria:       */
/* (C, H, S, B) = (C, 16, 63, 512)                                         */
/* Primera particion: la 0                                                 */
/* Sector 0 ==> MBR                                                        */
/* Primer sector de la particion 0 ==> Primer sector de la pista 1         */
/* (C, H, S) = (0, 1, 1) primer sector logico = 0*16*63+1*63+(1-1) = 63    */
/* 32MB = 32*1024*1024 Bytes = 64*1024 sectores = 65536 sectores           */
/* Ultimo sector de la particion 0 ==> 65536-1 = 65535                     */
/* C = 65535/(16*63) = 65535/1008 = 65,                                    */
/*     resto = 65535-(65*1008) = 65535-65520 = 15                          */
/* H = 15/63 = 0                                                           */
/*     resto = 15-0*63 = 15-0 = 15                                         */
/* S = 15+1 = 16                                                           */
/* (C, H, S) = (65, 0, 16)                                                 */
/* No obstante el numero de sector del ultimo sector debe ser 63 (para     */
/* poder deducir de el el numero de sectores [63]) y el numero de cabezas  */
/* debe ser 15 para poder deducir de el el numero de cabezas [16].         */
/* Por tanto lo unico que hay que calcular es el mayor numero de cilindros */
/* C de tal manera que C*16*63 + 15*63 + 63 <= 65536.                      */ 
/*                                                                         */
/* C <= (65536 - 15*63 - 63)/(16*63) = (65536 - 16*63)/(16*63) =           */
/*    = (65536/(16*63))-1 = 65536/1008 - 1 = 65 - 1 = 64                   */
/*                                                                         */
/* En conclusion: (C, H, S) = (64, 15, 63)                                 */
/*                                                                         */
/* Numero de sectores en la particion = (0, 1, 1) --- (64, 15, 63)         */
/*                                                                         */
/*  (64*16*63 + 15*63 + (63-1)) - (0*16*63 + 1*63 + (1-1)) + 1 =           */
/*  (64512 + 945 + 62) - (0 + 63 + 0) + 1 = 65519 - 63 + 1 = 65457         */
/*                                                           =====         */
