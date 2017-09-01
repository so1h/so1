/* ----------------------------------------------------------------------- */
/*                                 e_cursor.c                              */
/* ----------------------------------------------------------------------- */
/*          programa que muestra un error en la emulación del 80186        */
/*                            por parte de Fake86                          */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>    /* escribirStr, escribirInt, escribirHex */
#include <so1pub.h\stdio.h>                                        /* getc */

void main ( void )
{
	byte_t F, C ; 
    escribirStr("\n\n Programa que muestra el uso de lseek para situar el cursor ") ;
    getch() ;
    escribirStr("\n escribirCursor(40, 20) ") ;
    escribirCursor(40, 20) ;
	escribirCar('\xFE') ;
    getch() ;
    escribirStr("\n escribirCursor(10, 20) ") ;
    escribirCursor(10, 20) ;
	escribirCar('\xFE') ;
    getch() ;
	escribirStr(" Hola\n") ;
    getch() ;
    escribirStr("\n\n leerCursor(F, C) = ") ;
    escribirInt(leerCursor(&F, &C), 1) ;
	escribirStr(" F = ") ;
    escribirInt(F, 1) ;
	escribirStr(" C = ") ;
    escribirInt(C, 1) ;
    getch() ;
    escribirStr("\n\n establecerLineas(0) = ") ;
    escribirInt(establecerLineas(0), 1) ;
    getch() ;
    escribirStr("\n\n establecerLineas(10) = ") ;
    escribirInt(establecerLineas(10), 1) ;
    getch() ;
    escribirStr("\n\n establecerLineas(28) = ") ;
    escribirInt(establecerLineas(28), 1) ;
    getch() ;
    escribirStr("\n\n establecerLineas(50) = ") ;
    escribirInt(establecerLineas(50), 1) ;
    getch() ;
    escribirStr("\n\n establecerLineas(50) = ") ;
    escribirInt(establecerLineas(50), 1) ;
    getch() ;
	
}

