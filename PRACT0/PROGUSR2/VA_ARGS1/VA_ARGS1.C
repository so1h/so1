/* ----------------------------------------------------------------------- */
/*                                va_args.c                                */
/* ----------------------------------------------------------------------- */
/*       programa que ilustra las funciones con argumentos variables       */
/*      (se utiliza la biblioteca escribir evitando el uso de printf)      */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\stdargs.h>         /* va_list, va_start, va_arg, va_end */
#include <so1pub.h\escribir.h> 
#include <so1pub.h\stdio.h>                                     /* getchar */ 

int funcion ( const char * format, ... ) ; 

#pragma argsused

int main ( void ) 
{

	funcion("%s%c%i%x%X%o%d%u%p%p", 
	        "uno", 'd', -1234, 0xabcd, 0xabcd, 1024, 123, 257, 0xabcd, 0x1234) ;		
	getchar() ;
	
	funcion("%lx%lX%li%lu%ld%li%lp%lp", 
	        0xabcdef01, 0xabcdef01, 
			12345678, 12345678, 12345678, -12345678, 
			MK_FP(0xABCD, 0x1234), MK_FP(0xEFEA, 0x4321)) ;		
	getchar() ;
	
	return(0) ;
}

int vfuncion ( const char * format, va_list args ) ; 

int funcion ( const char * format, ... ) 
{
    va_list aptr ;
    va_start(aptr, format) ;
    vfuncion(format, aptr) ;
    va_end(aptr) ;
    return(0) ;
}

int vfuncion ( const char * format, va_list args ) 
{
	char * ptr = (char *)format ;
	char cualificador ;
	int i = 1 ;                  /* format es el argumento 0 de la funcion */
	
    escribirStr(
	    "\n"
	    "\n"
		" (vfuncion) format = \""
	) ;
	escribirStr(format) ;
	escribirStr(
	    "\"\n"
	    "\n args = \""
	) ;
	escribirStr(args) ;
	escribirStr("\"\n") ;
	
	while (*ptr != '\0') 
	{
		if (*ptr++ != '%') continue ;
		cualificador = '\0' ;
		if (*ptr == 'l') 
		{ 
	        cualificador = 'l' ;
			ptr++ ;
		}
		escribirStr("\n *ptr = ") ;
		escribirCar(*ptr) ;
		escribirStr(" arg[") ; escribirInt(i++, 1) ; escribirStr("] = ") ;
		if (cualificador != 'l') 
		switch (*ptr) 
		{	
		    case 's' : escribirStr(va_arg(args, char *   ))    ; break ;
			case 'c' : escribirCar(va_arg(args, char     ))    ; break ;
			case 'i' : escribirInt(va_arg(args, int      ), 1) ; break ;
			case 'x' : va_arg(args, unsigned )                 ; break ;
            case 'X' : escribirHex(va_arg(args, unsigned ), 4) ; break ;
			case 'o' : va_arg(args, unsigned )                 ; break ;
			case 'd' : escribirDec(va_arg(args, unsigned ), 1) ; break ;
			case 'u' : escribirDec(va_arg(args, unsigned ), 1) ; break ;
		    case 'p' : escribirHex((unsigned)va_arg(args, void *), 4) ; break ;
            case 'n' : va_arg(args, unsigned )                 ; break ;
            default : continue ;
		}
		else                                        /* cualificador == 'l' */
		switch (*ptr) 
		{	
			case 'i' : escribirLInt(va_arg(args, long int ), 1)      ; break ;
			case 'x' : va_arg(args, unsigned long )                  ; break ;
            case 'X' : escribirLHex(va_arg(args, unsigned long ), 8) ; break ;
			case 'o' : va_arg(args, unsigned long )                  ; break ;
			case 'd' : escribirLDec(va_arg(args, unsigned long ), 1) ; break ;
			case 'u' : escribirLDec(va_arg(args, unsigned long ), 1) ; break ;
		    case 'p' : escribirPtr(va_arg(args, pointer_t ))         ; break ;
            default : continue ;
		}
	}

	escribirStr("\n") ;
    
	return(0) ;
}

