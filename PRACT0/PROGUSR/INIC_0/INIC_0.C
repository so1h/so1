/* ----------------------------------------------------------------------- */
/*                                 inic_0.c                                */
/* ----------------------------------------------------------------------- */
/*                          proceso inicial de SO1                         */
/* ----------------------------------------------------------------------- */
/*               (fase 0 para lanzar los drivers del sistema)              */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                                      /* printf */
#include <so1pub.h\strings.h>                                   /* strcmpu */
#include "iniccode.h"                                            /* inic_0 */

void formato ( void )
{
    printf(" formato: INIC_0 [ numConsolas ticsPorRodaja | -h ] ") ;
}

void help ( void )
{
    printf(
        "\n"
        "\n"
    ) ;
    formato() ;
    printf(
        "\n"
        "\n"
        " proceso responsable de las inicializaciones de SO1 \n"
    ) ;
}

int valor ( const char * str )          /* str numero de a lo mas 4 cifras */
{                                                             /* 0 .. 9999 */
	int i = 0 ;
	char car ;
	word_t acum = 0 ;
	while (((car = str[i++]) != '\0') && (i < 4)) 
	    if (('0' <= car) && (car <= '9')) 
			acum = 10*acum + (car - '0') ;
		else
			return(-1) ;
	return(acum) ;	
}	

int main ( int argc, char * argv [ ] )
{
	int numConsolas ;
	int ticsPorRodaja ;
	
	if (argc == 3) {
		numConsolas = valor(argv[1]) ;
		ticsPorRodaja = valor(argv[2]) ;
        if ((numConsolas > 0) && (ticsPorRodaja > 0))    	
			
		    return(inic_0(numConsolas, ticsPorRodaja)) ;                 /**/
	}

    if ((argc == 2) && (!strcmpu(argv[1], "-h")))
    {
        if (getpid() != 1) help() ;
        return(0) ;
    }
    else
    {
        if (getpid() != 1) formato() ;
        return(-1) ;
    }
}