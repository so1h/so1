/* ----------------------------------------------------------------------- */
/*                                  inic.c                                 */
/* ----------------------------------------------------------------------- */
/*                          proceso inicial de SO1                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                                      /* printf */
#include <so1pub.h\strings.h>                                   /* strcmpu */
#include <so1.h\iniccode.h>                                        /* inic */

void formato ( void )
{
    printf(" formato: INIC [ -h ] ") ;
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

int main ( int argc, char * argv [ ] )
{
    if (argc == 1) return(inic()) ;
    else if ((argc == 2) && (!strcmpu(argv[1], "-h")))
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
