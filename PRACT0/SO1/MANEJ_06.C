/* ----------------------------------------------------------------------- */
/*                               manej_06.c                                */
/* ----------------------------------------------------------------------- */
/*          manejador de las llamadas al sistema del grupo AH = 06         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>                              /* USER2SYSTEM */
#include <so1.h\procesos.h>

void so1_manejador_06 ( void ) {                       /* ah = 6 ; int SO1 */

    switch (tramaProceso->AX) 
	{
	  
    case USER2SYSTEM :                                           /* 0x0600 */
                                                            /* user2system */ 				
		tramaProceso->Flags |= 0x0001 ;                          /* CF = 1 */ 
		asm stc ;                                                /* CF = 1 */
		
		_DX = tramaProceso->CS ;                          /* jmp far CS:IP */
        asm push dx ;
        _DX = tramaProceso->IP ;
        asm push dx ;
    	asm retf ;	
		
        break ;                                              

    default: ;

    }
}