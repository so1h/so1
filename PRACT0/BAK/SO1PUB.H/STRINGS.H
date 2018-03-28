/* ----------------------------------------------------------------------- */
/*                                strings.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef STRINGS_H
#define STRINGS_H

#include <so1pub.h\tipos.h>

int strlen ( const char far * str ) ;

char far * strcpy ( char far * destino, const char far * origen ) ;

char far * strncpy ( char far * destino, const char far * origen, word_t n ) ;

word_t strlcpy ( char far * destino, const char far * origen, word_t n ) ;

int strcmp ( const char far * str1, const char far * str2 ) ;

int strcmpu ( const char far * str1, const char far * str2 ) ;

int strncmp ( const char far * str1, const char far * str2, word_t n ) ;

int strncmpu ( const char far * str1, const char far * str2, word_t n ) ;

#endif /* STRINGS_H */

#if (FALSE) 

#define iguales(str1, str2) (!strcmp(str1, str2)) 

#define igualesHasta(str1, str2, n) (!strncmp(str1, str2, n)) 

#define igualesSalvoMayusculas(str1, str2) (!strcmpu(str1, str2)) 

#define igualesSalvoMayusculasHasta(str1, str2) (!strncmpu(str1, str2, n)) 

#endif