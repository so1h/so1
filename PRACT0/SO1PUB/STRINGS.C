/* ----------------------------------------------------------------------- */
/*                                strings.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1pub.h\ctype.h>                                     /* toupper */
#include <so1pub.h\strings.h>

int strlen ( const char far * str ) 
{
	int i = 0 ;
    while (*str++ != '\0') i++ ;
    return(i) ;	
}

char far * strcpy ( char far * destino, const char far * origen ) 
{
    while (*origen != '\0')
        *destino++ = *origen++ ;
    *destino = '\0' ;
	return(destino) ;
}

char far * strncpy ( char far * destino, const char far * origen, word_t n ) 
{
   word_t i ;
   for ( i = 0 ; i < n && *origen != '\0' ; i++ ) *destino++ = *origen++ ;
   for ( ; i < n ; i++ ) *destino++ = '\0' ;
   return(destino-n) ;
}

word_t strlcpy ( char far * destino, const char far * origen, word_t n ) 
{
    int i = n ;
    while ((*origen != '\0') && (i-- > 0))
        *destino++ = *origen++ ;
    *destino = '\0' ;
	return(n-i) ;
}

int strcmp ( const char far * str1, const char far * str2 ) 
{
    while ((*str1 == *str2) && (*str1 != '\0')) {
        str1++ ;
        str2++ ;
    }
    return(((unsigned)*str1 - (unsigned)*str2)) ;
}

int strcmpu ( const char far * str1, const char far * str2 ) 
{
    while ((toupper(*str1) == toupper(*str2)) && (*str1 != '\0')) {
        str1++ ;
        str2++ ;
    }
    return(((unsigned)toupper(*str1) - (unsigned)toupper(*str2))) ;
}

int strncmp ( const char far * str1, const char far * str2, word_t n ) 
{
    word_t i = 0 ;
    while ((i < n) &&
           (*str1 == *str2) && (*str1 != (char)0)) {
        str1++ ;
        str2++ ;
        i++ ;
    }
	if (i < n) return(((unsigned)*str1 - (unsigned)*str2)) ; 
	else return(0) ;
}

int strncmpu ( const char far * str1, const char far * str2, word_t n ) 
{
    word_t i = 0 ;
    while ((i < n) &&
           (toupper(*str1) == toupper(*str2)) && (*str1 != (char)0)) {
        str1++ ;
        str2++ ;
        i++ ;
    }
	if (i < n) return(((unsigned)toupper(*str1) - (unsigned)toupper(*str2))) ; 
	else return(0) ;
}