/* ----------------------------------------------------------------------- */
/*                                killpid.c                                */
/* ----------------------------------------------------------------------- */
/*       comando para matar procesos (directamente sin enviar senial)      */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                             /* printf, getchar */
#include <so1pub.h\scanner.h>    /* inicScanner, obtenSimb, simb, s_numero */
#include <so1pub.h\strings.h>                                   /* strcmpu */

void formato ( void )
{
    printf(" formato: KILLPID [ pid | -a | -h ] ") ;
}

void help ( void )
{
    printf(
	    ""                                                               "\n"
	    ""                                                               "\n"
    ) ;
	formato() ;
    printf(
    	""                                                               "\n"
        " mata al proceso pid o a todos (-a)"                            "\n"
	) ;
}

int main ( int argc, char * argv [ ] )
{
    int i = 0 ;
    if (argc != 2) formato() ;
    else if (!strcmpu(argv[1], "-h")) help() ;
    else if (!strcmpu(argv[1], "-a")) killpid(-1) ;
    else
    {
        while (argv[1][i] != (char)0)
        {
            comando[0][i] = argv[1][i] ;
            i++ ;
        }
        comando[0][i] = (char)0 ;
        inicScanner() ;
        obtenSimb() ;
        if (simb == s_numero)
        {
            switch (killpid(num))
            {
            case -1 :
                printf(" no se permite matar al proceso 0 ") ;
				return(-1) ;
//              break ;
            case -2 :
                printf(" no existe ningun proceso con ese pid ") ;
				return(-2) ;
//              break ;
            }
        }
        else { 
		    formato() ;
			return(1) ;
		}
    }
//	getchar() ;
	return(0) ;
}

