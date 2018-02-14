/* ----------------------------------------------------------------------- */
/*                                  exec.c                                 */
/* ----------------------------------------------------------------------- */
/*             programa de prueba de la llamada al sistema exec            */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                             /* printf, getchar */

#include <so1pub.h\debug.h>                               /* puntoDeParada */

int main ( void )
{
    int err ;
    printf(
        ""                                                                 "\n"
        ""                                                                 "\n"
//      " Prueba de exec. Se va a ejecutar err = exec(\"HELP\", \"help\")   \n"
//      " Prueba de exec. Se va a ejecutar err = exec(\"AYUDA\", \"ayuda\") \n"
        " Prueba de exec. Se va a ejecutar err = exec(\"SH\", \"sh\")       \n"
        ""                                                                 "\n"
        " Pulse una tecla para continuar. "
    ) ;
	getchar() ;
    puntoDeParada() ;                                              /* exec */
//  err = exec("HELP", "help") ;              /* "cabe" en el proceso EXEC */
//  err = exec("AYUDA", "ayuda") ;         /* no "cabe" en el proceso EXEC */
    err = exec("SH", "sh") ;               /* no "cabe" en el proceso EXEC */
    printf(
        " No ha podido llevarse a cabo con exito el exec, por"             "\n"
        " lo que sigue ejecutandose el proceso original"                   "\n"
        ""                                                                 "\n"
        " El codigo de error devuelto ha sido err = %i"                    "\n"
        ""                                                                 "\n"
        " Pulse una tecla para terminar ..."                               "\n",
		err
    ) ;
    getchar() ;
    printf("\n") ;
	return(0) ;
}

