/* ----------------------------------------------------------------------- */
/*                                 k_msj.h                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef K_MSJ_H
#define K_MSJ_H

#include <so1pub.h\tipos.h>                                       /* pindx */
#include <so1pub.h\ll_s_msj.h>                                /* mensaje_t */

void k_send ( pindx_t pindx, mensaje_t * msj ) ;

void k_receive ( pindx_t pindx, mensaje_t * msj ) ;  /* pindx puede ser ANY */

void k_sendrec ( pindx_t pindx, mensaje_t * msj ) ;

void k_notify ( pindx_t pindx ) ;

#endif /* K_MSJ_H */