/* ----------------------------------------------------------------------- */
/*                                  ccb.c                                  */
/* ----------------------------------------------------------------------- */
/*                     colas de funciones de call back                     */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ccb.h>

int inicCcb ( ccb_t ccb, word_t max ) {
  int i ;
  if (ccb == ccbNulo) return(-1) ;
  ccb->max = max ;
  ccb->num = 0 ;
  ccb->in = 0 ;
  ccb->out = 0 ;
  for ( i = 0 ; i < max ; i++ )
    ccb->callBack[i] = cbNulo ;
  return(0) ;
}

int encolarCcb ( callBack_t cb, ccb_t ccb ) {
  if (ccb == ccbNulo) return(-1) ;
  if (cb == cbNulo) return(-2) ;
  if (ccb->num >= ccb->max) return(-3) ;
  ccb->callBack[ccb->in] = cb ;
  ccb->in = (ccb->in + 1) % ccb->max ;
  ccb->num++ ;
  return(0) ;
}

callBack_t desencolarCcb ( ccb_t ccb ) {
  callBack_t cb ;
  if (ccb == ccbNulo) return(ccbNulo) ;
  if (ccb->num == 0) return(cbNulo) ;
  cb = ccb->callBack[ccb->out] ;
  ccb->out = (ccb->out + 1) % ccb->max ;
  ccb->num-- ;
  return(cb) ;
}

int eliminarCcb ( callBack_t cb, ccb_t ccb ) {
  int i ;
  int ind ;
  int num ;
  if (ccb == ccbNulo) return(-1) ;                              /* ccb mal */
  if (cb == cbNulo) return(-2) ;                                 /* cb mal */
  num = ccb->num ;
  if (num == 0) return(-3) ;                                  /* ccb vacio */
  ind = ccb->out ;
  for ( i = 0 ; i < num ; i++ ) {
    if (ccb->callBack[ind] == cb) break ;
    ind = (ind + 1 ) % ccb->max ;
  }
  if (i == num) return(-4) ;                      /* no se encuentra el cb */
  ccb->callBack[ind] = cbNulo ;
  if (ind  == ccb->out)
    desencolarCcb(ccb) ;
  else
    ccb->callBack[ind] = desencolarCcb(ccb) ;
  return(0) ;
}

int eliminarSegCcb ( word_t segmento, ccb_t ccb ) {
  int i ;
  int ind ;
  int num ;
  if (ccb == ccbNulo) return(-1) ;                              /* ccb mal */
  num = ccb->num ;
  if (num == 0) return(-2) ;                                  /* ccb vacio */
  ind = ccb->out ;
  for ( i = 0 ; i < num ; i++ ) {
    if (seg((pointer_t)ccb->callBack[ind]) == segmento) break ;
    ind = (ind + 1 ) % ccb->max ;
  }
  if (i == num) return(-3) ;                      /* no se encuentra el cb */
  ccb->callBack[ind] = cbNulo ;
  if (ind  == ccb->out)
    desencolarCcb(ccb) ;
  else
    ccb->callBack[ind] = desencolarCcb(ccb) ;
  return(0) ;
}

int vaciarCcb ( ccb_t ccb ) {
  int i ;
  if (ccb == ccbNulo) return(-1) ;
  ccb->num = 0 ;
  ccb->in = 0 ;
  ccb->out = 0 ;
  for ( i = 0 ; i < ccb->max ; i++ )
    ccb->callBack[i] = cbNulo ;
  return(0) ;
}

int atenderCcb ( ccb_t ccb ) {
  callBack_t cb ;
  int num ;
  if (ccb == ccbNulo) return(-1) ;                            /* error ccb */
  num = ccb->num ;
  while (num > 0) {
    cb = desencolarCcb(ccb) ;
    if (cb == cbNulo) return(-2) ;                             /* error cb */
    switch (cb(ccb->arg)) {                /* ejecucion cb() del call Back */
    case 0  : break ;                                 /* fin total   de cb */
    case 1  : encolarCcb(cb, ccb) ;                   /* fin parcial de cb */
              break ;
    default : return(-3) ;                         /* error retorno del cb */
    }
    num-- ;
    asm cli        /* inicialmente cb() tiene las interrupciones inhibidas */
  }                          /* pero puede activarlas durante su ejecucion */
  return(0) ;
}

