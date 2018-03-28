/* ----------------------------------------------------------------------- */
/*                                 puertos.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\puertos.h>

byte_t inportb ( word_t port ) {
  asm mov dx,port
  asm in al,dx
  asm xor ah,ah
  return _AX ;
}

word_t inport ( word_t port ) {
  asm mov dx,port ;
  asm in ax,dx ;
  return _AX ;
}

void outport ( word_t port, word_t val ) {
  asm mov dx,port
  asm mov ax,val
  asm out dx,ax
}

void outportb ( word_t port, byte_t val ) {
  asm mov dx,port
  asm mov al,val
  asm out dx,al
}

/* retardar_io se necesita para ralentizar la E/S con algun hardware     */

//#define retardar_io( ) asm { jmp $+2 ; jmp $+2 ; } ;
#define retardar_io( ) asm { db 0x0EB, 0x00 ; db 0x0EB, 0x00 ; } ;

byte_t inportb_r ( byte_t port ) {
  int __ret = inportb(port) ;
  retardar_io() ;
  return(__ret) ;
}

void outportb_r ( byte_t port, byte_t val ) {
  outportb(port, val) ;
  retardar_io() ;
}

/* timer 8254 (ver DataSheet) */

word_t contadorTimer0 ( void ) {  /* valor actual del contador 0 del timer */
//asm mov al,11010010b       /* 11 01 001 0 : ReadBack Command, contador 0 */
  asm mov al,0D2h            /* 11 01 001 0 : ReadBack Command, contador 0 */
  asm out 43h,al
//asm jmp $+2
  asm db 0EBh, 00h ;
  asm in al,40h
  asm mov ah,al
//asm jmp $+2
  asm db 0EBh, 000h ;
  asm in al,40h
  asm xchg ah,al
  return(_AX) ;
}


