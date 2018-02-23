/* ----------------------------------------------------------------------- */
/*                                def_proc.h                               */
/* ----------------------------------------------------------------------- */
/*                           Gestion de procesos                           */
/* ----------------------------------------------------------------------- */

#ifndef DEF_PROC_H
#define DEF_PROC_H

#include <so1pub.h\tipos.h>     /* word_t, dword_t, bool_t, pid_t, pindx_t */ 
//                                  /* isr_t, dfs_t, rindx_t, uid_t, gid_t */
#include <so1pub.h\const.h>             /* maxProcesos, dfMax, maxRecursos */
#include <so1pub.h\fcntl.h>                                    /* modoAp_t */
#include <so1pub.h\def_trm.h>                                   /* trama_t */
#include <so1pub.h\scanner.h>                                /* tamComando */
#include <so1pub.h\c2c.h>                                         /* c2c_t */
#include <so1pub.h\ccb.h>                                         /* ccb_t */

#define STDIN  0                                       /* entrada estandar */
#define STDOUT 1                                       /* salida estandar  */
#define STDERR 2                                       /* error estandar   */

#define SEEK_SET   0           /* lseek posiciona desde el principio       */
#define SEEK_CUR   1           /* lseek posiciona desde la posicion actual */
#define SEEK_END   2           /* lseek posiciona desde el final           */


typedef enum {
  libre, preparado, ejecutandose, bloqueado       /* estados de un proceso */
} estado_t ;

typedef struct {
  modoAp_t modoAp ;                                    /* modo de apertura */
  dfs_t    dfs ;                      /* descriptor de fichero del sistema */
  dword_t  pos ;                                               /* posicion */
} dfa_t ;                  /* descriptor de fichero abierto por un proceso */

#define camposPublicos        /* campos publicos del descriptor de proceso */\
                                                         /*------CPU------ */\
  pid_t        pid ;                                                         \
  estado_t     estado ;                                                      \
  rindx_t      esperandoPor ;      /* recurso por el que se esta esperando */\
  trama_t      far * trama ;                              /* trama = SS:SP */\
  bool_t       noStatus ;              /* TRUE => puede morir directamente */\
  int          status ;        /* estado de terminacion si se queda zombie */\
  pindx_t      ppindx ;                         /* pindx del proceso padre */\
  pindx_t      hpindx ;     /* pindx del hijo por el que se esta esperando */\
  c2c_t        c2cHijos ;                        /* cola de procesos hijos */\
                                                         /*----Memoria---- */\
  word_t       CSProc ;     /* segmento de codigo = inicio de la particion */\
  word_t       tamCodigo ;                   /* tamaño del codigo en bytes */\
  word_t       desplBSS ;               /* CS:desplBSS primer byte del BSS */\
  word_t       desplPila ;            /* Pila: DS:desplPila a DS:SPInicial */\
  word_t       tam ;              /* tamanio de la particion en paragrafos */\
  dword_t      tamFichero ;      /* tamanio del fichero ejecutable cargado */\
  char         programa[12] ;     /* nombre del fichero ejecutable cargado */\
  char         comando[tamComando] ;                            /* comando */\
                                                         /*------E/S------ */\
  word_t       nfa ;         /* numero de ficheros abiertos por el proceso */\
  dfa_t        tfa [dfMax] ;   /* tabla de ficheros abiertos (del proceso) */\
                                                                             \
  uid_t        uid ;                                                         \
  gid_t        gid ;                                                         \
                                                                             \
  /* dword_t      tCPU ; */   /* tiempo de CPU consumido (en tics*(2**16)) */

typedef struct {
  camposPublicos
} descProceso_t ;                                 /* descriptor de proceso */

typedef enum {
  flibre, fRegular, fedBloques, fedCaracteres, tuberia
} tipoFichero_t ;

typedef struct {
  tipoFichero_t  tipo ;
  char           nombre[9] ;
  rindx_t        rindx ;
  int            menor ;                                   /* numero menor */
  word_t         shareMode ;
  int            contAp_L ;                    /* contador de aperturas L  */
  int            contAp_E ;                    /* contador de aperturas E  */
} descFichero_t ;

typedef enum {
  rLibre,
  rDCaracteres, rDBloques,
  rTuberia,
  rGP,
  rGM,
  rDB,
  rSF,
  rOtro
} tipoRecurso_t ;

typedef int      far ( far * open_t      ) ( int dfs, modoAp_t modo ) ;
typedef int      far ( far * release_t   ) ( int dfs ) ;
typedef int      far ( far * read_t      ) ( int dfs, pointer_t dir, word_t nbytes ) ;
typedef int      far ( far * aio_read_t  ) ( int dfs, pointer_t dir, word_t nbytes ) ;
typedef int      far ( far * write_t     ) ( int dfs, pointer_t dir, word_t nbytes ) ;
typedef int      far ( far * aio_write_t ) ( int dfs, pointer_t dir, word_t nbytes ) ;
typedef long int far ( far * lseek_t     ) ( int dfs, long int pos, word_t whence ) ;
typedef int      far ( far * fcntl_t     ) ( int dfs, word_t cmd, word_t arg ) ;
typedef int      far ( far * ioctl_t     ) ( int dfs, word_t request, word_t arg ) ;

typedef int      far ( far * eliminar_t ) ( pindx_t pindx ) ;

typedef struct {

  char          nombre[12] ;                         /* nombre del recurso */
  tipoRecurso_t tipo ;
  ccb_t         ccb ;                    /* cola de funciones de call back */
  pindx_t       pindx ;                /* identificador del proceso driver */
  c2c_t         c2cFichRec ;    /* cola de ficheros que dan paso al driver */
  byte_t        numVI ;             /* numero de vectores de int. 0, 1 o 2 */
  byte_t        nVInt[2] ;           /* tabla de numeros de vector de int. */
  byte_t        irq[2] ;  /* tabla irqs. 0 <= irq[i] < 16 => int. hardware */
  isr_t         isr[2] ;  /* tabla de rutinas de servicio de interrupcion  */

  open_t        open ;
  release_t     release ;
  read_t        read ;
  aio_read_t    aio_read ;
  write_t       write ;
  aio_write_t   aio_write ;
  lseek_t       lseek ;
  fcntl_t       fcntl ;
  ioctl_t       ioctl ;

  eliminar_t    eliminar ;

} descRecurso_t ;

/* recursos (simples) por los que se espera bloqueado */

#define rec_hijo            (rindx_t)(- 1)       /* terminacion de un hijo */
#define rec_zombie          (rindx_t)(- 2)          /* para proceso zombie */
#define rec_desinstalacion  (rindx_t)(- 3)          /* para proceso driver */ 

typedef struct {                                    /* informacion variada */
  /* word_t ticsPorRodaja ; */
  word_t SP0 ;
  word_t SP0_So1 ;
  word_t IMR ;
  word_t modoSO1 ;                         /* modoSO1_(Cmd,Bin,Com,Exe,Bs) */
  word_t far * ptrDebugWord ;     /* puntero a debugWord (so1.h\dbgword.h) */
} info_t ;

typedef struct {                        /* ver startUserCode en AJUSTUSR.C */
  byte_t magicbyte[3] ;            /* deben ser los bytes 0x8C 0xC8 y 0xBB */
  word_t segDatosSR ;  /* segmento DS (sin reubicar) del proceso (fichero) */
  byte_t magicbyte5 ;                             /* debe ser el byte 0xB9 */
  word_t finData ;   /* fin de los datos inicializados del proceso (en DS) */
  byte_t magicbyte8 ;                             /* debe ser el byte 0xBA */
  word_t finBSS ; /* fin de los datos no inicializados del proceso (en DS) */
  byte_t magicbyte11 ;                            /* debe ser el byte 0xBA */
  word_t startData ;      /* comienzo de los datos en el segmento de datos */
  byte_t relleno1[12] ;
  word_t SP0 ;
  byte_t relleno2[5] ;                            /* debe ser el byte 0xC3 */
  word_t desplMain ;
  byte_t relleno3[2] ;                    /* deben ser los bytes 0x53 0xBA */
  word_t desplFinish ;               /* funcion de terminacion por defecto */
  byte_t relleno4[2] ;                         /* deben ser los bytes 0xBA */  
  word_t desplDSig ;                          /* direccion de __sighandler */
} cabecera_t ;                           /* cabecera de fichero ejecutable */

typedef struct {
  union {
    dobleEnlace_t Libres [ maxProcesos + 1 ] ;                     /* Pila */
    dobleEnlace_t Ocupados [maxProcesos + 2 ] ;         /* Pila + Eliminar */
  } e2DescProceso ;
  union {
    dobleEnlace_t Libres [ dfsMax + 1 ] ;                             /* P */
    dobleEnlace_t Ocupados [ dfsMax + 2 ] ;                       /* P + E */
  } e2DescFichero ;
  union {
    dobleEnlace_t Libres [ maxRecursos + 1 ] ;                        /* P */
    dobleEnlace_t Ocupados [ maxRecursos + 2 ] ;                  /* P + E */
  } e2DescRecurso ;
  dobleEnlace_t e2Hijos [ maxProcesos + maxProcesos ] ;    /* P+E de hijos */
  dobleEnlace_t e2Preparados [ maxProcesos + 1 ] ;   /* Cola de preparados */  /* unir en una sola cola */
  dobleEnlace_t e2Urgentes [ maxProcesos + 1 ] ; /* Desbloqueados urgentes */  /* ver prepararDesbloqueadosUrgentes */
  dobleEnlace_t e2POrdenados [ maxProcesos + 1 ] ;          /* Particiones */
  dobleEnlace_t e2PDormidos [ maxProcesos + 1 ] ;              /* Dormidos */
  dobleEnlace_t e2FichRec [ dfsMax + maxRecursos ] ;      /* Fich. de Rec. */
} e2PFR_t ;

typedef enum {
  DPLibres, DPOcupados, PPreparados, PUrgentes, POrdenados, PDormidos,
  DFLibres, DFOcupados,
  DRLibres, DROcupados,
  numColasPFR
} cPFR_t ;

typedef pindx_t far ( far * sigProceso_t ) ( ) ;

typedef void far ( far * activarProceso_t ) ( pindx_t pindx ) ;

typedef void far ( far * buscarNuevoProcesoActual_t ) ( void ) ;

typedef void far ( far * bloquearProcesoActual_t ) ( rindx_t rindx ) ;

typedef struct {
  pindx_t              far * ptrIndProcesoActual ;
  trama_t far *        far * ptrTramaProceso ;
  bool_t               far * ptrActivarAlEpilogo ;
  descProceso_t        far * ptrDescProceso ;         /* tabla de procesos */
  word_t                     tamDescProceso ;
  descFichero_t        far * ptrDescFichero ;         /* tabla de ficheros */
  descRecurso_t        far * ptrDescRecurso ;         /* tabla de recursos */
  c2c_t                far * ptrC2cPFR ;
  int                  far * ptrNivelActivacionSO1 ;
  bool_t               far * ptrEnHalt ;
  bool_t               far * ptrHayTic ;
  ccb_t                far * ptrCcbAlEpilogo ;
  word_t               far * ptrSS_Proceso ;
  word_t               far * ptrSP_Proceso ;
  word_t                     SP0_So1 ;        /* SP0 del sistema operativo */
  dword_t              far * ptrContRodajas ;
  int                  far * ptrContTicsRodaja ;
  rti_t                far * ptrVIOrg ;

  sigProceso_t               sigProceso ;
  activarProceso_t           activarProceso ;
  buscarNuevoProcesoActual_t buscarNuevoProcesoActual ;
  bloquearProcesoActual_t    bloquearProcesoActual ;         /* no retorna */
} descSO1_t ;

#endif /* DEF_PROC_H */

