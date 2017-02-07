/* ----------------------------------------------------------------------- */
/*                                  exec.c                                 */
/* ----------------------------------------------------------------------- */
/*             programa de prueba de la llamada al sistema exec            */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\debug.h>                               /* puntoDeParada */

void main ( void )
{
    int err ;
    escribirStr(
        "\n"
        "\n"
//      " Prueba de exec. Se va a ejecutar err = exec(\"HELP\", \"help\") \n"
//      " Prueba de exec. Se va a ejecutar err = exec(\"AYUDA\", \"ayuda\") \n"
        " Prueba de exec. Se va a ejecutar err = exec(\"SH\", \"sh\") \n"
        "\n"
        " Pulse una tecla para continuar. "
    ) ;
    leer(STDIN) ;
    puntoDeParada() ;                                              /* exec */
//  err = exec("HELP", "help") ;                /* cabe en el proceso EXEC */
//  err = exec("AYUDA", "ayuda") ;           /* no cabe en el proceso EXEC */
    err = exec("SH", "sh") ;                 /* no cabe en el proceso EXEC */
    escribirStr(
        " No se ha podido llevar a cabo con exito el exec \n"
        " y sigue ejecutandose el proceso original \n"
        "\n"
        " el codigo de error devuelto ha sido err = "
    ) ;
    escribirInt(err, 1) ;
    escribirStr(
        "\n"
        " Pulse una tecla para terminar ... \n"
    ) ;
    leer(STDIN) ;
    escribirLn() ;
}

