/* ----------------------------------------------------------------------- */
/*                               pantalla.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef PANTALLA_H
#define PANTALLA_H

#include <so1pub.h\tipos.h>                              /* word_t, byte_t */
#include <so1pub.h\def_pant.h>                    /* pantalla_t, display_t */

word_t tamPantalla ( byte_t numFilas ) ;

void pantallazo ( pantalla_t far * pantalla,
                  word_t numFilas,
                  char car, 
				  char atr,
                  byte_t esqSupDeF, 
				  byte_t esqSupDeC,
                  byte_t esqInfIzF, 
				  byte_t esqInfIzC ) ;

void borrarPantalla ( pantalla_t far * pantalla, word_t numFilas ) ;

void borrarCPantalla ( pantalla_t far * pantalla, word_t numFilas ) ;

void copiarPantalla ( pantalla_t far * p1, pantalla_t far * p2, word_t numFilas ) ;

void copiarCPantalla ( pantalla_t far * p1, pantalla_t far * p2, word_t numFilas ) ;

void scrollPantalla ( pantalla_t far * pantalla, word_t numFilas ) ;

void scrollCPantalla ( pantalla_t far * pantalla, word_t numFilas ) ;

#if (FALSE )

int printCarDisplay ( display_t far * display, char car ) ;

#endif

#endif /* PANTALLA_H */