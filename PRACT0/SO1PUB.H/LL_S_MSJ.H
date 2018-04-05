/* ----------------------------------------------------------------------- */
/*                               ll_s_msj.h                                */
/* ----------------------------------------------------------------------- */
/*   biblioteca de funciones de interfaz de las llamadas al sistema SO1    */
/* ----------------------------------------------------------------------- */

#ifndef LL_S_MSJ_H
#define LL_S_MSJ_H

#include <so1pub.h\tipos.h>

#define mensaje_t void                                   /* cualquier cosa */

#define tamMensaje 16                              /* mensajes de 16 bytes */

#define nVIntSO1 0x60 

#define ANY ((pindx_t)0xFAD1)           /* para la comunicacion asimetrica */

#define SEND    0x0500
#define RECEIVE 0x0501
#define SENDREC 0x0502
#define NOTIFY  0x0503
#define ECHO    0x0504

/* comparar con MINIX3\include\minix\_ipc.h */

int send ( pindx_t pindx, mensaje_t * msj ) ;

int receive ( pindx_t pindx, mensaje_t * msj ) ;

int sendrec ( pindx_t pindx, mensaje_t * msj ) ;

int notify ( pindx_t pindx ) ;

int echo ( pindx_t pindx, mensaje_t * msj ) ;

#endif /* LL_S_MSJ_H */