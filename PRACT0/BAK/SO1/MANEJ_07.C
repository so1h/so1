/* ----------------------------------------------------------------------- */
/*                               manej_04.c                                */
/* ----------------------------------------------------------------------- */
/*          manejador de las llamadas al sistema del grupo AH = 07         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_sig.h>
#include <so1pub.h\tipos.h>
#include <so1.h\procesos.h>
#include <so1.h\blockpr.h>                        /* bloquearProcesoActual */
#include <so1pub.h\memory.h>                                     /* memcpy */

void so1_manejador_07 ( void ) {                       /* ah = 7 ; int SO1 */

    switch (tramaProceso->AL) 
	{
	                                                               	
    case 0x00 :                                                    /* 0x00 */
   	   	{                                                          /* kill */
		   	int sig ;
   			pid_t pid ;
	   		pindx_t pindx ;

		   	sig = tramaProceso->SI ;
   			pid = tramaProceso->BX ;
   			pindx = indice(pid) ;
			
   			if ((sig < 0) || (sig > 20)) tramaProceso->AX = 1 ;   /* error */
   			else if (pindx < 0) tramaProceso->AX = 2 ;            /* error */  
   			else 
   		    {      
  	   	   		trama_t far * tramaHandler ;                /* nueva trama */
   		    	word_t far * ptrWord ;      /* para apilar sig y dir. ret. */
				int errorAX = 0 ;   /* error llamada al sistema bloqueante */
		    	__sighandler_t far * __sighandler ;
				handler_t far * __handlerReturn ;
	     		__sighandler= MK_FP(tramaProceso->DS, 0x0096) ; /* map proceso */
				__handlerReturn = MK_FP(tramaProceso->CS, 0x008C) ; /* map proceso */
	   			if (pid != indProcesoActual)
		   		{
	   				descProceso[indProcesoActual].trama = tramaProceso ;
		   			descProceso[indProcesoActual].estado = preparado ;
   	                encolarPC2c(indProcesoActual, (ptrC2c_t)&c2cPFR[PPreparados]) ;
   					indProcesoActual = pindx ;
   			        tramaProceso = descProceso[indProcesoActual].trama ; 
   	            }	
			
//              /* modificamos la pila de manera que la trama original se  */
//              /* sustituye por el parametro real sig, la direccion de    */
//              /* retorno del proceso y la trama de pila modificada como  */
//              /* si fueramos a saltar al manejador en la direccion:      */
//              /*               CSProc:__sighandler[sig]                  */
//              /*                                                         */  
  
				if (descProceso[indProcesoActual].estado == bloqueado)  
				{
                    if (descProceso[indProcesoActual].esperandoPor == rec_senial) 
					{
						descProceso[indProcesoActual].estado = preparado ;
                    }
                    else  					
	    				/* la llamada al sistema bloqueante pendiente falla */ /* !! puede enmascararse */
		    			errorAX = -1 ;
				}
     			tramaHandler = (trama_t far *)
				    (((pointer_t)tramaProceso) + 2*sizeof(word_t)) ;
                memcpy(tramaHandler, tramaProceso, sizeof(trama_t)) ;
			    ptrWord = (word_t far *)tramaProceso ; 
			    
				*ptrWord++ = (word_t)sig ;           /* parametro real sig */     
                *ptrWord++ = tramaHandler->IP ;   /* dir retorno (proceso) */
				*ptrWord++ = errorAX ;                  /* codigo de error */
				*ptrWord   = (word_t)__handlerReturn ;      /* dir retorno */
				
                tramaHandler->CS = descProceso[indProcesoActual].CSProc ;
                tramaHandler->IP = (word_t)(__sighandler[sig]) ;
				descProceso[indProcesoActual].trama = tramaProceso ;
            }				
            break ;
        }
    case 0x01 :                                                    /* 0x01 */
	    {                                                     /* sigaction */
			int sig = tramaProceso->SI ;
			struct sigaction * act ;
			struct sigaction * oact ; 
			__sighandler_t far * __sighandler ;
			__sighandler= MK_FP(tramaProceso->DS, 0x0096) ; /* map proceso */
			act = (struct sigaction *)tramaProceso->BX ;     /* new action */
			oact = (struct sigaction *)tramaProceso->DX ;    /* old action */
			oact->sa_handler = __sighandler[sig] ;
			__sighandler[sig] = act->sa_handler ;
            break ;
        }
    case 0x02 :                                                    /* 0x02 */
	    {                                                         /* alarm */
            word_t seconds = tramaProceso->CX ;
			/* poner callBack en el timer */
			
			break ;
        }
    case 0x03 :  
	    {	                                                       /* 0x03 */
	                                                              /* pause */
			bloquearProcesoActual(rec_senial) ;													  
            break ;
        }
    case 0x04 :  
	    {	                                                       /* 0x04 */
	                                                         /* sigsuspend */
			bloquearProcesoActual(rec_senial) ;													  
            break ;
        }
    default: ;
	
    }

}

