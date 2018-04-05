/* ----------------------------------------------------------------------- */
/*                               manej_05.c                                */
/* ----------------------------------------------------------------------- */
/*          manejador de las llamadas al sistema del grupo AH = 05         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                     /* pindx_t */
#include <so1pub.h\ptrc2c.h>                /* encolarPC2c, desencolarPC2c */
#include <so1pub.h\memory.h>                                     /* memcpy */
#include <so1pub.h\ll_s_msj.h>                                      /* ANY */
#include <so1.h\procesos.h>                             /* descProceso, .. */
#include <so1.h\blockpr.h>                        /* bloquearProcesoActual */


/* comunicacion directa, capacidad nula, por copia */ 

/* comparar con MINIX3\kernel\proc.c */

/* mirar: Operating Systems Design and Implementation, 2.6.9. Interprocess */
/* communication in MINIX 3.                                               */   

//#if (FALSE)
#if (TRUE)
typedef struct {
	c2c_t remitentes ;           /* esta cola se necesita para receive ANY */
    c2c_t destinatarios ;	        /* esta cola se necesita para send ANY */
} buzon_t ; 

dobleEnlace_t eBuzon [ maxProcesos + maxProcesos + maxProcesos ] ;

buzon_t buzon [ maxProcesos ] ;

void inicBuzones ( void ) 
{
	pindx_t i ;
	for ( i = 0 ; i < maxProcesos ; i++ ) 
	{
    	inicPC2c(&buzon[i].remitentes, &eBuzon, maxProcesos + i, TRUE) ;
    	inicPC2c(&buzon[i].destinatarios, &eBuzon, 2*maxProcesos + i, TRUE) ; 
	}
}
#endif

pindx_t destinatarioFinal ( pindx_t pindx ) 
{
	descProceso_t * ptrDPPindx ;
	if (pindx == ANY) 
	{
		if (buzon[indProcesoActual].destinatarios.numElem > 0)
			return(buzon[indProcesoActual].destinatarios.primero) ;
		else 
			return(-1) ;
	}
	ptrDPPindx = (descProceso_t *)&descProceso[pindx] ; 
	if ((ptrDPPindx->estado == bloqueado) &&
	    (ptrDPPindx->esperandoPor == rec_buzon) &&
		(ptrDPPindx->trama->AX == RECEIVE) &&                   /* receive */
        ((ptrDPPindx->trama->BX == indProcesoActual) || 
		 (ptrDPPindx->trama->BX == (word_t)ANY)))  		
	    return(pindx) ;
	else 
		return(-1) ;
}	

void enviarA ( pindx_t pindx ) 
{
	pointer_t origen ;
	pointer_t destino ;
	descProceso_t * ptrDPPindx ;
	
	ptrDPPindx = (descProceso_t *)&descProceso[pindx] ;
	origen = MK_FP(tramaProceso->DS, tramaProceso->DX) ;
    destino = MK_FP(ptrDPPindx->trama->DS, ptrDPPindx->trama->DX) ;
    memcpy(destino, origen, 16) ;           
    tramaProceso->AX = 0x0000 ;
	ptrDPPindx->trama->AX = 0x0000 ;
	eliminarPC2c(pindx, &buzon[indProcesoActual].destinatarios) ;
    ptrDPPindx->estado = preparado ;
    encolarPC2c(pindx, &c2cPFR[PPreparados]) ;  		
}

pindx_t remitenteFinal ( pindx_t pindx ) 
{
	descProceso_t * ptrDPPindx ;
	if (pindx == ANY) 
	{
		if (buzon[indProcesoActual].remitentes.numElem > 0)
			return(buzon[indProcesoActual].remitentes.primero) ;
		else 
			return(-1) ;
	}
	ptrDPPindx = (descProceso_t *)&descProceso[pindx] ; 
	if ((ptrDPPindx->estado == bloqueado) &&
	    (ptrDPPindx->esperandoPor == rec_buzon) &&
		(ptrDPPindx->trama->AX == SEND) &&                         /* send */
        ((ptrDPPindx->trama->BX == indProcesoActual) || 
		 (ptrDPPindx->trama->BX == (word_t)ANY)))  		
	    return(pindx) ;
	else 
		return(-1) ;
}	

void recibirDe ( pindx_t pindx ) 
{
	pointer_t origen ;
	pointer_t destino ;
	descProceso_t * ptrDPPindx ;
	
	ptrDPPindx = (descProceso_t *)&descProceso[pindx] ;
    origen = MK_FP(ptrDPPindx->trama->DS, ptrDPPindx->trama->DX) ;
	destino = MK_FP(tramaProceso->DS, tramaProceso->DX) ;
    memcpy(destino, origen, 16) ;           
    tramaProceso->AX = 0x0000 ;
	ptrDPPindx->trama->AX = 0x0000 ;
	eliminarPC2c(pindx, &buzon[indProcesoActual].remitentes) ;
    ptrDPPindx->estado = preparado ;
    encolarPC2c(pindx, &c2cPFR[PPreparados]) ;  		
}

void so1_manejador_05 ( void ) {                       /* ah = 5 ; int SO1 */

    pindx_t i ;
    pindx_t pindx = tramaProceso->DX ;
	descProceso_t * ptrDPPindx ; 

	if (!((0 <= pindx) && (pindx < maxProcesos)))
    {
	    if (pindx != ANY)
	    {
            tramaProceso->AX = -1 ;
            return ;
        }
    }
	
    switch (tramaProceso->AX) {
	  
    case SEND :                                                  /* 0x0500 */
                                                                   /* send */ /* send(pindx, &msg) */
		if ((pindx = destinatarioFinal(pindx)) != -1) 														   
  			enviarA(pindx) ;	
        else 
		{
			/* falta comprobar que no se va a llegar a un interbloqueo */
			encolarPC2c(indProcesoActual, &buzon[pindx].remitentes) ;
			bloquearProcesoActual(rec_buzon) ;			
		}
        break ;                                              

    case RECEIVE :                                               /* 0x0501 */ /* receive(pindx, &msg) */
                                                                /* receive */ 
		if ((pindx = remitenteFinal(pindx)) != -1) 														   
  			recibirDe(pindx) ;	
        else 
		{
			/* falta comprobar que no se va a llegar a un interbloqueo */
			encolarPC2c(indProcesoActual, &buzon[pindx].destinatarios) ;
			bloquearProcesoActual(rec_buzon) ;			
		}
		break ;
		
    case SENDREC :                                               /* 0x0502 */
                                                                /* sendrec */ /* sendrec(pindx, &msg) */
																
        break ;                                              

    case NOTIFY :                                                /* 0x0503 */ /* notify(pindx) */
                                                                 /* notify */
																 
        break ;

    case ECHO :                                                  /* 0x0504 */ /* echo(pindx, &msg) */
                                                                   /* echo */
																   
        break ;

    default: ;

    }
}