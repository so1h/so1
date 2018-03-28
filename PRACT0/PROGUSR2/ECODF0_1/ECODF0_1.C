/* ----------------------------------------------------------------------- */
/*                               ecodf0_1.c                                */
/* ----------------------------------------------------------------------- */
/*   programa que hace eco de la entrada estandar en la salida estandar    */
/*      (el programa termina tras 
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */
#include <so1pub.h\carsctrl.h>                                      /* ESC */
#include <so1pub.h\strings.h>                                   /* strcmpu */

int formato ( void )
{
    printf(" formato: ECODF0_1 [ -h ] ") ;
	return(-1) ;
}

int help ( void )
{
    printf(
	  ""                                                                 "\n"
	  ""                                                                 "\n"
	  " formato : ECODF0_1 [ -h ]"                                       "\n"
	  ""                                                                 "\n"
	  "      hace eco de STDIN en STDOUT (Esc = fin)"                    "\n"
	  ""                                                                 "\n"
	  " opciones:"                                                       "\n"
	  ""                                                                 "\n"
	  "      -h : muestra este help"                                     "\n"
	  ""                                                                 "\n"
	  " autor:"                                                          "\n"
	  ""                                                                 "\n"
	  "      Pedro Pablo Lopez Rodriguez (c) 2018"                       "\n"
	  ""                                                                 "\n"
	) ;
	return(0) ;
}

void ecodf0_1 ( void ) 
{
    char car ;	
	
    for ( ; /* car != ESC */ ; ) {
        
		read(STDIN, &car, 1) ;          /* se podria poner car = getchar() */
        
		if (car == ESC) break ;                         /* punto de salida */
		
		write(STDOUT, &car, 1) ;         /* se podria poner putchar(car) ; */
		if (car == '\n') 
		    write(STDOUT, (char *)&"\r", 1) ;           /* putchar('\r') ; */
	}
}

int main ( int argc, char * argv [ ] )
{
	if (argc == 1) 
	{
		write(STDOUT, (char *)&"\n\r", 2) ;            
		ecodf0_1() ;
		return(0) ;
	}
	else if ((argc == 2) && !strcmpu(argv[1], "-h")) return(help()) ;
	else return(formato()) ;
}