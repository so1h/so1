/* ----------------------------------------------------------------------- */
/*                                 bios.h                                  */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef BIOS_H
#define BIOS_H

#include <so1pub.h\tipos.h>
#include <so1pub.h\bios_0.h>

typedef struct {
  byte_t h ;                                  /* numero de cabeza (8 bits) */
  word_t cs ;             /* numero de cilindro y sector: ccccccccCCssssss */
} CSH_t ;

/* resetControlerBIOS: reinicia el controlador del disco (disquete)        */
/*                devuelve como resultado una palabra con:                 */
/*                  0000 si todo ha ido bien                               */
/*                  00XX si ha habido algún error                          */

int resetControlerBIOS ( void ) ;

/* leerSectorCSH: CSH indica la cabeza, el sector y el cilindro            */
/*                unidadBIOS (0 = A:, 1 = B:, 80 = C:, ...)                */
/*                dir es la direccion del bufer de memoria                 */
/*                devuelve como resultado una palabra con:                 */
/*                  0000 si todo ha ido bien                               */
/*                  00XX si ha habido algún error                          */

int leerSectorCSH ( CSH_t * CSH, byte_t unidadBIOS, pointer_t dir ) ;

int escribirSectorCSH ( CSH_t * CSH, byte_t unidadBIOS, pointer_t dir ) ;

/* leerSectorLBA: sectorLogico (de la unidad)                              */
/*                unidadBIOS (0 = A:, 1 = B:, 80 = C:, ...)                */
/*                dir es la direccion del bufer de memoria                 */
/*                devuelve como resultado una palabra con:                 */
/*                  0000 si todo ha ido bien                               */
/*                  00XX si ha habido algún error                          */

int leerSectorLBA ( dword_t sectorLogico, byte_t unidadBIOS, pointer_t dir ) ;

int escribirSectorLBA ( dword_t sectorLogico, byte_t unidadBIOS, pointer_t dir ) ;

#endif /* BIOS_H */


#if (FALSE)

typedef struct {
  word_t sectores ;
  word_t cabezas ;
  word_t cilindros ;
} geometria_t ;

extern geometria_t geom [ 4 ] ;

/* leerSectorBIOS: sect es el numero de sector logico                      */
/*                 unidad es el numero de la unidad (0 = A:, 1 = B:)       */
/*                 dir es la direccion del bufer de memoria                */
/*                 devuelve como resultado una palabra con:                */
/*                   0000 si todo ha ido bien                              */
/*                   00XX si ha habido algún error                         */

int leerSectorBIOS ( word_t sect, byte_t unidad, pointer_t dir ) ;

int escribirSectorBIOS ( word_t sect, byte_t unidad, pointer_t dir ) ;

int leerSector ( dword_t sectorLogico, byte_t unidad, pointer_t dir ) ;

#endif