/* ----------------------------------------------------------------------- */
/*                                inic_end.c                               */
/* ----------------------------------------------------------------------- */
/*                          proceso inicial de SO1                         */
/* ----------------------------------------------------------------------- */
/*       (fase final de cierre de consolas tras un HALT o SHUTDOWN)        */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>        /* escribirStr, escribirInt, escribirLn */
#include <so1pub.h\strings.h>                                   /* strcmpu */
#include <so1pub.h\bios_0.h>            /* printStrBIOS ... esperarTicBIOS */
#include <so1pub.h\put.h>                 /* putCar, putLn, putStr, putDec */
#include <so1pub.h\interpre.h>                      /* interpretarComandos */

void formato ( void )
{
    printf(" formato: INIC_END [ numConsolas | -h ] ") ;
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

int inic_end ( word_t numConsolas, word_t timeout ) {

    int i ;
    int j ;
    int dfCon ;
    char nombre [9] ;

//    /* enviamos a todas las consolas el mensaje de terminacion inminente */

    close(STDOUT) ;            /* no va a usarse mas por parte del proceso */

    strcpy(nombre, "CONX") ;
    for ( i = 0 ; i < numConsolas ; i++ )
    {
        nombre[3] = '0' + i ;
        if ((dfCon = open(nombre, O_WRONLY)) > 0)
        {
            if (i > 0) putLn(dfCon) ;
            putStr(dfCon, "\n El sistema se cerrara en ") ;
            putDec(dfCon, timeout, 1) ;
            putStr(dfCon, " segundos ") ;
            close(dfCon) ;                  /* luego se vuelve a abrir */
        }
    }
    for ( j = 0 ; j < timeout ; j++ )
    {
        esperarTicsBIOS(18) ;
        for ( i = 0 ; i < numConsolas ; i++ )
        {
            nombre[3] = '0' + i ;
            if ((dfCon = open(nombre, O_WRONLY)) > 0)
            {
                putCar(dfCon, '.') ;
                close(dfCon) ;
            }
        }
    }

    if (getppid() == 0) exit(timeout) ;                /* fin proceso INIT */

    return(timeout) ;                                  /* fin funcion inic */

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
	char car ;
	int numConsolas ;
	int timeout ;
	
	if (argc == 3) {
		numConsolas = valor(argv[1]) ;
		timeout = valor(argv[2]) ;
        if ((numConsolas > 0) && (timeout > 0))    	
			
     		return(inic_end(numConsolas, timeout)) ;           
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
