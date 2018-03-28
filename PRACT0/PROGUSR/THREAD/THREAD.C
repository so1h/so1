/* ----------------------------------------------------------------------- */
/*                                 thread.c                                */
/* ----------------------------------------------------------------------- */
/*            programa de prueba de la llamada al sistema thread           */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */
#include <so1pub.h\carsctrl.h>                                      /* ESC */

#include <so1pub.h\debug.h>                               /* puntoDeParada */

void * funcion ( void * arg ) 
{
	while (TRUE) {
        (*(pointer_t)MK_FP(0xB800, (word_t)arg))++ ;
		yield() ;
	}		
}	

void * eco ( void * arg )                                 /* eco CON3 CON6 */
{
	char car ;
	int dfIn = open("CON5", O_RDONLY) ; 
	int dfOut = open("CON6", O_WRONLY) ; 
	while (TRUE) 
	{
		read(dfIn, &car, 1) ;
		write(dfOut, &car, 1) ;
	}
}	

int main ( void )
{
	
//	thread(eco,     0x2FFE, (void *)0x0000) ;
	thread(funcion, 0x1FFE, (void *)0x0000) ;
	thread(funcion, 0x0FFE, (void *)0x0010) ;
	funcion((void *)0x0020) ;
	return(0) ;
}