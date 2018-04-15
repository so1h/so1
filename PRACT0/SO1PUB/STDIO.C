/* ----------------------------------------------------------------------- */
/*                                  stdio.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdargs>           /* va_list, va_start, va_arg, va_end */

//#include <so1pub.h\escribir.h>                            /* escribirStr */

int putchar ( char car ) 
{
    write(STDOUT, (pointer_t)&car, 1) ; 
	return(0) ;
}	

extern int vsprintf ( char far * buf, const char far * fmt, va_list args ) ;

static char bufer [ 2000 ] ;	

static int writeStr ( const char * str )        /* sustituye a escribirStr */
{
    char car ;
    int cont = 0 ;
    while ((car = *str++) != (char)0) 
	{
        if (car == '\n') 
	    {
	        putchar('\r') ;
			cont++ ; 
		}
        putchar(car) ; 
		cont++ ;
    }
    return(cont) ;
}

int sprintf ( char far * str, const char far * fmt, ... ) 
{
	int len ;
    va_list aptr ;
    va_start(aptr, fmt) ;
    len = vsprintf(str, fmt, aptr) ;
    va_end(aptr) ;
	return(len) ;
}

char far * strprintf ( const char far * fmt, ... ) 
{
    va_list aptr ;
    va_start(aptr, fmt) ;
    vsprintf(bufer, fmt, aptr) ;
    va_end(aptr) ;
	return((char far *)&bufer) ;
}

int printf ( const char far * fmt, ... ) 
{
    int ret ;
    va_list aptr ;
    va_start(aptr, fmt) ;
    ret = vsprintf(bufer, fmt, aptr) ;
    va_end(aptr) ;
//	escribirStr(bufer) ;
	writeStr(bufer) ;
    return(ret) ;	
}

char getchar ( void )
{
    return(leer(STDIN)) ;
}

