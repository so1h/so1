/* ----------------------------------------------------------------------- */
/*                                  ccb.h                                  */
/* ----------------------------------------------------------------------- */
/*                     colas de funciones de call back                     */
/* ----------------------------------------------------------------------- */

#ifndef CCB_H
#define CCB_H

#include <so1pub.h\tipos.h>                                /* word_t, NULL */

typedef int far ( far * callBack_t ) ( void far * arg ) ;     /* call back */

#define cbNulo (callBack_t)NULL

typedef struct {
  word_t num ;                               /* numero actual de callBacks */
  word_t in ;                     /* indice de la primera componente libre */           
  word_t out ;                     /* indice del primer callBack disponible*/ 
  word_t max ;                               /* numero m�ximo de callBacks */
  void far * arg ;                                   /* argumento devuelto */
  callBack_t callBack [ ] ;                 /* array circular de callBacks */
} descCcb_t ;

typedef descCcb_t far * ccb_t ;

#define ccbNulo (ccb_t)NULL

#define tamCcb (sizeof(descCcb_t) + max * sizeof(callBack_t))

int inicCcb ( ccb_t ccb, word_t max ) ;

int encolarCcb ( callBack_t cb, ccb_t ccb ) ;

callBack_t desencolarCcb ( ccb_t ccb ) ;

int eliminarCcb ( callBack_t cb, ccb_t ccb ) ;

int eliminarSegCcb ( word_t segmento, ccb_t ccb ) ;

int vaciarCcb ( ccb_t ccb ) ;

int atenderCcb ( ccb_t ccb ) ;

#endif /* CCB_H */