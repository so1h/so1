/* ----------------------------------------------------------------------- */
/*                                killpid.c                                */
/* ----------------------------------------------------------------------- */
/*               comando para enviar señales a los procesos                */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                             /* printf, getchar */
#include <so1pub.h\scanner.h>    /* inicScanner, obtenSimb, simb, s_numero */
#include <so1pub.h\strings.h>                                   /* strcmpu */

void formato ( void )
{
    printf(" formato: KILL [ [ -n ] pid | -h | -l ] ") ;
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
    	""                                                               "\n"
        " envia una senial a un proceso o grupo de procesos"             "\n"
        ""                                                               "\n"
        " opciones: (pid >= 0 ==> proceso, pid < 0 ==> grupo)"           "\n"
        ""                                                               "\n"
		"      -n : numero o nombre de senial (ver lista -l)"            "\n"
		"      -h : muestra este help"                                   "\n"
        "      -l : muestra la lista de seniales"                        "\n"
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

