/* ----------------------------------------------------------------------- */
/*                                 memory.h                                */
/* ----------------------------------------------------------------------- */
/*                  Funciones de manipulacion de memoria                   */
/* ----------------------------------------------------------------------- */

/* Copia los n bytes que comienzan en origen a partir de destino. Devuelve */
/* como resultado destino. Si hay solapamiento entre los intervalos        */
/* (origen, origen + n - 1) y (destino, destino + n -1) el resultado es    */
/* indefinido (ANSI).                                                      */   
/* Hay que asegurarse de que offset(origen)+n-1 y offset(destino)+n-1 no   */
/* son mayores de 0xFFFF.                                                  */         

#ifndef MEMORY_H
#define MEMORY_H

#include <so1pub.h\tipos.h>                             /* word_t, dword_t */

void * memcpy_n ( void * destino, const void * origen, word_t n ) ;

void far * memcpy ( void far * destino, const void far * origen, word_t n ) ;

void far * normaliza ( void far * dir ) ;

void far * memcpy_fd ( void far * destino, const void far * origen, dword_t n ) ;

void * memmove_n ( void * destino, const void * origen, word_t n ) ;

#endif /* MEMORY_H */