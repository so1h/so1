/* ----------------------------------------------------------------------- */
/*                                  video.c                                */
/* ----------------------------------------------------------------------- */
/*           prueba de llamadas al BIOS correspondientes al video          */
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
		" Prueba del llamadas al BIOS relativas al video."               "\n"
		""                                                               "\n"
	) ;

	asm mov ax,0x0000 ;
	asm int 0x10 ;

    getchar() ;
    printf("\n") ;
	return(0) ;
}