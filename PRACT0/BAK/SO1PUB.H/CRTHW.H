/* ----------------------------------------------------------------------- */
/*                                  crthw.h                                 */
/* ----------------------------------------------------------------------- */
/*        hardware del controlador de video (sin utilizar el bios)         */
/* ----------------------------------------------------------------------- */

#ifndef CRTHW_H
#define CRTHW_H

#include <so1pub.h\tipos.h>

void inicCrtHw ( void ) ;

void goToXYHw ( byte_t fila, byte_t columna ) ;

void setCursorHw ( byte_t linea1, byte_t linea2 ) ;

void ocultaCursorHw ( void ) ;

#endif /* CRTHW_H */