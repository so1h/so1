/* ----------------------------------------------------------------------- */
/*                                bios_rat.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\pantalla.h>
#include <so1pub.h\pic.h>
#include <so1pub.h\bios_rat.h>

#include <so1.h\ajustes.h>
#include <so1.h\procesos.h>
#include <so1.h\plot.h>

/* Raton (int 33h) */

static bool_t hayMouseBIOS ;

bool_t hayDriverRaton ( void ) {
  pointer_t far * ptrDriver =
    (pointer_t far *)(0x33*4) ;
  return(**ptrDriver != 0xCF) ;           /* 0xCF = codigo maquina de IRET */
}

bool_t hayRatonBIOS ( void ) {
  return(hayMouseBIOS) ;
}

word_t resetRatonBIOS ( word_t * numBotones ) {
  word_t estado ;
  word_t num ;
  asm mov ax,0000h
  asm int 33h
  asm mov num,bx
  asm mov estado,ax
  *numBotones = num ;
  hayMouseBIOS = (estado == 0xFFFF) ;
  return(estado) ;
}

void mostrarCursorRatonBIOS ( void ) {
  asm mov ax,0001h
  asm int 33h
}

void ocultarCursorRatonBIOS ( void ) {
  asm mov ax,0002h
  asm int 33h
}

void leerRatonBIOS ( word_t * W, word_t * X, word_t * Y ) {

  word_t W0 ;                                     /* W = 00000000 00000MRL */
  word_t X0 ;
  word_t Y0 ;

  asm mov ax,0003h
  asm int 33h
  asm mov W0,bx
  asm mov X0,cx
  asm mov Y0,dx
  *W = W0 ;
  *X = X0 ;
  *Y = Y0 ;
}

void situarCursorRatonBIOS ( word_t X, word_t Y ) {
  asm mov cx,X
  asm mov dx,Y
  asm mov ax,0004h
  asm int 33h
}

void installMouseEventHandler ( handler_t handler ) {

  asm mov cx,007fh                           /* capturar todos los eventos */
  asm mov ax,000ch
  asm les dx,handler
  asm int 33h

}

void uninstallMouseEventHandler ( handler_t handler ) {

  asm mov cx,0000h                           /* capturar todos los eventos */
  asm mov ax,000ch
  asm les dx,handler
  asm int 33h

}

void versionRatonBIOS ( word_t * menor,
                        word_t * mayor,
                        word_t * tipo,
                        word_t * irq ) {
  word_t v ;
  word_t w ;
  asm mov ax,0024h
  asm int 33h
  asm mov v,bx
  asm mov w,cx
  *menor = (v >> 8) ;
  *mayor = (v & 0x00FF) ;
  *tipo = (w >> 8) ;
  *irq = (w & 0x00FF) ;
}

