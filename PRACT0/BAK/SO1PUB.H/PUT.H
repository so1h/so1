/* ----------------------------------------------------------------------- */
/*                                  put.h                                  */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef PUT_H
#define PUT_H

#include <so1pub.h\tipos.h>

int putCar ( int df, char car ) ;

int putLn ( int df ) ;

int putStr ( int df, const char * str ) ;

int putStrFar ( int df, const char far * str ) ;

int putStrHasta ( int df, const char * str, word_t n, bool_t lleno ) ;

int putStrFarHasta ( int df, const char far * str, word_t n, bool_t lleno ) ;

int putStrIntenso ( int df, const char * str ) ;

int putDec ( int df, word_t num, word_t l ) ;

int putLDec ( int df, dword_t num, word_t l ) ;

int putInt ( int df, int num, word_t l ) ;

int putLInt ( int df, long int num, word_t l ) ;

int putHex ( int df, word_t num, word_t l ) ;

int putLHex ( int df, dword_t num, word_t l ) ;

int putBin ( int df, word_t num, word_t l ) ;

int putLBin ( int df, dword_t num, word_t l ) ;

int putPtr ( int df, pointer_t ptr ) ;

#endif /* PUT_H */
