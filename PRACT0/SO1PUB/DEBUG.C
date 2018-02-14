/* ----------------------------------------------------------------------- */
/*                                  debug.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                        /* retf */
#include <so1pub.h\bios_0.h>

#define _P 50h
#define _u 75h
#define _n 6Eh
#define _t 74h
#define _o 6Fh
#define __ 20h                                             /* espacio  ' ' */
#define _d 64h
#define _e 65h

#define _a 61h
#define _r 72h
#define _S 53h
#define _O 4Fh
#define _1 31h

void puntoDeParada ( void ) {
  asm {
    ret ;
//  db 'Punto De Parada SO1' ;
    db _P,_u,_n,_t,_o,__,_d,_e,__,_P,_a,_r,_a,_d,_a,__,_S,_O,_1 ;
  }
}

bool_t debug_parar = FALSE ;

void assert ( int condicion, char far * str ) {
  if (!condicion) {
    printStrBIOS(str) ;
    leerTeclaBIOS() ;
  }
}

word_t valorFlags ( void ) {
  word_t flags ;
  asm {
    pushf ;
    pop ax ;
    mov flags,ax ;
  }
  return(flags) ;
}

void mostrarFlags ( void ) {
  word_t flags ;
  asm {
    pushf ;
    pop ax ;
    mov flags,ax ;
  }
  printStrBIOS("\n flags = ") ;
  printHexBIOS(flags, 4) ;
  if (flags & 0x0200) printStrBIOS(" ints permitidas ") ;
  else printStrBIOS(" ints inhibidas ") ;
  asm {
    mov ax,flags ;
    push ax ;
    popf ;
  }
}

