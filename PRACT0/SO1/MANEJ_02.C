/* ----------------------------------------------------------------------- */
/*                               manej_02.c                                */
/* ----------------------------------------------------------------------- */
/*       manejadores de las llamadas al sistema de soporte de drivers      */
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
/* AX: 0200H ==> obtenInfoSO1                                              */
/* AX: 0201H ==> crearRecurso                                              */
/* AX: 0202H ==> crearFichero                                              */
/* AX: 0203H ==> esperarDesinstalacion                                     */
/* AX: 0204H ==> destruirRecurso                                           */
/* AX: 0205H ==> encolarCcbRecurso                                         */
/* AX: 0206H ==> eliminarCcbRecurso                                        */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\copia.h>
#include <so1pub.h\strings.h>
#include <so1pub.h\ll_s_so1.h>
#include <so1pub.h\ccb.h>
#include <so1pub.h\bios_0.h>
#include <so1.h\ajustes.h>
#include <so1.h\ajustsp.h>
#include <so1.h\interrup.h>
#include <so1.h\procesos.h>
#include <so1.h\blockpr.h>
#include <so1.h\recursos.h>
#include <so1.h\gm.h>

void cargarDescSO1 ( descSO1_t far * descSO1 ) {

  descSO1->ptrIndProcesoActual      = (pindx_t far *)&indProcesoActual ;
  descSO1->ptrTramaProceso          = (trama_t far * far *)&tramaProceso ;
  descSO1->ptrActivarAlEpilogo      = (bool_t far *)&activarAlEpilogo1 ;
  descSO1->ptrDescProceso           = (descProceso_t far *)&descProceso ;
  descSO1->tamDescProceso           = sizeof(descProcesoExt_t) ;
  descSO1->ptrDescFichero           = (descFichero_t far *)&descFichero ;
  descSO1->ptrDescRecurso           = (descRecurso_t far *)&descRecurso ;
  descSO1->ptrC2cPFR                = (c2c_t far *)&c2cPFR ;
  descSO1->ptrNivelActivacionSO1    = (int far *)&nivelActivacionSO1 ;
  descSO1->ptrEnHalt                = (bool_t far *)&enHalt ;
  descSO1->ptrHayTic                = (bool_t far *)&hayTic ;
  descSO1->ptrCcbAlEpilogo          = (ccb_t far *)&ccbAlEpilogo ;
  descSO1->ptrSS_Proceso            = (word_t far *)&SS_Proceso ;
  descSO1->ptrSP_Proceso            = (word_t far *)&SP_Proceso ;
  descSO1->SP0_So1                  = SP0_SO1 ;
  descSO1->ptrContRodajas           = (dword_t far *)&contRodajas ;
  descSO1->ptrContTicsRodaja        = (int far *)&contTicsRodaja ;
  descSO1->ptrVIOrg                 = (rti_t far *)&VIOrg ;
  descSO1->sigProceso               =
    (sigProceso_t)pointer(_CS, (word_t)sigProceso) ;
  descSO1->activarProceso           =
    (activarProceso_t)pointer(_CS, (word_t)activarProceso) ;
  descSO1->buscarNuevoProcesoActual =
    (buscarNuevoProcesoActual_t)pointer(_CS, (word_t)buscarNuevoProcesoActual) ;
  descSO1->bloquearProcesoActual    =
    (bloquearProcesoActual_t)pointer(_CS, (word_t)bloquearProcesoActual) ;
}

extern void so1_manejador_02 ( ) {                     /* ah = 8 ; int SO1 */

  switch (tramaProceso->AL) {

  case 0x00 : {                                                  /* 0x0200 */
    descSO1_t far * descSO1 ;                              /* obtenInfoSO1 */
    descSO1 = (descSO1_t far *)pointer(tramaProceso->ES, tramaProceso->BX) ;
    cargarDescSO1(descSO1) ;
    break ;
  }
  case 0x01 : {                                                  /* 0x0201 */
    descRecurso_t far * dR ;                               /* crearRecurso */
    rindx_t rindx ;
    dR = (descRecurso_t far *)pointer(tramaProceso->ES, tramaProceso->BX) ;
    rindx = crearRec(dR) ;
    tramaProceso->AX = rindx ;
    break ;
  }
  case 0x02 : {                                                  /* 0x0202 */
    char far * nombre ;                                    /* crearFichero */
    rindx_t rindx ;
    word_t menor ;
    tipoFichero_t tipo ;
    nombre = (char far *)pointer(tramaProceso->ES, tramaProceso->BX) ;
    rindx = tramaProceso->CX ;
    menor = tramaProceso->DX ;
    tipo = tramaProceso->SI ;
    tramaProceso->AX = crearFich(nombre, rindx, menor, tipo) ;
    break ;
  }                                                              /* 0x0203 */
  case 0x03 : {                                   /* esperarDesinstalacion */
    bloquearProcesoActual(rec_desinstalacion) ;
    break ;
  }                                                              /* 0x0204 */
  case 0x04 : {                                         /* destruirRecurso */
    tramaProceso->AX =
      destruirRec((char far *)pointer(tramaProceso->ES, tramaProceso->BX)) ;
    break ;
  }                                                              /* 0x0205 */
  case 0x05 : ;                                       /* encolarCcbRecurso */
  case 0x06 : {                                      /* eliminarCcbRecurso */
    callBack_t cb ;
    rindx_t rindx ;
    bool_t enc = FALSE ;
    int res ;
    rindx = c2cPFR[DROcupados].primero ;                  /* ver si existe */
    while (rindx != c2cPFR[DROcupados].cabecera) {
      if (iguales(descRecurso[rindx].nombre,
                  (char far *)pointer(tramaProceso->CX, tramaProceso->BX))) {
        enc = TRUE ;
        break ;                                          /* sale del bucle */
      }
      rindx = c2cPFR[DROcupados].e[rindx].sig ;
    }
    if (!enc) res = -1 ;                           /* el recurso no existe */
    else {
      cb = (callBack_t)pointer(tramaProceso->ES, tramaProceso->DX) ;
      switch (tramaProceso->AL) {
      case 0x05 :
        res = encolarCcb(cb, descRecurso[rindx].ccb) ;   /* 0, -1, ..., -3 */
        break ;
      case 0x06 :
        res = eliminarCcb(cb, descRecurso[rindx].ccb) ;  /* 0, -1, ..., -4 */
        break ;
      }
      if (res < 0) res-- ;
    }
    tramaProceso->AX = res ;
    break ;
  }
  default : ;
  }

}

