/* ----------------------------------------------------------------------- */
/*                                   s0.h                                  */
/* ----------------------------------------------------------------------- */
/*                               sistema cero                              */
/* ----------------------------------------------------------------------- */

#ifndef S0_H
#define S0_H

#include <so1pub.h\tipos.h>                                      /* word_t */

#define hayDOS    0x01
#define hayNT     0x02
#define hayQemu   0x04
#define hayBochs  0x08
#define hayDBox   0x10
#define hayNTVDM  0x20
#define hayFake86 0x40
#define hayVDos   0x80

void mirarLoQueHay ( word_t * loQueHay ) ;

void mostrarLoQueHay ( word_t loQueHay ) ;

void tirarS0 ( word_t loQueHay ) ;

#endif /* S0_H */