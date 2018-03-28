/* ----------------------------------------------------------------------- */
/*                                printmem.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef PRINTGEN_H
#define PRINTGEN_H

#include <so1pub.h\tipos.h>

int printCarMem ( pointer_t ptr, char car ) ;

int printLnMem ( pointer_t ptr ) ;

int printStrMem ( pointer_t ptr, const char far * str ) ;

int printStrMemHasta ( pointer_t ptr, const char far * str, word_t n, bool_t lleno ) ;

int printDecMem ( pointer_t ptr, word_t num, word_t l ) ;

int printLDecMem ( pointer_t ptr, dword_t num, word_t l ) ;

int printIntMem ( pointer_t ptr, int num, word_t l ) ;

int printLIntMem ( pointer_t ptr, long int num, word_t l ) ;

int printHexMem ( pointer_t ptr, word_t num, word_t l ) ;

int printLHexMem ( pointer_t ptr, dword_t num, word_t l ) ;

int printBinMem ( pointer_t ptr, word_t num, word_t l ) ;

int printLBinMem ( pointer_t ptr, dword_t num, word_t l ) ;

int printPtrMem ( pointer_t ptr, pointer_t ptr ) ;

#endif /* PRINTGEN_H */
