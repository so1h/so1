/* ----------------------------------------------------------------------- */
/*                                printmem.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1pub.h\carsctrl.h>
#include <so1pub.h\printgen.h>

pointer_t ptrMem ;

int printCarMemoria ( char car ) {
  ptrMem[0] = car ;
  ptrMem++ ;
}

int printCarMem ( pointer_t ptr, char car ) {
  ptr[0] = car ;
  return(1) ;
}

int printLnMem ( pointer_t ptr ) {
  ptr[0] = CR ;
  ptr[1] = LF ;
  return(2) ;
}

int printStrMem ( pointer_t ptr, const char far * str ) {
  return(printGenStr(str, (printCar_t)printCarMem)) ;
}

int printStrMemHasta ( pointer_t ptr, const char far * str, word_t n, bool_t lleno ) {
  return(printGenStrHasta(str, n, lleno, (printCar_t)printCarMem)) ;
}

int printDecMem ( pointer_t ptr, word_t num, word_t l ) {
  return(printGenDec(str, n, lleno, (printCar_t)printCarMem) ;
}

int printLDecMem ( pointer_t ptr, dword_t num, word_t l ) {
  return(0) ;
}

int printIntMem ( pointer_t ptr, int num, word_t l ) {
  return(0) ;
}

int printLIntMem ( pointer_t ptr, long int num, word_t l ) {
  return(0) ;
}

int printHexMem ( pointer_t ptr, word_t num, word_t l ) {
  return(0) ;
}

int printLHexMem ( pointer_t ptr, dword_t num, word_t l ) {
  return(0) ;
}

int printBinMem ( pointer_t ptr, word_t num, word_t l ) {
}

int printLBinMem ( pointer_t ptr, dword_t num, word_t l ) {
}

int printPtrMem ( pointer_t ptr, pointer_t ptr ) {
}

#endif /* PRINTGEN_H */
