/* ----------------------------------------------------------------------- */
/*                               def_timer.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef TIMER_H
#define TIMER_H

#include <so1pub.h\tipos.h>

typedef struct {
  dword_t contTics ;
  dword_t contRodajas ;
  word_t  contTicsRodaja ;
  word_t  ticsPorRodaja ;
} argCbTimer_t ;

#endif /* TIMER_H */
