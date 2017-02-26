/* ----------------------------------------------------------------------- */
/*                                printvid.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1pub.h\memvideo.h>                         /* printCarVideo */
#include <so1pub.h\printgen.h>  /* printGen(Ln|Str|StrHasta|Dec|Int|..) */

int printLnVideo ( ) {
  return(printGenLn(printCarVideo)) ;
}

int printStrVideo ( char * str ) {
  return(printGenStr(str, printCarVideo)) ;
}

int printStrHastaVideo ( char * str, word_t n, bool_t lleno ) {
  return(printGenStrHasta(str, n, lleno, printCarVideo)) ;
}

int printDecVideo ( word_t num, word_t l ) {
  return(printGenDec(num, l, printCarVideo)) ;
}

int printIntVideo ( int num, word_t l ) {
  return(printGenInt(num, l, printCarVideo)) ;
}

int printLIntVideo ( long int num, word_t l ) {
  return(printGenLInt(num, l, printCarVideo)) ;
}

int printLDecVideo ( dword_t num, word_t l ) {
  return(printGenLDec(num, l, printCarVideo)) ;
}

int printHexVideo ( word_t num, word_t l ) {
  return(printGenHex(num, l, printCarVideo)) ;
}

int printLHexVideo ( dword_t num, word_t l ) {
  return(printGenLHex(num, l, printCarVideo)) ;
}

int printPtrVideo ( pointer_t ptr ) {
  return(printGenPtr(ptr, printCarVideo)) ;
}

