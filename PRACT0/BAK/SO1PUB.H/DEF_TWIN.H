/* ----------------------------------------------------------------------- */
/*                                def_twin.h                               */
/* ----------------------------------------------------------------------- */
/*                            Ventanas de texto                            */
/* ----------------------------------------------------------------------- */

#ifndef DEF_TWIN_H
#define DEF_TWIN_H

#include <so1pub.h\tipos.h>
#include <so1pub.h\pantalla.h>

struct window_t {
  pantalla_t     pantalla ;           /* guarda las posiciones no visibles */
  byte_t         num ;                             /* numero de la ventana */
  pindx_t        pindx ;
  byte_t         esqSupIzF ;
  byte_t         esqSupIzC ;
  byte_t         esqInfDeF ;
  byte_t         esqInfDeC ;
  byte_t         esqSupIzFAnt ;
  byte_t         esqSupIzCAnt ;
  byte_t         esqInfDeFAnt ;
  byte_t         esqInfDeCAnt ;
  byte_t         atr ;                    /* atributo normal de la ventana */
  byte_t         atrMarco ;                          /* atributo del marco */
  byte_t         atrCursor ;                        /* atributo del cursor */
  byte_t         atrInvertido ;                     /* atributo del cursor */
  byte_t         invertido ;      /* indica si se ha invertido el atributo */
  byte_t         intenso ;    /* indica si se ha intensificado el atributo */
  byte_t         cursorF ;               /* fila de la posicion del cursor */
  byte_t         cursorC ;            /* columna de la posicion del cursor */
  byte_t         linea1 ;             /* linea1 y linea2: forma del cursor */
  byte_t         linea2 ;
  byte_t         lin1bak ;            /* linea1 y linea2: forma del cursor */
  byte_t         lin2bak ;
  char           nombre[20] ;
  char           boxCar[7] ;
  struct window_t far * sig ;                         /* siguiente ventana */
  struct window_t far * ant ;                          /* anterior ventana */
} ;

typedef struct window_t far * ptrWindow_t ;

#endif /* DEF_TWIN_H */




