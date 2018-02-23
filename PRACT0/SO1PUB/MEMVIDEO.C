/* ----------------------------------------------------------------------- */
/*                                memvideo.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1pub.h\carsctrl.h>                  /* CR, LF, FF, HT, BS, BEL */
#include <so1pub.h\bios_0.h>                          /* printCarBIOS(BEL) */
#include <so1pub.h\pantalla.h>             /* numFilas, borrarPantalla ... */
#include <so1pub.h\bios_crt.h>                                  /* ptrPant */

byte_t cursorF ;

byte_t cursorC ;

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
    case FF  : borrarCPantalla(ptrPant, numFilas) ;
               /* borrarPantalla(ptrPant, numFilas) ; */
               cursorF = 0 ;
               cursorC = 0 ;
               break ;
    case CR  : cursorC = 0 ;
               break ;
    case LF  : if (++cursorF == numFilas) {
                 scrollCPantalla(ptrPant, numFilas) ;
                 cursorF = numFilas-1 ;
               }
               break ;
    case BS  : if (cursorC > 0) {
                 cursorC-- ;
                 ptrPant->t[cursorF][cursorC].car = ' ' ;
               }
               else printCarBIOS(BEL) ;
               break ;
    case HT  : car = ' ' ; break ;                            /* Tabulador */
    case BEL : printCarBIOS(BEL) ;
               break ;
    default  : 
#if (TRUE)
               if (cursorC < 80)               /* ptrBiosArea->VIDEO-width */
	               ptrPant->t[cursorF][cursorC].car = car ;
               cursorC++ ;
#else	
	           ptrPant->t[cursorF][cursorC].car = car ;
               if (++cursorC == 80) {          /* ptrBiosArea->VIDEO-width */
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

