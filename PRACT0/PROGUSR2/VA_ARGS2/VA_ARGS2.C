/* ----------------------------------------------------------------------- */
/*                               va_args2.c                                */
/* ----------------------------------------------------------------------- */
/*       programa que ilustra las funciones con argumentos variables       */
/*                          (se utiliza printf)                            */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\stdargs.h>         /* va_list, va_start, va_arg, va_end */
#include <so1pub.h\stdio.h>                               /* printf, getch */ 

int funcion ( const char * format, ... ) ; 

int main ( void ) 
{

	funcion("%s%c%i%x%X%o%d%u%p%p", 
	        "uno", 'd', -1234, 0xabcd, 0xabcd, 1024, 123, 257, 0xabcd, 0x1234) ;		
	getchar() ;
	
	funcion("%lx%lX%li%lu%ld%li", 
	        0xabcdef01, 0xabcdef01, 
			12345678, 12345678, 12345678, -12345678) ;		
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
	
    printf(
	    "\n"
	    "\n"
		" (vfuncion) format = \"%s\"\n"
	    "\n"
		" args = \"%s\"\n", format, args
	) ;
	
	while (*ptr != '\0') 
	{
		if (*ptr++ != '%') continue ;
		cualificador = '\0' ;
		if (*ptr == 'l') 
		{ 
	        cualificador = 'l' ;
			ptr++ ;
		}
		printf("\n *ptr = %c arg[%i] = ", *ptr, i++) ;
		if (cualificador != 'l') 
		switch (*ptr) 
		{	
		    case 's' : printf("%s", va_arg(args, char *   )) ; break ;
			case 'c' : printf("%c", va_arg(args, char     )) ; break ;
			case 'i' : printf("%i", va_arg(args, int      )) ; break ;
			case 'x' : printf("%x", va_arg(args, unsigned )) ; break ;
            case 'X' : printf("%X", va_arg(args, unsigned )) ; break ;
			case 'o' : printf("%o", va_arg(args, unsigned )) ; break ;
			case 'd' : printf("%d", va_arg(args, unsigned )) ; break ;
			case 'u' : printf("%u", va_arg(args, unsigned )) ; break ;
		    case 'p' : printf("%p", va_arg(args, void *   )) ; break ;
            case 'n' : printf("%n", va_arg(args, unsigned )) ; break ;
            default : continue ;
		}
		else                                        /* cualificador == 'l' */
		switch (*ptr) 
		{	
			case 'i' : printf("%li", va_arg(args, long int      )) ; break ;
			case 'x' : printf("%lx", va_arg(args, unsigned long )) ; break ;
            case 'X' : printf("%lX", va_arg(args, unsigned long )) ; break ;
			case 'o' : printf("%lo", va_arg(args, unsigned long )) ; break ;
			case 'd' : printf("%ld", va_arg(args, unsigned long )) ; break ;
			case 'u' : printf("%lu", va_arg(args, unsigned long )) ; break ;
            default : continue ;
		}
	}

    printf("\n") ;
    
	return(0) ;
}

#if (0)

#include <so1pub.h\stdio.h>                               /* printf, getch */
#include <so1pub.h\stdargs.h>         /* va_list, va_start, va_arg, va_end */

#include <so1pub.h\escribir.h> 

int funcion ( const char * format, ... ) ; 

int main ( ) 
{
	int i ;

    printf("\n argc = %i\n", argc) ;
	for ( i = 0 ; i < argc ; i++ ) {
	    printf("\n argv[%i] = \"%s\"", i, argv[i]) ;
	}
	printf("\n") ;
	
	funcion("%s%c%i%x%X%o%d%u%p", 
	        "uno", 'd', -1234, 0xabcd, 0xabcd, 1024, 123, 257, argv) ;		
	getchar() ;
	
	funcion("%s%c%i%x%X%o%d%u%p", 
	        "uno", 'd', -12345678, 0xabcdef01, 0xabcdef01, 1024, -123, 257, argv) ;		
	getchar() ;
	
	funcion("%n%s", 
	        "nada", "todo") ;
	getchar() ;
	
	funcion("%s%c%i%x%X%o%d%u%p%n%s", 
	        "uno", 'd', -12345678, 0xabcdef01, 0xabcdef01, 1024, -123, 257, argv, "nada", "todo") ;

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

char formato_0 [ 80 ] = "\n arg[%i] = %s" ; 

int vfuncion ( const char * format, va_list args ) 
{
	char * ptr = (char *)format ;
//	char formato [ 80 ] = "\n arg[%i] = %s" ; 
	char formato [ 80 ] ;
	int i, j = 0 ;
	
    for ( j = 0 ; j < 80 ; j++ ) 
		formato[j] = formato_0[j] ; 

    printf("\n vfuncion\n") ;
	
    printf("\n format = \"%s\"\n", format) ;  	

    printf("\n args = \"%s\"\n", args) ;  
	
	while (*ptr != '\0') 
	{
		if (*ptr++ != '%') continue ;
//		printf("\n *ptr = %c ", *ptr) ;
		switch (*ptr) {
		    case 's' :
			case 'c' :
			case 'i' : 
			case 'x' :
            case 'X' : 
			case 'o' : 
			case 'd' : 
			case 'u' :
		    case 'p' :
            case 'n' : 
         		formato[13] = *ptr++ ;
//				printf(" car = %c ", formato[13]) ;
                printf(formato, i++, va_arg(args, char *)) ;
                break ;				
            default : continue ;
		}
	}

	printf("\n") ;
    
	return(0) ;
}

#endif
