/* ----------------------------------------------------------------------- */
/*                                  usrdbg.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef USRDBG_H
#define USRDBG_H

/* llamarAlPuntoDeParadaSO1 se utiliza para simplificar la tarea de        */
/* indicar al depurador que se detenga en una determinada instruccion de   */
/* un programa de usuario de SO1. El punto de parada debe ponerse en la    */
/* direccion de memoria correspondiente a la funcion del sistema SO1       */
/* puntoDeParadaSO1, la cual se obtiene juntando el segmento de carga de   */
/* SO1 con el desplazamiento de la funcion puntoDeParadaSO1, el cual       */
/* puede obtenerse inspeccionando el fichero SO1.MAP.                      */

void llamarAlPuntoDeParadaSO1 ( void ) ;

#endif /* USRDBG_H */
