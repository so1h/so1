/* ----------------------------------------------------------------------- */
/*                               manej_05.c                                */
/* ----------------------------------------------------------------------- */
/*          manejador de las llamadas al sistema del grupo AH = 05         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                     /* pindx_t */
#include <so1pub.h\ptrc2c.h>                /* encolarPC2c, desencolarPC2c */
#include <so1pub.h\memory.h>                                     /* memcpy */
#include <so1pub.h\strings.h>                                    /* strcpy */
#include <so1pub.h\ll_s_msj.h>                          /* ANY, tamMensaje */
#include <so1.h\procesos.h>                             /* descProceso, .. */
#include <so1.h\blockpr.h>                        /* bloquearProcesoActual */
 
#include <so1pub.h\bios_0.h>                               /* printStrBIOS */    

/* comunicacion directa, capacidad nula, por copia, con la posibilidad de  */
/* asimetria solo en la recepcion (receive(ANY, msj)).                     */
/*                                                                         */
/* Si se admitera asimetria en el envio (send(ANY, msj)) se requeriria     */
/* el uso de timestamps en las operaciones para que por ejemplo al enviar  */
/* si hay receptores normales y ANY elegir como destinatario al que tenga  */
/* menor timestamp (es decir hay ejecutado receive antes). Mantener los    */
/* timestamp puede tener un coste cuando se agote el contador y deban      */
/* reordenarse los timestamp asignados. Por suerte nos ahorramos esto.     */ 

/* comparar con MINIX3\kernel\proc.c */

/* mirar: Operating Systems Design and Implementation, 2.6.9. Interprocess */
/* communication in MINIX 3.                                               */   

/* destinatarioFinal determina el pindx del proceso destinatario que esta  */
/* bloqueado esperando el mensaje, y lo elimina de la cola en la que este. */ 
/* si no hay destinatario final devuelve -1.                               */ 

/* tramaProceso->AX == SENDREC indica SENDREC enviando (0x0502) */

#define REPLY_PENDING 0x5502      /* indica SENDREC pendiente de respuesta */

pindx_t destinatarioFinal ( pindx_t pindx ) 
{
	descProceso_t * ptrDPPindx ;
	ptrDPPindx = (descProceso_t *)&descProceso[pindx] ; 
	if ((ptrDPPindx->estado == bloqueado) &&
	    (ptrDPPindx->esperandoPor == rec_buzon) &&
		((ptrDPPindx->trama->AX == RECEIVE) ||                  /* receive */
		 (ptrDPPindx->trama->AX == REPLY_PENDING)))  /* sendrec recibiendo */
	{
        if (ptrDPPindx->trama->BX == indProcesoActual) /* del proc. actual */
        {
            eliminarPC2c(pindx, &ptrDPActual->c2cReceptores) ;
            return(pindx) ;
        }
        else if (ptrDPPindx->trama->BX == (word_t)ANY)    /* de cualquiera */
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
	
	if (pindx == ANY) 
	{
		if (ptrDPActual->c2cEmisores.numElem > 0)
			return(desencolarPC2c(&ptrDPActual->c2cEmisores)) ;
		else
		    return(-1) ;
	}
	ptrDPPindx = (descProceso_t *)&descProceso[pindx] ; 	
	if ((ptrDPPindx->estado == bloqueado) &&
	    (ptrDPPindx->esperandoPor == rec_buzon) &&
		((ptrDPPindx->trama->AX == SEND) ||                        /* send */
	     (ptrDPPindx->trama->AX == SENDREC)))                   /* sendrec */
	{	
        if (ptrDPPindx->trama->BX == indProcesoActual) 
        {
            eliminarPC2c(pindx, &ptrDPActual->c2cEmisores) ;
            return(pindx) ;
        }
		else return(-1) ;
    } 
	else return(-1) ;
}	

void transmitirMensaje ( trama_t far * tramaPOrigen, 
                         trama_t far * tramaPDestino,
                         pindx_t pindxOrg )  
{
	mensaje_0_t far * origen ;
	mensaje_0_t far * destino ;
	origen  = MK_FP(tramaPOrigen ->DS, tramaPOrigen ->DX) ;
    destino = MK_FP(tramaPDestino->DS, tramaPDestino->DX) ;
    memcpy(destino, origen, tamMensaje) ;     
	destino->pindxOrg = pindxOrg ;
}

void recogerNotificacion ( pindx_t pindx ) 
{
    mensaje_0_t far * destino ;               /* cocinamos la notificacion */
    destino = (mensaje_0_t far *)MK_FP(tramaProceso->DS, tramaProceso->DX) ;
    destino->pindxOrg = pindx ;                   /* recibimos el mensaje  */ 
    destino->tipo = 0 ;                     
	strcpy((char far *)&destino->info, "notificacion") ;
    ptrDPActual->notificaciones &= ~(1 << pindx) ;                /* hecho */
}

void so1_manejador_05 ( void ) {                       /* ah = 5 ; int SO1 */

    word_t operacion = tramaProceso->AX ;
	pindx_t i ;
    pindx_t pindx = tramaProceso->BX ;
    pindx_t pindxRes ;
	descProceso_t * ptrDPPindx ; 

	if (!((0 <= pindx) && (pindx < maxProcesos)))
    {
	    if ((pindx != ANY) || (operacion != RECEIVE)) 
	    {
            tramaProceso->AX = -1 ;                               /* error */
            return ;
        }
    }
	
//  /* por seguridad es conveniente actualizar el campo trama del proceso  */
//  /* actual, ya que en algun momento podria accederse a la trama del     */
//  /* proceso actual (sin saber que es el) con el campo trama del         */
//  /* descriptor.                                                         */	
	
	ptrDPActual->trama = tramaProceso ;     /* actualizamos el campo trama */
	
    switch (operacion) {
	  
    case SENDREC :                                               /* 0x0502 */
                                                                /* sendrec */ /* sendrec(pindx, &msg) */
//                /* indicar algo para hacer en send + receive indivisible */														

    case SEND :                                                  /* 0x0500 */
                                                                   /* send */ /* send(pindx, &msg) */
		if ((pindxRes = destinatarioFinal(pindx)) != -1) 														   
		{
			transmitirMensaje(tramaProceso, descProceso[pindxRes].trama, indProcesoActual) ;  
            ptrDPPindx = (descProceso_t *)&descProceso[pindxRes] ;
            ptrDPPindx->trama->AX = 0x0000 ;             /* exito pindxRes */
            ptrDPPindx->estado = preparado ;
//          /* valorar si queremos que se ejecute antes el proceso actual  */
//          /* o el proceso que se acaba de desbloquear.                   */			
//          encolarPC2c(pindxRes, &c2cPFR[PPreparados]) ;
            encolarPC2c(pindxRes, &c2cPFR[PUrgentes]) ; 
            if (operacion == SEND) 
			{ 
		        tramaProceso->AX = 0x0000 ;        /* exito proceso actual */
//				ptrDPActual->trama = tramaProceso ; 
//				buscarNuevoProcesoActual() ;
				break ;	
			}	
	    }
        else 
		{
//           /* faltaria comprobar que no va a llegarse a un interbloqueo. */
//			ponerTimeStamp() ;  /* para controlar el orden de los bloqueos */
			encolarPC2c(indProcesoActual, &descProceso[pindx].c2cEmisores) ;
			bloquearProcesoActual(rec_buzon) ;             /* (no retorna) */		
		}

    case RECEIVE :                                               /* 0x0501 */ /* receive(pindx, &msg) */
                                                                /* receive */ 
		if (operacion == RECEIVE)  
		{							
            dword_t notificaciones = ptrDPActual->notificaciones ;	
            if ((pindx == ANY) && (notificaciones != 0x00000000)) 
			    for ( pindx = 0 ; !(notificaciones & (1 << pindx)) ; pindx++ ) ; 
            if ((pindx != ANY) && (notificaciones & (1 << pindx))) 
    	    {                                    
		        recogerNotificacion(pindx) ;
    			tramaProceso->AX = 0x0000 ;        /* exito proceso actual */ 
	    		break ;
            }				
		    else if ((pindxRes = remitenteFinal(pindx)) != -1) 		
			{
                transmitirMensaje(descProceso[pindxRes].trama, tramaProceso, pindxRes) ;
                ptrDPPindx = (descProceso_t *)&descProceso[pindxRes] ;
			    if (ptrDPPindx->trama->AX == SEND)
		    	{
                    ptrDPPindx->trama->AX = 0x0000 ;     /* exito pindxRes */
                    ptrDPPindx->estado = preparado ;
                    encolarPC2c(pindxRes, &c2cPFR[PPreparados]) ;
//                  encolarPC2c(pindxRes, &c2cPFR[PUrgentes]) ;
			    }
			    else                   /* ptrDPPindx->trama->AX == SENDREC */
		    	{
					ptrDPPindx->trama->AX = REPLY_PENDING ;  
                    encolarPC2c(pindxRes, &ptrDPActual->c2cReceptores) ;
			    }
		    	tramaProceso->AX = 0x0000 ;        /* exito proceso actual */ 
			    break ;
			}
		}
//      /* faltaria comprobar que no va a llegarse a un interbloqueo. */
//	    ponerTimeStamp() ;      /* para controlar el orden de los bloqueos */
		if (pindx != ANY) 
		{
		    if (operacion == SENDREC) tramaProceso->AX = REPLY_PENDING ;
			encolarPC2c(indProcesoActual, &descProceso[pindx].c2cReceptores) ;
        }			
		else 
     		encolarPC2c(indProcesoActual, &c2cPFR[PReceptAny]) ;
		bloquearProcesoActual(rec_buzon) ;		           /* (no retorna) */
		
    case NOTIFY :                                                /* 0x0503 */ /* notify(pindx) */
                                                                 /* notify */
	    if (pindx == ANY) tramaProceso->AX = -1 ;                 /* error */
		else if ((descProceso[pindx].trama->AX == RECEIVE) &&  /* !SENDREC */ /* para no interferir los SENDREC */
		         ((pindxRes = destinatarioFinal(pindx)) != -1)) 														   
		{
			mensaje_0_t mensaje ;
            word_t valorDS = tramaProceso->DS ;                /* salvamos */ 
//			mensaje.pindxOrg = indProcesoActual ;
			mensaje.tipo = 0 ;                 /* mensaje de notificacion */
			strcpy((char far *)&mensaje.info, "notificacion") ;
         	tramaProceso->DS = DS_SO1 ;         
			tramaProceso->DX = FP_OFF(&mensaje) ;
			transmitirMensaje(tramaProceso, descProceso[pindxRes].trama, indProcesoActual) ;  
            ptrDPPindx = (descProceso_t *)&descProceso[pindxRes] ;
            ptrDPPindx->trama->AX = 0x0000 ;             /* exito pindxRes */
            ptrDPPindx->estado = preparado ;
//          /* valorar si queremos que se ejecute antes el proceso actual  */
//          /* o el proceso que se acaba de desbloquear.                   */			
//          encolarPC2c(pindxRes, &c2cPFR[PPreparados]) ;
            encolarPC2c(pindxRes, &c2cPFR[PUrgentes]) ; 
			tramaProceso->DS = valorDS ;                    /* restauramos */
	        tramaProceso->AX = 0x0000 ;                           /* exito */
	    }
        else         /* sin bloquearse dejar constancia de la notificacion */ 
		{
            descProceso[pindx].notificaciones |= (1 << indProcesoActual) ;
		}
        break ;
		
    case ECHO :                                                  /* 0x0504 */ /* echo(pindx, &msg) */
                                                                   /* echo */
																   
        break ;

    default: ;

    }
}