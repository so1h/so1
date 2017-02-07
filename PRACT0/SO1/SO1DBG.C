/* ----------------------------------------------------------------------- */
/*                                  so1dbg.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                      /* byte_t */
#include <so1pub.h\puertos.h>            /* PORT_PS2_DATA, PORT_PS2_STATUS */

/* puntoDeParadaSO1 es una funcion pensada para poner puntos de parada en  */
/* el codigo del sistema SO1 (y los programas de usuario). Para poner un   */
/* punto de parada dentro de SO1 basta llamar a la funcion                 */
/* puntoDeParadaSO1(). Luego en el depurador (bochs dgui) debemos poner    */
/* como punto de parada (comando b) la direccion correspondiente al        */
/* segmento de carga de SO1 (por ejemplo 0x0060) y el desplazamiento de la */
/* funcion puntoDeParadaSO1(), el cual puede ser consultado en el fichero  */
/* SO1.MAP o SO1BIN.MAP. Como a la hora de enlazar SO1 ponemos so1dbg.obj  */
/* como uno de los primeros ficheros objeto la direccion de                */
/* puntoDeParadaSO1 normalmente no variara al recompilar.                  */

void far puntoDeParadaSO1 ( void ) {
  asm {
//#ifndef IDE
//    retf ;
//    db 'Punto De Parada SO1' ;
//#else
    db 0xCB ;
    db 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31,0x31, 0x31, 0x31 ;
    db 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31 ;
//#endif
  }
}

/* inicTeclado hace un reset del controlador del teclado, lo que es        */
/* necesario para que las demas funciones leerScancodeListo, leerScancode  */
/* y esperarScanCode funcionen correctamente.                              */

void inicTeclado ( void ) {
  asm {
    xor cx,cx ;
@10:
    in al,PORT_PS2_STATUS ;        /* espera para poder enviar un comando */
    test al,02 ;
    loopne @10 ;
    mov al,0d1h ;     /* comando escribir en el puerto de salida del 8042 */
    out PORT_PS2_STATUS,al ;
    xor cx,cx ;
@11:
    in al,PORT_PS2_STATUS ;
    test al,02 ;
    loopne @11 ;
//  mov al,0dfh ;              /* enviado al puerto de salida: A20, reset */
    mov al,0ddh ;              /* enviado al puerto de salida:      reset */
    out PORT_PS2_DATA,al ;
    xor cx,cx ;
@12:
    in al,PORT_PS2_STATUS ;
    test al,02 ;
    loopne @12 ;
  }
}

/* leerScancodeListo devuelve el codigo de scan de la ultima tecla         */
/* introducida o un 0x00 si ese codigo ya ha sido leido del registro de    */
/* datos, del teclado.                                                     */

byte_t leerScancodeListo ( void ) {
//puntoDeParadaSO1() ;
  asm in al,PORT_PS2_DATA ;          /* PORT_PS2_DATA (so1pub.h\puertos.h) */
  return(_AL) ;
}

#define codigoLeerScanCode( x )                                              \
  asm {                                                                      \
@ ## x ## :                                                                  \
    nop ;                                          /* necesario un retardo */\
    in al,PORT_PS2_STATUS ;                                                  \
    and al,01h ;                                                             \
    je @ ## x ## ;                                                           \
    in al,PORT_PS2_DATA ;                                                    \
  }

/* leerScancode espera (de forma activa) a que haya un codigo de scan      */
/* disponible (distinto de 0x00) en el registro de datos, del teclado y lo */
/* devuelve como resultado.                                                */

byte_t leerScancode ( void ) {
  codigoLeerScanCode(20) ;
  return(_AL) ;
}

