/* ----------------------------------------------------------------------- */
/*                               def_timer.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef TIMER_H
#define TIMER_H

#include <so1pub.h\tipos.h>                             /* word_t, dword_t */

typedef struct {
  dword_t contTics ;
  dword_t contRodajas ;
  int  contTicsRodaja ;
  word_t  ticsPorRodaja ;
} argCbTimer_t ;

#endif /* TIMER_H */
