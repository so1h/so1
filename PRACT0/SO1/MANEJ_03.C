/* ----------------------------------------------------------------------- */
/*                               manej_03.c                                */
/* ----------------------------------------------------------------------- */
/*          manejador de las llamadas al sistema del grupo AH = 03         */
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
/* AX: 0300H ==> obtenInfoPS                                               */
/* AX: 0301H ==> obtenInfoMEM                                              */
/* AX: 0302H ==> obtenInfoPFR                                              */
/* AX: 0303H ==> obtenInfoFAB                                              */
/* AX: 0304H ==> obtenInfoINFO                                             */
/* AX: 0305H ==> getdisk                                                   */
/* AX: 0306H ==> findFirstSo1                                              */
/* AX: 0307H ==> findNextSo1                                               */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>           /* OBTENINFOPS, OBTENINFOMEM, ... */
#include <so1pub.h\tipos.h>                               /* MK_FP, FP_SEG */
#include <so1pub.h\ptrc2c.h>                                   /* ptrC2c_t */
#include <so1pub.h\memory.h>                                     /* memcpy */
#include <so1.h\dbgword.h>                                    /* debugWord */
#include <so1.h\ajustsp.h>                                      /* SP0_SO1 */
#include <so1.h\ajustes.h>        /* CS_SO1, desplCab, IMRInicial, modoSO1 */
#include <so1.h\gm.h>                         /* listaLibres, tamBloqueMax */
#include <so1.h\procesos.h>   /* descProceso, descFichero, descRecurso ... */
#include <so1.h\sf.h>     /* findFirstSo1, findNextSo1, unidadLogicaActual */

static void copiarDescProcesos ( bloquePFR_t far * ptrPFR ) 
{
    int i ;
    descProceso_t far * dP = ptrPFR->descProceso ;
    for ( i = 0 ; i < maxProcesos ; i++ ) {
        memcpy(&dP[i], &descProceso[i], sizeof(descProceso_t)) ;
        if (descProceso[i].estado != libre)
            dP[i].c2cHijos.e = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2Hijos) ;
    }
}

static void copiarDescFicheros ( bloquePFR_t far * ptrPFR ) 
{
    int i ;
    descFichero_t far * dF = ptrPFR->descFichero ;
    for ( i = 0 ; i < dfsMax ; i++ )  
        memcpy(&dF[i], &descFichero[i], sizeof(descFichero_t)) ;
}

static void copiarDescRecursos ( bloquePFR_t far * ptrPFR ) 
{
    int i ;
    descRecurso_t far * dR = ptrPFR->descRecurso ;
    for ( i = 0 ; i < maxRecursos ; i++ ) 
	{
        memcpy(&dR[i], &descRecurso[i], sizeof(descRecurso_t)) ;
        if (descRecurso[i].tipo != rLibre)
            dR[i].c2cFichRec.e = (dobleEnlace_t far *)&(ptrPFR->e2PFR->e2FichRec) ;
    }
}

static void transferirBPFR ( bloquePFR_t far * ptrPFR, word_t regAX ) 
{
	
	ptrC2c_t ptrC2cPFR ;
	e2PFR_t far * ptrE2PFR ; 
	
    memcpy(ptrPFR->e2PFR, &e2PFR, sizeof(e2PFR_t)) ;
    memcpy(ptrPFR->c2cPFR, &c2cPFR, numColasPFR*sizeof(c2c_t)) ;

    copiarDescProcesos(ptrPFR) ;                         /* requiere e2PFR */
    if (regAX == OBTENINFOPFR) 
	{
        copiarDescFicheros(ptrPFR) ;                     /* requiere e2PFR */
        copiarDescRecursos(ptrPFR) ;                     /* requiere e2PFR */
    }

	ptrC2cPFR = (ptrC2c_t)&ptrPFR->c2cPFR[0] ;
	ptrE2PFR = (e2PFR_t far *)&ptrPFR->e2PFR[0] ; 
	 
    ptrC2cPFR[DPLibres   ].e = (dobleEnlace_t far *)&(ptrE2PFR->e2DescProceso) ;
    ptrC2cPFR[DPOcupados ].e = (dobleEnlace_t far *)&(ptrE2PFR->e2DescProceso) ;
    ptrC2cPFR[PPreparados].e = (dobleEnlace_t far *)&(ptrE2PFR->e2Procesos) ;
    ptrC2cPFR[PUrgentes  ].e = (dobleEnlace_t far *)&(ptrE2PFR->e2Procesos) ;
    ptrC2cPFR[PDormidos  ].e = (dobleEnlace_t far *)&(ptrE2PFR->e2Procesos) ;
    ptrC2cPFR[POrdenados ].e = (dobleEnlace_t far *)&(ptrE2PFR->e2POrdenados) ;
    ptrC2cPFR[DFLibres   ].e = (dobleEnlace_t far *)&(ptrE2PFR->e2DescFichero) ;
    ptrC2cPFR[DFOcupados ].e = (dobleEnlace_t far *)&(ptrE2PFR->e2DescFichero) ;
    ptrC2cPFR[DRLibres   ].e = (dobleEnlace_t far *)&(ptrE2PFR->e2DescRecurso) ;
    ptrC2cPFR[DROcupados ].e = (dobleEnlace_t far *)&(ptrE2PFR->e2DescRecurso) ;

    if (regAX == OBTENINFOMEM) 
	{
        tramaProceso->AX = FP_SEG(listaLibres) ;
        tramaProceso->DX = tamBloqueMax ;
    }
}

void so1_manejador_03 ( void )                         /* ah = 3 ; int SO1 */
{
    switch (tramaProceso->AX) 
	{
    case OBTENINFOPS  :                        /* obtenInfoPS  *//* 0x0300 */
    case OBTENINFOMEM :                        /* obtenInfoMEM *//* 0x0301 */
    case OBTENINFOPFR :                        /* obtenInfoPFR *//* 0x0302 */
	    {
            bloquePFR_t far * ptrPFR ;
            ptrPFR = (bloquePFR_t far *)
			    MK_FP(tramaProceso->ES, tramaProceso->BX) ;
            transferirBPFR(ptrPFR, tramaProceso->AX) ;
		}
        break ;
		
    case OBTENINFOFAB :                                          /* 0x0303 */
	    {
		    descriptor_de_fichero_t far * df ;
            df = (descriptor_de_fichero_t far *)           /* obtenInfoFAB */
                MK_FP(tramaProceso->ES, tramaProceso->BX) ;
            if (df != (descriptor_de_fichero_t far *)NULL)
                memcpy(df, &tablaFichAbiertos, sizeof(tablaFichAbiertos)) ;
            else
                inicTablaFichAbiertos() ;     /* borramos la tabla de f.a. */
		}	
        break ;
		
    case OBTENINFOINFO :                                         /* 0x0304 */
	    {
            info_t far * info ;
            cabecera_t far * cabecera ;
            cabecera = (cabecera_t far *)MK_FP(CS_SO1, desplCab()) ;
            info = (info_t far *)                         /* obtenInfoINFO */
                MK_FP(tramaProceso->ES, tramaProceso->BX) ;
//          info->ticsPorRodaja = ticsPorRodaja ;
            info->SP0 = cabecera->SP0 ;         /* info->SP0 = SPInicial ; */
            info->SP0_So1 = SP0_SO1 ;
            info->IMR = IMRInicial ;
            info->modoSO1 = modoSO1() ;
            info->ptrDebugWord = (word_t far *)MK_FP(_DS, FP_OFF(&debugWord)) ;
		}
        break ;
		
    case GETDISK :                                               /* 0x0305 */ 
        tramaProceso->AX = (word_t)unidadLogicaActual ;         /* getdisk */
        break ;

    case FINDFIRST :                                             /* 0x0306 */
        tramaProceso->AX = (word_t)
            findFirstSo1(                                     /* findFirst */
                tramaProceso->BL,
                (ffblk_t far *)MK_FP(tramaProceso->DS, tramaProceso->DX)
            ) ;
        break ;
		
    case FINDNEXT :                                              /* 0x0307 */
        tramaProceso->AX = (word_t)
            findNextSo1(                                       /* findNext */
                (ffblk_t far *)MK_FP(tramaProceso->DS, tramaProceso->DX)
            ) ;
      break ;

    default: ;

    }
}