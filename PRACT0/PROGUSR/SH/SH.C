/* ----------------------------------------------------------------------- */
/*                                  sh.c                                   */
/* ----------------------------------------------------------------------- */
/*                     interprete de comandos para so1                     */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                                      /* printf */

#include <so1pub.h\interpre.h>                      /* interpretarComandos */
#include <so1pub.h\saludos.h>
#include <so1pub.h\strings.h>

void formato ( void )
{
    /*  escribirStrIntenso(" formato: SH [ CONX | COMx | -h ] ") ; */
    printf(" formato: SH [ CONX | COMx | -h ] ") ;
}

void help ( void )
{
    printf(
        "\n"
        "\n"
        " formato: SH [ CONx | COMx | -h ]                  \n"
        "\n"
        " crea un nuevo interprete de comandos cuya entrada \n"
        " y salida estandar se redirigen al dispositivo que \n"
        " se indica como argumento.                         \n"
    ) ;
}

char strStd [  ] [ 16 ] =
{
    "entrada",
    "salida",
    "salida de error"
} ;

void main ( int argc, char * argv [ ] )
{

    char nombreDispositivo [12] ;
    int numConsola ;
    pid_t pid ;
    int res ;

    if (argc == 2)
    {
        if (iguales(argv[1], "-h" ))
        {
            help() ;
            return ;
        }
        else
        {
            copiarStr(argv[1], nombreDispositivo) ;
            res = redirigirSTDIO((char *)nombreDispositivo) ;
            switch (res)
            {
            case  0 :
                break ;
            case -1 :
                ;
            case -2 :
                ;
            case -3 : /* escribirStrIntenso(" no se ha podido cerrar la ") ; */
                printf(" no se ha podido cerrar la ") ;
                printf(strStd[-res-1]) ;
                printf(" estandar ") ;
                exit(res) ;
            case -4 :
                ;
            case -5 :
                ;
            case -6 :
                ;
            case -7 : /* escribirStrIntenso(" no se ha podido abrir \"") ; */
                printf(" no se ha podido abrir \"") ;
                /* escribirStrIntenso(nombreDispositivo) ; */
                printf((char *)nombreDispositivo) ;
                /* escribirStrIntenso("\"") ; */
                printf("\"") ;
                if (res != -4)
                {
                    /* escribirStrIntenso(" como ") ; */
                    printf(" como ") ;
                    printf(strStd[-res-5]) ;
                    printf(" estandar ") ;
                }
                exit(res) ;
            default :
                formato() ;
                exit(-8) ;
            }
        }
    }
    else if (argc > 2)
    {
        formato() ;
        return ;
    }
    /*
        if (iguales(nombreDispositivo, "CON"))
            ventanaPropia() ;
    */

    if (argc == 1) printf("\n") ;                /* sólo por pura estética */

    mostrarSaludo() ;
    res = interpretarComandos() ;
    if (getppid() != 1) exit(res) ;                 /* el padre no es inic */
    if ((pid = createProcess("CONSOLA", "consola -n")) < 0)
    {
        if (getpid() == getppid())       /* SH de los creados al principio */
            exec("LOGIN", "login") ;   /* y no uno creado desde la consola */
    }
    else
    {
        waitpid(pid, (int far *)&numConsola) ;
        if (numConsola > 0)                /* SH creado en CON1, CON2, ... */
            exec("LOGIN", "login") ;   /* y no uno creado desde la consola */
    }
    exit(res) ;

}

