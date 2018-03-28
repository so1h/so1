/* ----------------------------------------------------------------------- */
/*                               leetimer.c                                */
/* ----------------------------------------------------------------------- */
/*             programa para mostrar el uso de fichero TIMER               */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */
#include <so1pub.h\strings.h>                           /* strcpy, strcmpu */

void formato ( void )
{
    printf(" formato: LEETIMER [ nTics | -h ] ") ;
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
        " lee bytes del dispositivo TIMER"                               "\n"
	    ""                                                               "\n"
        " opciones: (por defecto nTics = 1)"                             "\n"
		""                                                               "\n"
        "   nTics : ejecuta un read(df, dir, nTics)"                     "\n"
        "      -h : muestra este help"                                   "\n"
	    ""                                                               "\n"
	) ;
}

#define nTicsMax 360

typedef struct
{
    dword_t a ;
    dword_t b ;
    word_t c ;
    word_t d;
} st_t ;

void leeTimer ( word_t nTics )
{
    int i ;
    byte_t buf [nTicsMax] ;
    int dfTimer ;
    st_t * st ;

    if (nTics > nTicsMax)
    {
        printf(" el numero de tics debe ser <= %i", nTicsMax) ;
        return ;
    }

    dfTimer = open("TIMER", O_RDONLY) ;
    if (dfTimer < 0)
    {
        printf(" recurso TIMER no disponible ") ;
		return ;
	}

    printf("\n\n %i x leer(dfTimer) ... ", nTics) ;
		
    for ( i = 0 ; i < nTics ; i++ )
        printf("%i ", (byte_t)leer(dfTimer)) ;
		
    printf("\n\n buf = ") ;
		
    for ( i = 0 ; i < nTics ; i++ )
    {
        buf[i] = '\0' ;
        printf("%i ", buf[i]) ;
    }
    printf("\n\n read(dfTimer, %04X:%04X, %i) ... ", seg(NULL), off(NULL) , nTics) ;
    read(dfTimer, NULL, nTics) ;
    printf("\n\n buf = ") ;
    for ( i = 0 ; i < nTics ; i++ )
        printf("%i ", buf[i]) ;

	printf("\n\n read(dfTimer, %04X:%04X, %i) ... ", seg(buf), off(buf) , nTics) ;
    read(dfTimer, (pointer_t)buf, nTics) ;
    printf("\n\n buf = ") ;
    for ( i = 0 ; i < nTics ; i++ )
        printf("%i ", buf[i]) ;
		
    printf("\n ") ;

    printf("\n\n aio_read(dfTimer, %04X:%04X, %i) ... ", seg(buf), off(buf), nTics) ;
    aio_read(dfTimer, (pointer_t)&buf, nTics) ;
    for ( i = 0 ; i < nTics ; i++ )
        printf("%i ", buf[i]) ;
    st = (st_t *)&buf ;
    printf(
        "\n\n a = %08lX\n b = %08lX\n c = %04X\n d = %04X\n", 
     	st->a, st->b, st->c, st->d
    ) ;

    close(dfTimer) ;
}

descProceso_t descProceso [ maxProcesos ] ;

descFichero_t descFichero [ dfsMax ] ;

descRecurso_t descRecurso [ maxRecursos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

int main ( int argc, char * argv [ ] )
{
    word_t nTics ;

    obtenInfoPFR ((descProceso_t far *)&descProceso,
                  (descFichero_t far *)&descFichero,
                  (descRecurso_t far *)&descRecurso,
                  (e2PFR_t far *)&e2PFR,
                  (c2c_t far *)&c2cPFR
                 ) ;

    if (argc <= 2)
    {
        if ((argc == 2) && !strcmpu(argv[1], "-h"))
        {
            help() ;
            return(0) ;
        }
        if (argc == 1) nTics = 1 ;
        else
        {
            strcpy(comando[0], argv[1]) ;
            inicScanner() ;
            obtenSimb() ;
            if (simb != s_numero)
            {
                putchar('\a') ;
                formato() ;
                printf(
				    "\n error: nTics debe ser un numero y no \"%s\"\n", 
					argv[1]
				) ;
                return(1) ;
            }
            else nTics = num ;
        }
        leeTimer(nTics) ;
    }
    else
        formato() ;
//	getchar() ;
	return(0) ;
}