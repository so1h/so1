/* ----------------------------------------------------------------------- */
/*                                 crthw.c                                 */
/* ----------------------------------------------------------------------- */
/*        hardware del controlador de video (sin utilizar el bios)         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                      /* byte_t */

/* Las siguientes direcciones pueden encontrarse en el HelpPC en           */
/* Hardware Data and Specifications y 6845                                 */

#define CRTCPort1    0x03D4                 /* Registro de indice del CRTC */
#define CRTCPort2    0x03D5                   /* Registro de dato del CRTC */
#define CRTCAddHi    0x0C       /* Reg. byte alto de la direccion de video */
#define CRTCAddLo    0x0D       /* Reg. byte bajo de la direccion de video */
#define CRTCCurHi    0x0E     /* Reg. byte alto de la direccion del cursor */
#define CRTCCurLo    0x0F     /* Reg. byte bajo de la direccion del cursor */
#define CRTCCurStart 0x0A                 /* Reg. cursor start (scan line) */
#define CRTCCurEnd   0x0B                 /* Reg. cursor end   (scan line) */

byte_t CRTC0C ;                   /* Reg. CRT 0C byte alto de la direccion */
byte_t CRTC0D ;                   /* Reg. CRT 0D byte bajo de la direccion */

void inicCrtHw ( void ) {

  /* Establece como pantalla por defecto para el hardware VGA estandar la  */
  /* pantalla de video 0.                                                  */

  CRTC0C = (byte_t)0 ;
  CRTC0D = (byte_t)0 ;

  asm mov al,CRTCAddHi                             /* Indice del byte alto */
  asm mov dx,CRTCPort1                                 /* Puerto de indice */
  asm out dx,al
  asm mov al,CRTC0C                        /* valor del byte alto a enviar */
  asm mov dx,CRTCPort2                                   /* Puerto de dato */
  asm out dx,al

  asm mov al,CRTCAddLo                             /* Indice del byte bajo */
  asm mov dx,CRTCPort1                                 /* Puerto de indice */
  asm out dx,al
  asm mov al,CRTC0D                        /* valor del byte bajo a enviar */
  asm mov dx,CRTCPort2                                   /* Puerto de dato */
  asm out dx,al

}

void goToXYHw ( byte_t fila, byte_t columna ) {

  word_t despl ;

  despl = (fila*80)+columna ;

  asm mov dx,CRTCPort1                                 /* Puerto de indice */
  asm mov al,CRTCCurLo
  asm out dx,al                            /* Indice 0Fh para el byte bajo */
  asm mov dx,CRTCPort2                                   /* Puerto de dato */
  asm mov ax,despl
  asm out dx,al                                   /* Enviamos el byte bajo */

  asm mov dx,CRTCPort1                                 /* Puerto de indice */
  asm mov al,CRTCCurHi
  asm out dx,al                                /* Indice para el byte alto */
  asm mov dx,CRTCPort2                                   /* Puerto de dato */
  asm mov ax,despl
  asm shr ax,8
  asm out dx,al                                   /* Enviamos el byte alto */

}

void setCursorHw ( byte_t linea1, byte_t linea2 ) {

  asm mov dx,CRTCPort1                                 /* Puerto de indice */
  asm mov al,CRTCCurStart
  asm out dx,al                            /* Indice 0Fh para el byte bajo */
  asm mov dx,CRTCPort2                                   /* Puerto de dato */
  asm mov ax,linea1
  asm out dx,al                                   /* Enviamos el byte bajo */

  asm mov dx,CRTCPort1                                 /* Puerto de indice */
  asm mov al,CRTCCurEnd
  asm out dx,al                                /* Indice para el byte alto */
  asm mov dx,CRTCPort2                                   /* Puerto de dato */
  asm mov ax,linea2
  asm out dx,al                                   /* Enviamos el byte alto */

}

void ocultaCursorHw ( void ) {
  setCursorHw(0x20, 0x00) ;
}


