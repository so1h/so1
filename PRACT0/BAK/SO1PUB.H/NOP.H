/* ----------------------------------------------------------------------- */
/*                                  nop.h                                  */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef NOP_H
#define NOP_H

void hacerNada ( char * arg ) ;

#define evitarWarningArgNotUsed(arg) hacerNada((char *)&arg) ;

/* #pragma option -w-par */      /* omitir warning parameter is never used */
/* #pragma warn -par */          /* omitir warning parameter is never used */
/* #pragma option -wpar *      /* permitir warning parameter is never used */
/* #pragma warn +par           /* permitir warning parameter is never used */

#endif /* NOP_H */
