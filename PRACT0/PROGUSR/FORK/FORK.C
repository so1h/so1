/* ----------------------------------------------------------------------- */
/*                                  fork.c                                 */
/* ----------------------------------------------------------------------- */
/*             programa de prueba de la llamada al sistema fork            */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>            /* escribirStr, escribirInt, ... */

#include <so1pub.h\debug.h>                               /* puntoDeParada */

void main ( void )
{
//  pid_t pid ;
    pid_t hpid ;
    char car ;
    int status ;

    escribirStr(
        "\n"
        "\n"
        " Prueba de fork. Se va a ejecutar fork() \n"
        "\n"
        " Pulse una tecla para continuar. \n"
        "\n"
    ) ;
    leer(STDIN) ;
//  puntoDeParada() ;                                   /* punto de Parada */
    hpid = fork() ;                                                /* fork */
    if (hpid == 0)
    {
        escribirStr("\n fork() me ha devuelto ") ;
        escribirInt(hpid, 1) ;
        escribirStr(
            " por lo que soy el proceso Hijo que se ha creado \n"
            "\n"
            " Pulse una tecla para continuar ('F' para terminar). \n"
            "\n"
        ) ;
        leer(STDIN) ;
        car = (char)0 ;
        while (car != 'F')
        {
            asm cli ;
            if (leerAsciiListo(STDIN) != (char)0)
                car = leer(STDIN) ;
            asm sti
            escribirCar('H') ;
        }
        escribirStr(
            "\n"
            "\n"
            " El Hijo ha terminado \n"
        ) ;
//      puntoDeParada() ;                               /* punto de Parada */
    }                                                              /* exit */
    else
    {
        escribirStr("\n fork() me ha devuelto ") ;
        escribirInt(hpid, 1) ;
        escribirStr(
            " por lo que soy el proceso Padre que ya existia \n"
            "\n"
            " Pulse una tecla para continuar ('F' para terminar). \n"
            "\n"
        ) ;
        leer(STDIN) ;
        car = (char)0 ;
        while (car != 'F')
        {
            asm cli
            if (leerAsciiListo(STDIN) != (char)0)
                car = leer(STDIN) ;
            asm sti
            escribirCar('P') ;
        }
        escribirStr(
            "\n"
            "\n"
            " El Padre ejecuta waitpid esperando a que termine el hijo \n"
        ) ;
//      puntoDeParada() ;                               /* punto de Parada */
        /* pid = */ waitpid(hpid, &status) ;                    /* waitpid */
        escribirStr(
            "\n"
            "\n"
            " El Padre ha terminado \n"
        ) ;
//      puntoDeParada() ;                               /* punto de Parada */
    }                                                              /* exit */
}

