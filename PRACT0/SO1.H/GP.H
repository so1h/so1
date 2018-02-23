/* ----------------------------------------------------------------------- */
/*                                   gp.h                                  */
/* ----------------------------------------------------------------------- */
/*                           Gestor de Procesos:                           */
/* ----------------------------------------------------------------------- */

#ifndef GP_H
#define GP_H

#include <so1pub.h\tipos.h>

pid_t k_fork ( ) ;

pid_t k_exec ( const char far * nombre,
               const char far * comando ) ;

pid_t k_waitpid ( pid_t pid, int far * statloc ) ;

void k_exit ( int status ) ;

pid_t k_getpid ( void ) ;

pindx_t k_getpindx ( void ) ;

int k_killpid ( int pid ) ;

extern rindx_t rec_gp ;

pindx_t inicGP ( void ) ;

#endif /* GP_H */
