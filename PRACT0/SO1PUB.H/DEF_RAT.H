/* ----------------------------------------------------------------------- */
/*                                 def_rat.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef DEF_RAT_H
#define DEF_RAT_H

#include <so1pub.h\tipos.h>                              /* byte_t, bool_t */ 
#include <so1pub.h\def_pant.h>                    /* maxColumnas, maxFilas */

#define maxX 8*maxColumnas                                   /* 8*80 = 640 */

#define maxY 8*maxFilas                                      /* 8*50 = 400 */

typedef struct {
  byte_t S0 ;                                          /* byte estado 8042 */
  byte_t B0 ;                                         /* Byte 0 = 00SS1MRL */
  byte_t B1 ;                                         /* Byte 1 = X        */
  byte_t B2 ;                                         /* Byte 2 = Y        */
  word_t W0 ;
  int incX ;
  int incY ;
  int X ;
  int Y ;
  int F ;
  int C ;
  int botonIz ;
  int botonDe ;
  int botonMe ;
  int botonIzAnt ;
  int botonDeAnt ;
  int botonMeAnt ;
  bool_t visible ;            /* F, C es visible en la ventana del proceso */
  bool_t forzarLectura ;
} estadoRaton_t ;

#endif /* DEF_RAT_H */
