/* ----------------------------------------------------------------------- */
/*                                  sh.c                                   */
/* ----------------------------------------------------------------------- */
/*                     interprete de comandos para so1                     */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */

#include <so1pub.h\interpre.h>                      /* interpretarComandos */
#include <so1pub.h\saludos.h>
#include <so1pub.h\strings.h>                           /* strcpy, strcmpu */

void formato ( void )
{
    printf(" formato: SH [ CONX | COMx | -h ] ") ;
}

void help ( void )
{
    printf(
        ""                                                               "\n"
        ""                                                               "\n"
        " formato: SH [ CONx | COMx | -h ]"                              "\n"
        ""                                                               "\n"
        " crea un nuevo interprete de comandos cuya entrada"             "\n"
        " y salida estandar se redirigen al dispositivo que"             "\n"
        " se indica como argumento."                                     "\n"
    ) ;
}

char strStd [  ] [ 16 ] =
{
    "entrada",
    "salida",
    "salida de error"
} ;

int main ( int argc, char * argv [ ] )
{
    char nombreDispositivo [12] ;
    int numConsola ;
    pid_t pid ;
    int res ;

    if (argc == 2)
    {
        if (!strcmpu(argv[1], "-h" ))
        {
            help() ;
            return(0) ;
        }
        else
        {
            strcpy(nombreDispositivo, argv[1]) ;
            res = redirigirSTDIO((char *)nombreDispositivo) ;
            switch (res)
            {
            case  0 :
                break ;
            case -1 :
                ;
            case -2 :
                ;
            case -3 : 
                printf(" no ha podido cerrarse la %s estandar ", strStd[-res-1]) ;
                return(res) ;
            case -4 :
                ;
            case -5 :
                ;
            case -6 :
                ;
            case -7 : 
                printf(" no ha podido abrirse \"%s\"", nombreDispositivo) ;
                if (res != -4)
                    printf(" como %s estandar ", strStd[-res-5]) ;
                return(res) ;
            default :
                formato() ;
                return(-8) ;
            }
        }
    }
    else if (argc > 2)
    {
        formato() ;
        return(1) ;
    }
    /*
        if (iguales(nombreDispositivo, "CON"))
            ventanaPropia() ;
    */

    if (argc == 1) printf("\n") ;                /* sólo por pura estética */

    mostrarSaludo() ;
	
    res = interpretarComandos() ;        /* interpretacion de los camandos */
	
    if (getppid() != 1)                             /* el padre no es inic */
		return(res) ;                                   /* fin del proceso */             
		
    if ((pid = createProcess("CONSOLA", "consola -nq")) < 0)      /* falla */
    {
        if (getpid() == getpindx())      /* SH de los creados al principio */
            exec("LOGIN", "login") ;   /* y no uno creado desde la consola */
    }
    else                                      /* consola -nq no ha fallado */
    {
        waitpid(pid, (int far *)&numConsola) ;
        if (numConsola > 0)                /* SH creado en CON1, CON2, ... */
            exec("LOGIN", "login") ;   /* y no uno creado desde la consola */
    }
    return(res) ;

}