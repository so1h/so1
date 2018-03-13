/* ----------------------------------------------------------------------- */
/*                                bios_rat.h                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef BIOS_RAT_H
#define BIOS_RAT_H

#include <so1pub.h\tipos.h>
#include <so1pub.h\bios_0.h>

bool_t hayDriverRaton ( void ) ;

bool_t hayRatonBIOS ( void ) ;

word_t resetRatonBIOS ( word_t * numBotones ) ;

void versionRatonBIOS ( word_t * menor,
                        word_t * mayor,
                        word_t * tipo,
                        word_t * irq ) ;

void mostrarCursorRatonBIOS ( void ) ;

void ocultarCursorRatonBIOS ( void ) ;

void leerRatonBIOS ( word_t * W, word_t * X, word_t * Y ) ;

void situarCursorRatonBIOS ( word_t X, word_t Y ) ;

void far rti_raton_BIOS ( void ) ;

void installMouseEventHandler ( handler_t handler ) ;

void uninstallMouseEventHandler ( handler_t handler ) ;

void far ratonHandler ( void ) ;

#endif /* BIOS_RAT_H */