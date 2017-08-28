/* ----------------------------------------------------------------------- */
/*                                  fake86.c                               */
/* ----------------------------------------------------------------------- */
/*          programa que muestra un error en la emulación del 80186        */
/*                            por parte de Fake86                          */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>    /* escribirStr, escribirInt, escribirHex */
#include <so1pub.h\stdio.h>                                       /* getch */

void funcion ( int i ) {
    escribirStr("\n\n   funcion: valor actual del parametro = ") ;
	escribirInt(i, 1) ;
}

int funcionAsm ( int i ) {
	asm cli ;
//	while(TRUE) ; 
	asm push bp
	asm mov bp,sp
	asm mov ax,[bp+4] ;              /* esta es la instruccion mal emulada */
	asm pop bp
	asm sti
	return(_AX) ;
}

void funcion2 ( void ) {
	int i ;
	i = -1 ;
    escribirStr("\n\n   funcion: variable local i = ") ;
	escribirInt(i, 1) ;
}

void funcion3 ( byte_t i ) {
    escribirStr("\n\n   funcion: valor actual del parametro = ") ;
	escribirInt(i, 1) ;
}

void main ( void )
{
	int local ; 
    escribirStr("\n\n Se muestra un error de emulacion en el paso de parametros ") ;
    escribirStr("\n\n Se va a llamar a una funcion con funcion(0) ") ;
    getch() ;
	funcion(0) ;
    escribirStr("\n\n Se va a llamar a una funcion con funcion(1) ") ;
    getch() ;
	funcion(1) ;
    escribirStr("\n\n Se va a llamar a una funcion con funcion(-1) ") ;
    getch() ;
	funcion(-1) ;
    escribirStr("\n\n Se va a llamar a una funcion con funcion(-2) ") ;
    getch() ;
	funcion(-2) ;
    getch() ;
    escribirStr("\n\n Se va a llamar a una funcion en ensamblador con funcionAsm(-1) ") ;
    getch() ;
    escribirStr("\n\n   funcionAsm: valor actual del parametro = ") ;
	escribirInt(funcionAsm(-1), 1) ;
    getch() ;	
    escribirStr("\n\n Se va a llamar a una funcion2 con variable local i ") ;
	funcion2() ;
    getch() ;	
    escribirLn() ;
	
}

