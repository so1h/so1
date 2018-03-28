/* ----------------------------------------------------------------------- */
/*                                 dokill.c                                */
/* ----------------------------------------------------------------------- */
/*  codigo comun de las llamadas killpid y de cerrar ventana con el raton  */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1pub.h\ptrc2c.h>
#include <so1.h\ajustes.h>
#include <so1.h\procesos.h>
#include <so1.h\blockpr.h>                     /* buscarNuevoProcesoActual */
/* #include <so1.h\textwin.h> */

int doKill ( pindx_t pindx ) {                 /* el proceso pindx termina */

  static pindx_t ppindx ;
  static int err = 0 ;

  ppindx = descProceso[pindx].ppindx ;
  if (descProceso[pindx].noStatus) {
    eliminarPC2c(pindx, (ptrC2c_t)&descProceso[ppindx].c2cHijos) ;
    matarProcIndx(pindx) ;                          /* cierra la vent. ... */
  }
  else if ((descProceso[ppindx].estado == bloqueado) &&
           (descProceso[ppindx].esperandoPor == rec_hijo) &&
           ((descProceso[ppindx].hpindx == pindx) ||
            (descProceso[ppindx].hpindx == -1))) {
    descProceso[ppindx].trama->AX = tramaProceso->BX ;           /* status */
    descProceso[ppindx].trama->BX = descProceso[pindx].pid ; /* pidDifunto */
    descProceso[ppindx].estado = preparado ;
    encolarPC2c(ppindx, (ptrC2c_t)&c2cPFR[PUrgentes]) ;
    eliminarPC2c(pindx, (ptrC2c_t)&descProceso[ppindx].c2cHijos) ;
    matarProcIndx(pindx) ;                /* matarProcIndx cierra la vent. */
/*
    if (pindx == indProcesoDeSuperficie)
      focalizarWin(descProceso[ppindx].win) ;
*/
  }
  else {                                                   /* queda zombie */
    descProceso[pindx].estado = bloqueado ;
    descProceso[pindx].esperandoPor = rec_zombie ;
    descProceso[pindx].status = tramaProceso->BX ;
    terminarProcIndx(pindx) ;                       /* cierra la vent. ... */
  }

  if (pindx == indProcesoActual)
    buscarNuevoProcesoActual() ;

  return(err) ;

}
