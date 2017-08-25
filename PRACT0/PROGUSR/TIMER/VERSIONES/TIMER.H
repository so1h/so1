/* ----------------------------------------------------------------------- */
/*                                 timer.h                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef TIMER_H
#define TIMER_H

#include <so1pub.h\tipos.h>

typedef struct {
  word_t contTics ;
  word_t ticsPorRodaja ;
  word_t contTicsRodaja ;
} argCbTimer_t ;

extern word_t ticsPorRodaja ;

extern word_t contTicsReloj ;                /* 0, 1, 2, ... ticsPorRodaja */

extern rec_timer ;

void inicTimer ( void ) ;

#endif /* TIMER_H */
