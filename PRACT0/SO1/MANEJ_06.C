/* ----------------------------------------------------------------------- */
/*                               manej_06.c                                */
/* ----------------------------------------------------------------------- */
/*          manejador de las llamadas al sistema del grupo AH = 06         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1.h\procesos.h>

void so1_manejador_06 ( void ) {                       /* ah = 6 ; int SO1 */

    bool_t Ok ;

    switch (tramaProceso->AL) {
	  
    case 0x00 :                                                    /* 0x00 */
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