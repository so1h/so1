/* ----------------------------------------------------------------------- */
/*                                  c2c.c                                  */
/* ----------------------------------------------------------------------- */
/*                 colas doblemente enlazadas con cabecera                 */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\c2c.h>

int posicionC2c ( int i, c2c_t c2c ) {  /* devuelve -1 si i no esta en c2c */
  codigoPosicionC2c(i, c2c)
}

void eliminarC2c ( int i, c2c_t c2c ) {
  codigoEliminarC2c(i, c2c)                    /* supuesto estaC2c(i, c2c) */
}

void apilarC2c ( int i, c2c_t c2c ) {
  codigoApilarC2c(i, c2c)
}

void encolarC2c ( int i, c2c_t c2c ) {
  codigoEncolarC2c(i, c2c)
}

int desencolarC2c ( c2c_t c2c ) {
  int i = c2c.primero ;
  codigoEliminarC2c(i, c2c)
  return(i) ;
}

void inicializarC2c( c2c_t far * c2c, dobleEnlace_t far * e, int cabecera, bool_t compartida ) {
  c2c->primero = cabecera ;
  c2c->cabecera = cabecera ;
  c2c->numElem = 0 ;
  c2c->e = e ;
  inicEnlace()
}
