/* ----------------------------------------------------------------------- */
/*                                  usrdbg.c                               */
/* ----------------------------------------------------------------------- */
/*    funcion llamarAlPuntoDeParadaSO1 para facilitar la depuracion de     */
/*                            programas de usuario                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>            /* byte_t, word_t, address_t, MK_FP */
#include <so1pub.h\ll_s_so1.h>                                 /* nVIntSO1 */

/* llamarAlPuntoDeParadaSO1 se utiliza para simplificar la tarea de        */
/* indicar al depurador que se detenga en una determinada instruccion de   */
/* un programa de usuario de SO1. El punto de parada debe ponerse en la    */
/* direccion de memoria correspondiente a la funcion del sistema SO1       */
/* puntoDeParadaSO1, la cual se obtiene juntando el segmento de carga de   */
/* SO1 con el desplazamiento de la funcion puntoDeParadaSO1, el cual       */
/* puede obtenerse inspeccionando el fichero SO1.MAP:                      */
/*                                                                         */
/* ...                                                                     */
/* Module: obj\so1dbg.obj(so1dbg.c)                                        */
/* 0000:00b2*     _puntoDeParadaSO1 <----------- CS_SO1:00B2               */
/* 0000:00c5      _inicTeclado                                             */
/* 0000:00e6*     _leerScancodeListo                                       */ 
/* 0000:00e9      _leerScancode                                            */
/*                                                                         */
/* Si como es habitual SO1 esta cargado en la direccion 0060:0000, la      */
/* direccion segmentada efectiva de puntoDeParadaSO1 seria: 0060:00B2, y   */
/* la direccion lineal: 0x006B2.                                           */
/*                                                                         */
/* La idea es poner ese punto de parada en el depurador (comando b 0x006B2 */
/* por ejemplo en Bochs) de manera que cuando se ejecute puntoDeParadaSO1  */
/* la ejecución se detenga en 0x006B2, desde donde continuando paso a paso */
/* con la ejecucion se retornaria a la siguiente intruccion a esa llamada. */

#define OFFSET_PuntoDeParadaSO1 0x00B2        /* tomar el valor de SO1.MAP */
                                               /* funcion puntoDeParadaSO1 */
void limiteSuperior ( void ) ;											   
											   
void llamarAlPuntoDeParadaSO1 ( void ) {
  address_t far * ptrTVI = MK_FP(0x0000, 0x0000) ;
  word_t CS_SO1 = ptrTVI[nVIntSO1].segment ;
  _AX = OFF(limiteSuperior)-2 ;                             /* __continuar */
  asm {
    push cs
//  push OFFSET @
//  push OFFSET _continuar
    push ax                                     /* apilamos CS:__continuar */                
    push CS_SO1 ;     
    push OFFSET_PuntoDeParadaSO1 ;     /* apilamos CS_SO1:PuntoDeParadaSO1 */    
    retf ;                           /* saltamos a CS_SO1:PuntoDeParadaSO1 */
//@: ;
__continuar: ;           /* que luego devuelve el control a CS:__continuar */  
  }
}

void limiteSuperior ( void )                                  /* no borrar */
{
}