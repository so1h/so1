/* ----------------------------------------------------------------------- */
/*                                  mem.c                                  */
/* ----------------------------------------------------------------------- */
/*     comando para conocer los bloques de memoria asignados y libres      */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */
#include <so1pub.h\strings.h>                                   /* strcmpu */

descProceso_t descProceso[maxProcesos] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

ptrBloque_t listaLibres ;

word_t tamBlqMax ;

void mostrarParticion ( pindx_t pindx )
{
	printf(
	    "%04X:0000 %04X Ps ocupada %3i%4i %-12s %-25s",
        descProceso[pindx].CSProc,
        descProceso[pindx].tam,
        pindx, 
		descProceso[pindx].pid,
		descProceso[pindx].programa,
        descProceso[pindx].comando
	) ;
}

void mostrarHueco ( ptrBloque_t ptrBloque )
{
	printf(
        "%04X:%04X %04X Ps libre ",
        seg((pointer_t)ptrBloque),
		off((pointer_t)ptrBloque),
		ptrBloque->tam
	) ;
}

void mostrarOtroBloqueOcupado( word_t segmento, word_t tam )
{
	printf(
	    "%04X:0000 %04X Ps ocupado ", segmento, tam
	) ;
}

void formato ( void )
{
    printf(
        " formato: MEM [ -l | -h ] "
    ) ;
}

void help ( void )
{
    printf(
        ""                                                               "\n"
        ""                                                               "\n"
	) ;
	formato() ;
	printf(
        ""                                                               "\n"
        ""                                                               "\n"
        " muestra el estado de la memoria gestionada"                    "\n"
        " como una lista de bloques libres ordenados"                    "\n"
        " por direccion (segmento:0000)."                                "\n"
		""                                                               "\n"
		" opciones:"                                                     "\n"
        ""                                                               "\n"
        "   -l : muestra solo los bloques libres"                        "\n"
        "   -h : muestra este help"                                      "\n"
    ) ;
}

void mem ( bool_t mostrarTodos )
{
    int i = 0 ;
    pindx_t pindx = 0 ;
    ptrBloque_t ptrBloque ;
    word_t tam ;
    word_t segmento ;

    obtenInfoMEM((descProceso_t far *)&descProceso,
                 (e2PFR_t far *)&e2PFR,
                 (c2c_t far *)&c2cPFR,
                 (ptrBloque_t *)&listaLibres,
                 (word_t *)&tamBlqMax) ;

    ptrBloque = (ptrBloque_t)pointer(listaLibres->sig, 0x0000) ;
    tam = descProceso[pindx].tam ;
    segmento = descProceso[pindx].CSProc ;

    printf(
	    " (mayor bloque libre: %04X Ps = %i KB)"                                     "\n"
        ""                                                                           "\n"
        " part inicio    tam     estado  ind pid programa     comando"               "\n"
        " ---- --------- ------- ------- --- --- ------------ ------------------------\n",
		tamBlqMax, tamBlqMax/64
    ) ;

    if (!mostrarTodos)
    {
        while (seg((pointer_t)ptrBloque) != seg((pointer_t)listaLibres))
        {
            printf(" %4i ", i++) ;
            mostrarHueco(ptrBloque) ;
            ptrBloque = (ptrBloque_t)pointer(ptrBloque->sig, 0x0000) ;
			printf("\n") ;
        }
        return ;
    }

    printf("\n %4i ", i++) ;
    mostrarParticion(pindx) ;
    pindx = c2cPFR[POrdenados].e[pindx].sig ;

    while ((segmento + tam) != seg((pointer_t)listaLibres))
    {
        printf("\n %4i ", i++) ;
        if ((segmento + tam) == seg((pointer_t)ptrBloque))
        {
            mostrarHueco(ptrBloque) ;
            segmento = seg((pointer_t)ptrBloque) ;
            tam = ptrBloque->tam ;
            ptrBloque = (bloque_t far *)pointer(ptrBloque->sig, 0x0000) ;
        }
        else if ((segmento + tam) == descProceso[pindx].CSProc)
        {
            mostrarParticion(pindx) ;
            segmento = descProceso[pindx].CSProc ;
            tam = descProceso[pindx].tam ;
            pindx = c2cPFR[POrdenados].e[pindx].sig ;
        }
        else if ((seg((pointer_t)ptrBloque) < descProceso[pindx].CSProc) ||
                 ((pindx == 0) && (ptrBloque != listaLibres)))
        {
            mostrarOtroBloqueOcupado(
                segmento+tam,
                seg((pointer_t)ptrBloque)-(segmento+tam)
            ) ;
            segmento = segmento+tam ;
            tam = seg((pointer_t)ptrBloque)-segmento ;
        }
        else if (pindx != 0)
        {
            mostrarOtroBloqueOcupado(
                segmento+tam,
                descProceso[pindx].CSProc-(segmento+tam)
            ) ;
            segmento = segmento+tam ;
            tam = descProceso[pindx].CSProc-segmento ;
        }
        else
        {
            mostrarOtroBloqueOcupado(
                segmento+tam,
                seg((pointer_t)listaLibres)-(segmento+tam)
            ) ;
            break ;
        }
    }

    printf("\n") ;
	
}

int main ( int argc, char * argv [ ] )
{
    if (argc == 1) mem(TRUE) ;
    else if (argc == 2)
        if (!strcmpu(argv[1], "-h")) help() ;
        else if (!strcmpu(argv[1], "-l")) mem(FALSE) ;
        else formato() ;
    else formato() ;
//	getchar() ;
	return(0) ;
}
