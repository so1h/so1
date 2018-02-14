/* ----------------------------------------------------------------------- */
/*                                  time.c                                 */
/* ----------------------------------------------------------------------- */
/*                   programa que muestra la hora actual                   */
/*                      (leyendola del fichero TIMER)                      */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>
#include <so1pub.h\stdio.h>                             /* printf, getchar */

void time ( void )
{
    int df = open("RELOJ", O_RDONLY) ;
    char strHora [9] ;
    if (df < 0) printf(" recurso RELOJ no disponible ") ;
    else
    {
        strHora[8] = (char)0 ;
        read(df, (pointer_t)&strHora, 8) ;
        close(df) ;
        printf(" %s", strHora) ;
    }
}

int main ( void )
{
    time() ;
//	getchar() ;
	return(0) ;
}


