/* ----------------------------------------------------------------------- */
/*                                  inic.c                                 */
/* ----------------------------------------------------------------------- */
/*                          proceso inicial de SO1                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                                      /* printf */

#include <so1pub.h\strings.h>
#include <so1pub.h\scanner.h>

#include <so1.h\iniccode.h>

void formato ( void )
{
    printf(
        " formato: INIT [ -h ] "
    ) ;
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

void main ( int argc, char * argv [ ] )
{
    if (argc == 1) exit(inic()) ;
    else if ((argc == 2) && (iguales(argv[1], "-h")))
    {
        if (getpid() != 1) help() ;
        exit(0) ;
    }
    else
    {
        if (getpid() != 1) formato() ;
        exit(-1) ;
    }
}
