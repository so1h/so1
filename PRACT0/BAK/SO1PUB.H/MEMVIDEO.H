/* ----------------------------------------------------------------------- */
/*                                memvideo.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef MEMVIDEO_H
#define MEMVIDEO_H

#include <so1pub.h\tipos.h>                                      /* byte_t */
#include <so1pub.h\pantalla.h>                         /* printCarPantalla */

extern byte_t cursorF ;

extern byte_t cursorC ;

extern byte_t lineaOriginal1 ;

extern byte_t lineaOriginal2 ;

void inicMemVideo ( void ) ;

int finMemVideo ( void ) ;

void goToXYVideo ( byte_t F, byte_t C ) ;

int printCarVideo ( char car ) ;

#endif /* MEMVIDEO_H */