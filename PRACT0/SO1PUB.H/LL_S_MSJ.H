/* ----------------------------------------------------------------------- */
/*                               ll_s_msj.h                                */
/* ----------------------------------------------------------------------- */
/*   biblioteca de funciones de interfaz de las llamadas al sistema SO1    */
/* ----------------------------------------------------------------------- */

#ifndef LL_S_MSJ_H
#define LL_S_MSJ_H

#include <so1pub.h\tipos.h>

#define ANY (-1) 

/* comparar con MINIX3\include\minix\_ipc.h */

int send ( pindx_t pindx, void * msj ) ;

int receive ( pindx_t pindx, void * msj ) ;

int sendrec ( pindx_t pindx, void * msj ) ;

int notify ( pindx_t pindx ) ;

int echo ( pindx_t pindx, void * msj ) ;

#endif /* LL_S_MSJ_H */