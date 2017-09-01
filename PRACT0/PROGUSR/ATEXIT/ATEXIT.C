/* ----------------------------------------------------------------------- */
/*                                 atexit.c                                */
/* ----------------------------------------------------------------------- */
/*                  programa de prueba de la macro atexit                  */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>              /* necesario para macro atexit */
#include <so1pub.h\stdio.h>                               /* printf, getch */
#include <so1pub.h\ajustusr.h>                             /* macro atexit */

void finish_1 ( void )
{ 
    printf("\n atexit: Adios mundo.") ;
    getch() ;
    printf("\n") ;
	exit(0) ;
}

void main ( void )
{
    atexit(finish_1) ;
    printf("\n\n Hola mundo.") ;
    getch() ;
    printf("\n") ;
}