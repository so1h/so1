/* ----------------------------------------------------------------------- */
/*                                ptrc2c.c                                 */
/* ----------------------------------------------------------------------- */
/*       puntero lejano a una cola doblemente enlazada con cabecera        */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ptrc2c.h>

int posicionPC2c ( int i, ptrC2c_t ptrC2c ) {    /* -1 si i no esta en c2c */
  codigoPosicionPC2c(i, ptrC2c)
}

void eliminarPC2c ( int i, ptrC2c_t ptrC2c ) {
  codigoEliminarPC2c(i, ptrC2c)            /* supuesto estaPC2c(i, ptrC2c) */
}

void apilarPC2c ( int i, ptrC2c_t ptrC2c ) {
  codigoApilarPC2c(i, ptrC2c)
}

void encolarPC2c ( int i, ptrC2c_t ptrC2c ) {
  codigoEncolarPC2c(i, ptrC2c)
}

int desencolarPC2c ( ptrC2c_t ptrC2c ) {
  int i = ptrC2c->primero ;
  codigoEliminarPC2c(i, ptrC2c)
  return(i) ;
}

void inicializarPC2c( ptrC2c_t ptrC2c, dobleEnlace_t far * e, int cabecera, bool_t compartida ) {
  ptrC2c->primero = cabecera ;
  ptrC2c->cabecera = cabecera ;
  ptrC2c->numElem = 0 ;
  ptrC2c->e = e ;
  inicEnlace()
}
