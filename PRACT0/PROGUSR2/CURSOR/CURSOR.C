/* ----------------------------------------------------------------------- */
/*                                  cursor.c                               */
/* ----------------------------------------------------------------------- */
/*    programa que muestra como leer y modificar la posicion del cursor    */
/*                           de texto de la consola                        */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\stdio.h>                             /* printf, getchar */
#include <so1pub.h\carsctrl.h>                                      /* ESC */ 
#include <so1pub.h\ctype.h>                                     /* tolower */ 
#include <so1pub.h\escribir.h>               /* leerCursor, escribirCursor */ 
//                                     /* numeroDeLineas, establecerLineas */

int main ( void )
{
	byte_t F, C, FAnt, CAnt ; 
	char car ;
	
	leerCursor(&F, &C) ;
	printf(
	    ""                                                               "\n"
		""                                                               "\n"
        " programa que juega con el cursor de texto"                     "\n"
		""                                                               "\n"
		" numeroDeLineas() = %2i"                                        "\n"
		""                                                               "\n"
		" leerCursor(&F, &C) ==> F = %i C = %i"                          "\n"
		""                                                               "\n"
		" pulse una tecla para continuar ... ", numeroDeLineas(), F, C
	) ;
	getchar() ;
	
    putchar('\f') ;	
    escribirStr("\n\n establecerLineas(0) = ") ;
    escribirInt(establecerLineas(0), 1) ;
    getchar() ;
    
    putchar('\f') ;	
    escribirStr("\n\n establecerLineas(10) = ") ;
    escribirInt(establecerLineas(10), 1) ;
    getchar() ;
    
    putchar('\f') ;	
    escribirStr("\n\n establecerLineas(28) = ") ;
    escribirInt(establecerLineas(28), 1) ;
    getchar() ;
    
    putchar('\f') ;	
    escribirStr("\n\n establecerLineas(50) = ") ;
    escribirInt(establecerLineas(50), 1) ;
    getchar() ;
    
    putchar('\f') ;	
    escribirStr("\n\n establecerLineas(25) = ") ;
    escribirInt(establecerLineas(25), 1) ;
    getchar() ;
    
    putchar('\f') ;
	F = 12 ;
	C = 40 ;
	escribirCursor(2, 2) ;
	printf("F = %2i C = %2i (teclas i, j, k, l y Esc)", F, C) ;    
	escribirCursor(F, C) ;
	putchar(0xFE) ;
	escribirCursor(F, C) ;
    do 
	{	
	    FAnt = F ;
	    CAnt = C ;
		car = tolower(getchar()) ;
		switch(car) 
		{
			case 'h' : if (C > 0) C-- ; break ;
			case 'j' : if (F < (numeroDeLineas()-1)) F++ ; break ;
			case 'k' : if (F > 0) F-- ; break ;
			case 'l' : if (C < 79) C++ ; break ;
		}
		if ((F != FAnt) || (C != CAnt)) 
		{
         	escribirCursor(2, 2) ;
		    printf("F = %2i C = %2i (teclas i, j, k, l y Esc)", F, C) ;    
         	if (F >= numeroDeLineas()) F = numeroDeLineas()-1 ;  
         	escribirCursor(F, C) ;
    		putchar(0xFE) ;
         	escribirCursor(F, C) ;
		}
	}
	while (car != ESC) ;
	
	return(0) ;
}

