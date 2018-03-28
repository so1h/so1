/* ----------------------------------------------------------------------- */
/*                                  fork.c                                 */
/* ----------------------------------------------------------------------- */
/*             programa de prueba de la llamada al sistema fork            */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */
#include <so1pub.h\carsctrl.h>                                      /* ESC */

#include <so1pub.h\debug.h>                               /* puntoDeParada */

int main ( void )
{
//  pid_t pid ;
    pid_t hpid ;
    char car ;
    int status ;

    printf(
        ""                                                               "\n"
        ""                                                               "\n"
        " Prueba de fork. Se va a ejecutar fork()"                       "\n"
        ""                                                               "\n"
        " Pulse una tecla para continuar."                               "\n"
        ""                                                               "\n"
    ) ;
    
	getchar() ;
	
    puntoDeParada() ;                                   /* punto de Parada */
	
    hpid = fork() ;                                                /* fork */
    
	if (hpid == 0)
    {
        printf(
		    ""                                                           "\n"
		    " fork() me ha devuelto %i por lo que soy el proceso Hijo que se ha creado \n"
            ""                                                           "\n"
            " Pulse una tecla para continuar (ESC para terminar)."       "\n"
            ""                                                           "\n",
			hpid
        ) ;
		getchar() ;
		
        car = '\0' ;
        while (car != ESC)
        {
            asm cli ;
            if (leerAsciiListo(STDIN) != '\0')
                car = getchar() ;
            asm sti
            putchar('H') ;
        }
		
        printf(
            ""                                                           "\n"
            ""                                                           "\n"
            " El Hijo ha terminado"                                      "\n"
        ) ;
//      puntoDeParada() ;                               /* punto de Parada */
    }                                                              /* exit */
    else
    {
        printf(
		    ""                                                           "\n"
		    " fork() me ha devuelto %i por lo que soy el proceso Padre que ya existia \n"
            ""                                                           "\n"
            " Pulse una tecla para continuar (ESC para terminar)."       "\n"
            ""                                                           "\n",
            hpid
        ) ;
		getchar() ;
		
        car = '\0' ;
        while (car != ESC)
        {
            asm cli
            if (leerAsciiListo(STDIN) != '\0')
                car = getchar() ;
            asm sti
            putchar('P') ;
        }
		
        printf(
            ""                                                           "\n"
            ""                                                           "\n"
            " El Padre ejecuta waitpid esperando a que termine el hijo"  "\n"
        ) ;
		
//      puntoDeParada() ;                               /* punto de Parada */

        /* pid = */ waitpid(hpid, &status) ;                    /* waitpid */
		
        printf(
            ""                                                           "\n"
            ""                                                           "\n"
            " El Padre ha terminado"                                     "\n"
        ) ;
		
//      puntoDeParada() ;                               /* punto de Parada */
    }                                                              /* exit */
	return(0) ;
}

