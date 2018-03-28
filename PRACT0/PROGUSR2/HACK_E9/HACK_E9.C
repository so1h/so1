/* ----------------------------------------------------------------------- */
/*                                 hack_e9.c                               */
/* ----------------------------------------------------------------------- */
/*              prueba del hack E9 disponible en bochs y qemu              */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>       /* biblioteca llamadas al sistema SO1 */ 
#include <so1pub.h\stdio.h>                             /* printf, getchar */
#include <so1pub.h\carsctrl.h>                                      /* ESC */

int main ( void )
{   
    char car ;
    printf(
	    ""                                                               "\n"
		""                                                               "\n"
		" Prueba del hack E9 disponible en bochs y qemu."                "\n"
		""                                                               "\n"
	) ;

    for ( ; ; ) 
	{
		car = getchar() ;
		if (car == ESC) break ;
		putchar(car) ;
		if (car == '\n') putchar('\r') ;
		asm 
		{
			mov al,car ;
		    out 0xE9,al ;
		}
		
	} 
 	
    printf("\n") ;
	return(0) ;
}