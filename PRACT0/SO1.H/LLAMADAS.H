/* ----------------------------------------------------------------------- */
/*                               llamadas.h                                */
/* ----------------------------------------------------------------------- */
/*                           llamadas al sistema                           */
/* ----------------------------------------------------------------------- */

#ifndef LLAMADAS_H
#define LLAMADAS_H

#include <so1pub.h\tipos.h>

#define nVIntSO1     0x60            /* numero de vector utilizado por SO1 */

void far isr_SO1 ( void ) ;

#endif /* LLAMADAS_H */

