/* ----------------------------------------------------------------------- */
/*                               ejecutar.h                                */
/* ----------------------------------------------------------------------- */
/*            busqueda, carga y ejecucion de comandos externos             */
/* ----------------------------------------------------------------------- */

#ifndef EJECUTAR_H
#define EJECUTAR_H

#include <so1pub.h\tipos.h>                              /* pid_t, pindx_t */

pid_t kk_fork ( void ) ;

pindx_t preEjecutar ( const char far * nombre,
                      const char far * comando,
                            pindx_t    pindx ) ;

#endif /* EJECUTAR_H */
