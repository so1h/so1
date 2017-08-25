/* ----------------------------------------------------------------------- */
/*                                blockpr.h                                */
/* ----------------------------------------------------------------------- */
/*             Operaciones relacionadas con el bloqueo de procesos         */
/* ----------------------------------------------------------------------- */

#ifndef BLOCKPR_H
#define BLOCKPR_H

#include <so1pub.h\tipos.h>              /* word_t, bool_t, FALSE, rindx_t */
#include <so1pub.h\def_proc.h>                        /* c2cPFR, PUrgentes */
#include <so1pub.h\ptrc2c.h>                                   /* ptrC2c_t */
#include <so1pub.h\ccb.h>                                    /* atenderCcb */

#include <so1.h\ajustes.h>                              /* DS_SO1, SP0_SO1 */
#include <so1.h\procesos.h>    /* activarProceso, sigProceso, ccbAlEpilogo */

extern word_t SS_Proceso ;    /* pila proceso que hace una llamada al s.o. */

extern word_t SP_Proceso ;                   /* o que ha sido interrumpido */

extern word_t SS_Tarea ;            /* pila tarea que ha sido interrumpida */

extern word_t SP_Tarea ;

extern int nivelActivacionSO1 ;                /* 0 => proceso, 1 => tarea */

extern bool_t enHalt ;           /* se interrumpio en el hlt de SigProceso */

extern bool_t activarAlEpilogo1 ;

            /* activar al epilogo (de nivel 1) el primer proceso preparado */

extern bool_t hayTic ;             /* se acaba de producir un tic de reloj */

    /* hayTic se utiliza en activarProceso para determinar mejor la rodaja */

                 /* prologo de una rti o llamada al sistema (int nVIntSO1) */

#define prologo_rti_sin_PUSHA()                                   /* macro */\
                                                                             \
  asm push es ;                                                              \
  asm push ds ;                                                              \
                                                                             \
  establecerDS_SO1() ;           /* establecer el segmento de datos de SO1 */\
                                                                             \
  if (++nivelActivacionSO1 == 1) {                                           \
    asm mov SS_Proceso,ss ;                /* salvamos la pila del proceso */\
    asm mov SP_Proceso,sp ;                                                  \
    asm mov ss,DS_SO1 ;                     /* establecemos la pila de SO1 */\
    asm mov sp,SP0_SO1 ;                                                     \
  }                                /* en otro caso la pila es ya la de SO1 */\
                                                              /* fin macro */

#define prologo_rti()                                             /* macro */\
                                                                             \
  asm pusha ;          /* apilamos todos los registros del proceso (80286) */\
                                         /* ax, cx, dx, bx, sp, bp, si, di */\
  prologo_rti_sin_PUSHA() ;                                                  \
                                                              /* fin macro */

#define epilogo_rti()                                             /* macro */\
                                                                             \
  if ((nivelActivacionSO1 == 1) || enHalt) {                                 \
    if (ccbAlEpilogo->num > 0) atenderCcb(ccbAlEpilogo) ;                    \
    if (c2cPFR[PUrgentes].numElem > 0)                                       \
      prepararDesbloqueadosUrgentes() ;                                      \
    if (activarAlEpilogo1) {                                                 \
      activarAlEpilogo1 = FALSE ;                                            \
      activarProceso(sigProceso()) ;                                         \
    }                                                                        \
  }                                                                          \
  if (nivelActivacionSO1-- == 1) {                                           \
    asm mov ss,SS_Proceso ;            /* establecemos la pila del proceso */\
    asm mov sp,SP_Proceso ;                                                  \
  }                                                                          \
  asm pop ds ;             /* restauramos el segmento de datos del proceso */\
  asm pop es ;                                                               \
                                                                             \
  asm popa ;        /* restauramos todos los registros del proceso (80286) */\
                                         /* di, si, bp, sp, bx, dx, cx, ax */\
  asm iret ;                                                                 \
                                                              /* fin macro */

void prepararDesbloqueadosUrgentes ( void ) ;                                \

  /* un proceso puede terminar durante una llamada o durante una rti       */
  /* o ser expulsado */

void far buscarNuevoProcesoActual ( void ) ;

  /* en principio solo se bloquean procesos desde las llamadas al sistema  */
  /* pero vamos a contemplar bloqueos incluso desde una rti                */

void far bloquearProcesoActual ( rindx_t rindx ) ;

  /* NOTA: estas funciones se declaran far con el fin de poder ser         */
  /* llamadas desde drivers. Cuando se utilicen en hay que incluir este    */
  /* fichero de cabeceras, porque sino el compilador considera que son     */
  /* funciones near por defecto. El linker no se enteraría del error, pero */
  /* en ejecución sucedería que no accederíamos correctamente a los        */
  /* parametros, ya que la dirección de retorno metida en la pila sería    */
  /* de tamaño palabra en vez de doble palabra.                            */

#endif /* BLOCKPR_H */



