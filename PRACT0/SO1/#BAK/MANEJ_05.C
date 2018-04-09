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

#include <so1pub.h\bios_0.h>                               /* printStrBIOS */    

/* comunicacion directa, capacidad nula, por copia */ 

/* comparar con MINIX3\kernel\proc.c */

/* mirar: Operating Systems Design and Implementation, 2.6.9. Interprocess */
/* communication in MINIX 3.                                               */   

/* destinatarioFinal determina el pindx del proceso destinatario que esta  */
/* bloqueado esperando el mensaje, y lo elimina de la cola en la que este. */ 
/* si no hay destinatario final devuelve -1.                               */ 

pindx_t destinatarioFinal ( pindx_t pindx ) 
{
	descProceso_t * ptrDPPindx ;
#if (FALSE) 	
	pindx_t pindx_1 ;
	pindx_t pindx_2 ;
	word_t ts_1 = 0 ;
	word_t ts_2 = 0 ;

	if (pindx == ANY) 
	{
		if (ptrDPActual->c2cReceptores.numElem > 0)
		{
			pindx_1 = ptrDPActual->c2cReceptores.primero ;
			ts_1 = descProceso[pindx_2].timeStamp ;
		}
        if (c2cPFR[PReceptAny].numElem > 0)
		{
			pindx_2 = c2cPFR[PReceptAny].primero ;
			ts_2 = descProceso[pindx_2].timeStamp ;
        }		

		if ((ts_1 == 0) && (ts_2 == 0)) return(-1) ;
        else if (ts_1 > ts_2) 
		{
			desencolarPC2c(&ptrDPActual->c2cReceptores) ;
		    return(pindx_1) ;
		}	
		else 
		{
			desencolarPC2c(&c2cPFR[PReceptAny]) ;
		    return(pindx_2) ;
		}
	}
#endif 	
	ptrDPPindx = (descProceso_t *)&descProceso[pindx] ; 
	if ((ptrDPPindx->estado == bloqueado) &&
	    (ptrDPPindx->esperandoPor == rec_buzon) &&
		(ptrDPPindx->trama->AX == RECEIVE))                     /* receive */
	{
        if (ptrDPPindx->trama->BX == indProcesoActual) 
        {
            eliminarPC2c(pindx, &ptrDPActual->c2cReceptores) ;
            return(pindx) ;
        }
        else if (ptrDPPindx->trama->BX == (word_t)ANY) 
		{
            eliminarPC2c(pindx, &c2cPFR[PReceptAny]) ;
            return(pindx) ;
        }
		else return(-1) ;
    } 
	else return(-1) ;
}	

/* remitenteFinal determina el pindx del proceso remitente bloqueado que   */
/* enviara el mensaje, y lo elimina de la cola en la que este.             */ 
/* si no hay destinatario final devuelve -1.                               */ 

pindx_t remitenteFinal ( pindx_t pindx ) 
{
	descProceso_t * ptrDPPindx ;
	pindx_t pindx_1 ;
	pindx_t pindx_2 ;
	word_t ts_1 = 0 ;
	word_t ts_2 = 0 ;
	
	if (pindx == ANY) 
	{
		if (ptrDPActual->c2cEmisores.numElem > 0)
		{
			pindx_1 = ptrDPActual->c2cEmisores.primero ;
			ts_1 = descProceso[pindx_2].timeStamp ;
		}
        if (c2cPFR[PEmisorAny].numElem > 0)
		{
			pindx_2 = c2cPFR[PEmisorAny].primero ;
			ts_2 = descProceso[pindx_2].timeStamp ;
        }		

		if ((ts_1 == 0) && (ts_2 == 0)) return(-1) ;
        else if (ts_1 > ts_2) 
		{
			desencolarPC2c(&ptrDPActual->c2cEmisores) ;
		    return(pindx_1) ;
		}	
		else 
		{
			desencolarPC2c(&c2cPFR[PEmisorAny]) ;
		    return(pindx_2) ;
		}
	}
	ptrDPPindx = (descProceso_t *)&descProceso[pindx] ; 
	if ((ptrDPPindx->estado == bloqueado) &&
	    (ptrDPPindx->esperandoPor == rec_buzon) &&
		(ptrDPPindx->trama->AX == SEND))                           /* send */
	{	
        if (ptrDPPindx->trama->BX == indProcesoActual) 
        {
            eliminarPC2c(pindx, &ptrDPActual->c2cEmisores) ;
            return(pindx) ;
        }
        else if (ptrDPPindx->trama->BX == (word_t)ANY) 
		{
            eliminarPC2c(pindx, &c2cPFR[PEmisorAny]) ;
            return(pindx) ;
        }
		else return(-1) ;
    } 
	else return(-1) ;
}	

void transmitirMensaje ( trama_t far * tramaPOrigen, 
                         trama_t far * tramaPDestino, 
						 pindx_t       pindxBloqueado ) 
{
int i ;	
	pointer_t origen ;
	pointer_t destino ;
	origen  = MK_FP(tramaPOrigen ->DS, tramaPOrigen ->DX) ;
    destino = MK_FP(tramaPDestino->DS, tramaPDestino->DX) ;
    memcpy(destino, origen, 16) ;     
    descProceso[pindxBloqueado].trama->AX = 0x0000 ;              /* exito */
    descProceso[pindxBloqueado].estado = preparado ;
    encolarPC2c(pindxBloqueado, &c2cPFR[PPreparados]) ;  		
}

static word_t timeStamp = 0 ;           /* para ordenar los procesos (ANY) */

    /* los timestamps asignados solo pueden tomar valores entre 1 y 0xFFFF */

void ponerTimeStamp ( void ) {
	
	pindx_t indiceTS [ maxProcesos ] ;
	int i, j, aux ;
	int numTS ;
	descProceso_t * ptrDPi ; 
    
	if (timeStamp == 0xFFFF)                           /* caso excepcional */
	{                                               /* timestamps agotados */
		numTS = 0 ;           /* calculamos el numero de timestamps en uso */
		ptrDPi = (descProceso_t *)&descProceso[0] ;
		for ( i = 0 ; i < maxProcesos ; i++ )
        {			
			if ((ptrDPi->estado == bloqueado) && 
		        (ptrDPi->esperandoPor == rec_buzon)) 
				indiceTS[numTS++] = i ;
			ptrDPi++ ; 	
		}
		for ( i = 0 ; i < (numTS-1) ; i++ )  /* ordenamos segun timestamps */
		{
			timeStamp = descProceso[indiceTS[i]].timeStamp ;
     		for ( j = i + 1 ; j < numTS ; j++ ) 
		    {
				if (timeStamp > descProceso[indiceTS[j]].timeStamp) 
				{
					aux = indiceTS[j] ;                  /* intercambiamos */
					indiceTS[j] = indiceTS[i] ;
					indiceTS[i] = aux ;
				}					
		    }
		}	
		for ( i = 0 ; i < numTS ; i++ )     /* asignamos nuevos timestamps */
			descProceso[indiceTS[i]].timeStamp = i + 1 ;
		timeStamp = numTS ;             /* nuevo ultimo timestamp asignado */
	}		
	ptrDPActual->timeStamp = ++timeStamp ;	                /* caso normal */
}

void so1_manejador_05 ( void ) {                       /* ah = 5 ; int SO1 */

    word_t operacion = tramaProceso->AX ;
	pindx_t i ;
    pindx_t pindx = tramaProceso->BX ;
    pindx_t pindxRes ;
	descProceso_t * ptrDPPindx ; 

	if (!((0 <= pindx) && (pindx < maxProcesos)))
    {
	    if ((pindx != ANY) || (operacion != RECEYVE)) 
	    {
            tramaProceso->AX = -1 ;                               /* error */
            return ;
        }
    }
	
    switch (tramaProceso->AX) {
	  
    case SENDREC :                                               /* 0x0502 */
                                                                /* sendrec */ /* sendrec(pindx, &msg) */
//                /* indicar algo para hacer en send + receive indivisible */														
        break ;                                              

    case SEND :                                                  /* 0x0500 */
                                                                   /* send */ /* send(pindx, &msg) */
		if ((pindxRes = destinatarioFinal(pindx)) != -1) 														   
		{
			transmitirMensaje(
		        tramaProceso, descProceso[pindxRes].trama, pindxRes
			) ;  
            if (tramaProceso->AX == SEND) 
			{ 
		        tramaProceso->AX = 0x0000 ;                       /* exito */
				break ;	
			}	
	    }
        else 
		{
//           /* faltaria comprobar que no va a llegarse a un interbloqueo. */
			ponerTimeStamp() ;  /* para controlar el orden de los bloqueos */
			if (pindx != ANY) 
				encolarPC2c(indProcesoActual, &descProceso[pindx].c2cEmisores) ;
			else
				encolarPC2c(indProcesoActual, &c2cPFR[PEmisorAny]) ;
			bloquearProcesoActual(rec_buzon) ;             /* (no retorna) */		
		}

    case RECEIVE :                                               /* 0x0501 */ /* receive(pindx, &msg) */
                                                                /* receive */ 
		if (tramaProceso->AX == RECEIVE) /**/ ;
		
		if ((pindxRes = remitenteFinal(pindx)) != -1) 		
        {			
            transmitirMensaje(
		        descProceso[pindxRes].trama, tramaProceso, pindxRes
			) ;  
			tramaProceso->AX = 0x0000 ;                           /* exito */ 
			break ;
		}
        else 
		{
//           /* faltaria comprobar que no va a llegarse a un interbloqueo. */
			ponerTimeStamp() ;  /* para controlar el orden de los bloqueos */
			if (pindx != ANY) 
				encolarPC2c(indProcesoActual, &descProceso[pindx].c2cReceptores) ;
			else
				encolarPC2c(indProcesoActual, &c2cPFR[PReceptAny]) ;
			bloquearProcesoActual(rec_buzon) ;		       /* (no retorna) */
		}
		
    case NOTIFY :                                                /* 0x0503 */ /* notify(pindx) */
                                                                 /* notify */
	    if (pindx == ANY) tramaProceso->AX = -1 ;                 /* error */
		else if ((pindxRes = destinatarioFinal(pindx)) != -1) 														   
		{
            word_t valorDS = tramaProceso->DS ;                /* salvamos */ 
         	tramaProceso->DS = DS_SO1 ;         /* mensaje de notificacion */
			tramaProceso->DX = FP_OFF(&indProcesoActual) ;
			transmitirMensaje(
		        tramaProceso, descProceso[pindxRes].trama, pindxRes
			) ;  
			tramaProceso->DS = valorDS ;                    /* restauramos */
	        tramaProceso->AX = 0x0000 ;                           /* exito */
	    }
        else         /* dejar constancia de la notificacion sin bloquearse */
		{
			
		}
        break ;
		
    case ECHO :                                                  /* 0x0504 */ /* echo(pindx, &msg) */
                                                                   /* echo */
																   
        break ;

    default: ;

    }
}