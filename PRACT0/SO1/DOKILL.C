/* ----------------------------------------------------------------------- */
/*                                 dokill.c                                */
/* ----------------------------------------------------------------------- */
/*                       codigo de la llamada exit                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                     /* pindx_t */
#include <so1pub.h\ptrc2c.h>                     /* (eliminar/encolar)PC2c */
#include <so1.h\procesos.h>             /* matarProcIndx, terminarProcIndx */
#include <so1.h\blockpr.h>                     /* buscarNuevoProcesoActual */

int doKill ( pindx_t pindx ) {              /* el proceso pindx termina    */
                                            /* con exit o debido a killpid */
    static pindx_t ppindx ;
    static int err = 0 ;

    ppindx = descProceso[pindx].ppindx ;
    if (descProceso[pindx].noStatus) {
        eliminarPC2c(pindx, (ptrC2c_t)&descProceso[ppindx].c2cHijos) ;
        err = matarProcIndx(pindx) ;                     
    }
    else if ((descProceso[ppindx].estado == bloqueado) &&
             (descProceso[ppindx].esperandoPor == rec_hijo) &&
             ((descProceso[ppindx].hpindx == pindx) ||
              (descProceso[ppindx].hpindx == -1))) {
        descProceso[ppindx].trama->AX = tramaProceso->BX ;       /* status */
        descProceso[ppindx].trama->BX = descProceso[pindx].pid ; /* pidDifunto */
        descProceso[ppindx].estado = preparado ;
        encolarPC2c(ppindx, (ptrC2c_t)&c2cPFR[PUrgentes]) ;
        eliminarPC2c(pindx, (ptrC2c_t)&descProceso[ppindx].c2cHijos) ;
        err = matarProcIndx(pindx) ;            
    }
    else if (tramaProceso->AL == 0x04)                             /* exit */                            
	{                                                      /* queda zombie */
        descProceso[pindx].estado = bloqueado ;
        descProceso[pindx].esperandoPor = rec_zombie ;
        descProceso[pindx].status = tramaProceso->BX ;
        err = terminarProcIndx(pindx) ;                  
    }
	else /* tramaProceso->AL == 0x0c */                         /* killpid */
	{
        eliminarPC2c(pindx, (ptrC2c_t)&descProceso[ppindx].c2cHijos) ;
        err = matarProcIndx(pindx) ;                     
	}

    if (pindx == indProcesoActual)
        buscarNuevoProcesoActual() ;

    return(err) ;
}