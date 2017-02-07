/* ----------------------------------------------------------------------- */
/*                                 def_sf.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef DEF_SF_H
#define DEF_SF_H

#include <so1pub.h\tipos.h>

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
  (tamCodigoMBR + sizeof(signatura) + sizeof(word_t))       /* 446 = 0x1BE */

typedef struct {                          /* Bloque de Parametros del BIOS */
  word_t  bytesPorSector ;
  byte_t  sectoresPorCluster ;
  word_t  sectoresReservados ;
  byte_t  numeroDeFATs ;
  word_t  entradasDirRaiz ;
  word_t  sectores16 ;
  byte_t  tipoDeMedio ;
  word_t  sectoresPorFAT ;
  word_t  sectoresPorPista ;            /* Extended BPB additions(DOS 4.0) */
  word_t  cabezas ;
  dword_t primerSector ;
  dword_t sectores ;
} BPB_t ;

#define tamBR        512                                          /* 0x200 */

#define tamCodigoBR  448                                          /* 0x1C0 */

typedef struct {                   /* plantilla para el sector de arranque */
  byte_t  instJMP [2] ;
  byte_t  instNOP ;
  char    OEM [8] ;
  BPB_t   BPB ;                           /* Bloque de Parametros del BIOS */
  byte_t  unidad ;
  byte_t  relleno ;
  byte_t  signaturaExt ;
  dword_t numeroDeSerie ;
  char    etiqueta [11] ;
  char    SF [8] ;
  byte_t  codigo [448] ;
  byte_t  signatura [2] ;
} boot_t ;

#if (sizeof(boot_t) != tamBR)
#error el tipo boot_t debe ocupar exactamente 512 bytes (un sector)
#endif

#define signaturaBR { 0x55, 0xAA }

#define maxUL 10                      /* maximo numero de unidades fisicas */

typedef struct {                                    /* unidad logica (BPB) */
  int ufindx ;
  int indParticion ;
  char letra ;
  BPB_t BPB ;
} descUnidadLogica_t ;

#define maxUF 4                       /* maximo numero de unidades logicas */

typedef struct {                                          /* unidad fisica */
  int unidadBIOS ;
  byte_t contUL ;
  descParticion_t descParticion [maxParticiones] ;
  int ulindx [maxParticiones] ;
} descUnidadFisica_t ;

typedef struct {                                  /* entrada de directorio */
  byte_t  nombre [8] ;
  byte_t  ext [3] ;
  byte_t  atr ;
  byte_t  relleno [10] ;
  word_t  hora ;
  word_t  fecha ;
  word_t  primerCluster ;
  dword_t tam ;
} entrada_t ;

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


