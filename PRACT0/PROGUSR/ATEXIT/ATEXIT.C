/* ----------------------------------------------------------------------- */
/*                                 atexit.c                                */
/* ----------------------------------------------------------------------- */
/*                  programa de prueba de la macro atexit                  */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>
#include <so1pub.h\ajustusr.h>                             /* macro atexit */

void finish_1 ( void )
{
    escribirStr(
        "\n"
        "\n"
        " atexit: Adios mundo."
    ) ;
    leer(STDIN) ;
    escribirLn() ;
    exit(0) ;
}

void main ( void )
{
    atexit(finish_1) ;
    escribirStr(
        "\n"
        "\n"
        " Hola mundo."
    ) ;
    leer(STDIN) ;
}

