/* ----------------------------------------------------------------------- */
/*                                memvideo.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>                                      /* byte_t */
#include <so1pub.h\bios_0.h>                         /* printCarBIOS('\a') */
#include <so1pub.h\pantalla.h>         /* borrarPantalla, printCarPantalla */
#include <so1pub.h\bios_crt.h>                                  /* ptrPant */
#include <so1pub.h\def_pant.h>              /* maxFilasAct, maxColumnasAct */

//     /* no separar las tres declaraciones siguientes (forman un display) */

pantalla_t far * pantallaVideo = MK_FP(0xB800,0x0000) ;            /* DATA */
byte_t cursorF = 0 ;                                               /* DATA */
byte_t cursorC = 0 ;                                               /* DATA */

void inicMemVideo ( void ) {
}

int finMemVideo ( void ) {
    return(0) ;
}

void goToXYVideo ( byte_t F, byte_t C ) 
{
    if (F >= maxFilasAct) return ;
    if (C >= maxColumnasAct) return ;
    cursorF = F ;
    cursorC = C ;
}

#if (TRUE)

int printCarVideo ( char car ) 
{
    byte_t numFilas = maxFilasAct ;	
    switch(car) {
    case '\f' : borrarCPantalla(ptrPant, numFilas) ;
//              borrarPantalla(ptrPant, numFilas) ; 
                cursorF = 0 ;
    case '\r' : cursorC = 0 ;
                break ;
    case '\n' : if (++(cursorF) == numFilas) {
                    scrollCPantalla(ptrPant, numFilas) ;
                    cursorF = numFilas-1 ;
                }
                break ;
    case '\b' : if (cursorC > 0) {
                    cursorC-- ;
                    ptrPant->t[cursorF][cursorC].car = ' ' ;
                }
                else printCarBIOS('\a') ;
                break ;
    case '\t' : car = ' ' ; break ;                           /* Tabulador */
    case '\a' : printCarBIOS('\a') ;
                break ;
    default   : 
#if (FALSE)
                if (cursorC < maxColumnasAct)            
	                ptrPant->t[cursorF][cursorC].car = car ;
                    cursorC++ ;
#else	
	            ptrPant->t[cursorF][cursorC].car = car ;
                if (++(cursorC) == maxColumnasAct) {     
                    cursorC = 0 ;
                    if (++cursorF == numFilas) {
                        scrollCPantalla(ptrPant, numFilas) ;
                        cursorF = numFilas-1 ;
                    }
                }
#endif
    }
    return(0) ;
}	
		
#else 
	
int printCarVideo ( char car ) 
{
	return(printCarDisplay((display_t far *)&pantallaVideo, car)) ;
}

#endif 