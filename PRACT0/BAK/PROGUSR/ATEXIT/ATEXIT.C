/* ----------------------------------------------------------------------- */
/*                                 atexit.c                                */
/* ----------------------------------------------------------------------- */
/*                  programa de prueba de la macro atexit                  */
/*       que permite disponer una funcion que se ejecute tras el main      */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>              /* necesario para macro atexit */
#include <so1pub.h\ajustusr.h>                             /* macro atexit */
#include <so1pub.h\stdio.h>                             /* printf, getchar */

int finish_1 ( void )  /* funcion que queremos que se ejecute tras el main */
{ 
    printf("\n atexit: Adios mundo.") ;
    getchar() ;
    printf("\n") ;
	return(1234) ;        /* codigo de terminacion devuelto por el comando */
}

int main ( void )
{
    printf("\n\n Hola mundo.") ;
	getchar() ;
    atexit(finish_1) ;
    printf("\n\n Nueva funcion final: atexit(finish_1).") ;
	getchar() ;
    printf("\n") ;
	return(0) ;
}