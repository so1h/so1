/* ----------------------------------------------------------------------- */
/*                               ll_s_u2s.h                                */
/* ----------------------------------------------------------------------- */
/*                    llamada al sistema user2system                       */
/* ----------------------------------------------------------------------- */

/* La llamada al sistema user2system devuelve el control a la instruccion  */
/* siguiente a la del trap (int nVIntSO1) pero como si se estuviera en el  */
/* manejador de una llamada al sistema. Por tanto las interrupciones estan */
/* inhibidas y la pila contiene: Flags, CS, IP, trama, ...                 */
/* Ademas la llamada deja activado el flag de acarreo (CF) tanto en el     */
/* registro de estado como en el registro del proceso almacenado en la     */
/* pila (tramaProceso->Flags).                                             */
/* Para que el programa retorne al modo usuario se ofrece la funcion       */ 
/* system2user que devuelve el control en modo usuario justo a la          */
/* instruccion siguiente a la de la llamada a system2user.                 */
/* En el caso de una llamada al sistema bloqueante, al retornar del        */
/* bloqueo a la instruccion siguiente a la del trap (int nVIntSO1) es      */
/* necesario distinguir si ese retorno se debe al desbloqueo o a la misma  */
/* llamada al sistema user2system. Con ese fin user2system establece ese   */
/* flag, ya que asi en el codigo ejecutado en modo system puede            */
/* desactivarse CF para indicar que la siguiente ejecucion de la           */
/* instruccion siguiente al trap se hace en modo usuario a causa del       */
/* propio desbloqueo.                                                      */

/* La llamada al sistema user2system solo puede utilizarse desde un thread */
/* del sistema operativo (es decir desde un proceso de usuario que tenga   */
/* su mismo espacio de direccionamiento) al tener que acceder a variales   */
/* y funciones como tramaProceso o bloquearProcesoActual. Tanto la llamada */
/* a user2system/macro_user2system como system2user/macro_system2user      */
/* deben estar al mismo nivel en cuanto a la pila (es decir no puede ser   */
/* ejecutada system2user en una funcion distinta que user2system).         */
/* Un ejemplo donde puede utilizarse user2systen seria en el driver DB.C.  */
/*                                                                         */
/* No obstante estas limitaciones podrian superarse con algo de cuidado,   */
/* incluso la de que el proceso comparta el espacio de direccionamiento,   */
/* gracias a la llamada al sistema obtenInfoSO1(dirDescSO1), de manera muy */
/* parecida a como se hace en los drivers: RETARDO, TIMER, CONSOLA, ...    */

/* ----------------------------------------------------------------------- */
/* ejemplos de uso:                                                        */
/* ----------------------------------------------------------------------- */

#if (FALSE)
	
#define CON_MACRO_U2S TRUE                                           /* Ok */ 
//#define CON_MACRO_U2S FALSE                                        /* Ok */
//#define NO_BLOQUEANTE TRUE                                         /* Ok */
#define NO_BLOQUEANTE FALSE                                          /* Ok */ 

//  while (TRUE) ;                                         /* para depurar */

#if (CON_MACRO_U2S)
    printStrBIOS("\n macro_user2system() y") ;
    macro_user2system() ;
#else
    printStrBIOS("\n user2system() y") ;
    user2system() ;
#endif

#if (NO_BLOQUEANTE)
    /* llamada al sistema no bloqueante */
    printStrBIOS(" llamada al sistema no bloqueante \n") ;
#if (CON_MACRO_U2S)
    macro_system2user() ;
#else
    system2user() ;
#endif
#else
    if (_FLAGS & 0x0001)  /* CF == 1 (system), CF == 0 (user) desbloqueado */
    {
        tramaProceso->Flags &= 0xFFFE ;                          /* CF = 0 */
        /* llamada al sistema bloqueante */
        printStrBIOS(" llamada al sistema bloqueante \n") ;
        bloquearProcesoActual(rec_db) ;
    }
#endif

#endif

/* ----------------------------------------------------------------------- */
/* ejemplos de uso sin compartir el espacio de direccionamiento de SO1:    */
/* ----------------------------------------------------------------------- */

#if (FALSE)	
    obtenInfoSO1(dirDescSO1) ;   /* ptrTramaProceso, bloquearProcesoActual */ 
    macro_user2system() ;
    if (_FLAGS & 0x0001)  /* CF == 1 (system), CF == 0 (user) desbloqueado */
    {
        (*ptrTramaProceso)->Flags &= 0xFFFE ;                    /* CF = 0 */
        /* llamada al sistema bloqueante */
        printStrBIOS(" llamada al sistema bloqueante \n") ;
        bloquearProcesoActual(rec_db) ;
    }
#endif 

/* ----------------------------------------------------------------------- */
/* fin ejemplos de uso:                                                    */
/* ----------------------------------------------------------------------- */

#ifndef LL_S_U2S_H
#define LL_S_U2S_H

#include <so1pub.h\ll_s_so1.h>                              /* USER2SYSTEM */
#include <so1pub.h\tipos.h>                                        /* retf */
#include <so1.h\procesos.h>                                /* tramaProceso */
#include <so1.h\ajustes.h>                                       /* CS_SO1 */

#define nVIntSO1 0x60

#define macro_user2system( )                                                 \
	asm                                                                      \
    {                                                                        \
		mov ax,USER2SYSTEM ;                                                 \
		int nVIntSO1 ;                                                       \
	}                                                                        \

#define macro_system2user( )                                                 \
	asm                                                                      \
	{                                                                        \
	    db 0xE8, 0x00, 0x00 ;                                /* call $ + 3 */\
	    pop ax ;                                                             \
	}                                                                        \
	tramaProceso->IP = _AX + 19 ;         /* instruccion siguiente al retf */\
    asm                                                                      \
	{                                                                        \
		pop ax ;                                                             \
	    push CS_SO1 ;                                                        \
	    push ax ;                                                            \
	    retf ;                               /* retorno de so1_manejador06 */\
	}                                                                        \
	
void user2system ( void ) ;

void system2user ( void ) ;

#endif /* LL_S_U2S_H */