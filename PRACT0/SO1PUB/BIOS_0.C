/* ----------------------------------------------------------------------- */
/*                                bios_0.c                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                        /* retf */
#include <so1pub.h\printgen.h>
#include <so1pub.h\biosdata.h>
#include <so1pub.h\bios_0.h>
#include <so1pub.h\pantalla.h>                                /* atrNormal */

/* ----------------------------------------------------------------------- */
/*                            tipo de ordenador                            */
/* ----------------------------------------------------------------------- */
/* tipo de ordenador */

byte_t tipoOrdenador ( void ) {
  byte_t far * ptr = MK_FP(0xFFFF, 0x000E) ;
  return(*ptr) ;
}

/* reinicio */

#if (FALSE)
void rebootBIOS ( void ) {
  asm int 19h
}
#endif

void rebootLegacy ( void ) {
  asm {
    push 0F000h ;                   /* equivalente a call F000:FFF0 reboot */
    push 0FFF0h ;
    retf ;
  }
}

/* Teclado (int 16h) */

byte_t tipoTeclado ( void ) {
  byte_t res ;
  asm mov ah,09h
  asm int 16h
  asm mov res,ah
  return(res) ;
}

#if (FALSE)

byte_t leerTeclaBIOS ( void ) {                /* bloqueante en bucle BIOS */
  char car ;
  asm mov ah,00h ;                    /* Llamada al BIOS: Leer del teclado */
  asm int 16h ;
  asm mov car,ah ;                         /* ah = codigo scan introducido */
  return(car) ;
}

byte_t leerTeclaExtBIOS ( void ) {        /* bloqueante en bucle BIOS [AT] */
  char car ;
  asm mov ah,10h ;    /* Llamada al BIOS: Leer teclado extendido 101 teclas*/
  asm int 16h ;
  asm mov car,ah ;                         /* ah = codigo scan introducido */
  return(car) ;
}

char leerCarBIOS ( void ) {                    /* bloqueante en bucle BIOS */
  char car ;
  asm mov ah,00h                      /* Llamada al BIOS: Leer del teclado */
  asm int 16h
  asm mov car,al                        /* al = caracter ascii introducido */
  return(car) ;     /* al == 0 => a continuacion: caracter ascii extendido */
}

#endif

/* leerTeclaBIOS parece que deja permitidas las interrupciones             */

word_t leerTeclaBIOS ( void ) {                /* bloqueante en bucle BIOS */
  word_t w ;
  asm mov ah,00h ;                    /* Llamada al BIOS: Leer del teclado */
  asm int 16h ;
  asm mov w,ax ;       /* ah = codigo scan introducido al = caracter ASCII */
  return(w) ;
}

word_t leerTeclaExtBIOS ( void ) {             /* bloqueante en bucle BIOS */
  word_t w ;
  asm mov ah,10h ;    /* Llamada al BIOS: Leer teclado extendido 101 teclas*/
  asm int 16h ;
  asm mov w,ax ;       /* ah = codigo scan introducido al = caracter ASCII */
  return(w) ;
}

word_t teclaListaBIOS ( void ) {            /* no bloqueante en bucle BIOS */
  word_t w ;
  asm mov ah,01h
  asm int 16h
  asm jne hayTecla
  return(0) ;
hayTecla:
  asm mov w,ax
  return(w) ;                             /* al = caracter, ah = scan code */
}

/* Teclado (Bios Data Area)  (no alteran/permiten el flag de interrupcion) */

word_t leerTeclaListaBDA ( void ) {
  word_t w = 0x0000 ;
  word_t start = ptrBiosArea->KBD_start ;
  word_t end1 = ptrBiosArea->KBD_end1 ;
  word_t out = ptrBiosArea->KBD_bufhead ;
  word_t in = ptrBiosArea->KBD_buftail ;
  int extended = ptrBiosArea->KBD_status.KBD_AT ;
  char ascii_code ;
  byte_t scan_code ;
  if (out != in) {
    /* w = *((word_t far *)MK_FP(0x040, out)) ; */
    w = ptrBiosArea->KBD_buffer[(out-start)/2] ;
    ascii_code = (char)w ;
    scan_code = (byte_t)(w >> 8) ;
    if (((ascii_code == (char)0xF0) && (scan_code != 0)) ||    /* 0xF0 liberacion tecla teclado 84 teclas */
        ((ascii_code == (char)0xE0) && !extended))             /* 0xE0 teclado de 101 teclas */
      w = w & 0xFF00 ;
    out = out + 2 ;
    if (out >= end1) out = start ;
    ptrBiosArea->KBD_bufhead = out ;
  }
  return(w) ;
}

word_t teclaListaBDA ( void ) {
  word_t w = 0x0000 ;
  word_t start = ptrBiosArea->KBD_start ;
  word_t out = ptrBiosArea->KBD_bufhead ;
  word_t in = ptrBiosArea->KBD_buftail ;
  if (out != in)
//  w = *((word_t far *)MK_FP(0x040, out)) ; 
    w = ptrBiosArea->KBD_buffer[(out-start)/2] ;
  return(w) ;
}

void cambiarTeclaListaBDA ( word_t w ) {
  word_t start = ptrBiosArea->KBD_start ;
  word_t out = ptrBiosArea->KBD_bufhead ;
  word_t in = ptrBiosArea->KBD_buftail ;
  if (out != in)
//  w = *((word_t far *)MK_FP(0x040, out)) ; 
    ptrBiosArea->KBD_buffer[(out-start)/2] = w ;
}	

/* Timer */

void esperarTicsBIOS ( word_t tics ) {
  dword_t contTics0 ;
  contTics0 = ptrBiosArea->BIOS_timer ;
  while (ptrBiosArea->BIOS_timer <= (contTics0 + tics)) { } ;
}

/* Pantalla (int 10h) */

#pragma warn -rvl

int printCarRawBIOS ( char car, byte_t pag ) {
  asm mov al,car
  asm mov bh,pag
  asm mov bl,07h
  asm mov cx,1
  asm mov ah,09h        /* Llamada al BIOS: Escribir caracter por pantalla */
  /* asm mov ah,0ah */  /* Llamada al BIOS: Escribir caracter por pantalla */
  asm int 10h
}

int printCarBIOS ( char car ) {
  asm mov al,car
  asm mov ah,0eh        /* Llamada al BIOS: Escribir caracter por pantalla */
  asm int 10h
}

int printCarPagBIOS ( char car, byte_t pag ) {
  byte_t columna = ptrBiosArea->CURSOR_pos[pag][0] ;
  byte_t fila = ptrBiosArea->CURSOR_pos[pag][1] ;
  byte_t columnaMax = (ptrBiosArea->VIDEO_width) - 1 ;
  byte_t filaMax = ptrBiosArea->VIDEO_lastrow ;
  switch (car) {
  case '\b' : if (fila > 0)
                goToXYPag(fila, --columna, pag) ;
              break ;
  case '\r' : goToXYPag(fila, 0, pag) ; break ;
  case '\n' : if (fila < filaMax)
                goToXYPag(++fila, columna, pag) ;
              else {
                scrollPagBDA(1, pag) ;          /* scrollPagBIOS(1, pag) ; */
                goToXYPag(filaMax, columna, pag) ; break ;
              }
              break ;
  default :
    if (' ' <= car) {
      printCarRawBIOS(car, pag) ;
      if (columna < columnaMax)
        goToXYPag(fila, ++columna, pag) ;
      else if (fila < filaMax)
        goToXYPag(++fila, 0, pag) ;
      else {
        scrollPagBDA(1, pag) ;                /* scrollPagBIOS(1, pag) ; */
        goToXYPag(filaMax, 0, pag) ;
      }
    }
  }
}

int printCarAtrPagBIOS ( char car, byte_t atr, byte_t pag ) {
  asm mov al,car
  asm mov bh,pag
  asm mov bl,atr
  asm mov ah,0eh        /* Llamada al BIOS: Escribir caracter por pantalla */
  asm int 10h
}

int printLnBIOS () {
  printCarBIOS('\r') ;
  printCarBIOS('\n') ;
}

int printStrBIOS ( const char far * str ) {
  printGenStr(str, printCarBIOS) ;
}

int printStrHastaBIOS ( const char far * str, word_t n, bool_t lleno ) {
  printGenStrHasta(str, n, lleno, printCarBIOS) ;
}

int printDecBIOS ( word_t num, word_t l ) {
  printGenDec(num, l, printCarBIOS) ;
}

int printLDecBIOS ( dword_t num, word_t l ) {
  printGenLDec(num, l, printCarBIOS) ;
}

int printIntBIOS ( int num, word_t l ) {
  printGenInt(num, l, printCarBIOS) ;
}

int printLIntBIOS ( long int num, word_t l )
{
  printGenLInt(num, l, printCarBIOS) ;
}

int printHexBIOS ( word_t num, word_t l )
{
  printGenHex(num, l, printCarBIOS) ;
}

int printLHexBIOS ( dword_t num, word_t l )
{
  printGenLHex(num, l, printCarBIOS) ;
}

int printBinBIOS ( word_t num, word_t l )
{
    printGenBin(num, l, printCarBIOS ) ;
}

int printLBinBIOS ( dword_t num, word_t l )
{
    printGenLBin( num, l, printCarBIOS ) ;
}

int printPtrBIOS ( pointer_t ptr )
{
  printGenPtr(ptr, printCarBIOS) ;
}

static char dig [17] = "0123456789ABCDEF" ;

#pragma warn -par

int printWordBIOS ( word_t w ) {
  printCarBIOS(dig[((lh_t *)&w)->hi >> 4]) ;
  printCarBIOS(dig[((lh_t *)&w)->hi & 0x0F]) ;
  printCarBIOS(dig[((lh_t *)&w)->lo >> 4]) ;
  printCarBIOS(dig[((lh_t *)&w)->lo & 0x0F]) ;
}

#pragma warn +par

int printCadBIOS ( const char far * cad ) {
  word_t i = 0 ;
  while (cad[i] != (char)0) {
    if (cad[i] == '\n') printCarBIOS('\r') ;
    printCarBIOS(cad[i]) ;
    i++ ;
  }
}

#pragma warn +rvl

byte_t modoDeVideo ( void ) {
  byte_t res ;
  asm mov ah,0fh
  asm int 10h
  asm mov res,al             /* ah = ancho de pantalla, al = modo de video */
  return(res) ;                                      /* bh = pagina activa */
}

void establecerModoDeVideo ( byte_t modo ) {
  asm mov ah,00h
  asm mov al,modo
  asm int 10h
}

byte_t paginaActiva ( void ) {
  byte_t res ;
  asm mov ah,0fh
  asm int 10h
  asm mov res,bh
  return(res) ;
}

void establecerPaginaActiva ( byte_t pag ) {
  asm mov ah,05h
  asm mov al,pag
  asm int 10h
}

typedef struct {
  byte_t bModes1 ;
  byte_t bModes2 ;
  byte_t bModes3 ;
  byte_t reserved_1 [4] ;
  byte_t bScanLnsFlgs ;
  byte_t bFontBlks ;
  byte_t bMaxFonts ;
  byte_t rMiscFlags1 ;
  byte_t rMiscFlags2 ;
  byte_t reserved_2 [2] ;
  byte_t rSaveCaps ;
  byte_t reserved_3 ;
} VgaStaticFnalityRec_t ;                                  /* TECHHelp 6.0 */

typedef struct {
  byte_t F ;
  byte_t C ;
} posCursor_t ;

typedef struct {
  VgaStaticFnalityRec_t
  far *       pfrFnality ;
  byte_t      bCurMode ;
  word_t      bCrtClms ;
  word_t      wCrtBufLen ;
  word_t      pCrtBPgStart ;
  posCursor_t posCursor [8] ;
  word_t      wCrsrType ;
  byte_t      bCurPg ;
  word_t      wCrtcPort ;
  byte_t      bModeSetReg ;
  byte_t      bClrSetReg ;
  byte_t      bCrtRows ;
  word_t      bCrtPoints ;
  byte_t      bCurDcc ;
  byte_t      bAltDcc ;
  word_t      wMaxClrs ;
  byte_t      bMaxPgs ;
  byte_t      bScanLnsCode ;
  byte_t      bFont1 ;
  byte_t      bFont2 ;
  byte_t      rMiscFlags ;
  byte_t      reserved_1 [3] ;
  byte_t      bVidMemCode ;
  byte_t      rSaveStatus ;
  byte_t      reserved_2 [13] ;
} VgaDinamicStateReg_t ;                                   /* TECHHelp 6.0 */

byte_t numPagsVideo ( void ) {
  VgaDinamicStateReg_t far * pVDSR ;
  byte_t res ;
  asm mov ah,1bh
  asm mov bx,0000h
  asm les di,pVDSR ;
  asm int 10h
  asm mov res,bh
/*
printStrBIOS("\n pVDSR->bMaxPgs = ") ;
printDecBIOS(pVDSR->bMaxPgs, 1) ;
printStrBIOS("\n res = ") ;
printHexBIOS(res, 1) ;
printStrBIOS(" ") ;
  if (res == 0x1b) return(pVDSR->bMaxPgs) ;
  else return(1) ;
*/
  return(pVDSR->bMaxPgs) ;
}

void scrollPagBDA ( byte_t numLineas, byte_t pag ) {
  byte_t far * ptr ;
  int i, j ;
  int totalLineas ;
  word_t segMemVideo ;
  if (ptrBiosArea->VIDEO_mode == 7) segMemVideo = 0xB000 ;
  else segMemVideo = 0xB800 ;
  ptr = MK_FP(segMemVideo, pag*ptrBiosArea->VIDEO_pagesize) ;
  if (numLineas == 0) {
    for ( i = 0 ; i < ptrBiosArea->VIDEO_pagesize ; i = i + 2 ) {
      ptr[i] = ' ' ;
      ptr[i+1] = 0x07 ;
    }
    ptrBiosArea->CURSOR_pos[pag][0] = 0 ;
    ptrBiosArea->CURSOR_pos[pag][1] = 0 ;
  }
  else {
    totalLineas = ptrBiosArea->VIDEO_pagesize/(2*ptrBiosArea->VIDEO_width) ;
    for ( i = 0 ; i < totalLineas-numLineas ; i++ ) {
      for ( j = 0 ; j < 2*ptrBiosArea->VIDEO_width ; j++ ) {
        ptr[(i*2*ptrBiosArea->VIDEO_width)+j] = ptr[((i+numLineas)*2*ptrBiosArea->VIDEO_width)+j] ;
        ptr[(i*2*ptrBiosArea->VIDEO_width)+j + 1] = ptr[((i+numLineas)*2*ptrBiosArea->VIDEO_width)+j + 1] ;
      }
    }
    for ( i = totalLineas-numLineas ; i < totalLineas ; i++ ) {
      for ( j = 0 ; j < 2*ptrBiosArea->VIDEO_width ; j = j + 2 ) {
        ptr[(i*2*ptrBiosArea->VIDEO_width)+j] = ' ' ;
        ptr[(i*2*ptrBiosArea->VIDEO_width)+j + 1] = 0x07 ;
      }
    }
    if (ptrBiosArea->CURSOR_pos[pag][1] >= numLineas)
      ptrBiosArea->CURSOR_pos[pag][1] = ptrBiosArea->CURSOR_pos[pag][1] - numLineas ;
    else
      ptrBiosArea->CURSOR_pos[pag][1] = 0 ;
  }
}

void scrollPagBIOS ( byte_t numLineas, byte_t pag ) {
  byte_t oldPagActiva = ptrBiosArea->VIDEO_pagActiva ;
  if (pag != oldPagActiva) {
    establecerPaginaActiva(pag) ;
    scrollBIOS(numLineas) ;
    establecerPaginaActiva(oldPagActiva) ;
  }
  else
    scrollBIOS(numLineas) ;
}

void scrollBIOS ( byte_t numLineas ) {
  byte_t Col1 = (ptrBiosArea->VIDEO_width) - 1 ;
  byte_t Fila1 = ptrBiosArea->VIDEO_lastrow ;
  asm mov ah,06h
  asm mov cx,0000h                                /* dh = Fila0, dl = Col0 */
  asm mov dl,Col1
  asm mov dh,Fila1
  asm mov al,numLineas
  asm mov bh,atrNormal
  asm int 10h
}

void goToXYPag ( byte_t fila, byte_t columna, byte_t pag ) {
  asm mov ah,02h                      /* establecer la posicion del cursor */
  asm mov bh,pag                                        /* pagina de video */
  asm mov dh,fila
  asm mov dl,columna
  asm int 10h
}

void goToXYBIOS ( byte_t fila, byte_t columna ) {
  byte_t numPagActiva = ptrBiosArea->VIDEO_pagActiva ;
  asm mov ah,02h                      /* establecer la posicion del cursor */
  asm mov bh,numPagActiva                               /* pagina de video */
  asm mov dh,fila
  asm mov dl,columna
  asm int 10h
}

void setCursorBIOS ( byte_t linea1, byte_t linea2 ) {
  asm mov ah,01h             /* establecer el tama¤o y la forma del cursor */
/* asm mov al,modo */                              /* modo = modo de video */
  asm mov ch,linea1
  asm mov cl,linea2
  asm int 10h
}

void ocultaCursorBIOS ( void ) {
  setCursorBIOS(0x20, 0x00) ;
}

void readXYPagBIOS ( byte_t far * fila,
                     byte_t far * columna,
                     byte_t far * linea1,
                     byte_t far * linea2,
                     byte_t pag ) {
  asm mov ah,03h                 /* leer la posicion del cursor y su forma */
  asm mov bh,pag                                        /* pagina de video */
  asm int 10h
  *fila    = _DH ;
  *columna = _DL ;
  *linea1  = _CH ;
  *linea2  = _CL ;
}

void readXYBIOS ( byte_t far * fila,
                  byte_t far * columna,
                  byte_t far * linea1,
                  byte_t far * linea2 ) {
  byte_t numPagActiva = ptrBiosArea->VIDEO_pagActiva ;
  asm mov ah,03h                 /* leer la posicion del cursor y su forma */
  asm mov bh,numPagActiva                               /* pagina de video */
  asm int 10h
  *fila    = _DH ;
  *columna = _DL ;
  *linea1  = _CH ;
  *linea2  = _CL ;
}

/* Memoria (int 12h) */
                                         /* tambien en BIOS AREA 0000:0413 */
word_t memBIOS ( void ) {  /* memoria reportada por el BIOS (en Kilobytes) */
  asm int 12h ;
  return(_AX) ;
}

/* Puerto serie COM1 (int 14h) */

   /* Las siguientes funciones permiten enviar y recibir caracteres por el */
   /* puerto COM1. Hay que tener en cuenta que el BIOS permite las         */
   /* interrupciones durante la ejecucion de esas llamadas, por lo que no  */
   /* deben ser utilizadas desde el sistema operativo propiamente dicho.   */

void enviarBIOS ( char car ) {
  asm mov al, car
  asm mov ah, 01h
  asm mov dx, 0000h                                   /* puerto serie COM1 */
  asm int 14h
}

char recibirBIOS ( void ) {
  char car ;
  asm mov ah, 02h
  asm mov dx, 0000h                                   /* puerto serie COM1 */
  asm int 14h
  asm mov car, al
  return(car) ;
}

/* APM */

bool_t hayApmBIOS ( word_t far * version ) {
  bool_t res ;
  word_t v ;
  asm mov ax,5300h
  asm mov bx,0000h
  asm int 15h
  asm jnc hayApm
  res = TRUE ;
  asm jmp comun
hayApm: ;
  res = TRUE ;
comun:
  asm mov v,ax
  *version = v ;
  return(res) ;
}

int connectApmBIOS ( void ) {
  byte_t res ;
  asm mov ax,5301h
  asm mov bx,0000h
  asm int 15h
  asm jc apmConnectError
  return(0) ;
apmConnectError: ;
  asm mov res,ah
  return(res) ;
}

int disconnectApmBIOS ( void ) {
  byte_t res ;
  asm mov ax,5304h
  asm mov bx,0000h
  asm int 15h
  asm jc apmDisconnectError
  return(0) ;
apmDisconnectError: ;
  asm mov res,ah
  return(res) ;
}

void cpuIdleBIOS ( void ) {
  asm mov ax,5305h ;
  asm int 15h
}

void cpuBusyBIOS ( void ) {
  asm mov ax,5306h ;
  asm int 15h
}
