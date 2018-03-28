/* ----------------------------------------------------------------------- */
/*                                 bios.h                                  */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#ifndef BIOS_H
#define BIOS_H

#include <so1pub.h\tipos.h>                              /* byte_t, word_t */
#include <so1pub.h\bios_0.h>

//                                                      /* Disco (int 13h) */

#define cmd_reset        0x00
#define cmd_read_sector  0x02
#define cmd_write_sector 0x03
#define cmd_get_params   0x08

typedef struct {
  byte_t h ;                                  /* numero de cabeza (8 bits) */
  word_t cs ;             /* numero de cilindro y sector: ccccccccCCssssss */
} CSH_t ;

int getDriveParams ( byte_t unidadBIOS, CSH_t * CSH ) ;

/* resetControllerBIOS: reinicia el controlador del disco (disquete)       */
/*                devuelve como resultado una palabra con:                 */
/*                  0000 si todo ha ido bien                               */
/*                  00XX si ha habido algún error                          */

int resetControllerBIOS ( void ) ;

/* leerSectorCSH: CSH indica la cabeza, el sector y el cilindro            */
/*                unidadBIOS (0 = A:, 1 = B:, 80 = C:, ...)                */
/*                dir es la direccion del bufer de memoria                 */
/*                devuelve como resultado una palabra con:                 */
/*                  0000 si todo ha ido bien                               */
/*                  00XX si ha habido algún error                          */

int leerSectorCSH ( CSH_t * CSH, byte_t unidadBIOS, pointer_t dir ) ;

int escribirSectorCSH ( CSH_t * CSH, byte_t unidadBIOS, pointer_t dir ) ;

int opSectorCSH ( CSH_t * CSH, byte_t unidadBIOS, pointer_t dir, byte_t cmd ) ;

/* leerSectorLBA: sectorLogico (de la unidad)                              */
/*                unidadBIOS (0 = A:, 1 = B:, 80 = C:, ...)                */
/*                dir es la direccion del bufer de memoria                 */
/*                devuelve como resultado una palabra con:                 */
/*                  0000 si todo ha ido bien                               */
/*                  00XX si ha habido algún error                          */

int leerSectorLBA ( dword_t sectorLogico, byte_t unidadBIOS, pointer_t dir ) ;

int escribirSectorLBA ( dword_t sectorLogico, byte_t unidadBIOS, pointer_t dir ) ;

int opSectorLBA ( dword_t sectorLogico, byte_t unidadBIOS, pointer_t dir, byte_t cmd ) ;

#endif /* BIOS_H */