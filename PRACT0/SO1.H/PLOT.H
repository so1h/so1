/* ----------------------------------------------------------------------- */
/*                                 plot.h                                  */
/* ----------------------------------------------------------------------- */
/*                     envio de datos de monitorizacion                    */
/* ----------------------------------------------------------------------- */

#ifndef PLOT_H
#define PLOT_H

#include <so1pub.h\tipos.h>

#define Plot_LPT1 0x00                                  /* LPT por polling */
#define Plot_LPT2 0x01
#define Plot_LPT3 0x02
#define Plot_LPT4 0x03
#define Plot_COM1 0x04       /* COM polling/interrupciones segun Plot_Ints */
#define Plot_COM2 0x05
#define Plot_COM3 0x06
#define Plot_COM4 0x07
#define Plot_E9   0x08                                        /* puerto E9 */
#define Plot_File 0x10                                 /* Fichero en WinXP */
#define Plot_Ints 0x20                             /* Interrupciones (COM) */

extern bool_t plotProcesos ;

void plot ( char tipo, word_t ax, word_t cont ) ;

bool_t initPlot ( byte_t modo, char * filePlot ) ;

void finPlot ( void ) ;

#endif /* PLOT_H */
