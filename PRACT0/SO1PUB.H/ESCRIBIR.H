/* ----------------------------------------------------------------------- */
/*                                escribir.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef ESCRIBIR_H
#define ESCRIBIR_H

#include <so1pub.h\tipos.h>

int escribirCar ( char car ) ;

int escribirLn ( void ) ;

int escribirStr ( const char * str ) ;

int escribirStrFar ( const char far * str ) ;

int escribirStrHasta ( const char * str, word_t n, bool_t lleno ) ;

int escribirStrFarHasta ( const char far * str, word_t n, bool_t lleno ) ;

int escribirDec ( word_t num, word_t l ) ;

int escribirLDec ( dword_t num, word_t l ) ;

int escribirInt ( int num, word_t l ) ;

int escribirLInt ( long int num, word_t l ) ;

int escribirHex ( word_t num, word_t l ) ;

int escribirLHex ( dword_t num, word_t l ) ;

int escribirBin ( word_t num, word_t l ) ;

int escribirLBin ( dword_t num, word_t l ) ;

int escribirPtr ( pointer_t ptr ) ;

#endif /* ESCRIBIR_H */
