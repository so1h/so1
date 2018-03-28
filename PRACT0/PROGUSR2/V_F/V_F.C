/* ----------------------------------------------------------------------- */
/*                                  v_f.c                                  */
/* ----------------------------------------------------------------------- */
/*                     programa v_f para el sistema SO1                    */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\carsctrl.h>                                      /* BEL */
#include <so1pub.h\caracter.h>                                /* mayuscula */
#include <so1pub.h\strings.h>
#include <so1pub.h\msdos.h>              /* SEEK_SET_MSDOS, SEEK_END_MSDOS */

void formato ( void ) {
  escribirStr("\n\n") ;
  escribirStr(" formato: v_f [ fichero posicion | -h ] ") ;
  escribirLn() ;
}

void help ( void ) {
  escribirLn() ;
  escribirLn() ;
  escribirStr(" formato 1: v_f ") ;
                     escribirStr("muestra el formato del comando   \n\n") ;
  escribirStr(" formato 2: v_f -h ") ;
                                  escribirStr("muestra este help   \n\n") ;
  escribirStr(" formato 3: v_f fichero posicion ") ;
                                                    escribirStr("y \n\n") ;
  escribirStr(" formato 4: v_f fichero posicion*factor+desplazamiento \n\n") ;
  escribirStr("   visualiza los 256 bytes presentes en el fichero  \n") ;
  escribirStr("   indicado, a partir de la posicion indicada, con  \n") ;
  escribirStr("   factor de escala y desplazamiento opcionales.    \n\n") ;
  escribirStr(" autor: Pedro Pablo Lopez Rodriguez (S.O. 2014)     \n") ;
}

void main ( int argc, char * argv [ ] ) {

  int df ;
  dword_t pos ;
  dword_t tam ;
  dword_t factor ;
  dword_t despl ;
  dword_t posAux ;
  char buf [256] ;
  int leidos ;
  char carAux ;
  word_t * ptrWord ;
  dword_t * ptrDWord ;

  int i, j ;

  if (!hayMSDOS()) {
    escribirCar(BEL) ;                                         /* error -1 */
    escribirStr("\n\n error -1: este programa requiere MSDOS \n") ;
    exit(-1) ;
  }

  if (argc == 1) {
    formato() ;
    return ;
  }

  if (argc == 2) {
    if ((argv[1][0] == '-') && (mayuscula(argv[1][1]) == 'H') && (argv[1][2] == (char)0)) {
      help() ; return ;
    }
    escribirCar(BEL) ;                                         /* error -2 */
    formato() ;
    escribirStr("\n error -2: falta indicar una posicion dentro del fichero \n") ;
    exit(-2) ;
  }

  if (argc > 3) {
    escribirCar(BEL) ;                                         /* error -3 */
    formato() ;
    escribirStr("\n error -3: sobran los argumentos: ") ;
    for ( i = 3 ; i < argc ; i++ ) {
      escribirStr(argv[i]) ;
      escribirCar(' ') ;
    }
    escribirLn() ;
    exit(-3) ;
  }

  if ((df = openDOS(argv[1], O_RDONLY_MSDOS)) < 0) {
    escribirCar(BEL) ;                                         /* error -4 */
    formato() ;
    escribirStr("\n error -4: no se ha podido abrir el fichero: \"") ;
    escribirStr(argv[1]) ;
    escribirStr("\"\n") ;
    exit(-4) ;
  }

  copiarStr(argv[2], comando[0]) ;
  inicScanner() ;
  obtenSimb() ;
  if (simb == s_numero)
    pos = (dword_t)num ;
  else if (simb == s_numeroLargo)
    pos = (dword_t)numLargo ;
  else {
    escribirCar(BEL) ;                                         /* error -5 */
    formato() ;
    escribirStr("\n error -5: posicion debe ser un numero entero no negativo y no \"") ;
    escribirStr(argv[2]) ;
    escribirStr("\"\n") ;
    exit(-5) ;
  }

  if (car == '*') {
    obtenCar() ;
    obtenSimb() ;
    if (simb == s_numero)
      factor = (dword_t)num ;
    else if (simb == s_numeroLargo)
      factor = (dword_t)numLargo ;
    else {
      escribirCar(BEL) ;                                       /* error -6 */
      formato() ;
      escribirStr("\n error -6: factor debe ser un numero entero no negativo \n") ;
      exit(-6) ;
    }
  }
  else factor = (dword_t)1 ;

  carAux = car ;
  if ((carAux == '+') || (carAux == '-')) {
    obtenCar() ;
    obtenSimb() ;
    if (simb == s_numero)
      despl = (dword_t)num ;
    else if (simb == s_numeroLargo)
      despl = (dword_t)numLargo ;
    else {
      escribirCar(BEL) ;                                       /* error -7 */
      formato() ;
      escribirStr("\n error -7: desplazamiento debe ser un numero entero no negativo \n") ;
      exit(-7) ;
    }
  }
  else {
    despl = (dword_t)0 ;
    carAux = '+' ;
  }

  if ((carAux == '-') && ((despl/factor) > pos)) {
    escribirCar(BEL) ;                                         /* error -8 */
    formato() ;
    escribirStr("\n error -8: posicion*factor-desplazamiento debe ser >= 0 \n") ;
    exit(-8) ;
  }

  tam = (dword_t)0 ;
  lseekDOS(df, (dword_t *)&tam, SEEK_END_MSDOS) ;

  if ((factor == 0) ||
      ((carAux == '+') && (despl <= tam) && (pos <= ((tam-despl)/factor))) ||
      ((carAux == '-') && (pos <= ((tam+despl)/factor))))
    pos = (dword_t)(pos*factor+((carAux == '+') ? despl : -(long int)despl)) ;
  else {
    escribirCar(BEL) ;                                         /* error -9 */
    formato() ;
    escribirStr("\n error -9: posicion (*factor+despl) >= longitud del fichero (") ;
    escribirLDec(tam, 1) ;
    escribirStr(") ") ;
    escribirLn() ;
    exit(-9) ;
  }

          /* los datos de la linea de comandos fichero y pos son correctos */

  /* Aquí el alumno debe programar la lectura de los 256 */
  /* bytes del fichero y su visualización por pantalla   */
  /* de la misma manera que lo hace el programa ver_fich */

  escribirLn() ;
  escribirStr("\n Falta por programar la visualizacion del fichero \n") ;

  closeDOS(df) ;

}

