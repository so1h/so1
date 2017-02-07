/* ----------------------------------------------------------------------- */
/*                                strings.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1pub.h\strings.h>
#include <so1pub.h\caracter.h>

void copiarStr ( const char far * origen, char far * destino ) {
  while (*origen != (char)0)
    *destino++ = *origen++ ;
  *destino = (char)0 ;
}

void copiarStrHasta ( const char far * origen, char far * destino, word_t n ) {
  int i = n ;
  while ((*origen != (char)0) && (i-- > 0))
    *destino++ = *origen++ ;
  *destino = (char)0 ;
}

int iguales ( const char far * str1, const char far * str2 ) {
  while ((*str1 == *str2) && (*str1 != (char)0)) {
    str1++ ;
    str2++ ;
  }
  return((*str1 == *str2)) ;
}

int igualesHasta ( const char far * str1, const char far * str2, word_t n  ) {
  word_t i = 0 ;
  while ((i < n) &&
         (*str1 == *str2) && (*str1 != (char)0)) {
    str1++ ;
    str2++ ;
    i++ ;
  }
  return(i == n) ;
}

int igualesSalvoMayusculasHasta ( const char far * str1, const char far * str2, word_t n  ) {
  word_t i = 0 ;
  while ((i < n) &&
         (mayuscula(*str1) == mayuscula(*str2)) && (*str1 != (char)0)) {
    str1++ ;
    str2++ ;
    i++ ;
  }
  return(i == n) ;
}


