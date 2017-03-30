/* ----------------------------------------------------------------------- */
/*                                blockpr.c                                */
/* ----------------------------------------------------------------------- */
/*             Operaciones relacionadas con el bloqueo de procesos         */
/* ----------------------------------------------------------------------- */

#include <so1.h\ajustsp.h>
#include <so1.h\ajustes.h>
#include <so1.h\blockpr.h>

#include <so1pub.h\bios_0.h>

#include <so1.h\procesos.h>

word_t SS_Proceso ;           /* pila proceso que hace una llamada al s.o. */

word_t SP_Proceso ;                          /* o que ha sido interrumpido */

word_t SS_Tarea ;                   /* pila tarea que ha sido interrumpida */

word_t SP_Tarea ;

int nivelActivacionSO1 = 0 ;                   /* 0 => proceso, 1 => tarea */

bool_t enHalt = FALSE ;          /* se interrumpio en el hlt de SigProceso */

bool_t activarAlEpilogo1 = FALSE ;

            /* activar al epilogo (de nivel 1) el primer proceso preparado */

bool_t hayTic = FALSE ;         /* no se acaba de producir un tic de reloj */

    /* hayTic se utiliza en activarProceso para determinar mejor la rodaja */

#pragma warn +rvl

void prepararDesbloqueadosUrgentes ( void ) {
  while (c2cPFR[PUrgentes].numElem > 0)
    apilarPC2c(
      desapilarPC2c((ptrC2c_t)&c2cPFR[PUrgentes]),
      (ptrC2c_t)&c2cPFR[PPreparados]
    ) ;
//  encolarPC2c(
//    desencolarPC2c((ptrC2c_t)&c2cPFR[PUrgentes]),
//    (ptrC2c_t)&c2cPFR[PPreparados]
//  ) ;
}

  /* un proceso puede terminar durante una llamada o durante una rti       */
  /* o ser expulsado */

void far buscarNuevoProcesoActual ( void ) {                /* nivel 1 o 2 */

  establecerDS_SO1() ;           /* establecer el segmento de datos de SO1 */
                           /* esta funcion puede ser llamada por un driver */
/*
printStrWin(win_so, "\n nivelActivacionSO1 = ") ;
printDecWin(win_so, nivelActivacionSO1, 1) ;
*/
  if (nivelActivacionSO1 == 1) {         /* printStrWin(win_so, "\nbNP") ; */
    asm mov sp,SP0_SO1                                   /* reset pila SO1 */
    if (ccbAlEpilogo->num > 0) atenderCcb(ccbAlEpilogo) ;
    prepararDesbloqueadosUrgentes() ;
/*  printStrWin(win_so, "\n antes activarProceso ") ; */
    activarProceso(sigProceso()) ;
  }
  else activarAlEpilogo1 = TRUE ;

}

  /* en principio solo se bloquean procesos desde las llamadas al sistema  */
  /* pero vamos a contemplar bloqueos incluso desde una rti                */

void far bloquearProcesoActual ( rindx_t rindx ) {        /* nivel 1 (o 2) */

  establecerDS_SO1() ;           /* establecer el segmento de datos de SO1 */
                           /* esta funcion puede ser llamada por un driver */
  descProceso[indProcesoActual].trama = tramaProceso ;
  descProceso[indProcesoActual].estado = bloqueado ;
  descProceso[indProcesoActual].esperandoPor = rindx ;

  buscarNuevoProcesoActual() ;

}








