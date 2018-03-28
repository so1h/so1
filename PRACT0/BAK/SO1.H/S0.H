/* ----------------------------------------------------------------------- */
/*                                   s0.h                                  */
/* ----------------------------------------------------------------------- */
/*                               sistema cero                              */
/* ----------------------------------------------------------------------- */

#ifndef S0_H
#define S0_H

#include <so1pub.h\tipos.h>                                      /* word_t */

#define hayDOS    0x0001
#define hayNT     0x0002
#define hayQemu   0x0004
#define hayBochs  0x0008
#define hayDBox   0x0010
#define hayNTVDM  0x0020
#define hayFake86 0x0040
#define hayVDos   0x0080
#define hayMsdos  0x0100                 /* msdos player de Takeda Toshiya */

void mirarLoQueHay ( word_t * loQueHay ) ;

void mostrarLoQueHay ( word_t loQueHay ) ;

void tirarS0 ( word_t loQueHay ) ;

#endif /* S0_H */