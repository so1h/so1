/* ----------------------------------------------------------------------- */
/*                                recursos.h                               */
/* ----------------------------------------------------------------------- */
/*                  Tabla de Ficheros Abiertos del Sistema                 */
/* ----------------------------------------------------------------------- */

#ifndef RECURSOS_H
#define RECURSOS_H

#include <so1pub.h\def_proc.h>

void inicRecursos ( void ) ;

rindx_t crearRec ( descRecurso_t far * dR ) ;

dfs_t crearFich ( const char far * nombre,
                  rindx_t          rindx,
                  word_t           menor,
                  tipoFichero_t    tipo ) ;

int destruirFich ( dfs_t dfs ) ;

int destruirRec ( const char far * nombre ) ;

#endif /* RECURSOS_H */

