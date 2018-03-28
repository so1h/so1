/* ----------------------------------------------------------------------*/
/*                               menusem.c                               */
/* --------------------------------------------------------------------- */
/*       programa de usuario con un menu para probar los semaforos       */
/* --------------------------------------------------------------------- */

#include <so1pub.h\caracter.h>
#include <so1pub.h\ll_s_so1.h>     /* biblioteca llamadas al sistema SO1 */
#include <so1pub.h\printgen.h>
#include <so1pub.h\escribir.h>

#include <so1pub.h\scanner.h>
#include <so1pub.h\strings.h>

void main ( void ) {

  char car, opcion ;

  unsigned int sem = 0 ;

  unsigned int valor = 0 ;

  while (TRUE) {

    escribirCar('\f') ;              /* borra el contenido de la ventana */

    escribirStr(
      "\n"
      "\n"
      "  MENUSEM (c) Pedro Pablo Lopez Rodriguez (31-10-2016) [bn1002] \n"
      "\n"
      "  SO1: elija operacion: sem = "
    ) ;
    escribirCar('0' + sem) ;
    escribirStr(
      "\n"
      "\n"
      "    ('e') - elegir semaforo      \n"
      "\n"
      "    ('i') - inicializar semaforo \n"
      "\n"
      "    ('b') - bajar semaforo       \n"
      "\n"
      "    ('s') - subir semaforo       \n"
      "\n"
      "    ('t') - terminar el programa \n"
      "\n"
      "  Operacion = "
    ) ;

    opcion = mayuscula(leer(STDIN)) ;

    escribirStr("\n\n") ;

    switch (opcion) {
    case 'E' :
      escribirStr("  descriptor de semaforo = ") ;
      do {
        car = leer(STDIN) ;
        if ((car < '0') || ('9' < car)) escribirCar('\a') ;
      } while ((car < '0') || ('9' < car)) ;
      sem = (car-'0') ;
      break ;
    case 'I' :
      escribirStr("  valor del semaforo = ") ;
      do {
        car = leer(STDIN) ;
        if ((car < '0') || ('9' < car)) escribirCar('\a') ;
      } while ((car < '0') || ('9' < car)) ;
      valor = (car-'0') ;
      sem_inic(sem, valor) ;
      break ;
    case 'B' :
      escribirStr("  se va a bajar el semaforo ") ;
      sem_bajar(sem) ;
      break ;
    case 'S' :
      escribirStr("  se va a subir el semaforo ") ;
      sem_subir(sem) ;
      break ;
    case 'T' :
      exit(0) ;
    default :
      escribirCar('\a') ;
    }
  }
}
