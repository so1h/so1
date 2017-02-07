/* ----------------------------------------------------------------------- */
/*                                  kill.c                                 */
/* ----------------------------------------------------------------------- */
/*                       Un primer sistema operativo                       */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\scanner.h>
#include <so1pub.h\strings.h>

void formato ( void )
{
//  escribirStrIntenso(" formato: KILL [ pid | -a | -h ] ") ;
    escribirStr(" formato: KILL [ pid | -a | -h ] ") ;
}

void help ( void )
{
    escribirLn() ;
    escribirLn() ;
    escribirStr(" formato: KILL [ pid | -a | -h ]    \n\n") ;
    escribirStr(" mata al proceso pid o a todos (-a) \n") ;
}

void main ( int argc, char * argv [ ] )
{
    int i = 0 ;
    if (argc != 2) formato() ;
    else if (iguales(argv[1], "-h")) help() ;
    else if (iguales(argv[1], "-a")) kill(-1) ;
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
            switch (kill(num))
            {
            case -1 :
//              escribirStrIntenso(" no se permite matar al proceso 0 ") ;
                escribirStr(" no se permite matar al proceso 0 ") ;
                break ;
            case -2 :
//              escribirStrIntenso(" no existe ningun proceso con ese pid ") ;
                escribirStr(" no existe ningun proceso con ese pid ") ;
                break ;
            }
        }
        else formato() ;
    }
}

