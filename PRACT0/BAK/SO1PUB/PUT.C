/* ----------------------------------------------------------------------- */
/*                                   put.c                                 */
/* ----------------------------------------------------------------------- */
/*                     escritura formateada en un fichero                  */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>
#include <so1pub.h\printgen.h>
#include <so1pub.h\put.h>                                     /* putCar */

static int dfG ;                                        /* variable global */

static int putG ( char car ) {
  int n = write(dfG, (pointer_t)&car, 1) ;
  if (n < 1) return(-1) ;
  else return(1) ;
}

int putCar ( int df, char car ) {
  dfG = df ;
  return(putG(car)) ;
}

int putLn ( int df ) {
  dfG = df ;
  return(printGenLn(putG)) ;
}

int putStr ( int df, const char * str ) {
  dfG = df ;
  return(printGenStr(str, putG)) ;
}

int putStrHasta ( int df, const char * str, word_t n, bool_t lleno ) {
  dfG = df ;
  return(printGenStrHasta(str, n, lleno, putG)) ;
}

int putDec ( int df, word_t num, word_t l ) {
  dfG = df ;
  return(printGenDec(num, l, putG)) ;
}

int putLDec ( int df, dword_t num, word_t l ) {
  dfG = df ;
  return(printGenLDec(num, l, putG));
}

int putInt ( int df, int num, word_t l ) {
  dfG = df ;
  return(printGenInt(num, l, putG)) ;
}

int putLInt ( int df, long int num, word_t l ) {
  dfG = df ;
  return(printGenLInt(num, l, putG)) ;
}

int putHex ( int df, word_t num, word_t l ) {
  dfG = df ;
  return(printGenHex(num, l, putG)) ;
}

int putLHex ( int df, dword_t num, word_t l ) {
  dfG = df ;
  return(printGenLHex(num, l, putG)) ;
}

int putBin ( int df, word_t num, word_t l ) {
  dfG = df ;
  return(printGenBin(num, l, putG)) ;
}

int putLBin ( int df, dword_t num, word_t l ) {
  dfG = df ;
  return(printGenLBin(num, l, putG)) ;
}

int putPtr ( int df, pointer_t ptr ) {
  dfG = df ;
  return(printGenPtr(ptr, putG)) ;
}

