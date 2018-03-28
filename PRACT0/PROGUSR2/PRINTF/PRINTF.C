/* ----------------------------------------------------------------------- */
/*                                 printf.c                                */
/* ----------------------------------------------------------------------- */
/*           programa para comprobar el funcionamiento de printf           */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\stdio.h>                             /* printf, getchar */

int main ( void )
{
	printf(
        ""                                                               "\n"
	    ""                                                               "\n"
	    " Programa para comprobar el funcionamiento de printf"           "\n"
        ""                                                               "\n"
	    " s: \"%s\""                                                     "\n"
		" c: \'%c\'"                                                     "\n"
		" i: %i"                                                         "\n"
		" i: %i"                                                         "\n" 
		" x: %x"                                                         "\n"
		" X: %X"                                                         "\n"
		" o: %o"                                                         "\n"
		" d: %d"                                                         "\n"
		" u: %u"                                                         "\n"
		" p: %p"                                                         "\n" 
		" p: %p"                                                         "\n", 
	    "uno", 'd', 1234, -1234, 0xabcd, 0xabcd, 1024, 123, 257, 0xabcd, 0x12ba
	) ;		
//	getchar() ;	
	printf(
	    ""                                                               "\n"
		" li: %li"                                                       "\n"  
		" li: %li"                                                       "\n"  
		" lx: %lx"                                                       "\n"  
		" lX: %lX"                                                       "\n" 
		" lo: %lo"                                                       "\n" 
		" ld: %ld"                                                       "\n"  
		" lu: %lu"                                                       "\n", 
		12345678, -12345678,
        0xabcdef01, 0xabcdef01, 
		012345670, 12345678, 12345678
	) ;	
//	getchar() ;
	return(0) ;
}