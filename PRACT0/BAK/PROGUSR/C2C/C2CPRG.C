/* ----------------------------------------------------------------------- */
/*                                   c2c.c                                 */
/* ----------------------------------------------------------------------- */
/*              comando que muestra todas las colas c2c de SO1             */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */

#include <so1pub.h\strings.h>                           /* strcpy, strcmpu */
#include <so1pub.h\scanner.h>                    /* inicScanner, obtenSimb */

#include <so1pub.h\ptrc2c.h>                                   /* ptrC2c_t */

descProceso_t descProceso [ maxProcesos ] ;

descFichero_t descFichero [ dfsMax ] ;

descRecurso_t descRecurso [ maxRecursos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

char strCola [ ] [ 26 ] =
{
    "c2cPFR[DPLibres   ]",
    "c2cPFR[DPOcupados ]",
    "c2cPFR[DFLibres   ]",
    "c2cPFR[DFOcupados ]",
    "c2cPFR[DRLibres   ]",
    "c2cPFR[DROcupados ]",
    "c2cPFR[PPreparados]",
    "c2cPFR[PUrgentes  ]",
    "c2cPFR[POrdenados ]",
    "descProceso[0].c2cHijos",
    "descRecurso[0].c2cFichRec"
} ;

ptrC2c_t ptrC2c [ 11 ] ;

void ePC2c ( ptrC2c_t ptrC2c, char opcion)
{
    int i, j ;

    if (opcion == 'e')
		printf("%2i %2i %2i ", ptrC2c->numElem, ptrC2c->primero, ptrC2c->cabecera) ; 
	
    printf("[ ") ;
    i = ptrC2c->primero ;
    for ( j = 0 ; j < ptrC2c->numElem ; j++ )
    {
		printf("%i ", i) ; 
        /* eD(descProceso[i].pid, 1) ; */
        i = ptrC2c->e[i].sig ;
    }
    printf("] ") ;
}

void formato ( void )
{
    printf(" formato: COLA [ num | -n | -e | -h ] ") ;
}

void help ( void )
{
    printf(
        ""                                                                  "\n"
        ""                                                                  "\n"
        " formato : COLA [ num | -n | -e | -h ]"                            "\n"
        ""                                                                  "\n"
        "      -n : muestra todas las colas sin mas detalles"               "\n"
        "      -e : muestra ademas numElen, primero y cabecera"             "\n"
        "      -h : muestra este help"                                      "\n"
        "     num : muestra en detalle la cola con ese numero"              "\n"
		""                                                                  "\n"
        "       0 : DPLibres    : cola de descriptores de proceso libres"   "\n"
        "       1 : DPOcupados  : cola de descriptores de proceso ocupados" "\n"
        "       2 : DFLibres    : cola de descriptores de fichero libres"   "\n"
        "       3 : DFOcupados  : cola de descriptores de fichero ocupados" "\n"
        "       4 : DRLibres    : cola de descriptores de recurso libres"   "\n"
        "       5 : DROcupados  : cola de descriptores de recurso ocupados" "\n"
        "       6 : PPreparados : cola de procesos preparados"              "\n"
        "       7 : PUrgentes   : cola de procesos urgentes"                "\n"
        "       8 : POrdenados  : cola de procesos ordenados por direccion" "\n"
        "       9 : c2cHijos    : cola de procesos hijos de un proceso"     "\n"
        "      10 : c2cFichRec  : cola de ficheros de un recurso"           "\n"
        ""                                                                  "\n"
    ) ;
}

void colas ( char opcion, word_t num )
{
    pindx_t pindx ;
    rindx_t rindx ;
    int i ;

    if ((opcion == 'n') || (opcion == 'e'))
    {

        printf("\n") ;
		printf("\n c2cPFR[DPLibres   ] = ") ; ePC2c(&c2cPFR[DPLibres   ], opcion) ;
        printf("\n c2cPFR[DPOcupados ] = ") ; ePC2c(&c2cPFR[DPOcupados ], opcion) ;
        printf("\n c2cPFR[DFLibres   ] = ") ; ePC2c(&c2cPFR[DFLibres   ], opcion) ;
        printf("\n c2cPFR[DFOcupados ] = ") ; ePC2c(&c2cPFR[DFOcupados ], opcion) ;
        printf("\n c2cPFR[DRLibres   ] = ") ; ePC2c(&c2cPFR[DRLibres   ], opcion) ;
        printf("\n c2cPFR[DROcupados ] = ") ; ePC2c(&c2cPFR[DROcupados ], opcion) ;
        printf("\n c2cPFR[PPreparados] = ") ; ePC2c(&c2cPFR[PPreparados], opcion) ;
        printf("\n c2cPFR[PUrgentes  ] = ") ; ePC2c(&c2cPFR[PUrgentes  ], opcion) ;
        printf("\n c2cPFR[POrdenados ] = ") ; ePC2c(&c2cPFR[POrdenados ], opcion) ;
        printf("\n c2cPFR[PDormidos  ] = ") ; ePC2c(&c2cPFR[PDormidos  ], opcion) ;

		getchar() ;
		
        if (c2cPFR[DPOcupados].numElem > 0)
            printf("\n") ;
        for ( pindx = 0 ; pindx < maxProcesos ; pindx++ )
            if (descProceso[pindx].estado != libre)
            {
                printf("\n descProceso[%i].c2cHijos = ", pindx) ;
                ePC2c(&descProceso[pindx].c2cHijos, opcion) ;
            }

		getchar() ;	
			
        if (c2cPFR[DROcupados].numElem > 0)
            printf("\n") ;
		
        for ( rindx = 0 ; rindx < maxRecursos ; rindx++ )
            if (descRecurso[rindx].tipo != rLibre)
            {
                printf("\n descRecurso[%i].c2cFichRec = ", rindx) ;
                ePC2c(&descRecurso[rindx].c2cFichRec, opcion) ;
            }
        printf("\n") ;
    }
    else
    {
		printf("\n\n ") ;
        ePC2c(ptrC2c[num], 'n') ;
		
        printf("\n\n") ;
        printf(" %s.numElem  = %2i\n", strCola[num], ptrC2c[num]->numElem) ;
        printf(" %s.primero  = %2i\n", strCola[num], ptrC2c[num]->primero) ;
        printf(" %s.cabecera = %2i\n", strCola[num], ptrC2c[num]->cabecera) ;
        printf(" %s.e        = ", strCola[num]) ; 
		printf("%04X:%04X", SEG((pointer_t)ptrC2c[num]->e), 
		                    OFF((pointer_t)ptrC2c[num]->e)) ;

        printf("\n\n        i: ") ;
        for ( i = 0 ; i <= ptrC2c[num]->cabecera ; i++ )
            printf("%2i ", i) ;
		
        printf("\n ---------") ;
        for ( i = 0 ; i <= ptrC2c[num]->cabecera ; i++ )
            printf("---") ;
		
        printf("\n e[i].sig: ") ;
        for ( i = 0 ; i <= ptrC2c[num]->cabecera ; i++ )
            printf("%2i ", ptrC2c[num]->e[i].sig) ;
		
        printf("\n e[i].ant: ") ;		
        for ( i = 0 ; i <= ptrC2c[num]->cabecera ; i++ )
            printf("%2i ", ptrC2c[num]->e[i].ant) ;
		
        printf("\n") ;
    }
}

int main ( int argc, char * argv [ ] )
{

    obtenInfoPFR ((descProceso_t far *)&descProceso,
                  (descFichero_t far *)&descFichero,
                  (descRecurso_t far *)&descRecurso,
                  (e2PFR_t far *)&e2PFR,
                  (c2c_t far *)&c2cPFR
                 ) ;
				 
    if (argc == 2)
        if (!strcmpu(argv[1], "-h"))            /* argv[1] == "-h" o "-H" */ 
            help() ;
        else if (!strcmpu(argv[1], "-n"))       /* argv[1] == "-n" o "-N" */
            colas('n', 0) ;
        else if (!strcmpu(argv[1], "-e"))       /* argv[1] == "-e" o "-E" */
            colas('e', 0) ;
        else
        {
            strcpy(comando[0], argv[1]) ;
            inicScanner() ;
            obtenSimb() ;
            if (simb != s_numero)
            {
                putchar('\a') ;
				printf(
				    "\n"
					"\n"
				) ;
                formato() ;
                printf(
				    "\n"
					"\n"
					" error: num debe ser un numero no negativo y no \"%s\" \n", 
					argv[1]
				) ;
				return(-1) ;
            }
            else if (num <= 10)
            {
                ptrC2c[ 0] = &c2cPFR[DPLibres] ;
                ptrC2c[ 1] = &c2cPFR[DPOcupados] ;
                ptrC2c[ 2] = &c2cPFR[DFLibres] ;
                ptrC2c[ 3] = &c2cPFR[DFOcupados] ;
                ptrC2c[ 4] = &c2cPFR[DRLibres] ;
                ptrC2c[ 5] = &c2cPFR[DROcupados] ;
                ptrC2c[ 6] = &c2cPFR[PPreparados] ;
                ptrC2c[ 7] = &c2cPFR[PUrgentes] ;
                ptrC2c[ 8] = &c2cPFR[POrdenados] ;
                ptrC2c[ 9] = &descProceso[0].c2cHijos ;
                ptrC2c[10] = &descRecurso[0].c2cFichRec ;
                colas('f', num) ;
            }
            else
            {
                putchar('\a') ;
				printf(
				    "\n"
					"\n"
				) ;
                formato() ;
                printf(
				    "\n"
					"\n"
					" error: num (%u) debe ser <= 10 \n", num
				) ;
				return(-1) ;
            }
        }
    else if (argc == 1)
        colas('n', 0) ;
    else {
        formato() ;
		return(-1) ;
	}
	return(0) ;
}