/* ----------------------------------------------------------------------- */
/*                                saludos.c                                */
/* ----------------------------------------------------------------------- */
/*                 mensajes que se muestran al inicio de SO1               */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\colores.h>
#include <so1pub.h\ll_s_so1.h>
#include <so1pub.h\saludos.h>
#include <so1pub.h\stdio.h>                    /* printf, getchar, putchar */

void printStrRojo ( char * str ) {
//* setColorWindow(rojoClaro, azul, especial, especial) ; */
    printf(str) ;
//* escribirStr(str) ; */
//* setColorWindow(grisClaro, azul, especial, especial) ; */
}

void mostrarSaludoGrande ( void ) {
//* ponerEnIntenso(TRUE) ; */
    printf(
        " ‹‹‹‹ ‹‹‹‹ ‹ \n"
        " €  ﬂ €  € €  v1.7 \n" /* ponerEnIntenso(FALSE) ; printStrRojo(" CT31") ; ponerEnIntenso(TRUE) */ 
        " ﬂﬂﬂ€ €  € € \n"
        " €‹‹€ €‹‹€ €  07-09-2017 " /* ponerEnIntenso(FALSE) ; */
	) ;
}

void mostrarSaludo ( void ) {
//* ponerEnIntenso(TRUE) ; */
    printf(
        " ‹‹‹ ‹‹‹ ‹ \n"
        " €‹‹ € € € \n"
        " ‹‹€ €‹€ € v1.7 \n" /* ponerEnIntenso(FALSE) ; */
	) ;
}

