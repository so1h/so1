/* ----------------------------------------------------------------------- */
/*                                 def_sf.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef DEF_SF_H
#define DEF_SF_H

#include <so1pub.h\tipos.h>                     /* byte_t, word_t, dword_t */

#define maxFichAbiertos 10

typedef struct {
  byte_t  activa ;                                            /*   1 bytes */
  byte_t  cabezaInicial ;                                     /*   1 bytes */
  byte_t  sectorInicial ;                                     /*   1 bytes */
  byte_t  cilindroInicial ;                                   /*   1 bytes */
  byte_t  tipo ;                                              /*   1 bytes */
  byte_t  cabezaFinal ;                                       /*   1 bytes */
  byte_t  sectorFinal ;                                       /*   1 bytes */
  byte_t  cilindroFinal ;                                     /*   1 bytes */
  dword_t primerSector ;                                      /*   4 bytes */
  dword_t sectores ;                                          /*   4 bytes */
} descParticion_t ;                                           /*  16 bytes */

#define maxParticiones 4  /* numero de entradas en la tabla de particiones */

#define tamMBR       512                                          /* 0x200 */

#define tamCodigoMBR 440                                          /* 0x1B8 */

typedef struct {                                     /* master boot record */
  byte_t codigo [ tamCodigoMBR ] ;                            /* 440 bytes */
  dword_t WinDiskSig ;                                        /*   4 bytes */
  word_t relleno ;                                            /*   2 bytes */
  descParticion_t descParticion [ maxParticiones ] ;          /*  64 bytes */
  byte_t signatura [2] ;                                      /*   2 bytes */
} mbr_t ;                                                     /* 512 bytes */

#if (sizeof(mbr_t) != tamMBR)
#error el tipo mbr_t debe ocupar exactamente 512 bytes (un sector)
#endif

#define signaturaMBR { 0x55, 0xAA }

#define offsetTPart \
  (tamCodigoMBR + sizeof(dword_t) + sizeof(word_t))         /* 446 = 0x1BE */

typedef struct {                          /* Bloque de Parametros del BIOS */
  word_t  bytesPorSector ;                                     /*  2 bytes */
  byte_t  sectoresPorCluster ;                                 /*  1 byte  */
  word_t  sectoresReservados ;                                 /*  2 bytes */
  byte_t  numeroDeFATs ;                                       /*  1 byte  */
  word_t  entradasDirRaiz ;                                    /*  2 bytes */
  word_t  sectores16 ;                                         /*  2 bytes */
  byte_t  tipoDeMedio ;                                        /*  1 byte  */
  word_t  sectoresPorFAT ;                                     /*  2 bytes */
  word_t  sectoresPorPista ;  /* Extended BPB adds(DOS 4.0) */ /*  2 bytes */ 
  word_t  cabezas ;                                            /*  2 bytes */
  dword_t primerSector ;                                       /*  4 bytes */
  dword_t sectores ;                                           /*  4 bytes */
} BPB_t ;                                                      /* 25 bytes */

#define tamBR        512                                          /* 0x200 */

#define tamCodigoBR  448                                          /* 0x1C0 */

typedef struct {                   /* plantilla para el sector de arranque */
  byte_t  instJMP [2] ;                                       /*   2 bytes */  
  byte_t  instNOP ;                                           /*   1 bytes */
  char    OEM [8] ;                                           /*   8 bytes */
  BPB_t   BPB ;           /* Bloque de Parametros del BIOS */ /*  27 bytes */
  byte_t  unidad ;                                            /*   1 byte  */
  byte_t  relleno ;                                           /*   1 byte  */
  byte_t  signaturaExt ;                                      /*   1 byte  */
  dword_t numeroDeSerie ;                                     /*   4 bytes */
  char    etiqueta [11] ;                                     /*  11 bytes */
  char    SF [8] ;                                            /*   8 bytes */
  byte_t  codigo [448] ;                                      /* 448 bytes */
  byte_t  signatura [2] ;                                     /*   2 bytes */
} boot_t ;                                                    /* 512 bytes */

#if (sizeof(boot_t) != tamBR)
#error el tipo boot_t debe ocupar exactamente 512 bytes (un sector)
#endif

#define signaturaBR { 0x55, 0xAA }

#define maxUL 10       /* maximo numero de unidades logicas A, B, C, D, .. */

typedef struct {                         /* unidad logica (particion, BPB) */
  int    db ;                                        /* indice en d_bloque */
  byte_t ufindx ;                            /* indice de la unidad fisica */
  byte_t indParticion ;                          /* indice de la particion */
  char letra ;                       /* letra de la unidad: A, B, C, D, .. */
  BPB_t BPB ;               /* bloque de parametros del BIOS (superbloque) */
} descUnidadLogica_t ;                        /* 1 + 1 + 1 + 25 = 28 bytes */

#define maxUF 4                       /* maximo numero de unidades fisicas */

typedef struct {                                          /* unidad fisica */
  int    db ;             /* indice del dispositivo de bloques en d_bloque */
  byte_t unidadBIOS ; /* numero de la unidad segun el BIOS: 0x00, 0x80, .. */
  byte_t contUL ;                            /* numero de unidades logicas */
  descParticion_t descParticion [maxParticiones] ;
  byte_t ulindx [maxParticiones] ;      /* indices de las unidades logicas */
} descUnidadFisica_t ;                    /* 1 + 1 + 4*16 + 4*1 = 70 bytes */

typedef struct {                                  /* entrada de directorio */
  byte_t  nombre [8] ;                                         /*  8 bytes */
  byte_t  ext [3] ;                                            /*  3 bytes */
  byte_t  atr ;                                                /*  1 byte  */
  byte_t  relleno [10] ;                                       /* 10 bytes */
  word_t  hora ;                                               /*  2 bytes */
  word_t  fecha ;                                              /*  2 bytes */
  word_t  primerCluster ;                                      /*  2 bytes */
  dword_t tam ;                                                /*  4 bytes */
} entrada_t ;                                                  /* 32 bytes */

#define bytesPorEntrada sizeof(entrada_t)                            /* 32 */

typedef struct {
  bool_t libre ;                  /* descriptor de fichero libre u ocupado */
  entrada_t entrada ;              /* entrada del fichero en el directorio */
  byte_t unidadLogica ;     /* unidad logica donde se encuentra el fichero */
  int sectorLogico ;               /* sector logico ocupado por la entrada */
  int numEntrada ;                   /* numero de la entrada en ese sector */
} descriptor_de_fichero_t ;

typedef struct {
  char      ff_attrib ;
  unsigned  ff_ftime ;
  unsigned  ff_fdate ;
  long      ff_fsize ;
  char      ff_name [13] ;
  int       unidadLogica ;
  byte_t    bindx ;    /* indice (0..15) de la entrada actual en el buffer */
  word_t    eindx ; /* indice (0.. ) de la entrada actual en el directorio */
  pointer_t buffer ;  /* puntero a un bufer local del proceso de 512 bytes */
} ffblk_t ;

#endif /* DEF_SF_H */