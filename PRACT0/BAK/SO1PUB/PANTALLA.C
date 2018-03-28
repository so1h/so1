/* ----------------------------------------------------------------------- */
/*                               pantalla.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\def_pant.h>                  /* pantalla_t, maxColumnas */
//                                          /* maxFilasAct, maxColumnasAct */
#include <so1pub.h\bios_0.h>                         /* printCarBIOS('\a') */
#include <so1pub.h\memory.h>                          /* memcpy, memcpy_fd */
#include <so1pub.h\pantalla.h>                    /* pantalla_t, display_t */

word_t tamPantalla ( byte_t numFilas ) 
{
  return(numFilas*maxColumnas*sizeof(pos_t)) ;
}

void pantallazo ( pantalla_t far * pantalla,
                  word_t numFilas,
                  char car, 
				  char atr,
                  byte_t esqSupDeF, 
				  byte_t esqSupDeC,
                  byte_t esqInfIzF, 
				  byte_t esqInfIzC ) 
{
    int F, C ;
    pos_t pos ;
    if (esqInfIzF >= numFilas) return ;
    if (atr != 0x00) 
	{                                     /* si se modifican los atributos */
        pos.car = car ;
        pos.atr = atr ;
        for ( F = esqSupDeF ; F <= esqInfIzF ; F++ ) 
            for ( C = esqSupDeC ; C <= esqInfIzC ; C++ )
                pantalla->t[F][C] = pos ;
    }
    else 
	{                                     /* no se modifican los atributos */
        for ( F = esqSupDeF ; F <= esqInfIzF ; F++ )
            for ( C = esqSupDeC ; C <= esqInfIzC ; C++ )
                pantalla->t[F][C].car = car ;
    }
}

void borrarPantalla ( pantalla_t far * pantalla, word_t numFilas ) 
{
    pantallazo(pantalla, numFilas, ' ', atrNormal, 0, 0, numFilas-1, maxColumnas-1) ;
}

void borrarCPantalla ( pantalla_t far * pantalla, word_t numFilas ) 
{
    pantallazo(pantalla, numFilas, ' ', (char)0, 0, 0, numFilas-1, maxColumnas-1) ;
}

void copiarPantalla ( pantalla_t far * p1, pantalla_t far * p2, word_t numFilas ) 
{
    byte_t F, C ;
    for ( F = 0 ; F < numFilas ; F++ )
//      for ( C = 0 ; C < maxColumnas ; C++ )
//          p2->t[F][C] = p1->t[F][C] ;
        memcpy(&p2->t[F], &p1->t[F], maxColumnas << 1) ; 
}

void copiarCPantalla ( pantalla_t far * p1, pantalla_t far * p2, word_t numFilas ) 
{
    byte_t F, C ;
    for ( F = 0 ; F < numFilas ; F++ )
        for ( C = 0 ; C < maxColumnas ; C++ )
            p2->t[F][C].car = p1->t[F][C].car ;
}

void scrollPantalla ( pantalla_t far * pantalla, word_t numFilas ) 
{
    byte_t C ;
    pos_t pos ;
    pos.car = ' ' ;
    pos.atr = atrNormal ;
    numFilas-- ;
    copiarPantalla((pantalla_t far *)&pantalla->t[1][0], pantalla, numFilas) ;
    for ( C = 0 ; C < maxColumnas ; C++ ) pantalla->t[numFilas][C] = pos ;
}

void scrollCPantalla ( pantalla_t far * pantalla, word_t numFilas ) 
{
    byte_t C ;
    numFilas-- ;
    copiarCPantalla((pantalla_t far *)&pantalla->t[1][0], pantalla, numFilas) ;
    for ( C = 0 ; C < maxColumnas ; C++ ) pantalla->t[numFilas][C].car = ' ' ;
}

#if (FALSE)

int printCarDisplay ( display_t far * display, char car ) 
{
    byte_t numFilas = maxFilasAct ;	
	pantalla_t far * pantalla = display->pantalla ;
    switch(car) {
    case '\f' : borrarCPantalla(pantalla, numFilas) ;
//              borrarPantalla(pantalla, numFilas) ; 
                display->F = 0 ;
    case '\r' : display->C = 0 ;
                break ;
    case '\n' : if (++(display->F) == numFilas) {
                    scrollCPantalla(pantalla, numFilas) ;
                    display->F = numFilas-1 ;
                }
                break ;
    case '\b' : if (display->C > 0) {
                    (display->C)-- ;
                    pantalla->t[display->F][display->C].car = ' ' ;
                }
                else printCarBIOS('\a') ;
                break ;
    case '\t' : car = ' ' ; break ;                           /* Tabulador */
    case '\a' : printCarBIOS('\a') ;
                break ;
    default   : 
#if (FALSE)
                if (display->C < maxColumnasAct)            
	                pantalla->t[display->F][display->C].car = car ;
                    display->C++ ;
#else	
	            pantalla->t[display->F][display->C].car = car ;
                if (++(display->C) == maxColumnasAct) {     
                    display->C = 0 ;
                    if (++(display->F) == numFilas) {
                        scrollCPantalla(pantalla, numFilas) ;
                        display->F = numFilas-1 ;
                    }
                }
#endif
    }
    return(0) ;
}

#endif