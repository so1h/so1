/* ----------------------------------------------------------------------- */
/*                                  time.c                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

void time ( void )
{
    int df = open("RELOJ", O_RDONLY) ;
    char strHora [9] ;
    if (df < 0) escribirStr(" recurso RELOJ no disponible ") ;
    else
    {
        strHora[8] = (char)0 ;
        read(df, (pointer_t)&strHora, 8) ;
        escribirStr(" ") ;
        escribirStr((char *)strHora) ;
        close(df) ;
    }
}

void main ( void )
{
    time() ;
}


