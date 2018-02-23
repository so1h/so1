/* ----------------------------------------------------------------------- */
/*                                 login.c                                 */
/* ----------------------------------------------------------------------- */
/*              programa para dar entrada a una sesion en SO1              */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                             /* printf, putchar */
#include <so1pub.h\saludos.h>                       /* mostrarSaludoGrande */
#include <so1pub.h\scanner.h>                     /* inicScanner, obtenStr */
#include <so1pub.h\leerCmd.h>                                   /* leerCmd */
#include <so1pub.h\strings.h>                                   /* iguales */

int main ( void )
{
    char login [ tamComando ] ;
    char password [ tamComando ] ;
    int dfTimer ;
    int i ;

    for ( ; ; ) 
    {
        putchar('\f') ;                               /* borra la pantalla */
        mostrarSaludoGrande() ;
		
        printf(
            ""                                                           "\n"
            ""                                                           "\n"
            " so1 login: "
		) ;
        leerComando(FALSE, FALSE) ;
        for ( i = 0 ; i < tamComando ; i++ )
            login[i] = comando[inCmd][i] ;
//      printf("\n comando[inCmd] = %s login = %s", comando[inCmd], login) ;

        printf(
		    ""                                                           "\n"
		    " password: "
		) ;
        leerComando(FALSE, TRUE) ;
        for ( i = 0 ; i < tamComando ; i++ )
            password[i] = comando[inCmd][i] ;
//      printf("\n comando[inCmd] = %s password = %s", comando[inCmd], password) ;

        if (iguales(login, "usuario") && iguales(password, "alumno"))
        {
            setuid(1) ;
            setgid(1) ;
            putchar('\f') ;                           /* borra la pantalla */
            exec("SH", "sh") ;
        }
        else if (iguales(login, "root") && iguales(password, "toor"))
        {
            setuid(0) ;
            setgid(0) ;
            putchar('\f') ;                           /* borra la pantalla */
            exec("SH", "sh") ;
        }
        else
        {
            printf(
                ""                                                       "\n"
                ""                                                       "\n"
                "   identificacion incorrecta ..."
			) ;
            if ((dfTimer = open("TIMER", O_RDONLY)) >= 0)
            {
                read(dfTimer, MK_FP(0000,0000), 36) ; /* esperamos 36 tics */
                close(dfTimer) ;
            }
        }
    }
}

