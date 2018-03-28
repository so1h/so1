/* ----------------------------------------------------------------------- */
/*                                  usrdbg.h                               */
/* ----------------------------------------------------------------------- */
/*    funcion llamarAlPuntoDeParadaSO1 para facilitar la depuracion de     */
/*                            programas de usuario                         */
/* ----------------------------------------------------------------------- */

#ifndef USRDBG_H
#define USRDBG_H

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

void llamarAlPuntoDeParadaSO1 ( void ) ;

#endif /* USRDBG_H */
