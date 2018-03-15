/* ----------------------------------------------------------------------- */
/*                                 raton.h                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef RATON_H
#define RATON_H

#include <so1pub.h\tipos.h>                             /* byte_t, rindx_t */
#include <so1pub.h\def_rat.h>
#include <so1pub.h\printgen.h>

typedef estadoRaton_t argCbRaton_t ;

extern rindx_t rec_RATON ;

void inicRaton ( bool_t hayNT, bool_t hayDBox ) ;

#endif /* RATON_H */


