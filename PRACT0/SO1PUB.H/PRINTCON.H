/* ----------------------------------------------------------------------- */
/*                               printcon.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef PRINTCON_H
#define PRINTCON_H

#include <so1pub.h\tipos.h>

int printCarConsola ( byte_t con, car ) ;

int printLnConsola ( byte_t con ) ;

int printStrConsola ( byte_t con, const char * str ) ;

int printStrHastaConsola ( byte_t con, const char * str, word_t n, bool_t lleno ) ;

int printDecConsola ( byte_t con, word_t num, word_t l ) ;

int printIntConsola ( byte_t con, int num, word_t l ) ;

int printLIntConsola ( byte_t con, long int num, word_t l ) ;

int printLDecConsola ( byte_t con, dword_t num, word_t l ) ;

int printHexConsola ( byte_t con, word_t num, word_t l ) ;

int printLHexConsola ( byte_t con, dword_t num, word_t l ) ;

int printBinConsola ( word_t num, word_t l ) ;

int printLBinConsola ( dword_t num, word_t l ) ;

int printPtrConsola ( byte_t con, pointer_t ptr ) ;

#endif /* PRINTCON_H */