/* ----------------------------------------------------------------------- */
/*                               manej_04.c                                */
/* ----------------------------------------------------------------------- */
/*          manejador de las llamadas al sistema del grupo AH = 04         */
/* ----------------------------------------------------------------------- */

/* ======================================================================= */
/*  AX             activarTraza y analizarProcesos (AH = 4)                */
/* ======================================================================= */
/* 0400: activarTraza(numInstr,pid)                                        */
/* 0401: analizarProcesos(Ok)                                              */
/* ======================================================================= */

#include <so1pub.h\ll_s_so1.h>           /* ACTIVARTRAZA, ANALIZARPROCESOS */
#include <so1pub.h\puertos.h>
#include <so1.h\procesos.h>
/* #include <so1.h\traza.h> */
#include <so1.h\plot.h>

/*

void activarFlagTraza ( word_t numInstr, pindx_t pindx ) 
{
    word_t Flgs ;

    if (pindx == indProcesoActual)
        Flgs = tramaProceso->Flags ;
    else
        Flgs = descProceso[pindx].trama->Flags ;

    if (numInstr)
        Flgs = Flgs | 0x0100 ;
    else
        Flgs = Flgs & 0xFEFF ;

    if (pindx == indProcesoActual)
        tramaProceso->Flags = Flgs ;
    else
        descProceso[pindx].trama->Flags = Flgs ;
}
*/

void so1_manejador_04 ( void ) {                       /* ah = 4 ; int SO1 */

    bool_t Ok ;

    switch (tramaProceso->AL) 
	{
                                                                 /* 0x0400 */
    case ACTIVARTRAZA :                                    /* activarTraza */
/*
        numInstr = tramaProceso->BX ;
        pid = (pid_t)tramaProceso->DX ;
        if ((0 <= pid) && (pid < maxProcesos))
            activarFlagTraza(numInstr, indice(pid)) ;
        else if (pid == -1) 
		{
            for ( pindx = 0 ; pindx < maxProcesos ; pindx++ )
                activarFlagTraza(numInstr, pindx) ;
        }
        if (numInstr > 0) numInstrMax = numInstr ;
*/
        break ;

    case ANALIZARPROCESOS :                                      /* 0x0401 */
                                                       /* analizarProcesos */
        Ok = (bool_t)(!(tramaProceso->Flags & 0x0040)) ;    /* Ok = flag Z */
        if (!plotProcesos) 
		{
            if (Ok) plotProcesos = TRUE ;                    /* activacion */
            plot('i', 0, contadorTimer0()) ;
            plot('s', tramaProceso->AX, contadorTimer0()) ;
        }
        else 
		{
            if (!Ok) 
			{
                plot('e', 0, contadorTimer0()) ;
                plot('z', 0, contadorTimer0()) ; /* truco visualizar (analizad.) */
                plotProcesos = FALSE ;                    /* desactivacion */
            }
            else
                plot('s', tramaProceso->AX, contadorTimer0()) ;
        }
        break ;

    default: ;
  }
}