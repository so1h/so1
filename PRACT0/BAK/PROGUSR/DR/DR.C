/* ----------------------------------------------------------------------- */
/*                                   dr.c                                  */
/* ----------------------------------------------------------------------- */
/*               comando para ver los descriptores de recurso              */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                             /* printf, getchar */
#include <so1pub.h\strings.h>                           /* strcpy, strcmpu */
#include <so1pub.h\scanner.h>                /* inicScanner, obtenSimb ... */
#include <so1pub.h\ptrc2c.h>                                   /* ptrC2c_t */

void formato ( void )
{
    printf(
        ""                                                               "\n"
        ""                                                               "\n"
        " formato: DR [ rindx | -a | -h ] "                              "\n"                     
    ) ;
}

void help ( void )
{
    formato() ;
    printf(
        ""                                                               "\n"
        " muestra los campos del descriptor de recurso rindx"            "\n"
        ""                                                               "\n"
        " opciones: (por defecto -a)"                                    "\n"
        ""                                                               "\n"
        "   rindx : muestra en detalle solo ese recurso"                 "\n"
        "      -a : muestra todos los recursos"                          "\n"
        "      -h : muestra este help"                                   "\n"
    ) ;
}

void ePC2c ( ptrC2c_t ptrC2c )
{
    int i, j ;
	int numElem ;
	int primero ;
//	int cabecera ;
    numElem = ptrC2c->numElem ;
    primero = ptrC2c->primero ;
//  cabecera = ptrC2c->cabecera ;
//  printf("%i %i %i ", numElem, primero, cabecera) ;
    printf("[ ") ;
    i = primero ;
    for ( j = 0 ; j < numElem ; j++ )
    {
        printf("%i ", i) ;
        i = ptrC2c->e[i].sig ;
    }
    printf("] ") ;
}


void eCcb ( ccb_t ccb )
{
    int i, j ;
	printf(
	    " %i %i %i %i %04X:%04X [ ",  
		ccb->num, ccb->in, ccb->out, ccb->max, 
//		SEG((pointer_t)(ccb->arg)),       /* SEG funciona mal, usar FP_SEG */ 
//      OFF((pointer_t)(ccb->arg)) 
		FP_SEG(ccb->arg), FP_OFF(ccb->arg)                /* funciona bien */
	) ;
    j = ccb->out ;
//  for ( i = 0 ; (i < ccb->num) && (i < 2) ; i ++ ) 
    for ( i = 0 ; i < ccb->num ; i ++ ) 
    {
     	printf(
	        "%04X:%04X ", 
		    FP_SEG(ccb->callBack[j]), FP_OFF(ccb->callBack[j])
	    ) ;
        j = (j + 1) % ccb->max ;
    }
    printf("] ") ;
}

char strTipoRecurso [ ] [ 13 ] =
{
    "rLibre",
    "rDCaracteres",
    "rDBloques",
    "rTuberia",
    "rGP",
    "rGM",
    "rSF",
    "rOtro"
} ;

descProceso_t descProceso [ maxProcesos ] ;

descFichero_t descFichero [ dfsMax ] ;

descRecurso_t descRecurso [ maxRecursos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

void mostrarDescRecurso ( rindx_t rindx )
{

    tipoRecurso_t tipo = descRecurso[rindx].tipo ;
    byte_t numVI = descRecurso[rindx].numVI ;
    int i ;

    printf(
	    ""                                                               "\n"
		" descRecurso[%i].nombre     = \"%s\""                           "\n" 
		" descRecurso[%i].tipo       = %s (%i)"                          "\n"
		" descRecurso[%i].ccb        = ",
		rindx, descRecurso[rindx].nombre,
		rindx, strTipoRecurso[tipo], tipo,
        rindx
	) ;
    eCcb(descRecurso[rindx].ccb) ;
	
    printf(
	    ""                                                               "\n"
		" descRecurso[%i].pindx      = %i"                               "\n" 
		" descRecurso[%i].c2cFichRec = ",                                 
        rindx, descRecurso[rindx].pindx, 
		rindx
	) ;
    ePC2c(&descRecurso[rindx].c2cFichRec) ;

    printf(
	    ""                                                               "\n"
		" descRecurso[%i].numVI      = %i"                               "\n",
        rindx, numVI
	) ;
		
    for ( i = 0 ; i < numVI ; i++ )
    printf(
        ""                                                               "\n"
     	" descRecurso[%i].nVInt[%i]   = %02X"                            "\n"
     	" descRecurso[%i].irq[%i]     = %i"                              "\n"
     	" descRecurso[%i].isr[%i]     = %i"                              "\n",
	    rindx, i, descRecurso[rindx].nVInt[i], 
		rindx, i, descRecurso[rindx].irq[i], 
		rindx, i, descRecurso[rindx].isr[i]
    ) ;
	
    printf(
     	" descRecurso[%i].open       = %04X:%04X"                        "\n"
     	" descRecurso[%i].release    = %04X:%04X"                        "\n"
     	" descRecurso[%i].read       = %04X:%04X"                        "\n"
     	" descRecurso[%i].aio_read   = %04X:%04X"                        "\n"
     	" descRecurso[%i].write      = %04X:%04X"                        "\n"
     	" descRecurso[%i].aio_write  = %04X:%04X"                        "\n"
     	" descRecurso[%i].lseek      = %04X:%04X"                        "\n"
     	" descRecurso[%i].fcntl      = %04X:%04X"                        "\n"
     	" descRecurso[%i].ioctl      = %04X:%04X"                        "\n"
        ""                                                               "\n"
     	" descRecurso[%i].eliminar   = %04X:%04X"                        "\n",
        rindx, 
		  FP_SEG(descRecurso[rindx].open), 
		  FP_OFF(descRecurso[rindx].open), 
        rindx, 
		  FP_SEG(descRecurso[rindx].release), 
		  FP_OFF(descRecurso[rindx].release), 
        rindx, 
		  FP_SEG(descRecurso[rindx].read), 
		  FP_OFF(descRecurso[rindx].read), 
        rindx, 
		  FP_SEG(descRecurso[rindx].aio_read), 
		  FP_OFF(descRecurso[rindx].aio_read), 
        rindx, 
		  FP_SEG(descRecurso[rindx].write), 
		  FP_OFF(descRecurso[rindx].write), 
        rindx, 
		  FP_SEG(descRecurso[rindx].aio_write), 
		  FP_OFF(descRecurso[rindx].aio_write), 
        rindx, 
          FP_SEG(descRecurso[rindx].lseek), 
		  FP_OFF(descRecurso[rindx].lseek), 
        rindx, 
		  FP_SEG(descRecurso[rindx].fcntl), 
		  FP_OFF(descRecurso[rindx].fcntl), 
        rindx, 
		  FP_SEG(descRecurso[rindx].ioctl), 
		  FP_OFF(descRecurso[rindx].ioctl), 
        rindx, 
		  FP_SEG(descRecurso[rindx].eliminar),
		  FP_OFF(descRecurso[rindx].eliminar)
	) ;
}

void mostrarRecursos ( void )
{
    int rindx ;
    int i ;
    printf(
	    ""                                                                         "\n"
        ""                                                                         "\n"
		" rindx nombre       tp pindx   open    V0 irq0   isr 0   V1 irq1   isr 1   \n"
        " ----- ------------ -- ----- --------- -- ---- --------- -- ---- --------- \n"
	) ;
    for ( rindx = 0 ; rindx < maxRecursos ; rindx++ )
        if (descRecurso[rindx].pindx >= 0)
        {
	        printf(
	            " %5i %-12s%3i%6i %04X:%04X ", 
                rindx, 
                descRecurso[rindx].nombre,
                descRecurso[rindx].tipo, 
                descRecurso[rindx].pindx, 
                FP_SEG(descRecurso[rindx].open),
                FP_OFF(descRecurso[rindx].open)
	        ) ;
            for ( i = 0 ; i < descRecurso[rindx].numVI ; i++ )
            {
				printf("%02X %4i %04X:%04X ",
                    descRecurso[rindx].nVInt[i], 
                    descRecurso[rindx].irq[i], 
                    FP_SEG(descRecurso[rindx].isr[i]),
                    FP_OFF(descRecurso[rindx].isr[i])
                ) ;					
            }
	        printf("\n") ;
        }
}

int main ( int argc, char * argv [ ] )
{

    rindx_t rindx ;
    obtenInfoPFR ((descProceso_t far *)&descProceso,
                  (descFichero_t far *)&descFichero,
                  (descRecurso_t far *)&descRecurso,
                  (e2PFR_t far *)&e2PFR,
                  (c2c_t far *)&c2cPFR
                 ) ;
    if (argc == 2)
        if (!strcmpu(argv[1], "-h"))
            help() ;
        else if (!strcmpu(argv[1], "-a"))
            mostrarRecursos() ;
        else
        {
            strcpy(comando[0], argv[1]) ;
            inicScanner() ;
            obtenSimb() ;
            if (simb != s_numero)
            {
                putchar('\a') ;
                formato() ;
                printf("\n error: rindx debe ser un numero y no \"%s\"\n", argv[1]) ;
				return(1) ;
            }
            else
            {
                rindx = (rindx_t)num ;
                if ((0 <= rindx) && (rindx < maxRecursos))
                    if (descRecurso[rindx].tipo != rLibre)
                    {
                        printf("\n") ;
                        mostrarDescRecurso(rindx) ;
                    }
                    else
                    {
                        printf(" descriptor de recurso no utilizado ") ;
						return(2) ;
                    }
                else
                {
                    printf(" rindx erroneo ") ;
					return(3) ;
                }
            }
        }
    else if (argc == 1)
        mostrarRecursos() ;
    else
        formato() ;
//	getchar() ;
	return(0) ;
}

