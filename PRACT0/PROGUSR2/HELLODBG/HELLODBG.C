/* ----------------------------------------------------------------------- */
/*                                hellodbg.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/*                 programa hola mundo para el sistema SO1                 */
/*                  con posibilidad de depuracion (Bochs)                  */
/*                                                                         */
/*       Se ilustra la llamada a la funcion llamarAlPuntoDeParadaSO1       */
/*       de usrdbg que cede el control a un direccion conocida desde       */
/*       la cual se nos devuelve el control. Dicha dirección es la         */
/*       correspondiente a la funcion puntoDeParadaSO1. El valor           */
/*       concreto de esa direccion puede consultarse en SO1.MAP y en       */
/*       el momento actual es: 0x006B2, de manera que poniendo en el       */
/*       depurador un punto de parada ahi podemos detener la maquina       */
/*       y ejecutando paso a paso retornar a la direccion siguiente        */
/*       a la llamada: llamarAlPuntoDeParadaSO1.                           */ 
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\stdio.h>                             /* printf, getchar */

#include <so1pub.h\usrdbg.h>                   /* llamarAlPuntoDeParadaSO1 */

int main ( void )
{
    llamarAlPuntoDeParadaSO1() ;              /* cede el control a 0x006B2 */
	                            /* y desde ahí la funcion puntoDeParadaSO1 */
	printf("\n\n Hola mundo.") ;           /* nos devuelve el control aquí */ 
    
    getchar() ;
    printf("\n") ;
	return(0) ;
}