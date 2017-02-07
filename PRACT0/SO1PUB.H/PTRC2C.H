/* ----------------------------------------------------------------------- */
/*                                ptrc2c.h                                 */
/* ----------------------------------------------------------------------- */
/*       puntero lejano a una cola doblemente enlazada con cabecera        */
/* ----------------------------------------------------------------------- */

#ifndef PTRC2C_H
#define PTRC2C_H

#include <so1pub.h\c2c.h>

typedef c2c_t far * ptrC2c_t ;                   /* puntero lejano a c2c_t */

#define numElemPC2c(ptrC2c)      (((ptrC2c_t)ptrC2c)->numElem)
#define vaciaPC2c(ptrC2c)        (((ptrC2c_t)ptrC2c)->numElem == 0)
#define noVaciaPC2c(ptrC2c)      (((ptrC2c_t)ptrC2c)->numElem > 0))
#define primeroPC2c(ptrC2c)      (((ptrC2c_t)ptrC2c)->primero)
#define cabeceraPC2c(ptrC2c)     (((ptrC2c_t)ptrC2c)->cabecera)
#define ultimoPC2c(ptrC2c)       (((ptrC2c_t)ptrC2c)->e[(ptrC2c)->cabecera].ant)
#define siguientePC2c(i, ptrC2c) (((ptrC2c_t)ptrC2c)->e[i].sig)
#define anteriorPC2c(i, ptrC2c)  (((ptrC2c_t)ptrC2c)->e[i].ant)
#define estaPC2c(i, ptrC2c)      (((ptrC2c_t)ptrC2c)->e[i].sig != NULO)
#define noEstaPC2c(i, ptrC2c)    (((ptrC2c_t)ptrC2c)->e[i].sig == NULO)

#define codigoInsertarPC2c(i, anterior, ptrC2c) {                            \
  dobleEnlace_t far * e = ((ptrC2c_t)ptrC2c)->e ;                            \
  int siguiente = e[anterior].sig ;                                          \
  codigoInsertar(i, anterior, siguiente)                                     \
  if (ptrC2c->cabecera == anterior)                                          \
    ptrC2c->primero = i ;                                                    \
  ptrC2c->numElem++ ;                                                        \
}
#define codigoEliminarPC2c(i, ptrC2c) {    /* supuesto estaPC2c(i, ptrC2c) */\
  dobleEnlace_t far * e = ((ptrC2c_t)ptrC2c)->e ;                            \
  codigoEliminar(i) ;                                                        \
  if (i == ((ptrC2c_t)ptrC2c)->primero)                                      \
    ((ptrC2c_t)ptrC2c)->primero = e[i].sig ;                                 \
  e[i].sig = NULO ;                                                          \
  e[i].ant = NULO ;                                                          \
  ((ptrC2c_t)ptrC2c)->numElem-- ;                                            \
}
#define codigoApilarPC2c(i, ptrC2c) {           /* ptrC2c vacia o no vacia */\
  dobleEnlace_t far * e = ((ptrC2c_t)ptrC2c)->e ;                            \
  int anterior = ((ptrC2c_t)ptrC2c)->cabecera ;                              \
  int siguiente = ((ptrC2c_t)ptrC2c)->primero ;                              \
  codigoInsertar(i, anterior, siguiente)                                     \
  ((ptrC2c_t)ptrC2c)->primero = i ;                                          \
  ((ptrC2c_t)ptrC2c)->numElem++ ;                                            \
}
#define codigoEncolarPC2c(i, ptrC2c) {     /* supuesto noVaciaPC2c(ptrC2c) */\
  dobleEnlace_t far * e = ((ptrC2c_t)ptrC2c)->e ;                            \
  int siguiente = ((ptrC2c_t)ptrC2c)->cabecera ;                             \
  int anterior = e[siguiente].ant ;                                          \
  codigoInsertar(i, anterior, siguiente)                                     \
  ((ptrC2c_t)ptrC2c)->primero = ((ptrC2c_t)ptrC2c)->e[siguiente].sig ;       \
  ((ptrC2c_t)ptrC2c)->numElem++ ;                                            \
}
#define codigoPosicionPC2c(i, ptrc2c) {                                      \
  dobleEnlace_t far * e = ((ptrC2c_t)ptrC2c)->e ;                            \
  int cabecera = ((ptrC2c_t)ptrC2c)->cabecera ;                              \
  int primero = ((ptrC2c_t)ptrC2c)->primero ;                                \
  codigoPosicion(i)                                                          \
}

int posicionPC2c ( int i, ptrC2c_t c2c ) ;    /* -1 si i no esta en ptrC2c */

void eliminarPC2c ( int i, ptrC2c_t ptrC2c ) ;

void apilarPC2c ( int i, ptrC2c_t ptrC2c ) ;

void encolarPC2c ( int i, ptrC2c_t ptrC2c ) ;

int desencolarPC2c ( ptrC2c_t ptrC2c ) ;

#define desapilarPC2c(ptrC2c) desencolarPC2c(ptrC2c)

void inicializarPC2c ( ptrC2c_t ptrC2c, dobleEnlace_t far * e, int cabecera, bool_t compartida ) ;

#define inicPC2c(ptrC2c, e, cabecera, compartida)                            \
                                                                             \
  inicializarPC2c((ptrC2c_t) ptrC2c, (dobleEnlace_t far *) e, cabecera, compartida) ;

//void invertirPC2c ( ptrC2c_t ptrC2c ) ; /* intercambiar enlaces sig y ant */

#endif /* PTRC2C_H */
