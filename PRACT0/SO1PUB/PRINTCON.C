/* ----------------------------------------------------------------------- */
/*                               printcon.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1pub.h\printcon.h>

static int dfCon [ 10 ] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 } ;

static byte_t congen ;

static nombreCon = "CON0" ;

int pCon ( char car ) {
  if (dfCon[congen] < 0) {
    nombreCon[3] += congen ;
    dfCon[congen] = open(nombreCon, O_WRONLY) ;
  }
  return(put(dfCon[congen], car)) ;
}

int printCarConsola ( byte_t con, car ) {
  congen = con ;
  return(pCon(car)) ;
}


int printLnConsola ( byte_t con ) {
  congen = con ;
  return(printGenLn(pCon)) ;
}

int printStrConsola ( byte_t con, char * str ) {
  congen = con ;
  return(printGenStr(str, pCon)) ;
}

int printStrHastaConsola ( byte_t con, char * str, word_t n, bool_t lleno ) {
  congen = con ;
  return(printGenStrHasta(str, n, lleno, pCon)) ;
}

int printDecConsola ( byte_t con, word_t num, word_t l ) {
  congen = con ;
  return(printGenDec(num, l, pCon)) ;
}

int printIntConsola ( byte_t con, int num, word_t l ) {
  congen = con ;
  return(printGenInt(num, l, pCon)) ;
}

int printLIntConsola ( byte_t con, long int num, word_t l ) {
  congen = con ;
  return(printGenLInt(num, l, pCon)) ;
}

int printLDecConsola ( byte_t con, dword_t num, word_t l ) {
  congen = con ;
  return(printGenLDec(num, l, pCon)) ;
}

int printHexConsola ( byte_t con, word_t num, word_t l ) {
  congen = con ;
  return(printGenHex(num, l, pCon)) ;
}

int printLHexConsola ( byte_t con, dword_t num, word_t l ) {
  congen = con ;
  return(printGenLHex(num, l, pCon)) ;
}

int printPtrConsola ( byte_t con, pointer_t ptr ) {
  congen = con ;
  return(printGenPtr(ptr, pCon)) ;
}

