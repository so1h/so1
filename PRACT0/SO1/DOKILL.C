/* ----------------------------------------------------------------------- */
/*                                 dokill.c                                */
/* ----------------------------------------------------------------------- */
/*                       codigo de la llamada exit                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>                            /* EXIT, KILLPID */
#include <so1pub.h\tipos.h>                                     /* pindx_t */
#include <so1pub.h\ptrc2c.h>                     /* (eliminar/encolar)PC2c */
#include <so1.h\procesos.h>             /* matarProcIndx, terminarProcIndx */
#include <so1.h\blockpr.h>                     /* buscarNuevoProcesoActual */

int doKill ( pindx_t pindx )                /* el proceso pindx termina    */
{                                           /* con exit o debido a killpid */
    int err = 0 ;
    descProceso_t * ptrDPPindx = (descProceso_t *)&descProceso[pindx] ; 
    pindx_t ppindx = ptrDPPindx->ppindx ;
    descProceso_t * ptrDPPadre = (descProceso_t *)&descProceso[ppindx] ;
    if (ptrDPPindx->noStatus) {
        eliminarPC2c(pindx, (ptrC2c_t)&ptrDPPindx->c2cHijos) ;
        err = matarProcIndx(pindx) ;                     
    }
    else if ((ptrDPPadre->estado == bloqueado) &&
             (ptrDPPadre->esperandoPor == rec_hijo) &&
             ((ptrDPPadre->hpindx == pindx) ||
              (ptrDPPadre->hpindx == -1))) {
        ptrDPPadre->trama->AX = tramaProceso->BX ;               /* status */
        ptrDPPadre->trama->BX = ptrDPPindx->pid ;            /* pidDifunto */
        ptrDPPadre->estado = preparado ;
        encolarPC2c(ppindx, (ptrC2c_t)&c2cPFR[PUrgentes]) ;
        eliminarPC2c(pindx, (ptrC2c_t)&ptrDPPadre->c2cHijos) ;
        err = matarProcIndx(pindx) ;            
    }
    else if (tramaProceso->AX == EXIT)                             /* exit */                            
	{                                                      /* queda zombie */
        ptrDPPindx->estado = bloqueado ;
        ptrDPPindx->esperandoPor = rec_zombie ;
        ptrDPPindx->status = tramaProceso->BX ;
        err = terminarProcIndx(pindx) ;                  
    }
	else /* tramaProceso->AX == KILLPID */                      /* killpid */
	{
        eliminarPC2c(pindx, (ptrC2c_t)&ptrDPPadre->c2cHijos) ;
        err = matarProcIndx(pindx) ;                     
	}

    if (pindx == indProcesoActual)
        buscarNuevoProcesoActual() ;

    return(err) ;
}