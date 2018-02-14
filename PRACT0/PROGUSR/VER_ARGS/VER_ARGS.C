/* ----------------------------------------------------------------------- */
/*                                ver_args.c                               */
/* ----------------------------------------------------------------------- */
/*                  programa ver_args para el sistema SO1                  */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\stdio.h>                             /* printf, getchar */

int main ( int argc, char * argv [ ] ) 
{	
    int i ;
	
	printf(
	    ""                                                               "\n"
		""                                                               "\n"
		" VER_ARGS: muestra los argumentos argc y argv de main"          "\n"
		""                                                               "\n"
        "     argc = %i"                                                 "\n", 
		argc
	) ;
    
	for ( i = 0 ; i < argc ; i++ )
	    printf(
		    ""                                                           "\n"
		    "     argv[%i] = \"%s\" ", i, argv[i]
		) ;

	getchar() ;	
	printf("\n") ;
	return(0) ;
}