/* ----------------------------------------------------------------------- */
/*                                  debug.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef DEBUG_H
#define DEBUG_H

#include <so1pub.h\tipos.h>
#include <so1pub.h\startdata.h>
#include <so1pub.h\startbss.h>
#include <so1pub.h\finbss.h>

void puntoDeParada ( void ) ;

void assert ( int condicion, char far * str ) ;

word_t valorFlags ( void ) ;

void mostrarFlags ( void ) ;

#define breakpoint() {                                                       \
  asm int 3 ;                                                                \
}

#define paradaActiva() { while(TRUE) ; }

#define paradaTotal() {                                                      \
  asm cli ;                                                                  \
  asm hlt ;                                                                  \
}

extern bool_t debug_parar ;

#define paradaActivaCondicional() {                                          \
  if (debug_parar)                                                           \
    while(TRUE) ;                                                            \
}

#if (FALSE)

void tomarIP ( void ) {              /* pone en BX la direccion de retorno */
  asm push bp
  asm mov bp,sp
  asm mov bx,ss:[bp+2]                              /* BX = dir de retorno */
  asm pop bp
}

void mostrarPila ( word_t words ) {
  word_t far * ptrWord ;
  int i ;
  ptrWord = (word_t far *)pointer(_SS, _SP) ;
  for ( i = 0 ; i < words ; i++ ) {
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWord[i+4]) ;          /* 1 d.r. + 1 par. + 2 v.l. = 4 */
  }
}

#endif

#if (FALSE)

#define mostrarRegistros(ww) {                                              \
  tomarIP() ;                                                               \
  asm push bx ;                                                             \
  printStrWin(ww, "\n ") ;                                                  \
  printStrWin(ww, "\n CS:IP = ") ;                                          \
  printHexWin(ww, _CS, 4) ;                                                 \
  printStrWin(ww, ":") ;                                                    \
  asm pop bx ;                                                              \
  printHexWin(ww, _BX, 4) ;                                                 \
                                                                            \
  printStrWin(ww, "\n SS:SP = ") ;                                          \
  printHexWin(ww, _SS, 4) ;                                                 \
  printStrWin(ww, ":") ;                                                    \
  printHexWin(ww, _SP, 4) ;                                                 \
  printStrWin(ww, " BP = ") ;                                               \
  printHexWin(ww, _BP, 4) ;                                                 \
                                                                            \
  printStrWin(ww, " DS = ") ;                                               \
  printHexWin(ww, _DS, 4) ;                                                 \
  printStrWin(ww, " ES = ") ;                                               \
  printHexWin(ww, _ES, 4) ;                                                 \
  printStrWin(ww, "\n ") ;                                                  \
} ;

#define mostrarDescriptor(ww, ii) {                                         \
  printStrWin(ww, "\n d[ii].pid          = ") ;                             \
    printDecWin(ww, descriptor[ii].pid, 1) ;                                \
  printStrWin(ww, " pindx = ") ;                                            \
    printDecWin(ww, ii, 1) ;                                                \
  printStrWin(ww, " ppindx = ") ;                                           \
    printDecWin(ww, descriptor[ii].ppindx, 1) ;                             \
  printStrWin(ww, " hpindx = ") ;                                           \
    printDecWin(ww, descriptor[ii].hpindx, 1) ;                             \
  printStrWin(ww, "\n d[ii].estado       = ") ;                             \
    printDecWin(ww, descriptor[ii].estado, 1) ;                             \
  printStrWin(ww, " esperandoPor = ") ;                                     \
    printDecWin(ww, descriptor[ii].esperandoPor, 1) ;                       \
  printStrWin(ww, "\n d[ii].trama        = ") ;                             \
    printHexWin(ww, seg((pointer_t)descriptor[ii].trama), 4) ;              \
    printStrWin(ww, ":") ;                                                  \
    printHexWin(ww, off((pointer_t)descriptor[ii].trama), 4) ;              \
  printStrWin(ww, "\n d[ii].sig          = ") ;                             \
    printDecWin(ww, descriptor[ii].sig, 1) ;                                \
  printStrWin(ww, "\n d[ii].CSProc       = ") ;                             \
    printHexWin(ww, descriptor[ii].CSProc, 4) ;                             \
  printStrWin(ww, "\n d[ii].tamCodigo    = ") ;                             \
    printHexWin(ww, descriptor[ii].tamCodigo, 4) ;                          \
  printStrWin(ww, "\n d[ii].desplBSS     = ") ;                             \
    printHexWin(ww, descriptor[ii].desplBSS, 4) ;                           \
  printStrWin(ww, "\n d[ii].desplPila    = ") ;                             \
    printHexWin(ww, descriptor[ii].desplPila, 4) ;                          \
  printStrWin(ww, "\n d[ii].tam          = ") ;                             \
    printHexWin(ww, descriptor[ii].tam, 4) ;                                \
    printStrWin(ww, " Ps ") ;                                               \
  printStrWin(ww, "\n d[ii].sigPart      = ") ;                             \
    printDecWin(ww, descriptor[ii].sigPart, 1) ;                            \
  printStrWin(ww, "\n d[ii].antPart      = ") ;                             \
    printDecWin(ww, descriptor[ii].antPart, 1) ;                            \
  printStrWin(ww, "\n d[ii].tamFichero   = ") ;                             \
    printLDecWin(ww, descriptor[ii].tamFichero, 1) ;                        \
  printStrWin(ww, "\n d[ii].programa     = ") ;                             \
    printStrHastaWin(ww, descriptor[ii].programa, 12, FALSE) ;              \
  printStrWin(ww, "\n d[ii].comando      = ") ;                             \
    printStrWin(ww, descriptor[ii].comando) ;                               \
  printStrWin(ww, "\n d[ii].win          = ") ;                             \
    printHexWin(ww, seg((pointer_t)descriptor[ii].win), 4) ;                \
    printStrWin(ww, ":") ;                                                  \
    printHexWin(ww, off((pointer_t)descriptor[ii].win), 4) ;                \
  printStrWin(ww, "\n d[ii].teclado      = ") ;                             \
    printHexWin(ww, seg((pointer_t)descriptor[ii].teclado), 4) ;            \
    printStrWin(ww, ":") ;                                                  \
    printHexWin(ww, off((pointer_t)descriptor[ii].teclado), 4) ;            \
  printStrWin(ww, "\n d[ii].nfa          = ") ;                             \
    printDecWin(ww, descriptor[ii].nfa, 1) ;                                \
  printStrWin(ww, "\n ") ;                                                  \
}

/*
    printStrWin(ww, " off(startData) = ") ;                                 \
    printHexWin(ww, off((pointer_t)startData), 4) ;                         \
    printStrWin(ww, " off(startBSS) = ") ;                                  \
    printHexWin(ww, off((pointer_t)startBSS), 4) ;                          \
    printStrWin(ww, " off(finBSS) = ") ;                                    \
    printHexWin(ww, off((pointer_t)finBSS), 4) ;                            \
*/

#define mostrarTrama(ww, ii) {                                              \
  printStrWin(ww, "\n d[ii].pid          = ") ;                             \
    printDecWin(ww, descriptor[ii].pid, 1) ;                                \
  printStrWin(ww, " pindx = ") ;                                            \
    printDecWin(ww, ii, 1) ;                                                \
  printStrWin(ww, " ppindx = ") ;                                           \
    printDecWin(ww, descriptor[ii].ppindx, 1) ;                             \
  printStrWin(ww, " hpindx = ") ;                                           \
    printDecWin(ww, descriptor[ii].hpindx, 1) ;                             \
  printStrWin(ww, "\n d[ii].trama        = ") ;                             \
    printHexWin(ww, seg((pointer_t)descriptor[ii].trama), 4) ;              \
    printStrWin(ww, ":") ;                                                  \
    printHexWin(ww, off((pointer_t)descriptor[ii].trama), 4) ;              \
  printStrWin(ww, "\n ") ;                                                  \
  printStrWin(ww, "\n  DS   ES   DI   SI   BP   SP   BX   DX   CX   AX   IP   CS  Flgs ") ; \
  printStrWin(ww, "\n ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ") ; \
  printStrWin(ww, "\n ") ;                                                  \
  printHexWin(ww, descriptor[ii].trama->DS, 4) ; printStrWin(ww, " ") ;     \
  printHexWin(ww, descriptor[ii].trama->ES, 4) ; printStrWin(ww, " ") ;     \
  printHexWin(ww, descriptor[ii].trama->DI, 4) ; printStrWin(ww, " ") ;     \
  printHexWin(ww, descriptor[ii].trama->SI, 4) ; printStrWin(ww, " ") ;     \
  printHexWin(ww, descriptor[ii].trama->BP, 4) ; printStrWin(ww, " ") ;     \
  printHexWin(ww, descriptor[ii].trama->SP, 4) ; printStrWin(ww, " ") ;     \
  printHexWin(ww, descriptor[ii].trama->BX, 4) ; printStrWin(ww, " ") ;     \
  printHexWin(ww, descriptor[ii].trama->DX, 4) ; printStrWin(ww, " ") ;     \
  printHexWin(ww, descriptor[ii].trama->CX, 4) ; printStrWin(ww, " ") ;     \
  printHexWin(ww, descriptor[ii].trama->AX, 4) ; printStrWin(ww, " ") ;     \
  printHexWin(ww, descriptor[ii].trama->IP, 4) ; printStrWin(ww, " ") ;     \
  printHexWin(ww, descriptor[ii].trama->CS, 4) ; printStrWin(ww, " ") ;     \
  printHexWin(ww, descriptor[ii].trama->Flags, 4) ;                         \
  printStrWin(ww, "\n ") ;                                                  \
}

#endif

#endif /* DEBUG_H */
