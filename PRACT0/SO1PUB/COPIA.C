/* ----------------------------------------------------------------------- */
/*                                 copia.c                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                      /* incPtr */

void copia ( pointer_t origen, pointer_t destino, word_t tam ) {
  word_t i ;
  for ( i = 0 ; i < tam ; i++ )
    destino[i] = origen[i] ;
}

void copiaLarga ( pointer_t origen, pointer_t destino, dword_t tam ) {
  word_t * ptr = (word_t *)&tam ;
  word_t i, j ;
  for ( i = 0 ; i < ptr[1] ; i++) {
    j = 0 ;
    do {
      destino[j] = origen[j] ;
    } while (j++ != 0xFFFF) ;
    incPtr(&destino, 0x8000) ;
    incPtr(&destino, 0x8000) ;
    incPtr(&origen, 0x8000) ;
    incPtr(&origen, 0x8000) ;
  }
  copia(origen, destino, ptr[0]) ;
}



