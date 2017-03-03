/* ----------------------------------------------------------------------- */
/*                                printvid.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef PRINTVID_H
#define PRINTVID_H

#include <so1pub.h\tipos.h>

int printLnVideo ( void ) ;

int printStrVideo ( const char * str ) ;

int printStrHastaVideo ( const char * str, word_t n, bool_t lleno ) ;

int printDecVideo ( word_t num, word_t l ) ;

int printLDecVideo ( dword_t num, word_t l ) ;

int printIntVideo ( int num, word_t l ) ;

int printLIntVideo ( long int num, word_t l ) ;

int printHexVideo ( word_t num, word_t l ) ;

int printLHexVideo ( dword_t num, word_t l ) ;

int printBinVideo ( word_t num, word_t l ) ;

int printLBinVideo ( dword_t num, word_t l ) ;

int printPtrVideo ( pointer_t ptr ) ;

#endif /* PRINTVID_H */
