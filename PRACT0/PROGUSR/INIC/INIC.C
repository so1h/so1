/* ----------------------------------------------------------------------- */
/*                                  inic.c                                 */
/* ----------------------------------------------------------------------- */
/*                          proceso inicial de SO1                         */
/* ----------------------------------------------------------------------- */
/*       (consola del sistema en CON0, termina con HALT o SHUTDOWN)        */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>        /* escribirStr, escribirInt, escribirLn */
#include <so1pub.h\strings.h>                                   /* strcmpu */
#include <so1pub.h\bios_0.h>            /* printStrBIOS ... esperarTicBIOS */
#include <so1pub.h\put.h>                 /* putCar, putLn, putStr, putDec */
#include <so1pub.h\interpre.h>                      /* interpretarComandos */

void formato ( void )
{
    printf(" formato: INIC [ numConsolas | -h ] ") ;
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

int inic ( int numConsolas ) {

    int i ;
    int j ;
    int dfCon ;
    int status ;
    int timeout ;
    char nombre [9] ;
    char strArg [16] ;

    status = interpretarComandos() ;                              /* shell */

#if (TRUE)	
	sprintf(strArg, "INIC_END %i %i", numConsolas, status) ;
	exec("INIC_END", strArg) ;
	return(0) ;
#else 	
//    /* enviamos a todas las consolas el mensaje de terminacion inminente */

    timeout = status ;
    close(STDOUT) ;            /* no va a usarse mas por parte del proceso */

    if (timeout >= 0)
    {
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
    }

    if (getppid() == 0) exit(timeout) ;                /* fin proceso INIT */

    return(timeout) ;                                  /* fin funcion inic */
#endif
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
	
	if (argc == 2) {
		numConsolas = valor(argv[1]) ;
		if (numConsolas > 0) 
			
     		return(inic(numConsolas)) ;               /* inic(numConsolas) */
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