/* ----------------------------------------------------------------------- */
/*                                memvideo.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1pub.h\bios_0.h>                         /* printCarBIOS('\a') */
#include <so1pub.h\pantalla.h>             /* numFilas, borrarPantalla ... */
#include <so1pub.h\bios_crt.h>                                  /* ptrPant */

byte_t cursorF = 0 ;                                /* inicializacion DATA */

byte_t cursorC = 0 ;                                /* inicializacion DATA */

void inicMemVideo ( void ) {

}

int finMemVideo ( void ) {
  return(0) ;
}

void goToXYVideo ( byte_t F, byte_t C ) {
//  if (F >= numFilas) return ;
//  if (C >= numColumnas) return ;
  cursorF = F ;
  cursorC = C ;
}

int printCarVideo ( char car ) {
    switch(car) {
    case '\f' : borrarCPantalla(ptrPant, numFilas) ;
                /* borrarPantalla(ptrPant, numFilas) ; */
                cursorF = 0 ;
                cursorC = 0 ;
                break ;
    case '\r' : cursorC = 0 ;
                break ;
    case '\n' : if (++cursorF == numFilas) {
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
                if (cursorC < 80)              /* ptrBiosArea->VIDEO-width */
	                ptrPant->t[cursorF][cursorC].car = car ;
                cursorC++ ;
#else	
	            ptrPant->t[cursorF][cursorC].car = car ;
                if (++cursorC == 80) {         /* ptrBiosArea->VIDEO-width */
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