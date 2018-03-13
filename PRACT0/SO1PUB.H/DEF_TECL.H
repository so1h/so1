/* ----------------------------------------------------------------------- */
/*                                def_tecl.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef DEF_TECL_H
#define DEF_TECL_H

#include <so1pub.h\tipos.h>
#include <so1pub.h\ptrc2c.h>
#include <so1pub.h\def_scan.h>
#include <so1pub.h\const.h>                                 /* maxProcesos */
#include <so1pub.h\biosdata.h>

typedef union {
  struct {
    byte_t      scanCode ;                                         /* byte */
    KBDFLAGS_t  kbdFlags ;                                         /* word */
    KBDSTATUS_t kbdStatus ;                                        /* word */
  } key ;
  struct {
    word_t wl ;
    word_t wh ;
  } dws ;
  dword_t dw ;
} pulsacion_t ;

extern pulsacion_t pulsacion ;

#define tamBTeclado 256                   /* tamanio del bufer del teclado */

#define tamBRaw 16                        /* tamanio del bufer del teclado */

typedef struct {
  dobleEnlace_t e2Bloqueados [ maxProcesos + 2 ] ;
  c2c_t  bloqueados ;              /* cola de procesos esperando una tecla */
  char   bufer [tamBTeclado] ;                        /* bufer del teclado */
  int    in ;
  int    out ;
  int    ncar ;                       /* numero de caracteres sin procesar */
  c2c_t  bloqueadosRaw ;           /* cola de procesos esperando una tecla */
  byte_t buferRaw [ tamBRaw ] ;
  int    inRaw ;
  int    outRaw ;
  int    nRaw ;
} teclado_t ;

#endif /* DEF_TECL_H */