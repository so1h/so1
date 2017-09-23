/* ----------------------------------------------------------------------- */
/*                               interpre.c                                */
/* ----------------------------------------------------------------------- */
/*                     interpretacion de los comandos                      */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\carsctrl.h>                                      /* ESC */
#include <so1pub.h\caracter.h>                                /* mayuscula */
#include <so1pub.h\scanner.h>           /* tamComando, car, simb, num, str */
                                /* saltarBlancos, obtenSimb, obtenCar, ... */
#include <so1pub.h\leercmd.h>                               /* leerComando */
#include <so1pub.h\ll_s_so1.h>                           /* STDIN, STDOUT, */
                                       /* open, close, O_WRONLY, O_RDONLY, */
                             /* getpid, getppid, getpindx, leerAsciiListo, */
                                            /* retardoActivo, activarTraza */
#include <so1pub.h\escribir.h>             /* escribir(Car/Str/Dec/Hex/Ln) */
#include <so1pub.h\strings.h>                        /* copiarStr, iguales */
#include <so1pub.h\msdos.h>         /* hayMSDOS, MAXPATH, FA_DIREC, ffblk, */
                                             /* findfirstDOS, findnextDOS, */
                                                /* getdiskDOS, setdiskDOS, */
                                                 /* getcurdirDOS, chdirDOS */


char prompt [ tamComando ] = "SO1> " ;           /* puede contener caminos */


void cambiarPromptDOS ( int unidad ) {    /* actualiza camino en el prompt */
  char directorio [ MAXPATH ] ;
  int i, j ;
  getcurdirDOS(unidad, (char *)directorio) ;
  prompt[0] = 'A' + unidad ;
  prompt[1] = ':' ;
  prompt[2] = '\\' ;
  j = 3 ;
  for ( i = 0 ; directorio[i] != (char)0 ; i++ )
    prompt[j++] = directorio[i] ;
  prompt[j++] = '>' ;
  prompt[j++] = ' ' ;
  prompt[j] = (char)0 ;
}

/* redireccion de la entrada/salida estandar (y de error) a un dispositivo */

static char strStd [  ] [ 16 ] = {
  "entrada",
  "salida",
  "salida de error"
} ;

int redirigirSTDIO ( char * nombreDispositivo ) {
  int df, dfStd ;
  for ( dfStd = STDIN ; dfStd <= STDERR ; dfStd++ ) {
    if (close(dfStd) == -1) return(-1-dfStd) ;
    if (((df = open(nombreDispositivo, dfStd ? O_WRONLY : O_RDONLY)) == -1) || (df != dfStd)) {
      if (df != -1) {
        close(df) ;
        return(-4) ;
      }
      return(-5-dfStd) ;
    }
  }
  return(0) ;
}

/* ----------------------------------------------------------------------- */
/*       funciones de interpretacion de cada uno de los comandos           */
/* ----------------------------------------------------------------------- */

/* --------- interpretacion del comando exit ----------------------------- */

void interpretar_exit ( void ) {
  int num1 ;
  if (getppid() != 0) {
    saltarBlancos() ; obtenSimb() ; num1 = 1 ;
    if (car == '-') { obtenCar() ; obtenSimb() ; num1 = -1 ; }
    if (simb == s_fin) { num = 0 ; simb = s_exit ; return ; }
    if (simb == s_numero) { num = num1*num ; simb = s_exit ; return ; }
    escribirStr(" formato exit [num] ") ;
  }
  else {                  /* exit tecleado en la consola */
    escribirStr(
      "\a\n"
      "\n"
      " comando no permitido en la consola (use shutdown) \n"
    ) ;
  }
  simb = s_nulo ;
}

/* --------- interpretacion de los comandos halt, reboot y shutdown ------ */

void interpretar_salida ( simb_t simbolo ) {
  simb_t simbAux ;
  switch (simbolo) {                       /* exit, halt, reboot, shutdown */
    case s_exit   : interpretar_exit() ; return ;           /* simb = exit */
    case s_halt   : ;
    case s_reboot : ;
    case s_shutdn :
      simbAux = simbolo ;
      if (getuid() == 0) {                                         /* root */
        saltarBlancos() ; obtenSimb() ;
        if (simb == s_fin) { num = 0 ; simb = /* simbAux */ s_shutdn ; return ; }
        if (simb == s_numero) { simb = /* simbAux */ s_shutdn ; return ; }
        escribirStr(" formato ") ;
        if (simbAux == s_halt) escribirStr("halt") ;
        else if (simbAux == s_reboot) escribirStr("reboot") ;
        else escribirStr("shutdown") ;
        escribirStr(" [num] ") ;
      }
      else {                        /* usuario intentado cerrar el sistema */
        escribirStr(
          "\a comando solo permitido para root "
      ) ;
    }
    default : ;
  }
  simb = s_nulo ;
}

/* --------- interpretacion del comando ver ------------------------------ */

void interpretar_ver ( void ) {
  obtenStr() ;
  escribirStr("\n\n SO1 [Version 1.7 (07-09-2017)] ") ;
  if ((str[0] == '-') && (str[1] == 'C') && (str[2] == (char)0)) {
    escribirStr("\n\n Compilacion: __TURBOC__ == 0x") ;
    escribirHex(__TURBOC__, 4) ;
    escribirStr(" " __DATE__ " " __TIME__) ;
  }
  escribirLn() ;
}

/* --------- interpretacion del comando ptos ----------------------------- */

void escribirPtos ( void ) {
  char carPtos ;
  char opcion = ' ' ;
//  int contCol ;
  obtenStr() ;
  carPtos = str[0] ;
  if ((carPtos <= ' ') || ('~' < carPtos))
    carPtos = '.' ;
  else if ((carPtos == '-') && (mayuscula(str[1]) == 'V')) {
    opcion = 'V' ;
    carPtos = '1' ;
  }
  escribirLn() ;
//  contCol = 1 ;
  while (leerAsciiListo(STDIN) == (char)0) {
    retardoActivo(0) ;
    escribirCar(carPtos) ;
    if (opcion == 'V') carPtos = '0' + ((carPtos - '0' + 1) % 10) ;
/*
    contCol++ ;
    if (contCol > (tamWindow() >> 8)) {
      contCol = 1 ;
      escribirLn() ;
    }
*/
  }
}

void interpretar_ptos ( void ) {
  int dfRet ;
  if ((dfRet = open("RETARDO", O_RDONLY)) >= 0) {
    close(dfRet) ;
    escribirPtos() ;
  }
  else
    escribirStr(" recurso RETARDO no disponible ") ;
}

/* --------- interpretacion del comando status --------------------------- */

void interpretar_status ( bool_t * mostrarStatus ) {
  obtenSimb() ;
  if (simb == s_fin) {
    if (*mostrarStatus) escribirStr(" (activado) ") ;
    else escribirStr(" (desactivado) ") ;
    simb = s_status ;
    return ;
  }
  if (simb == s_ident) {
    if (str[0] == 'O') {
      if ((str[1] == 'N') && (str[2] == (char)0)) {
        *mostrarStatus = TRUE ;
        escribirStr(" (activado) ") ;
        simb = s_status ;
        return ;
      }
      if ((str[1] == 'F') && (str[2] == 'F') && (str[3] == (char)0)) {
        *mostrarStatus = FALSE ;
        escribirStr(" (desactivado) ") ;
        simb = s_status ;
        return ;
      }
    }
  }
  escribirStr("\a formato: status [ on | off ] ") ;
  simb = s_nulo ;
}

/* --------- interpretacion del comando dir ------------------------------ */

void listarDirectorio ( byte_t unidadLogica, char opcion ) ;

void listarDirectorioHost ( char * camino, char opcion ) ;

void interpretar_dir ( bool_t host, int unidadLogica ) {
  char camino [ tamComando ] ; 
  bool_t esCmdDir = (mayuscula(str[0]) == 'D') ;
//escribirStr(" ** str = \"") ; escribirStr(str) ; escribirStr("\"") ;
  if (!host) {                                        /* (dir|ls) [/W|-l] */ 
    obtenStr() ;
	if (esCmdDir && (str[0] == '/') && (mayuscula(str[1]) == 'W') && (str[2] == (char)0))
      listarDirectorio(unidadLogica, 'W') ;
    else if (!esCmdDir && (str[0] == '-') && (mayuscula(str[1]) == 'L') && (str[2] == (char)0))
      listarDirectorio(unidadLogica, ' ') ;
    else {
	  if (esCmdDir) 
        listarDirectorio(unidadLogica, ' ') ;
      else 
        listarDirectorio(unidadLogica, 'W') ; 
	}
  }
  else {                             /* (dir|ls) [camino] [/w|-l] [camino] */
    obtenStr() ;
    copiarStr(str, camino) ;
	if (esCmdDir && (str[0] == '/') && (mayuscula(str[1]) == 'W') && (str[2] == (char)0)) {
	  obtenStr() ;
      listarDirectorioHost((char *)str, 'W') ;
	}
    else if (!esCmdDir && (str[0] == '-') && (mayuscula(str[1]) == 'L') && (str[2] == (char)0)) {
	  obtenStr() ;
      listarDirectorioHost((char *)str, ' ') ;
	}
    else {
	  obtenStr() ;
	  if (esCmdDir) {
	    if ((str[0] == '/') && (mayuscula(str[1]) == 'W') && (str[2] == (char)0)) 
          listarDirectorioHost((char *)camino, 'W') ;
	    else 
          listarDirectorioHost((char *)camino, ' ') ;
	  }
      else
        listarDirectorioHost((char *)camino, 'W') ;
    }
  }
}

/* --------- interpretacion del comando ret ------------------------------ */

void interpretar_ret ( void ) {
  int dfRet ;
  if ((dfRet = open("RETARDO", O_RDONLY)) >= 0) {
    close(dfRet) ;
    obtenSimb() ;
    if (simb == s_fin) {
      escribirStr(" valor de retardo activo en SO1 = ") ;
      escribirLDec(retardoActivo(1), 1) ;
      simb = s_ret ;
      return ;
    }
    if ((simb == s_numero) || (simb == s_numeroLargo))
      if (numLargo > 1) {
        retardoActivo(numLargo) ;
        simb = s_ret ;
        return ;
      }
      else
        escribirStr("\a el numero debe ser > 1") ;
    else
      escribirStr("\a formato ret [ num ] ") ;
  }
  else
    escribirStr("\a recurso RETARDO no disponible ") ;
  simb = s_nulo ;
}

/* --------- interpretacion del comando cd ------------------------------- */

void interpretar_cd ( bool_t host, int unidad, bool_t hayDOS ) {
  int i ;
  struct ffblk fcb ;
  if (!host) {
    escribirStr("\a comando no implementado en SO1 (standalone) ") ;
    simb = s_nulo ;
    return ;
  }
  if (hayDOS) {
    obtenStr() ;
    for ( i = 0 ; str[i] != (char)0 ; i++ ) ;
    if (str[i-1] == '*') {
      if (findfirstDOS((char *)str, (struct ffblk *)&fcb, FA_DIREC) == 0)
        chdirDOS((char *)fcb.ff_name) ;
    }
    else
      chdirDOS(str) ;
    cambiarPromptDOS(unidad) ;
  }
}

/* --------- interpretacion del comando id ------------------------------- */

void interpretar_id ( void ) {
  escribirStr(" uid = ") ;
  escribirDec(getuid(), 1) ;
  escribirStr(" gid = ") ;
  escribirDec(getgid(), 1) ;
  if (getppid() == 0)
    escribirStr(" INIT.BIN pid = ") ;
  else
    escribirStr(" SH.BIN pid = ") ;
  escribirDec(getpid(), 1) ;
  escribirStr(" (pindx = ") ;
  escribirDec(getpindx(), 1) ;
  escribirCar(')') ;
}

/* --------- interpretacion del comando host ----------------------------- */

void interpretar_host ( bool_t * host, int unidad, bool_t hayDOS ) {
  if (hayDOS) {
    *host = TRUE ;
/*  unidad = getdiskDOS() ; */
    cambiarPromptDOS(unidad) ;
  }
  else
    escribirStr("\a MSDOS no presente en la memoria") ;
}

/* --------- interpretacion del comando nohost --------------------------- */

void interpretar_nohost ( bool_t * host ) {
  copiarStr("SO1> ", prompt) ;
  *host = FALSE ;
}

/* --------- interpretacion del comando type ----------------------------- */

int mostrarFicheroDOS ( char * nombre, word_t nFilas ) ;

void interpretar_type ( bool_t host ) {
  if (host) {
    obtenStr() ;
    saltarBlancos() ;
    obtenSimb() ;
    if (simb == s_fin) {
/*    mostrarFicheroDOS(str, tamWindow() & 0x00FF) ; */
      mostrarFicheroDOS(str, 80) ;
      simb = s_type ;
      return ;
    }
    if (simb == s_numero) {
      mostrarFicheroDOS(str, num) ;
      simb = s_type ;
      return ;
    }
    escribirStr("\a formato type fichero [num] ") ;
  }
  else {
    escribirStr("\a comando no implementado en SO1 (standalone) ") ;
  }
  simb = s_nulo ;
}

/* --------- interpretacion del comando traza ---------------------------- */

void interpretar_traza ( void ) {
  int num1 ;
  obtenSimb() ;
  if (simb == s_numero) {
    num1 = num ;
    saltarBlancos() ;
    obtenSimb() ;
    if (simb == s_fin)                                   /* proceso actual */
      activarTraza(num1, getpid()) ;
    else if (simb == s_numero)                              /* proceso pid */
      if ((int)num >= 0)
        activarTraza(num1, (pid_t)num) ;
      else {
        escribirStr("\a numero demasiado grande") ;
        simb = s_nulo ;
        return ;
      }
    else if ((simb == s_ident) &&                    /* todos los procesos */
             (str[0] == 'A') && (str[1] == 'L') &&
             (str[2] == 'L') && (str[3] == (char)0))
      activarTraza(num1, (pid_t)(-1)) ;
    else {
      escribirStr("\a se esperaba un pid, nada o \"all\"") ;
      simb = s_nulo ;
      return ;
    }
  }
  else {
    escribirStr("\a se esperaba un numero") ;
    simb = s_nulo ;
    return ;
  }
  simb = s_traza ;
}

/* --------- interpretacion del comando ctty ----------------------------- */

void interpretar_ctty ( void ) {
int resultado ;
  obtenStr() ;
  resultado = redirigirSTDIO(str) ;
  switch (resultado) {
  case  0 : break ;
  case -1 : ;
  case -2 : ;
  case -3 : escribirStr("\a no ha podido cerrarse la ") ;
            escribirStr(strStd[-resultado-1]) ;
            escribirStr(" estandar ") ;
            simb = s_nulo ;
            break ;
  case -4 : ;
  case -5 : ;
  case -6 : ;
  case -7 : escribirStr("\a no ha podido abrirse \"") ;
            escribirStr(str) ;
            escribirStr("\"") ;
            if (resultado != -4) {
              escribirStr(" como ") ;
              escribirStr(strStd[-resultado-5]) ;
              escribirStr(" estandar ") ;
            }
            simb = s_nulo ;
            break ;
  default : escribirStr("\a formato: CTTY ( CONx | COMx ) ") ;
            simb = s_nulo ;
  }
}

/* ----------------------------------------------------------------------- */
/*    tratamiento de errores al crear un proceso hijo (comando externo)    */
/* ----------------------------------------------------------------------- */

void entreComillas ( char * nombre ) {
  escribirCar('\"') ;
  escribirStr(nombre) ;
  escribirCar('\"') ;
}

void tratarError ( pid_t pid, char * nombre ) {

  int error ;

  if (pid >= 0) {                                /* no habria ningun error */
    escribirStr(" se ha cargado con pid ") ;
    escribirDec(pid, 1) ;
  }
  else {                                                   /* si hay error */
    error = pid ;
    escribirCar('\a') ;
    switch(error) {
    case -1 :
      escribirStr(" nombre incorrecto para un fichero: ") ;
      entreComillas(nombre) ;
      break ;
    case -2 :
      escribirStr(" la extension debe ser .bin: ") ;
      entreComillas(nombre) ;
      break ;
    case -3 :
      escribirStr(" la unidad 0 no esta lista ") ;
      entreComillas(nombre) ;
      break ;
    case -4 :
      escribirStr(" unidad ") ;
      escribirCar(nombre[0]) ;
      escribirStr(" no disponible") ;
      break ;
    case -5 :
      escribirStr(" fichero no encontrado: ") ;
      entreComillas(nombre) ;
      break ;
    case -6 :
      escribirStr(" fichero demasiado grande: ") ;
      entreComillas(nombre) ;
      break ;
    case -7 :
      escribirStr(" tama¤o del proceso erroneo") ;
      break ;
    case -8 :
      escribirStr(" no hay un bloque libre de tama¤o suficiente") ;
      break ;
    case -9 :
      escribirStr(" no pudo cargarse el fichero: ") ;
      entreComillas(nombre) ;
      break ;
    case -10:
      escribirStr(" no pudo crearse el proceso") ;
      break ;
    }
  }
}

/* ----------------------------------------------------------------------- */
/* ----------------------------------------------------------------------- */
/*     bucle de lectura analisis e interpretacion de todos los comandos    */
/* ----------------------------------------------------------------------- */
/* ----------------------------------------------------------------------- */

int interpretarComandos ( void ) {

  pid_t pid ;       /* pid del proceso hijo que ejecuta el comando externo */
  int status ;                                    /* estado de terminacion */
  bool_t hayDOS ;                            /* existe un DOS previo a SO1 */
  bool_t host = FALSE ;              /* se utiliza un DOS host, o solo SO1 */
  bool_t mostrarStatus = FALSE ;        /* mostrar status devuelto por los */
                                    /* comandos al terminar (exit(status)) */
  int unidadLogica ;                            /* unidadLogica SO1 actual */
  int unidad, unidad0 ;                               /* unidad DOS actual */

  info_t info ;

  obtenInfoINFO((info_t far *)&info) ;
  hayDOS = (info.modoSO1 != modoSO1_Bin) ;            /* no so1.bin (boot) */

   /* en el siguiente if, hay que exigir hayMSDOS() ya que si so1.com se   */
   /* lanza con el gestor de arranque SYSLINUX no tiene porque haber MSDOS */

  if ((unidadLogica = getdisk()) < 0)
    if (hayDOS && hayMSDOS()) {
      host = TRUE ;
      unidad = getdiskDOS() ;
      cambiarPromptDOS(unidad) ;
    }

  do {

    escribirLn() ;
    escribirStr(prompt) ;                              /* SO1> o X:\...> " */

    leerComando(TRUE, FALSE) ;      /* el comando se lee en comando[inCmd] */
        /* proporciona historia de comandos (flecha ^) y compleccion (tab) */

    inicScanner() ;                   /* comienzo del analisis del comando */
    saltarBlancos() ;
    if (car == (char)0) simb = s_fin ;
    else if (car == '!') ;                             /* comando anterior */
    else if (car == ':') simb = s_nohost ;                    /* comando : */
    else if (('A' <= car) && (car <= 'Z')) {
      identificador() ;
//    escribirStr("\n str = \"") ; escribirStr(str) ; escribirStr("\"\n") ;	  
      if ((simb == s_ident) &&
          (str[1] == (char)0) && (car == ':')) {             /* comando X: */
        unidad0 = str[0] - 'A' ;
        setdiskDOS(unidad0) ;
        unidad = getdiskDOS() ;
        if (unidad == unidad0) {
          simb = s_host ;                         /* continua en el switch */
        }
        else {
          escribirStr(" unidad ") ;
          escribirCar('A' + unidad0) ;
          escribirStr(" no disponible") ;
          simb = s_nulo ;                         /* continua en el switch */
        }
      }
      else ;    /* posible comando interno/externo (continua en el switch) */
    }
    else if (('0' <= car) && (car <= '9')) {       /* comando externo 9... */
      obtenStr() ;
      simb = s_ident ;                            /* continua en el switch */
    }
    else simb = s_nulo ;

    switch (simb) {
    case s_nulo   : escribirStr("\a comando incorrecto ") ; break ;
    case s_fin    : break ;                                 /* linea vacia */
    case s_echo   : escribirLn() ; escribirStr(&comando[inCmd][5]) ; break ;
    case s_exit   : interpretar_salida(s_exit) ; break ;            /* num */
    case s_halt   : interpretar_salida(s_halt) ; break ;            /* num */
    case s_reboot : interpretar_salida(s_reboot) ; break ;          /* num */
    case s_shutdn : interpretar_salida(s_shutdn) ; break ;          /* num */
    case s_su     : setuid(0) ; setgid(0) ; break ;
    case s_cls    : escribirCar('\f') ; break ;
    case s_ver    : interpretar_ver() ; break ;
    case s_ptos   : interpretar_ptos() ; break ;
    case s_status : interpretar_status((bool_t *)&mostrarStatus) ; break ;
    case s_dir    : interpretar_dir(host, unidadLogica) ; break ;
    case s_ret    : interpretar_ret() ; break ;
    case s_cd     : interpretar_cd(host, unidad, hayDOS) ; break ;
    case s_id     : interpretar_id() ; break ;
    case s_host   : interpretar_host((bool_t *)&host, unidad, hayDOS) ; break ;
    case s_nohost : interpretar_nohost((bool_t *)&host) ; break ;
    case s_type   : interpretar_type(host) ; break ;
    case s_traza  : interpretar_traza() ; break ;
    case s_ctty   : interpretar_ctty() ; break ;

    default      :                             /* ejecutar comando externo */

      while ((car != '&') && (car != (char)0)) obtenCar() ;
      pid = createProcess(str, comando[inCmd]) ;
      if (pid > 0) {
        if (car != '&') {                                    /* foreground */
          waitpid(pid, (int far *)&status) ;
          if (mostrarStatus) {
            escribirStr(" status = ") ;
            escribirInt(status, 1) ;
          }
        }
        else                                                 /* background */
          waitpid(pid, (int far *)NULL) ;       /* no espera nada del hijo */
      }
      else {
//      escribirStr("\n error: str = ") ;
//      escribirStr((char *)str) ;
//      escribirStr("\n error: comando = ") ;
//      escribirStr((char *)comando[inCmd]) ;
//      escribirStr("\n ") ;
        tratarError(pid, str) ;
      }
    }                                                    /* fin del switch */

  } while (simb > s_exit) ;           /* simb != exit/halt/reboot/shutdown */

  escribirLn() ;

  return(num) ;                       /* exit/halt/reboot/shutdown ==> num */
}

/* ----------------------------------------------------------------------- */
/*                    resto de funciones auxiliares                        */
/* ----------------------------------------------------------------------- */

int controlarFinYFilas ( char opcion, word_t contFilas, word_t nFilas ) {
  char car ;
  if (contFilas++ == (nFilas-2)) {
    contFilas = 0 ;
    escribirStr("\n Presione cualquier tecla para continuar . . . ") ;
    car = leer(STDIN) ;
    escribirStr("\r                                               \r") ;
    if (car == (char)27)                      /* Salir con tecla Esc */
      return(0) ;
  }
  else if (opcion != 'W')
    escribirLn() ;
  return(1) ;
}

void mostrarEntradaOpcionW ( ffblk_t * ffblk ) {

  char nombre [14] ;
  char car ;
  int i, j ;

  escribirCar(' ') ;
  i = 0 ;
  if (ffblk->ff_attrib & FA_DIREC)
    nombre[i++] = '[' ;
  j = 0 ;
  while ((car = ffblk->ff_name[j++]) != (char)0) {
    nombre[i++] = car ;
  }
  if (ffblk->ff_attrib & FA_DIREC)
    nombre[i++] = ']' ;
  nombre[i] = (char)0 ;
  escribirStrHasta((char *)nombre, 12, TRUE) ;

}

void mostrarEntradaOpcionL ( ffblk_t * ffblk ) {

  unsigned int dia, mes, anio ;
  unsigned int segs, mins, horas ;
  unsigned int unidades ;
  unsigned long int miles ;
  int i, j, k ;

  escribirCar(' ') ;
  if (ffblk->ff_name[0] == '.') {
    if (ffblk->ff_name[1] == (char)0)
      escribirStr(".            ") ;
    else if ((ffblk->ff_name[1] == '.') &&
             (ffblk->ff_name[2] == (char)0))
      escribirStr("..           ") ;
  }
  else {
    for ( i = 0 ; (ffblk->ff_name[i] != '.') &&
                  (ffblk->ff_name[i] != (char)0) ; i++ )
      escribirCar(ffblk->ff_name[i]) ;
    if (ffblk->ff_name[i] == (char)0)
      for ( j = i ; j <= 12 ; j++ )
        escribirCar(' ') ;
    else {
      for ( j = i ; j <= 8 ; j++ )
        escribirCar(' ') ;
      i++ ;
      for ( j = i ; ffblk->ff_name[j] != (char)0 ; j++ )
        escribirCar(ffblk->ff_name[j]) ;
      for ( k = 0 ; k <= (3 - (j - i)) ; k++ )
        escribirCar(' ') ;
    }
  }

  if (ffblk->ff_attrib & FA_DIREC)
    escribirStr("   <DIR>      ") ;
  else {
    miles = ffblk->ff_fsize / 1000 ;
    unidades = (unsigned int)(ffblk->ff_fsize % 1000) ;
    if (miles > 0) {
      escribirLDec(miles, 10) ;
      escribirCar('.') ;
      if (unidades < 10) escribirStr("00") ;
      else if (unidades < 100) escribirStr("0") ;
      escribirDec(unidades, 1) ;
    }
    else escribirDec(unidades, 14) ;
  }
  escribirStr("  ") ;

  dia = ffblk->ff_fdate & 0x001F ;
  if (dia < 10) escribirCar('0') ;
  escribirDec(dia, 1) ;
  escribirCar('/') ;
  mes = (ffblk->ff_fdate >> 5) & 0x000F ;
  if (mes < 10) escribirCar('0') ;
  escribirDec(mes, 1) ;
  escribirCar('/') ;
  anio = 1980 + (ffblk->ff_fdate >> 9) ;
  escribirDec(anio, 1) ;

  escribirStr("  ") ;
  horas = (ffblk->ff_ftime >> 11) & 0x001F ;
  if (horas < 10) escribirCar('0') ;
  escribirDec(horas, 1) ;
  escribirCar(':') ;
  mins = (ffblk->ff_ftime >> 5) & 0x003F ;
  if (mins < 10) escribirCar('0') ;
  escribirDec(mins, 1) ;

  escribirCar(':') ;
  segs = 2*(ffblk->ff_ftime & 0x001F) ;
  if (segs < 10) escribirCar('0') ;
  escribirDec(segs, 1) ;

}

void listarDirectorioHost ( char * camino, char opcion ) {

  int atr ;
  struct ffblk fcb ;
  char fspath[MAXPATH] ;
  int i ;
  int nFilas = 80 ;
  int contFilas = 0 ;
  int contFicheros = 0 ;

  /* nFilas = (tamWindow() & 0x00FF) ; */

  atr = FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_LABEL | FA_DIREC | FA_ARCH ;

  if (camino[0] == (char)0) copiarStr("*.*", fspath) ;
  else if (iguales(camino, ".")) copiarStr("*.*", fspath) ;
  else if (iguales(camino, "..")) copiarStr("..\\*.*", fspath) ;
  else {
    for ( i = 0 ; camino[i] != (char)0 ; i++ ) ;
    switch (camino[i-1]) {
      case '.'  : if (camino[i-2] == '.')
                    camino[i++] = '\\' ;
      case '\\' : camino[i++] = '*' ;
                  camino[i++] = '.' ;
                  camino[i++] = '*' ;
                  camino[i++] = (char)0 ;
    }
    copiarStr(camino, fspath) ;
  }

  if (findfirstDOS((char *)&fspath, (struct ffblk *)&fcb, atr) == -1) return ;

  if (fcb.ff_attrib & FA_DIREC) {
    for ( i = 0 ; fspath[i] != (char)0 ; i++ )
      if ((fspath[i] == '*') || (fspath[i] == '?'))
        break ;
    if (fspath[i] == (char)0) {
      fspath[i++] = '\\' ;
      fspath[i++] = '*' ;
      fspath[i++] = '.' ;
      fspath[i++] = '*' ;
      findfirstDOS((char *)&fspath, (struct ffblk *)&fcb, atr) ;
    }
  }

  escribirLn() ;
  escribirLn() ;

  if (opcion == 'W') {
    do {
      mostrarEntradaOpcionW((ffblk_t *)&fcb.ff_attrib) ;
      if ((++contFicheros % 6) == 0) {
        escribirLn() ;
        if (controlarFinYFilas(opcion, contFilas, nFilas) == 0) break ;
      }
    }
    while (findnextDOS((struct ffblk *)&fcb) == 0) ;
    escribirLn() ;
    return ;
  }

  do {
    mostrarEntradaOpcionL((ffblk_t *)&fcb.ff_attrib) ;
    if (controlarFinYFilas(opcion, contFilas, nFilas) == 0) break ;
  }
  while (findnextDOS((struct ffblk *)&fcb) == 0) ;

}

int mostrarFicheroDOS ( char * nombre, word_t nFilas ) {

  int df, n, i ;
  char buf [512] ;                        /* openDOS((pointer_t)nombre, 0) */
  int unidad, unidad0, unidad1 ;
  int contFilas = 0 ;
  char car ;
  unidad = getdiskDOS() ;
  if (('A' < nombre[0]) && (nombre[0] <= 'Z') && (nombre[1] == ':')) {
    unidad0 = nombre[0] - 'A' ;
    setdiskDOS(unidad0) ;
    unidad1 = getdiskDOS() ;
    setdiskDOS(unidad) ;
    if (unidad1 != unidad0) {
      escribirStr(" unidad ") ;
      escribirCar(nombre[0]) ;
      escribirStr(" no disponible") ;
      return(-1) ;
    }
  }

  df = open((char far *)nombre, O_RDONLY) ;
//escribirStr("\n df = ") ; escribirInt(df, 1) ;
//df = extendedOpenDOS((pointer_t)nombre, 0x2000, 0, 1) ;
  if (df > 0) {
    escribirLn() ;
//  while ((n = readDOS(df, (char *)&buf, 512)) > 0) */
    while ((n = read(df, (pointer_t)&buf, 512)) > 0)
      for ( i = 0 ; i < n ; i++ ) {
        if (buf[i] != '\n')
          escribirCar(buf[i]) ;
        else {
          if (contFilas++ == (nFilas-2)) {
            contFilas = 0 ;
            escribirStr("\n Presione cualquier tecla para continuar . . . ") ;
            car = leer(STDIN) ;
            escribirStr("\r                                               \r") ;
            if (car == ESC) {                        /* Salir con tecla Esc */
//              closeDOS(df) ; */
              close(df) ;
              return(0) ;
            }
          }
          else
            escribirLn() ;
        }
      }
    close(df) ;
//  closeDOS(df) ;
    return(0) ;
  }
  else {
    escribirStr(" no pudo abrirse el fichero ") ;
    escribirCar('\"') ;
    escribirStr(nombre) ;
    escribirCar('\"') ;
    return(-1) ;
  }
}

void listarDirectorio ( byte_t unidadLogica, char opcion ) {

  ffblk_t ffblk ;
  int nFilas = 80 ;
  int contFilas = 0 ;
  int contFicheros = 0 ;
  entrada_t entrada [16] ;
  ffblk.buffer = (pointer_t)&entrada ;

  escribirLn() ;
  escribirLn() ;

  if (findFirst(unidadLogica, (ffblk_t *)&ffblk) == 0) {     /* inicializa */
    do {                                  /* campos: unidad, bindx y eindx */
      if (opcion == 'W') {
        mostrarEntradaOpcionW((ffblk_t *)&ffblk) ;
        if ((++contFicheros % 6) == 0) {
          escribirLn() ;
          if (controlarFinYFilas(opcion, contFilas, nFilas) == 0) break ;
        }
      }
      else {
        mostrarEntradaOpcionL((ffblk_t *)&ffblk) ;
        if (controlarFinYFilas(opcion, contFilas, nFilas) == 0) break ;
      }
    }
    while (findNext((ffblk_t *)&ffblk) == 0) ;
    if (opcion == 'W')
      escribirLn() ;
  }
}
