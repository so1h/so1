/* ----------------------------------------------------------------------- */
/*                                dbgword.h                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef DBGWORD_H
#define DBGWORD_H

#include <so1pub.h\tipos.h>

extern word_t debugWord ;                  /* palabra global de depuracion */

/* Esta palabra (del segmento de datos de) SO1 tiene la finalidad de poder */
/* utilizarse para la depuracion del sistema operativo en situaciones en   */
/* las que el control fluye desde el interior del sistema a los divers o a */
/* los programas de usuario. El sistema conoce perfectamente la direccion  */
/* de esa variable. Desde cualquier modulo de SO1 no hay mas que incluir   */
/* so1.h\dbgword.h para poder leer y escribir en esa variable. Ademas para */
/* los programas de usuario o drivers no hay mas que hacer la llamada al   */
/* sistema obtenInfoINFO( info_t far * info ) para obtener en el campo:    */
/*                                                                         */
/*    info.ptrDebugWord                                                    */
/*                                                                         */
/* el puntero lejano a la palabra debugWord. De manera que podemor leer y  */
/* escribir debugWord sin mas que consultar:                               */
/*                                                                         */
/*    *indo.ptrDebugWord                                                   */
/*                                                                         */
/* Así gracias a esa variable global (sistema/usuario) podemos poner los   */
/* puntos de parada del tipo:                                              */
/*                                                                         */
/*    if (*info.ptrDebugWord & 0x0001) while (TRUE) ;                      */
/*                                                                         */

#endif /* DBGWORD_H */
