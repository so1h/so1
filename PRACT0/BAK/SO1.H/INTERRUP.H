/* ----------------------------------------------------------------------- */
/*                               interrup.h                                */
/* ----------------------------------------------------------------------- */
/*                  rutinas de tratamiento de interrupcion                 */
/* ----------------------------------------------------------------------- */

#ifndef INTERRUP_H
#define INTERRUP_H

#include <so1pub.h\tipos.h>
#include <so1pub.h\const.h>

extern rti_t VIOrg [ nVIntMax ] ;   /* vectores de interrupcion originales */

extern rindx_t recVInt [ nVIntMax ] ;      /* recurso que posee ese vector */

void far rti_00 ( void ) ;

#if !defined(TASM32)

void tratamientoComun ( void ) ;

#endif

void inicTVI ( void ) ;

void far redirigirInt ( byte_t nVInt, isr_t isr ) ;

void far redirigirIntHardware ( byte_t irq, isr_t isr ) ;

void restablecerInt ( int nVInt ) ;

#endif /* INTERRUP_H */
