/* ----------------------------------------------------------------------- */
/*                                 help.c                                  */
/* ----------------------------------------------------------------------- */
/*                     programa help para el sistema SO1                   */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>
#include <so1pub.h\strings.h>                                   /* iguales */

void formato ( void )
{
    escribirStr(" formato: ECODF0_1 [ -h ] ") ;
}

void help ( void )
{
    escribirStr(
	  "\n"
	  "\n"
	  " formato : ECODF0_1 [ -h ]                  \n"
	  "\n"
	  "      hace eco de STDIN en STDOUT (F = fin) \n"
	  "\n"
	  " opciones:                                  \n"
	  "\n"
	  "      -h : muestra este help                \n"
	  "\n"
	  " autor:                                     \n"
	  "\n"
	  "      Pedro Pablo Lopez Rodriguez (c) 2016  \n"
	  "\n"
	) ;
}


void main ( int argc, char * argv [ ] )
{
    char car ;
	if (argc == 1) 
	{
		write(STDOUT, (char *)&"\n\r", 2) ; 
	    do {
            read(STDIN, &car, 1) ;
	        write(STDOUT, &car, 1) ;
			if (car == '\n') 
				write(STDOUT, (char *)&"\r", 1) ; 
		}
	    while (car != 'F') ;
	    exit(0) ;
	}
	else if ((argc == 2) && 
	         ((iguales(argv[1], "-h") ||
              (iguales(argv[1], "-H"))))) 
	{
		help() ;
	}
	else 
	{
		formato() ;
		exit(1) ;
	}    	
}

