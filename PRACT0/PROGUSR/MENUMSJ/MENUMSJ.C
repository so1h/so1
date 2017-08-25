/* ----------------------------------------------------------------------*/
/*                                menumsj.c                              */
/* --------------------------------------------------------------------- */
/*        programa de usuario con un menu para probar los buzones        */
/* --------------------------------------------------------------------- */

#include <so1pub.h\caracter.h>
#include <so1pub.h\ll_s_so1.h>  /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\printgen.h>
#include <so1pub.h\escribir.h>

char str0[16] = "" ;

void main ( void ) {
  char car = ' ' ;
  unsigned mbox = 0 ;
  char far * msj = (char far *)&str0 ;
  char far * destino = (char far *)&str0 ;
  int ind ;

  while (TRUE) {

    escribirCar('\f') ;              /* borra el contenido de la ventana */

    escribirStr(
      "\n"
      "\n"
      "  MENUSEM (c) Pedro Pablo Lopez Rodriguez (07-11-2016) [bn1002] \n"
      "\n"
      "  SO1: elija operacion: \n"
      "\n"
      "  ('b') - buzon   ("
    ) ;
    escribirCar('0' + mbox) ;
    escribirStr(
      ")\n"
      "\n"
      "  ('m') - mensaje ("
    ) ;
    escribirStr(str0) ;
    escribirStr(
      ")\n"
      "\n"
      "  ('e') - enviar\n"
      "\n"
      "  ('r') - recibir\n"
      "\n"
      "  ('t') - terminar el programa\n"
      "\n"
    ) ;

    car = mayuscula(leer(STDIN)) ;

    switch (car) {
    case 'B' :
      escribirStr("  indice del buzon = ") ;
      do {
        car = leer(STDIN) ;
        if ((car < '0') || ('9' < car)) escribirCar('\a') ;
      } while ((car < '0') || ('9' < car)) ;
      mbox = (car-'0') ;
      break ;
    case 'M' :
      escribirStr("  mensaje = ") ;
      ind = 0 ;
      car = ' ' ;
      while (car != '\n') {
        car = leer(STDIN) ;
        if ((' ' <= car) && (car <= '~') && (ind < 15)) {
                  str0[ind] = car ;
          escribirCar(car) ;
          ind++ ;
        }
        else
          escribirCar('\a') ;
      }
      str0[ind] = '\0' ;
      break ;
    case 'E' :
      escribirStr("  se va a enviar el mensaje ") ;
      mbox_send(mbox, msj) ;
      break ;
    case 'R' :
      escribirStr("  se va a recibir un mensaje ") ;
      mbox_receive(mbox, destino) ;
      break ;
    case 'T' :
      exit(0) ;
    default :
      escribirCar('\a') ;
    }
  }
}
