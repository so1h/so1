/* ----------------------------------------------------------------------- */
/*                                  fake86.c                               */
/* ----------------------------------------------------------------------- */
/*          programa que muestra un error en la emulación del 80186        */
/*                            por parte de Fake86                          */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1pub.h\stdio.h>                             /* printf, getchar */

void funcion ( int i ) 
{
    printf("\n\n   funcion: valor actual del parametro = %i", i) ;
}

#pragma argsused         /* para evitar el warning relativo al parametro i */

int funcionAsm ( int i ) 
{ 
	asm cli ;
//	while(TRUE) ; 
	asm push bp
	asm mov bp,sp
	asm mov ax,[bp+4] ;             /* esta es una instruccion mal emulada */
	asm pop bp
	asm sti
	return(_AX) ;
}

void funcion2 ( void ) 
{
	int i ;
	i = -1 ;
    printf("\n\n   funcion: variable local i = %i", i) ;
}

void funcion3 ( byte_t i ) 
{
    printf("\n\n   funcion: valor actual del parametro = %i", i) ;
}

int main ( void )
{
	int local ; 
	printf(
	    ""                                                               "\n"
		""                                                               "\n"
		" Se muestra un error de emulacion en el paso de parametros"     "\n"
		""                                                               "\n"
		" Se va a llamar a una funcion con funcion(0)"
	) ;
    getchar() ;
	funcion(0) ;
    printf(
	    ""                                                               "\n"
		""                                                               "\n"
		" Se va a llamar a una funcion con funcion(1)"
	) ;
    getchar() ;
	funcion(1) ;
    printf(
	    ""                                                               "\n"
		""                                                               "\n"
		" Se va a llamar a una funcion con funcion(-1)"
	) ;
    getchar() ;
	funcion(-1) ;
    printf(
	    ""                                                               "\n"
		""                                                               "\n"
		" Se va a llamar a una funcion con funcion(-2)"
	) ;
    getchar() ;
	funcion(-2) ;
    getchar() ;
    printf(
	    ""                                                               "\n"
		""                                                               "\n"
		" Se va a llamar a una funcion en ensamblador con funcionAsm(-1)"
	) ;
    getchar() ;
    printf(
	    ""                                                               "\n"
		""                                                               "\n"
		"   funcionAsm: valor actual del parametro = %i", funcionAsm(-1)
	) ;
    getchar() ;	
    printf(
	    ""                                                               "\n"
		""                                                               "\n"
		" Se va a llamar a una funcion2 con variable local i ") ;
	funcion2() ;
    getchar() ;	
    printf("\n") ;
	return(0) ;
}