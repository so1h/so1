/* ----------------------------------------------------------------------- */
/*                               ll_s_u2s.c                                */
/* ----------------------------------------------------------------------- */
/*                    llamada al sistema user2system                       */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_u2s.h>     /* macro_user2system, macro_system2user */

/* Estas funciones intentan hacer lo mismo que la macro_user2system, pero  */
/* parece que NO FUNCIONAN BIEN. Por tanto lo mejos es utilizar las macros */
/* macro_user2system y system2user.                                        */

void user2system ( void )
{                                     
    asm pop bx ;                 /* (pila del usuario) dir de retorno a bx */
    macro_user2system() ;                                       /* sistema */
    _BX = tramaProceso->BX ;                        /* bx del usuario a bx */             
    asm push bx ;                  /* dir de retorno a la pila del sistema */
}

void system2user ( void ) /* usamos dx en vez de bx (problema al compilar) */
{    
    asm pop dx ;                 /* (pila del sistema) dir de retorno a dx */    
    tramaProceso->DX = _DX ;                        /* dx a dx del usuario */
    macro_system2user() ;                                       /* usuario */    
    asm push dx ;	               /* dir de retorno a la pila del usuario */
}