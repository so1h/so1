/* ----------------------------------------------------------------------- */
/*                                 sighello.c                              */
/* ----------------------------------------------------------------------- */
/*              infame programa hola mundo para el sistema SO1             */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>                                    /* pause */
#include <so1pub.h\stdio.h>                             /* printf, getchar */

int main ( void )
{
    printf("\n\n Hola mundo.") ;
//  getchar() ;
    pause() ;
    printf("\n") ;
	return(0) ;
}