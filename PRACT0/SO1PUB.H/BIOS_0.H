/* ----------------------------------------------------------------------- */
/*                                bios_0.h                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef BIOS_0_H
#define BIOS_0_H

#include <so1pub.h\tipos.h>

#define tO_PC     0xFF
#define tO_XT     0xFE
#define tO_PCjr   0xFD
#define tO_AT     0xFC
#define tO_XT640K 0xFB
#define tO_PS230  0xFA
#define tO_CPC    0xF9
#define tO_PS280  0xF8

byte_t tipoOrdenador ( void ) ;

void rebootLegacy ( void ) ;

#define rebootBIOS( ) { \
  asm int 19h ; \
}

/* E/S BIOS */

/*
   7    reservado
   6    int 16/AH=20h-22h soportadas (soporte teclado 122-teclas)
   5    int 16/AH=10h-12h suportadas (soporte teclado enhanced)
   4    int 16/AH=0Ah soportada
   3    int 16/AX=0306h soportada
   2    int 16/AX=0305h soportada
   1    int 16/AX=0304h soportada
   0    int 16/AX=0300h soportada
*/

byte_t tipoTeclado ( void ) ;

word_t leerTeclaBIOS ( void ) ;

word_t leerTeclaExtBIOS ( void ) ;

word_t teclaListaBIOS ( void ) ;   /* en WXP y DOSBOX deja int. permitidas */
                                             /* se ve facilmente con el TD */

#define leerCarBIOS( )                         /* bloqueante en bucle BIOS */\
  ((byte_t)leerTeclaBIOS())

#define leerCarListoBDA() ((char)leerTeclaLista())

word_t leerTeclaListaBDA ( void ) ;

word_t teclaListaBDA ( void ) ;


int printCarBIOS ( char car ) ;

int printCarPagBIOS ( char car, byte_t pag ) ;

int printCarAtrPagBIOS ( char car, byte_t atr, byte_t pag ) ;

int printLnBIOS ( void ) ;

int printStrBIOS ( char far * str ) ;

int printStrHastaBIOS ( char far * str, word_t n, bool_t lleno ) ;

int printDecBIOS ( word_t num, word_t l ) ;

int printLDecBIOS ( dword_t num, word_t l ) ;

int printIntBIOS ( int num, word_t l ) ;

int printLIntBIOS ( long int num, word_t l ) ;

int printHexBIOS ( word_t num, word_t l ) ;

int printLHexBIOS ( dword_t num, word_t l ) ;

int printPtrBIOS ( pointer_t ptr ) ;

int printWordBIOS ( word_t w ) ;

int printCadBIOS ( char far * cad ) ;

/* Timer */

void esperarTicsBIOS ( word_t tics ) ;

/* Pantalla */

byte_t modoDeVideo ( void ) ;

void establecerModoDeVideo ( byte_t modo ) ;

byte_t paginaActiva ( void ) ;

void establecerPaginaActiva ( byte_t num ) ;

byte_t numPagsVideo ( void ) ;

#define clrScrBIOS() scrollBIOS(0) ;

#define clrScrPagBIOS(pag) scrollPagBIOS(0, pag) ;

#define clrScrBDA() scrollPagBDA(0, 0) ;

#define clrScrPagBDA(pag) scrollPagBDA(0, pag) ;

void scrollPagBDA ( byte_t numLineas, byte_t pag ) ;

void scrollPagBIOS ( byte_t numLineas, byte_t pag ) ;

void scrollBIOS ( byte_t numLineas ) ;

void goToXYPag ( byte_t fila, byte_t columna, byte_t pag ) ;

void goToXYBIOS ( byte_t fila, byte_t columna ) ;

void setCursorBIOS ( byte_t linea1, byte_t linea2 ) ;

void ocultaCursorBIOS ( void ) ;

void readXYPagBIOS ( byte_t far * fila,
                     byte_t far * columna,
                     byte_t far * linea1,
                     byte_t far * linea2,
                     byte_t pag ) ;

void readXYBIOS ( byte_t far * fila,
                  byte_t far * columna,
                  byte_t far * linea1,
                  byte_t far * linea2 ) ;

   /* Las siguientes funciones permiten enviar y recibir caracteres por el */
   /* puerto COM1. Hay que tener en cuenta que el BIOS permite las         */
   /* interrupciones durante la ejecucion de esas llamadas, por lo que no  */
   /* deben ser utilizadas desde el sistema operativo propiamente dicho.   */

/* Memoria (int 12h) */
                                         /* tambien en BIOS AREA 0000:0413 */
word_t memBIOS ( void ) ;  /* memoria reportada por el BIOS (en Kilobytes) */

/* Puerto serie COM1 (int 14h) */

void enviarBIOS ( char car ) ;

char recibirBIOS ( void ) ;

/* APM */

bool_t hayApmBIOS ( word_t far * version ) ;

int connectApmBIOS ( void ) ;

int disconnectApmBIOS ( void ) ;

void cpuIdleBIOS ( void ) ;

void cpuBusyBIOS ( void ) ;

#endif /* BIOS_0_H */
