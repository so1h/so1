/* ----------------------------------------------------------------------- */
/*                                strings.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef STRINGS_H
#define STRINGS_H

#include <so1pub.h\tipos.h>

void copiarStr ( const char far * origen, char far * destino ) ;

void copiarStrHasta ( const char far * origen, char far * destino, word_t n ) ;

int iguales ( const char far * str1, const char far * str2 ) ;

int igualesHasta ( const char far * str1, const char far * str2, word_t n  ) ;

int igualesSalvoMayusculasHasta ( const char far * str1, const char far * str2, word_t n  ) ;

#endif /* STRINGS_H */

