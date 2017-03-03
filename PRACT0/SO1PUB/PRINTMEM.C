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
  ptrMem = ptr ;
  return(printGenStr(str, (printCar_t)printCarMem)) ;
}

int printStrMemHasta ( pointer_t ptr, const char far * str, word_t n, bool_t lleno ) {
  ptrMem = ptr ;
  return(printGenStrHasta(str, n, lleno, (printCar_t)printCarMem)) ;
}

int printDecMem ( pointer_t ptr, word_t num, word_t l ) {
  ptrMem = ptr ;
  return(printGenDec(num, l, (printCar_t)printCarMem) ;
}

int printLDecMem ( pointer_t ptr, dword_t num, word_t l ) {
  ptrMem = ptr ;
  return(printGenLDec(num, l, (printCar_t)printCarMem) ;
}

int printIntMem ( pointer_t ptr, int num, word_t l ) {
  ptrMem = ptr ;
  return(printGenInt(num, l, (printCar_t)printCarMem) ;
}

int printLIntMem ( pointer_t ptr, long int num, word_t l ) {
  ptrMem = ptr ;
  return(printGenLInt(num, l, (printCar_t)printCarMem) ;
}

int printHexMem ( pointer_t ptr, word_t num, word_t l ) {
  ptrMem = ptr ;
  return(printGenHex(num, l, (printCar_t)printCarMem) ;
}

int printLHexMem ( pointer_t ptr, dword_t num, word_t l ) {
  ptrMem = ptr ;
  return(printGenLHex(num, l, (printCar_t)printCarMem) ;
}

int printBinMem ( pointer_t ptr, word_t num, word_t l ) {
  ptrMem = ptr ;
  return(printGenBin(num, l, (printCar_t)printCarMem) ;
}

int printLBinMem ( pointer_t ptr, dword_t num, word_t l ) {
  ptrMem = ptr ;
  return(printGenLBin(num, l, (printCar_t)printCarMem) ;
}

int printPtrMem ( pointer_t ptr, pointer_t ptr2 ) {
  ptrMem = ptr ;
  return(printGenPtr(ptr2, (printCar_t)printCarMem) ;
}

#endif /* PRINTGEN_H */
