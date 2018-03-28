/* ----------------------------------------------------------------------- */
/*                                 testsegs.c                              */
/* ----------------------------------------------------------------------- */
/*  programa para ilustrar el uso de los segmentos CODE, DATA, BSS y STACK */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>
#include <so1pub.h\strings.h>                                 /* copiarStr */
#include <so1pub.h\carsctrl.h>                                       /* LF */

char vGI_1 = 'I' ;               /* DATA: variables Globales Inicializadas */
int  vGI_2 = 0x1111 ;
char vGI_3 [16] = "vGI_3 abcdefghi" ;

char vGNI_1 ;                  /* BSS: variables Globales No Inicializadas */
int  vGNI_2 ;
char vGNI_3 [16] ;

void funcion_2 ( char parF2_1, int parF2_2 ) ;                  /* forward */

#pragma option -w-aus /* omitir warning id. is assigned a value never used */
#pragma option -w-par            /* omitir warning parameter is never used */

void funcion_1 ( char parF1_1, int parF1_2 )
{

    char vLF1_1 = 'L' ;    /* STACK: variables Locales y parametros (PILA) */
    int  vLF1_2 = 0xBBBB ;
    char vLF1_3 [16] ;
    copiarStr("vLF1_3 abcdefgh", vLF1_3) ;                 /* CODE: codigo */
    escribirStr("\n\n funcion_1: comienzo") ;
    while (leer(STDIN) != LF) ;              /* esperar que se pulse INTRO */
    funcion_2('G', 0x4444) ;
    escribirStr("\n\n funcion_1: tras retornar de funcion_2") ;
    while (leer(STDIN) != LF) ;              /* esperar que se pulse INTRO */
}

void funcion_2 ( char parF2_1, int parF2_2 )
{

    char vLF2_1 = 'L' ;    /* STACK: variables Locales y parametros (PILA) */
    int  vLF2_2 = 0xCCCC ;
    char vLF2_3 [16] ;
    copiarStr("vLF2_3 abcdefgh", vLF2_3) ;                 /* CODE: codigo */
    escribirStr("\n\n funcion_2: comienzo") ;
    while (leer(STDIN) != LF) ;              /* esperar que se pulse INTRO */
}

#pragma option -wpar           /* permitir warning parameter is never used */

void main ( void )
{

    char vLMain_1 = 'L' ;  /* STACK: variables Locales y parametros (PILA) */
    int  vLMain_2 = 0xAAAA ;
    char vLMain_3 [16] ;
    copiarStr("vLMain_3 abcdef", vLMain_3) ;               /* CODE: codigo */

    vGNI_1 = 'N' ;
    vGNI_2 = 0x2222 ;
    copiarStr("vGNI_3 abcdefgh", vGNI_3) ;

    escribirStr("\n\n main: comienzo") ;
    while (leer(STDIN) != LF) ;              /* esperar que se pulse INTRO */
    funcion_1('P', 0x3333) ;
    escribirStr("\n\n main: tras retornar de funcion_1") ;
    while (leer(STDIN) != LF) ;              /* esperar que se pulse INTRO */
    escribirLn() ;
}

