/* ----------------------------------------------------------------------- */
/*                                 login.c                                 */
/* ----------------------------------------------------------------------- */
/*              programa para dar entrada a una sesion en SO1              */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                                      /* printf */
#include <so1pub.h\saludos.h>                       /* mostrarSaludoGrande */
#include <so1pub.h\scanner.h>                     /* inicScanner, obtenStr */
#include <so1pub.h\leerCmd.h>                                   /* leerCmd */
#include <so1pub.h\strings.h>                                   /* iguales */
#include <so1pub.h\msdos.h>                                     /* leerCmd */

#include <so1pub.h\debug.h>                               /* puntoDeParada */

void main ( void )
{

    char login [ tamComando ] ;
    char password [ tamComando ] ;
    int dfTimer ;
    int i ;

    do
    {
        printf("\f") ;
        mostrarSaludoGrande() ;
        printf(
            "\n"
            "\n"
            " so1 login: ") ;
        leerComando(FALSE, FALSE) ;
        for ( i = 0 ; i < tamComando ; i++ )
            login[i] = comando[inCmd][i] ;
//      escribirStr("\n comando[inCmd] = ") ;
//      escribirStr(comando[inCmd]) ;
//      escribirStr(" login = ") ;
//      escribirStr(login) ;

        printf("\n password: ") ;
        leerComando(FALSE, TRUE) ;
        for ( i = 0 ; i < tamComando ; i++ )
            password[i] = comando[inCmd][i] ;
//      escribirStr("\n comando[inCmd] = ") ;
//      escribirStr(comando[inCmd]) ;
//      escribirStr(" password = ") ;
//      escribirStr(password) ;
        if (iguales(login, "usuario") && iguales(password, "alumno"))
        {
            setuid(1) ;
            setgid(1) ;
            printf("\f") ;
            exec("SH", "sh") ;
        }
        else if (iguales(login, "root") && iguales(password, "toor"))
        {
            setuid(0) ;
            setgid(0) ;
            printf("\f") ;
            exec("SH", "sh") ;
        }
        else
        {
            printf(
                "\n"
                "\n"
                "   identificacion incorrecta ...") ;
            if ((dfTimer = open("TIMER", O_RDONLY)) >= 0)
            {
                read(dfTimer, MK_FP(0000,0000), 36) ; /* esperamos 36 tics */
                close(dfTimer) ;
            }
        }
    }
    while (TRUE) ;
}

