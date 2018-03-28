/* ----------------------------------------------------------------------- */
/*                                 crthw.c                                 */
/* ----------------------------------------------------------------------- */
/*        hardware del controlador de video (sin utilizar el bios)         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                              /* byte_t, word_t */

/* Las siguientes direcciones pueden encontrarse en el HelpPC en           */
/* Hardware Data and Specifications y 6845                                 */

/* puertos de indice (direccion) y de dato para acceder a puertos internos */

#define CRTCPort1    0x03D4                 /* Registro de indice del CRTC */
#define CRTCPort2    0x03D5                 /* Registro de dato   del CRTC */

/* indices de puertos internos del CRTC */

#define CRTCAddHi    0x0C       /* Reg. byte alto de la direccion de video */
#define CRTCAddLo    0x0D       /* Reg. byte bajo de la direccion de video */
#define CRTCCurHi    0x0E  /* Reg. byte alto del desplazamiento del cursor */ /* despl = (fila*80)+columna */
#define CRTCCurLo    0x0F  /* Reg. byte bajo del desplazamiento del cursor */
#define CRTCCurStart 0x0A                 /* Reg. cursor start (scan line) */
#define CRTCCurEnd   0x0B                 /* Reg. cursor end   (scan line) */

#define escribirRegistro( indiceRegistro, dato )                             \
  asm {                                                                      \
    mov al,indiceRegistro ;                                                  \
	mov dx,CRTCPort1 ;                                                       \
	out dx,al ;                                                              \
    mov al,dato ;                                                            \
	mov dx,CRTCPort2 ;                                                       \
	out dx,al ;                                                              \
  }	

void inicCrtHw ( void ) {

  /* Establece como pantalla por defecto para el hardware VGA estandar la  */
  /* pantalla de video en la direccion 0x0000.                             */

  escribirRegistro(CRTCAddHi, 0x00) ;                  /* CRTCAdd = 0x0000 */
  escribirRegistro(CRTCAddLo, 0x00) ;
  
}

void goToXYHw ( byte_t fila, byte_t columna ) {

  word_t despl ;
  
  byte_t desplHi, desplLo ;

  despl = (fila*80)+columna ;
  desplHi = (byte_t)(despl >> 8) ;
  desplLo = (byte_t)(despl) ;

  escribirRegistro(CRTCCurHi, desplHi) ;       
  escribirRegistro(CRTCCurLo, desplLo) ;
  
}

void setCursorHw ( byte_t linea1, byte_t linea2 ) {

  escribirRegistro(CRTCCurStart, linea1) ;
  escribirRegistro(CRTCCurEnd,   linea2) ;
  
}

void ocultaCursorHw ( void ) {
  setCursorHw(0x20, 0x00) ;
}


