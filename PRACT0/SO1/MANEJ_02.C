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

#include <so1pub.h\ll_s_so1.h>          /* OBTENINFOSO1, CREARRECURSO, ... */ 
#include <so1pub.h\strings.h>                                    /* strcmp */
#include <so1pub.h\memory.h>                                     /* memcpy */
#include <so1.h\ajustsp.h>                                      /* SP0_SO1 */
#include <so1.h\interrup.h>                                       /* VIOrg */
#include <so1.h\blockpr.h>           /* indProcesoActual, ptrDPActual, ... */
#include <so1.h\recursos.h>            /* crearRec, crearFich, destruirRec */
#include <so1.h\gm.h>                                  /* k_devolverBloque */

void cargarDescSO1 ( descSO1_t far * descSO1 ) 
{
    descSO1->ptrIndProcesoActual      = (pindx_t far *)&indProcesoActual ;
    descSO1->ptrDPActual              = (descProceso_t far *)ptrDPActual ; /* en cuarentena */
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
    descSO1->sigProceso               = MK_FP(_CS, (word_t)sigProceso) ;
    descSO1->activarProceso           = MK_FP(_CS, (word_t)activarProceso) ;
    descSO1->buscarNuevoProcesoActual = MK_FP(_CS, (word_t)buscarNuevoProcesoActual) ;
    descSO1->bloquearProcesoActual    = MK_FP(_CS, (word_t)bloquearProcesoActual) ;
}

extern void so1_manejador_02 ( )                       /* ah = 2 ; int SO1 */
{
    switch (tramaProceso->AX) 
	{

    case OBTENINFOSO1 :                                          /* 0x0200 */
	{                                                      /* obtenInfoSO1 */
        descSO1_t far * descSO1 ;                         
        descSO1 = MK_FP(tramaProceso->ES, tramaProceso->BX) ;
        cargarDescSO1(descSO1) ;
        break ;
    }
	
    case CREARRECURSO :                                          /* 0x0201 */
	{                                                      /* crearRecurso */
        descRecurso_t far * dR ;                          
        rindx_t rindx ;
		
        dR = MK_FP(tramaProceso->ES, tramaProceso->BX) ;
        rindx = crearRec(dR) ;
        tramaProceso->AX = rindx ;
        break ;
    }
	
    case CREARFICHERO :                                          /* 0x0202 */
	{                                                      /* crearFichero */
        char far * nombre ;                               
        rindx_t rindx ;
        word_t menor ;
        tipoFichero_t tipo ;
		
        nombre = MK_FP(tramaProceso->ES, tramaProceso->BX) ;
        rindx  = tramaProceso->CX ;
        menor  = tramaProceso->DX ;
        tipo   = tramaProceso->SI ;
        tramaProceso->AX = crearFich(nombre, rindx, menor, tipo) ;
        break ;
    }   
	
    case ESPERARDESINSTALACION :                                 /* 0x0203 */
	{                                             /* esperarDesinstalacion */
        rindx_t r ;
        word_t tamDATA       = tramaProceso->BX ;    /* BX = tamDATA       */
	    word_t finCodeDriver = tramaProceso->CX ;    /* CX = finCodeDriver */
	    word_t finishDriver  = tramaProceso->DX ;    /* DX = finishDriver  */
	    word_t tamPila       = tramaProceso->SI ;    /* SI = tamPila       */
	
        if (tamDATA != 0x0000)                        /* tamDATA != 0x0000 */
        {
		    word_t SP0 ;
    	    word_t tamPs ;                                   /* paragrafos */
	        word_t tamPilaPsMin ;                            /* paragrafos */
	        word_t tamPilaPs ;                               /* paragrafos */
	        word_t segBloqueLibre ;
		    word_t nuevoSegDatosSR ; 
		    word_t nuevoDS ; 
		    word_t nuevoSP0 ;
		    trama_t far * nuevaTramaProceso ;
			ptrC2c_t ptrC2cDROcupados ;
			descRecurso_t * ptrDRr ; 
  	
	     	tamDATA       = (tamDATA + 0x000F) & 0xFFF0 ;         /* bytes */
	        finCodeDriver = (finCodeDriver + 0x000F) & 0xFFF0 ;
		    tamPs = ptrDPActual->tam ;
//		    tamPilaPs = tramaProceso->CS + tamPs 
//		                  - (tramaProceso->DS + (FP_OFF(tramaProceso) >> 4)) ;
            tamPilaPsMin = (sizeof(trama_t) + 8 + 0x000F) >> 4 ; /* minimo */
              /* 8 bytes para 4 palabras: desplFinish, argc, argv y 0x0000 */
		    if (16*tamPilaPsMin < tamPila)           /* tamPila solicitado */
		    	tamPilaPs = (tamPila + 0x000F) >> 4 ; 
            else
			    tamPilaPs = tamPilaPsMin ;
            nuevoSegDatosSR = finCodeDriver >> 4 ;
            nuevoDS = tramaProceso->CS + nuevoSegDatosSR ;  
            segBloqueLibre = nuevoDS + (tamDATA >> 4) + tamPilaPs ;

			ptrC2cDROcupados = (ptrC2c_t)&c2cPFR[DROcupados] ;
            r = ptrC2cDROcupados->primero ; /* actualizar DS en call backs */
            while (r != ptrC2cDROcupados->cabecera) 
	    	{
				ptrDRr = (descRecurso_t *)&descRecurso[r] ;
                if (ptrDRr->pindx == indProcesoActual) 
			    {
				    if (ptrDRr->ccb->arg != NULL) 
			            ptrDRr->ccb->arg = 
			                MK_FP(nuevoDS, FP_OFF(ptrDRr->ccb->arg)) ;  		
                    ptrDRr->ccb = MK_FP(nuevoDS, FP_OFF(ptrDRr->ccb)) ; 
                }
                else 
		    	{
			    	int i, j ;
				    j = ptrDRr->ccb->out ;
    				for ( i = 0 ; i < ptrDRr->ccb->num ; i++ ) 
	    			{
                        if (FP_SEG(ptrDRr->ccb->callBack[j]) == tramaProceso->DS)
			    			ptrDRr->ccb->callBack[j] = 
				                MK_FP(nuevoDS, FP_OFF(ptrDRr->ccb->callBack[j])) ;
                        j = (j + 1) % (ptrDRr->ccb->max) ;
    				}	
	     		}
                r = ptrC2cDROcupados->e[r].sig ;
            } 

	    	memcpy(      /* copia de los primeros tamDATA bytes de la DATA */
		        MK_FP(nuevoDS, 0x0000),                         /* destino */
		        MK_FP(tramaProceso->DS, 0x0000),    /* comienzo de la DATA */
                tamDATA
		    ) ;               
	
            nuevaTramaProceso = MK_FP(
		        nuevoDS, 
		     	tamDATA + 16*tamPilaPs - sizeof(trama_t) - 8        /* 4 palabras */ 
		    ) ;			   
		
    		ptrDPActual->tam = segBloqueLibre - tramaProceso->CS ;
	    	ptrDPActual->desplBSS = tramaProceso->BX ;
	    	ptrDPActual->desplPila = tramaProceso->BX ;
		
            ((cabecera_t far *)MK_FP(tramaProceso->CS, 0x0000))->segDatosSR =
                nuevoSegDatosSR ;	
            ((cabecera_t far *)MK_FP(tramaProceso->CS, 0x0000))->finData =
                ptrDPActual->desplBSS ;
            ((cabecera_t far *)MK_FP(tramaProceso->CS, 0x0000))->finBSS =
                ptrDPActual->desplPila ;	
        
	    	SP0 = ((cabecera_t far *)MK_FP(tramaProceso->CS, 0x0000))->SP0 ;
		    nuevoSP0 = 16*(segBloqueLibre - nuevoDS)  ;	
	    	((cabecera_t far *)MK_FP(tramaProceso->CS, 0x0000))->SP0 = nuevoSP0 ;
		
		    memcpy(                         /* cuatro palabras: (8 nytes) */ 
		        MK_FP(nuevoDS, nuevoSP0-8),           /* dir. ret. finish */  
		     	MK_FP(tramaProceso->DS, SP0-8),       /* argc             */
		     	8                                     /* argv             */ 
		    ) ;                                       /* 0x0000 (atexit)  */ 
		  
            k_devolverBloque(segBloqueLibre, tramaProceso->CS + tamPs - segBloqueLibre) ; 

	    	tramaProceso = nuevaTramaProceso ;
		
            tramaProceso->DS    = nuevoDS ;  
	    	tramaProceso->ES    = nuevoDS ;
    		tramaProceso->AX    = 0x0000 ;
	    	tramaProceso->BX    = 0x0000 ;
     		tramaProceso->CX    = 0x0000 ;
    		tramaProceso->DX    = 0x0000 ;
	     	tramaProceso->SI    = 0x0000 ;
	     	tramaProceso->DI    = 0x0000 ;
    		tramaProceso->SP    = FP_OFF(tramaProceso) + 10*(sizeof(word_t)) ;
     		tramaProceso->BP    = 0x0000 ; 
	     	tramaProceso->IP    = finishDriver ;	
	    	tramaProceso->CS    = ptrDPActual->CSProc ;	
	     	tramaProceso->Flags = (_FLAGS & 0xF000) | 0x0202 ;	
	    }
        bloquearProcesoActual(rec_desinstalacion) ;  /* rec_desinstalacion */   
//      (no se retorna aqui, sino a la instruccion siguiente al trap) 	
        break ; 
    }                                         
                   
    case DESTRUIRRECURSO :                                       /* 0x0204 */
    {                                                   /* destruirRecurso */    
        tramaProceso->AX =
            destruirRec(
			    MK_FP(tramaProceso->ES, tramaProceso->BX),       /* nombre */
	            tramaProceso->CX                                /* matando */
			) ;
        break ;
    }                
	                                                    /* 0x0205 y 0x0206 */
    case ENCOLARCCBRECURSO  : ;                       /* encolarCcbRecurso */
    case ELIMINARCCBRECURSO :                        /* eliminarCcbRecurso */
	{
        callBack_t cb ;
        rindx_t rindx ;
        bool_t enc = FALSE ;
        int res ;
		ptrC2c_t ptrC2cDROcupados = (ptrC2c_t)&c2cPFR[DROcupados] ;
        rindx = ptrC2cDROcupados->primero ;               /* ver si existe */
        while (rindx != ptrC2cDROcupados->cabecera) 
		{
            if (!strcmp(descRecurso[rindx].nombre,
                        MK_FP(tramaProceso->CX, tramaProceso->BX))) 
			{
                enc = TRUE ;
                break ;                                  /* sale del bucle */
            }
            rindx = ptrC2cDROcupados->e[rindx].sig ;
        }
        if (!enc) res = -1 ;                       /* el recurso no existe */
        else 
		{
            cb = MK_FP(tramaProceso->ES, tramaProceso->DX) ;
            switch (tramaProceso->AX) 
			{
            case ENCOLARCCBRECURSO :                                  
                res = encolarCcb(cb, descRecurso[rindx].ccb) ;   
                break ;                                  /* 0, -1, ..., -3 */
            case ELIMINARCCBRECURSO :
                res = eliminarCcb(cb, descRecurso[rindx].ccb) ;  
                break ;                                  /* 0, -1, ..., -4 */
            }
            if (res < 0) res-- ;
        }
        tramaProceso->AX = res ;
        break ;
    }
	
    default : ;
	
    }
}