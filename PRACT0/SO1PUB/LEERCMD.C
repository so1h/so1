/* ----------------------------------------------------------------------- */
/*                                leercmd.c                                */
/* ----------------------------------------------------------------------- */
/*             lectura de comandos del interprete de comandos              */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\def_scan.h>     /* Home, End, Ins, Del, Fl(Iz/De/Up/Dn) */
#include <so1pub.h\carsctrl.h>                                      /* ESC */
#include <so1pub.h\caracter.h>                                /* mayuscula */
#include <so1pub.h\strings.h>             /* iguales(SalvoMayusculas)Hasta */
#include <so1pub.h\ll_s_so1.h>                   /* STDIN, leer, leerListo */
#include <so1pub.h\escribir.h>                 /* escribirCar, escribirStr */
#include <so1pub.h\scanner.h>       /* nMaxComandos, tamComando, simb, ... */

#define AutoCCInt TRUE      /* Auto Completado ('\t') de Comandos Internos */
#define AutoCCExt TRUE      /* Auto Completado ('\t') de Comandos Externos */

#if (AutoCCExt)
#include <so1pub.h\msdos.h>                                    /* ffblk */
#endif

char carAnt = (char)0 ;                    /* caracter leido anteriormente */
int comBusqueda = 0 ;              /* comienzo de busqueda autocompletando */
int iAnt = 0 ;                      /* posicio del anterior autocompletado */
int unidadLogica ;
int unidad ;
ffblk_t ffblk ;
struct ffblk fcb ;

/* -------- funciones para borrar total o parcialmente un comando -------- */

/* se utiliza que el caracter '\b' cuando se escribe en la consola no      */
/* borra el caracter acterior, sino que simplemente retrocede. Por ese     */
/* motivo hay que volver a escribir un ' ' para borrar, y luego volver a   */
/* retroceder. Esta licencia que se ha hecho, deberia evitarse en un       */
/* futuro. Una opcion es utilizar otro caracter para retroceder sin borrar */
/* o recordar lo escrito para que pueda borrarse y luego volverse a        */
/* escribir.                                                               */

void borrarUltimoCaracter ( int i ) {          /* borra el ultimo caracter */
  escribirCar('\b') ; escribirCar(' ') ; escribirCar('\b') ; /* en consola */
  comando[inCmd][i-1] = (char)0 ;                         /* en el comando */
}

void borrarCaracter ( int i ) {            /* borra un caracter intermedio */
  int j = i ;
  int k ;
  while (comando[inCmd][j] != (char)0) j++ ;
  escribirCar('\b') ;
  for ( k = i ; k < j ; k++ ) {
    escribirCar(comando[inCmd][k]) ;
    comando[inCmd][k-1] = comando[inCmd][k] ;
  }
  escribirCar(' ') ;
  escribirCar('\b') ;
  comando[inCmd][j-1] = (char)0 ;
  for ( k = i ; k < j ; k++ )
    escribirCar('\b') ;
}

void borrarLineaDeComando ( int i ) {             /* borra todo el comando */
  while (comando[inCmd][i] != (char)0) {          /* respentando el prompt */
    escribirCar(comando[inCmd][i]) ;
    i++ ;
  }
  while (i > 0) {
    borrarUltimoCaracter(i) ;
    i-- ;
  }
}

/* ----- copiarComando copia un comando anterior en el comando actual ---- */

int copiarComando ( int iC, int inCmd ) {
  int i = 0 ;
  while (comando[iC][i] != (char)0) {
    comando[inCmd][i] = comando[iC][i] ;
    escribirCar(comando[inCmd][i]) ;
    i++ ;
  }
  comando[inCmd][i] = (char)0 ;
  return(i) ;
}

/* ---------------- autocompletar con un comando interno ----------------- */
/*    (autocompletar el comando si es un prefijo de un comando interno)    */
/* ----------------------------------------------------------------------- */

int autocompletarCInt ( int   i ) {

  int j, k, n ;
  int largo ;
  int ind ;

#define volverTabAnterior() { \
  if (carAnt == '\t') {  /* acababa de realizarse un autocompletado previo */\
    for ( j = iAnt ; j < i ; j++ ) {     /* volver a la situacion anterior */\
      comando[inCmd][j] = (char)0 ;                                          \
      escribirCar('\b') ;                                                    \
    }                                                                        \
    i = iAnt ;                                                               \
  }                                                                          \
}

  volverTabAnterior() ;

  j = 0 ;
  while ((comando[inCmd][j] == ' ') && (j <= i))
    j++ ;                                             /* j = primer no ' ' */

  if (j < i) {
    k = comBusqueda ;
    for (ind = 0 ; ind < numCmds ; ind++ ) {
      largo = simbCmd[k].largo ;
      if ((i - j) < largo) {
        if (igualesSalvoMayusculasHasta(
             (char far *)&comando[inCmd][j],
             (char far *)simbCmd[k].str, (i - j))) {
          for ( n = j ; n < (j + largo) ; n++ )
            comando[inCmd][n] = simbCmd[k].str[n - j] ;
          for ( n = 0 ; n < (i - j) ; n++ )
            escribirCar('\b') ;
          escribirStr(simbCmd[k].str) ;
          iAnt = i ;
          i = j + largo ;
          comando[inCmd][i] = (char)0 ;
          comBusqueda = (k + 1) % numCmds ;
          break ;
        }
      }
      k = (k + 1) % numCmds ;
    }
  }
  return(i) ;
}

int autocompletarCExt ( int i ) {

  int j, k, n ;
  int largo ;

  char prefijo [tamComando] ;                   /* prefijo a autocompletar */

  volverTabAnterior() ;

  j = 0 ;
  while ((comando[inCmd][j] == ' ') && (j <= i))
    j++ ;                                             /* j = primer no ' ' */
  if (j < i) {
    if ((unidadLogica = getdisk()) >= 0) {
      if ((carAnt != '\t') || (findNext(&ffblk) != 0))
        if (findFirst(unidadLogica, &ffblk) != 0) return(i) ;
      do {
//      escribirStr("\n fichero = ") ;
//      escribirStr(ffblk.ff_name) ;
        k = 1 ;
        while ((ffblk.ff_name[k] != ' ') &&
               (ffblk.ff_name[k] != '.')) k++ ;
        largo = k ;
//      escribirStr("largo = ") ;
//      escribirDec(largo, 1) ;
        if ((i - j) < largo) {
          if (igualesSalvoMayusculasHasta(
               (char far *)&comando[inCmd][j],
               (char far *)ffblk.ff_name, (i - j))) {
            for ( n = j ; n < (j + largo) ; n++ )
              comando[inCmd][n] = ffblk.ff_name[n - j] ;
            for ( n = 0 ; n < (i - j) ; n++ )
              escribirCar('\b') ;
            escribirStrHasta(ffblk.ff_name, largo, FALSE) ;
            iAnt = i ;
            i = j + largo ;
            comando[inCmd][i] = (char)0 ;
            return(i) ;
          }
        }
      }
      while (findNext(&ffblk) == 0) ;
    }
    else {
      copiarStr((char *)&comando[inCmd][j], prefijo) ;
      copiarStr("*.BIN", (char *)&prefijo[i-j]) ;
//    escribirStr("\n prefijo = ") ;
//    escribirStr(prefijo) ;
//    escribirLn() ;
      if ((carAnt != '\t') || (findnextDOS((struct ffblk *)&fcb) != 0))
        if (findfirstDOS((char *)prefijo, (struct ffblk *)&fcb, FA_ARCH) == -1) return(i) ;
      do {
//      escribirStr("\n fichero = ") ;
//      escribirStr(fcb.ff_name) ;
        k = 1 ;
        while ((fcb.ff_name[k] != ' ') &&
               (fcb.ff_name[k] != '.')) k++ ;
        largo = k ;
//      escribirStr("largo = ") ;
//      escribirDec(largo, 1) ;
        if ((i - j) < largo) {
          if (igualesSalvoMayusculasHasta(
               (char far *)&comando[inCmd][j],
               (char far *)fcb.ff_name, (i - j))) {
            for ( n = j ; n < (j + largo) ; n++ )
              comando[inCmd][n] = fcb.ff_name[n - j] ;
            for ( n = 0 ; n < (i - j) ; n++ )
              escribirCar('\b') ;
            escribirStrHasta(fcb.ff_name, largo, FALSE) ;
            iAnt = i ;
            i = j + largo ;
            comando[inCmd][i] = (char)0 ;
            return(i) ;
          }
        }
      }
      while (findnextDOS((struct ffblk *)&fcb) == 0) ;
    }
  }
  return(i) ;
}

/* ----------------------------- leerComando ----------------------------- */

/* leeComando lee el siguiente comando en comando[inCmd] permitiendo en    */
/* parte su edicion. Los parametros autCcompletado y enmascarado sirven    */
/* para cambiar el comportamiento de leerComando para autocompletar los    */
/* comandos (con nombres de comandos internos o de ficheros ejecutables    */
/* .BIN), o tambien para enmascarar los caracteres introducidos con el     */
/* caracter * con el fin de que no sean visualizados en la pantalla.       */

void leerComando ( bool_t autoCompletado, bool_t enmascarado ) {

  char car = (char)0 ;                            /* ultimo caracter leido */
  byte_t scanCode ;
  bool_t sobreEscritura = FALSE ;
  int iC ;
  int i = 0 ;               /* posicion del caracter car en comando[inCmd] */
  int j ;

//word_t linea1 ;
//word_t linea2 ;
//getCursor((word_t far *)&linea1, (word_t far *)&linea2) ;
//escribirDec(linea1, 3) ; escribirDec(linea2, 3) ; */

  if ((nComandos == 0) || (comando[inCmd][0] != (char)0)) {
    inCmd = (inCmd + 1) % nMaxComandos ;            /* array/cola circular */
    if (nComandos < nMaxComandos)
      nComandos++ ;                                /* habra un comando mas */
    else
      outCmd = (outCmd + 1) % nMaxComandos ;      /* eliminamos un comando */
  }

  iC = inCmd ;

  comando[inCmd][0] = (char)0 ;        /* nuevo comando inicialmente vacio */

  while (TRUE) {
    carAnt = car ;                       /* guardamos el caracter anterior */
    if (((car = leer(STDIN)) == '\r') || (car == '\n'))       /* fin linea */
      break ;

    if (car == '\b')                                           /* bakspace */
      if (i > 0) {
        borrarCaracter(i) ;
        i-- ;
      }
      else escribirCar('\a') ;          /* pitido si el comando esta vacio */
    else if (car == (char)0) {                 /* caracter ascii extendido */
      scanCode = (byte_t)leer(STDIN) ;
//    escribirStr("\n scanCode = 0x") ;
//    escribirHex(scanCode, 2) ;
      switch (scanCode) {
      case Del :                                                   /* Supr */
        if (comando[inCmd][i] != (char)0) {
          escribirCar(comando[inCmd][i]) ;
          borrarCaracter(i+1) ;
        }
        else escribirCar('\a') ;
        break ;
      case Home :                                                /* Inicio */
        while (i > 0) {
          escribirCar('\b') ;
          i-- ;
        }
        break ;
      case End :                                                    /* Fin */
        while (comando[inCmd][i] != (char)0) {
          escribirCar(comando[inCmd][i]) ;
          i++ ;
        }
        break ;
      case Ins :                                                    /* Ins */
        sobreEscritura = !sobreEscritura ;
//      if (sobreEscritura)
//        setCursor(8, 12) ;
//      else
//        setCursor(linea1, linea2) ;
        break ;
      case FlIz :                                      /* Flecha izquierda */
        if (i > 0) {
          escribirCar('\b') ;
          i-- ;
        }
        break ;
      case FlDe :                                        /* Flecha derecha */
        if ((i < (tamComando-1)) && (comando[inCmd][i] != (char)0))
          escribirCar(comando[inCmd][i++]) ;
        break ;
      case FlUp :                                         /* Flecha arriba */
        if (iC != outCmd) {
          iC = (iC + nMaxComandos - 1) % nMaxComandos ;
          borrarLineaDeComando(i) ;
          i = copiarComando(iC, inCmd) ;
        }
        break ;
      case FlDn :                                          /* Flecha abajo */
        if ((iC != inCmd) && ((((iC + 1) % nMaxComandos)) != inCmd)) {
          iC = (iC + 1) % nMaxComandos ;
          borrarLineaDeComando(i) ;
          i = copiarComando(iC, inCmd) ;
        }
        break ;
      case ShiftTab :                 /* Shift-Tabulador ==> autocompletar */
        car = '\t' ;
        if (autoCompletado) {
//        i = autocompletarCInt(i) ;
          i = autocompletarCExt(i) ;
        }
        break ;
      default : ;
      }
    }
    else if (car == ESC) {                                          /* ESC */
      borrarLineaDeComando(i) ;
      comando[inCmd][0] = (char)0 ;
      i = 0 ;
    }
    else if (car == '\t') {          /* '\t' (tabulador ==> autocompletar) */
      if (autoCompletado) {
        i = autocompletarCInt(i) ;
//      i = autocompletarCExt(i) ;
      }
    }
    else {              /* introducir el caracter car en comando[inCmd][i] */

      if (i < (tamComando-1)) {
        if ((' ' <= car) /* && (car <= '~') */) {
          if (!enmascarado)
            escribirCar(car) ;
          else
            escribirCar('*') ;
          if (!sobreEscritura) {
            j = i ;
            while (comando[inCmd][j] != (char)0) {
              escribirCar(comando[inCmd][j]) ;
              j++ ;
            }
            comando[inCmd][j+1] = (char)0 ;
            while (j > i) {
              comando[inCmd][j] = comando[inCmd][j-1] ;
              escribirCar('\b') ;
              j-- ;
            }
          }
          comando[inCmd][i++] = car ;
        }
      }
      else escribirCar('\a') ;
    }
  }                                                       /* fin del while */

  if ((car == '\r') && (leerListo(STDIN) == '\n'))          /* '\r' + '\n' */
    leer(STDIN) ;                                             /* leer '\n' */

  while (comando[inCmd][i] != (char)0) {
    escribirCar(comando[inCmd][i]) ;
    i++ ;
  }
  i-- ;
  while ((i >= 0) && (comando[inCmd][i] == ' ')) {    /* eliminar ' ' izda */
    comando[inCmd][i] = (char)0 ;
    i-- ;
  }

//getCursor((word_t far *)&linea1, (word_t far *)&linea2) ;

}

