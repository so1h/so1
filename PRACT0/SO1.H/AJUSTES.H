/* ----------------------------------------------------------------------- */
/*                                ajustes.h                                */
/* ----------------------------------------------------------------------- */
/*    Ajuste de los registros de segmento y salto al programa principal    */
/* ----------------------------------------------------------------------- */

#ifndef AJUSTES_H
#define AJUSTES_H

#include <so1pub.h\tipos.h>
#include <so1pub.h\def_proc.h>

#define ptrMagicByteSO1 ((pointer_t)(MK_FP(CS_SO1, 0x0000)))

#define ptrMagicByteUsr ((pointer_t)(MK_FP(CS_SO1, (word_t)startCabecera)))


void far startBin ( void ) ;  /* punto de entrada so1.bin (boot) o so1.com */

void startExe ( void ) ;                       /* punto de entrada so1.exe */

void startCommon ( void ) ;

void startCabecera ( void ) ;

void tomarIP ( void ) ;

void finish ( void ) ;

void segDatos ( void ) ;

modoSO1_t modoSO1 ( void ) ;  /* modo en que se ejecuta SO1: bin, exe, ... */

word_t desplCab ( void ) ;  /* desplazam. de la cabecera (cab en startBin) */

byte_t unidadBIOS ( void ) ;       /* numero BIOS de la unidad de arranque */

void pilaInicialProvisional ( void ) ;

extern word_t CS_SO1 ;                        /* segmento de codigo de SO1 */

extern word_t DS_SO1 ;                         /* segmento de datos de SO1 */

extern word_t BSS_SO1 ;                             /* comienzo BSS de SO1 */

extern word_t IMRInicial ;     /* mascara de interrupcion inicial del 8259 */

                                    /* guardar el segmento de datos de SO1 */
							
#if defined(TASM32)
#define guardarDS_SO1()                                           /* macro */\
  asm mov word ptr cs:[OFFSET segDatos],ds ;                  /* fin macro */
#else
#define guardarDS_SO1()                                           /* macro */\
  asm db 2eh, 8ch, 1eh ; \
  asm dw OFFSET segDatos ;                                    /* fin macro */
#endif

#if defined(TASM32)
#define establecerDS_SO1()                                        /* macro */\
  asm mov ds, word ptr cs:[OFFSET segDatos]                   /* fin macro */
#else
#define establecerDS_SO1()                                        /* macro */\
  asm db 2eh, 8eh, 1eh ; \
  asm dw OFFSET segDatos ;                                    /* fin macro */
#endif

#endif /* AJUSTES_H */