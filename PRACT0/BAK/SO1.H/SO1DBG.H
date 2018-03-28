/* ----------------------------------------------------------------------- */
/*                                  so1dbg.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef SO1DBG_H
#define SO1DBG_H

#include <so1pub.h\tipos.h>                                      /* byte_t */

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

void far puntoDeParadaSO1 ( void ) ;

/* inicTeclado hace un reset del controlador del teclado, lo que es        */
/* necesario para que las demas funciones leerScancodeListo, leerScancode  */
/* y esperarScanCode funcionen correctamente. Otra posibilidad peor seria  */
/* permitir las interrupciones.                                            */

void inicTeclado ( void ) ;

/* leerScancodeListo devuelve el codigo de scan de la ultima tecla         */
/* introducida o un 0x00 si ese codigo ya ha sido leido del registro de    */
/* datos, del teclado.                                                     */

byte_t leerScancodeListo ( void ) ;

/* leerScancode espera (de forma activa) a que haya un codigo de scan      */
/* disponible (distinto de 0x00) en el registro de datos, del teclado y lo */
/* devuelve como resultado.                                                */

byte_t leerScancode ( void ) ;

/* esperarScancode hace lo mismo que leerScancodeListo                     */

#define esperarScancode() leerScancode()

#endif /* SO1DBG_H */
