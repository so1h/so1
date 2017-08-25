/* ----------------------------------------------------------------------- */
/*                                  hello.c                                */
/* ----------------------------------------------------------------------- */
/*                 programa hola mundo para el sistema SO1                 */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\stdio.h>                               /* printf, getch */

#include <so1pub.h\usrdbg.h>                   /* llamarAlPuntoDeParadaSO1 */

void main ( void )
{
    llamarAlPuntoDeParadaSO1() ;

    printf("\n\n Hola mundo.") ;
    getch() ;
    printf("\n") ;
}

