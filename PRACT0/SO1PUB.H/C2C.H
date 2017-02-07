/* ----------------------------------------------------------------------- */
/*                                  c2c.h                                  */
/* ----------------------------------------------------------------------- */
/*                 colas doblemente enlazadas con cabecera                 */
/* ----------------------------------------------------------------------- */

#ifndef C2C_H
#define C2C_H

#include <so1pub.h\tipos.h>

#define NULO ((int)-1)

typedef struct {
  int sig ;
  int ant ;
} dobleEnlace_t ;                                          /* enlace doble */

typedef struct {
  int numElem ;
  int primero ;
  int cabecera ;
  dobleEnlace_t far * e ;
} c2c_t ;                /* cola circular doblemente enlazada con cabecera */

#define numElemC2c(c2c)      (c2c.numElem)
#define vaciaC2c(c2c)        (c2c.numElem == 0)
#define noVaciaC2c(c2c)      (c2c.numElem > 0)
#define primeroC2c(c2c)      (c2c.primero)
#define cabeceraC2c(c2c)     (c2c.cabecera)
#define ultimoC2c(c2c)       (c2c.e[c2c.cabecera].ant)
#define siguienteC2c(i, c2c) (c2c.e[i].sig)
#define anteriorC2c(i, c2c)  (c2c.e[i].ant)
#define estaC2c(i, c2c)      (c2c.e[i].sig != NULO)             /* cuidado */
#define noEstaC2c(i, c2c)    (c2c.e[i].sig == NULO)             /* cuidado */

#define codigoInsertar(i, anterior, siguiente) {                             \
  e[i].sig = siguiente ;                                                     \
  e[i].ant = anterior ;                                                      \
  e[anterior ].sig = i ;                                                     \
  e[siguiente].ant = i ;                                                     \
}
#define codigoInsertarC2c(i, anterior, c2c) {                                \
  dobleEnlace_t far * e = c2c.e ;                                            \
  int siguiente = e[anterior].sig ;                                          \
  codigoInsertar(i, anterior, siguiente)                                     \
  if (c2c.cabecera == anterior)                                              \
    c2c.primero = i ;                                                        \
  c2c.numElem++ ;                                                            \
}
#define codigoEliminar(i) {                                                  \
  int ant = e[i].ant ;                                                       \
  int sig = e[i].sig ;                                                       \
  e[ant].sig = sig ;                                                         \
  e[sig].ant = ant ;                                                         \
}
#define codigoEliminarC2c(i, c2c) {            /* supuesto estaC2c(i, c2c) */\
  dobleEnlace_t far * e = c2c.e ;                                            \
  codigoEliminar(i) ;                                                        \
  if (i == c2c.primero) c2c.primero = e[i].sig ;                             \
  e[i].sig = NULO ;                                                          \
  e[i].ant = NULO ;                                                          \
  c2c.numElem-- ;                                                            \
}
#define codigoApilarC2c(i, c2c) {                  /* c2c vacia o no vacia */\
  dobleEnlace_t far * e = c2c.e ;                                            \
  int anterior = c2c.cabecera ;                                              \
  int siguiente = c2c.primero ;                                              \
  codigoInsertar(i, anterior, siguiente)                                     \
  c2c.primero = i ;                                                          \
  c2c.numElem++ ;                                                            \
}
#define codigoEncolarC2c(i, c2c) {                 /* c2c vacia o no vacia */\
  dobleEnlace_t far * e = c2c.e ;                                            \
  int siguiente = c2c.cabecera ;                                             \
  int anterior = e[siguiente].ant ;                                          \
  codigoInsertar(i, anterior, siguiente)                                     \
  c2c.primero = c2c.e[siguiente].sig ;                                       \
  c2c.numElem++ ;                                                            \
}
#define codigoPosicion(i) {                                                  \
  int j, k ;                                                                 \
  if (i == cabecera) return(0) ;                             /* posision 0 */\
  j = primero ;                                                              \
  k = 1 ;                                                                    \
  while ((j != cabecera) && (j != i)) {                                      \
    j = e[j].sig ;                                                           \
    k++ ;                                                                    \
  }                                                                          \
  if (j != cabecera) return(k) ;                             /* posision k */\
  return(-1) ;                                         /* i no esta en c2c */\
}
#define codigoPosicionC2c(i, c2c) {                                          \
  dobleEnlace_t far * e = c2c.e ;                                            \
  int cabecera = c2c.cabecera ;                                              \
  int primero = c2c.primero ;                                                \
  codigoPosicion(i)                                                          \
}

int posicionC2c ( int i, c2c_t c2c ) ;  /* devuelve -1 si i no esta en c2c */

void eliminarC2c ( int i, c2c_t c2c ) ;

void apilarC2c ( int i, c2c_t c2c ) ;

void encolarC2c ( int i, c2c_t c2c ) ;

int desencolarC2c ( c2c_t c2c ) ;

#define desapilarC2c(c2c) desencolarC2c(c2c)

#define inicEnlace() {                                                       \
  int i ;                                                                    \
  e[cabecera].sig = cabecera ;                                               \
  e[cabecera].ant = cabecera ;                                               \
  if (compartida) return ;                                                   \
  for ( i = 0 ; i < cabecera ; i++ ) {                                       \
    e[i].sig = NULO ;                                                        \
    e[i].ant = NULO ;                                                        \
  }                                                                          \
}

void inicializarC2c ( c2c_t far * c2c, dobleEnlace_t far * e, int cabecera, bool_t compartida ) ;

#define inicC2c(c2c, e, cabecera, compartida)                                \
                                                                             \
  inicializarC2c((c2c_t far *) c2c, (dobleEnlace_t far *) e, cabecera, compartida) ;

//void invertirC2c ( c2c_t far * c2c ) ;   /* intercambiar enlaces sig y ant */

#endif /* C2C_H */

/***************************************************************************/
/* invariantes que debe cumplir c2c:                                       */
/*                                                                         */
/* 1) c2c.primero == c2c.e[c2c.cabecera].sig                    (cabecera) */
/* 2) i == c2c.e[c2c.e[i].sig].ant                          (doble enlace) */
/* 3) i == c2c.e[c2c.e[i].ant].sig                                         */
/* 4) si hacemos:                                               (circular) */
/*    i = c2c.primero ;                                                    */
/*    for ( j = 0 ; j < c2c.numElem ; j++ )                                */
/*      i = c2c.e[i].sig ;                                                 */
/*    se cumple i == c2c.cabecera e i ha tomado c2c.numElem + 1 valores    */
/*    distintos, todos menores o iguales que c2c.cabecera.                 */
/*    Para cualquier otro j distinto de esos c2c.numElem + 1 valores tal   */
/*    que 0 <= j < c2c.cabecera, se cumple                                 */
/*                                                                         */
/*                    c2c.e[j].sig == c2c.e[j].ant == NULO                 */
/*                                                                         */
/* 5) c2c esta vacia si y solo si c2c.primero == c2c.cabecera              */
/* 6) c2c esta vacia si y solo si c2c.numElem == 0                         */
/* 7) si (0 <= j < c2c.cabecera) j esta en (pertenece a) c2c si y solo si  */
/*    c2c.e[j].sig != NULO                                                 */
/*                                                                         */
/* Estos invariantes se podrían relajar para permitir que la memoria que   */
/* utilizan los enlaces se comparta entre varias colas disjuntas, como por */
/* ejemplo la cola de descriptores de proceso libres y la cola de          */
/* descriptores de proceso preparados. Lo mismo se puede aplicar a las     */
/* diferentes colas de procesos bloqueados siempre que sean disjuntas.     */
/*                                                                         */
/***************************************************************************/

