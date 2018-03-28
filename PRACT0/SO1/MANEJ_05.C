/* ----------------------------------------------------------------------- */
/*                               manej_05.c                                */
/* ----------------------------------------------------------------------- */
/*          manejador de las llamadas al sistema del grupo AH = 05         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1.h\procesos.h>

/* comunicacion directa, capacidad nula, por referencia */ 

/* comparar con MINIX3\kernel\proc.c */

/* mirar: Operating Systems Design and Implementation, 2.6.9. Interprocess */
/* communication in MINIX 3.                                               */   

typedef struct {
	c2c_t remitentes ;
    c2c_t destinatarios ;	
	void far * msg ;
} buzon_t ; 

buzon_t buzones [ maxProcesos ] ;


void so1_manejador_05 ( void ) {                       /* ah = 5 ; int SO1 */

    pindx_t pindx = tramaProceso->DX ;
	descProceso_t * ptrActual ; 
	descProceso_t * ptrOtro ; 

	if ((0 <= pindx) && (pindx < maxProcesos)) 
	{
        tramaProceso->AX = -1 ;
        return ;
    }
    ptrDProcesoOtro = (descProceso_t *)&descProceso[pindx] ;	

    switch (tramaProceso->AL) {
	  
    case 0x01 :                                                    /* 0x01 */
                                                                   /* send */ /* send(pindx, &msg) */
        if (buzon[indProcesoActual].destinatarios.numElem > 0)
            pindx     			
        break ;                                              

    case 0x02 :                                                    /* 0x02 */ /* receive(pindx, &msg) */
                                                                /* receive */ 
																
		if ((ptrOtro->estado == bloqueado) &&
            (ptrdescProceso[pindx].esperadoPor == rec_enviar) &&
            ((descProceso[pindx].trama->DX == indProcesoActual) ||
			(desc) /* no es necesario un nuevo campo para otro DX */
		{
			ptrMsg = MK_FP(tramaProceso->ES, tramaProceso->BX) ;
			*ptrMsg = descProceso[pindx].msg ; /* no es necesario un nuevo campo ES:BX */
			descProceso[pindx].estado == preparado ;
			encolarPC2c(pindx, c2cPFR[PPreparados]) ;
			tramaProceso->AX = 0 ;
		}				
        else 
		{
			/* hay que comprobar que existe pindx */
			descProceso[indProcesoActual].otro = pindx ;
			bloquearProcesoActual(rec_recibir) ;	
        } 			
        break ;

    case 0x00 :                                                    /* 0x00 */
                                                                /* sendrec */ /* sendrec(pindx, &msg) */
																
        break ;                                              

    case 0x03 :                                                    /* 0x03 */ /* notify(pindx) */
                                                                 /* notify */
																 
        break ;

    case 0x04 :                                                    /* 0x04 */ /* echo(pindx, &msg) */
                                                                   /* echo */
																   
        break ;

    default: ;

    }
}