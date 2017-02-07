
#include <so1pub.h\printgen.h>
#include <so1pub.h\escribir.h>
#include <so1pub.h\stdargs.h>
#include <so1pub.h\vprinter.h>

size_t printGenN ( const void * ptr, size_t n, printCar_t p ) ;

size_t printGenN ( const void * ptr, size_t n, printCar_t p ) {
  int len = n ;
  while (n--) {
    if (p(*((char *)ptr)++) != 0) return(0) ;
  }
  return(len) ;
}

size_t __pN ( const void * ptr, size_t n ) {
  return(printGenN(ptr, n, escribirCar)) ;
}

int printf ( const char far * fmt, ... ) {
  asm push si ;
  asm push di ;
//return( __vprinter ( (printCarN_t *) __pN, fmt, (void _ss *) _va_ptr) ) ;
  __vprinter ( (printCarN_t *) __pN, fmt, (void _ss *) _va_ptr) ;
  asm pop di ;
  asm pop si ;
  return(_AX) ;
}

