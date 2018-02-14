/* ----------------------------------------------------------------------- */
/*                                  usrdbg.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>            /* byte_t, word_t, address_t, MK_FP */
#include <so1pub.h\ll_s_so1.h>                                 /* nVIntSO1 */

/* llamarAlPuntoDeParadaSO1 se utiliza para simplificar la tarea de        */
/* indicar al depurador que se detenga en una determinada instruccion de   */
/* un programa de usuario de SO1. El punto de parada debe ponerse en la    */
/* direccion de memoria correspondiente a la funcion del sistema SO1       */
/* puntoDeParadaSO1, la cual se obtiene juntando el segmento de carga de   */
/* SO1 con el desplazamiento de la funcion puntoDeParadaSO1, el cual       */
/* puede obtenerse inspeccionando el fichero SO1.MAP.                      */

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

void limiteSuperior ( void ) 
{
}

