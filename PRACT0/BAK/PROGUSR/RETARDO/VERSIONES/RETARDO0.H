/* ----------------------------------------------------------------------- */
/*                                retardo.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef RETARDO_H
#define RETARDO_H

#include <so1pub.h\tipos.h>

typedef int far ( * retardarProceso_t ) ( void ) ;

extern rindx_t rec_retardo ;

dword_t inicRetardo ( void ) ;

#endif /* RETARDO_H */
