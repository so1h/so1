/* ----------------------------------------------------------------------- */
/*                                saludos.c                                */
/* ----------------------------------------------------------------------- */
/*                 mensajes que se muestran al inicio de SO1               */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>
#include <so1pub.h\colores.h>
#include <so1pub.h\ll_s_so1.h>
#include <so1pub.h\printgen.h>
#include <so1pub.h\escribir.h>
#include <so1pub.h\saludos.h>
#include <so1pub.h\strings.h>

void printStrRojo ( char * str ) {
/*setColorWindow(rojoClaro, azul, especial, especial) ; */
  escribirStr(str) ;
/*setColorWindow(grisClaro, azul, especial, especial) ; */
}

void mostrarSaludoGrande ( void ) {
/*ponerEnIntenso(TRUE) ; */
  escribirStr(" ‹‹‹‹ ‹‹‹‹ ‹ ") ; escribirLn() ;
  escribirStr(" €  ﬂ €  € € ") ; /* ponerEnIntenso(FALSE) ; printStrRojo(" CT31") */ ; escribirStr(" v1.4") ; /* ponerEnIntenso(TRUE) */ ; escribirLn() ;
  escribirStr(" ﬂﬂﬂ€ €  € € ") ; escribirLn() ;
  escribirStr(" €‹‹€ €‹‹€ € ") ; /* ponerEnIntenso(FALSE) ; */
  escribirStr(" 07-09-2015") ;
}

void mostrarSaludo ( void ) {
/*ponerEnIntenso(TRUE) ; */
  escribirStr(" ‹‹‹ ‹‹‹ ‹ ") ; escribirLn() ;
  escribirStr(" €‹‹ € € € ") ; escribirLn() ;
  escribirStr(" ‹‹€ €‹€ € ") ;
/*ponerEnIntenso(FALSE) ; */
  escribirStr("v1.4") ;
  escribirLn() ;

}

