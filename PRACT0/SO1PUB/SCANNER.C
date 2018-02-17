/* ----------------------------------------------------------------------- */
/*                                scanner.c                                */
/* ----------------------------------------------------------------------- */
/*  funciones para el analisis de los comandos del interprete de comandos  */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\caracter.h>                                /* mayuscula */
#include <so1pub.h\strings.h>             /* iguales(SalvoMayusculas)Hasta */
#include <so1pub.h\scanner.h>       /* nMaxComandos, tamComando, simb, ... */

//#include <so1pub.h\escribir.h>               /* escribirCar, escribirStr */

char comando [ nMaxComandos ][ tamComando ] ;      /* historia de comandos */
                                                          /* cola circular */
int inCmd = 0 ;                      /* indice de la primera entrada libre */
int outCmd = 1 ;                   /* indice de la primera entrada ocupada */
int nComandos = 0 ;                       /* numero de comandos en la cola */

char car ;                   /* ultimo caracter leido en el comando actual */

word_t pos ;                       /* posision de car en el comando actual */

simb_t simb ;                 /* ultimo simbolo leido en el comando actual */

word_t num ;                              /* valor del ultimo numero leido */

dword_t numLargo ;                        /* valor del ultimo numero leido */

char str [ tamComando ] ;                           /* ultimo string leido */

regSimbCmd_t simbCmd [ ] = {         /* tabla de símbolos de los comandos */
  { "CD"       , 2, s_cd     },
  { "ID"       , 2, s_id     },
  { "LS"       , 2, s_dir    },
  { "SU"       , 2, s_su     },
  { "CLS"      , 3, s_cls    },
  { "DIR"      , 3, s_dir    },
  { "RET"      , 3, s_ret    },
  { "VER"      , 3, s_ver    },
  { "CTTY"     , 4, s_ctty   },
  { "ECHO"     , 4, s_echo   },
  { "EXIT"     , 4, s_exit   },
  { "HALT"     , 4, s_halt   },
  { "HOST"     , 4, s_host   },
  { "PTOS"     , 4, s_ptos   },
  { "TYPE"     , 4, s_type   },
  { "CLEAR"    , 5, s_cls    },
  { "TRAZA"    , 5, s_traza  },
  { "NOHOST"   , 6, s_nohost },
  { "STATUS"   , 6, s_status },
  { "REBOOT"   , 6, s_reboot },
  { "SHUTDOWN" , 8, s_shutdn }
} ;

int numCmds = (sizeof(simbCmd)/sizeof(regSimbCmd_t)) ;

void inicScanner ( void ) {             /* prepara el analisis del comando */
  pos = 0 ;
  car = ' ' ;
}

void obtenCar ( void ) {       /* obtiene el siguiente caracter a analizar */
  if ((pos < tamComando) && (car != (char)0))
    car = mayuscula(comando[inCmd][pos++]) ;
  else
    car = (char)0 ;
//escribirLn() ;
//escribirCar(car) ;
}

void saltarBlancos ( void ) {           /* saltamos los siguientes blancos */
  while (car == ' ') obtenCar() ;
}

/* --- numDec analiza y obtiene el valor del siguiente numero decimal ---- */

#define dwordMax    (dword_t)0xFFFFFFFFL                     /* 4294976295 */
#define umbralSup   (dword_t)(0xFFFFFFFFL/10L)               /* 429497629  */
#define umbralSup10 ((dword_t)(10L*(0xFFFFFFFFL/10L)))       /* 4294976290 */

void numDec ( void ) {                /* analizar siguiente numero decimal */

  dword_t acum = (car - '0') ;

#if (FALSE)                                             /* para depuracion */
  escribirStr("\n dwordMax    = ") ;
  escribirLDec(dwordMax, 1) ;
  escribirStr("\n umbralSup   = ") ;
  escribirLDec(umbralSup, 1) ;
  escribirStr("\n umbralSup10 = ") ;
  escribirLDec(umbralSup10, 1) ;
  escribirStr("\n") ;
#endif

  obtenCar() ;
  while (('0' <= car) && (car <= '9')) {
    if (acum < umbralSup) {       /* escribirCar('<') ; escribirCar(car) ; */
      acum = 10*acum + (car - '0') ;
      obtenCar() ;
    }
    else if (acum == umbralSup) { /* escribirCar('=') ; escribirCar(car) ; */
      if ((car - '0') <= 5 ) {
        acum = umbralSup10 + (car - '0') ;
        obtenCar() ;
      }
      else {
        simb = s_nulo ;
        return ;
      }
    }
    else {                        /* escribirCar('>') ; escribirCar(car) ; */
      simb = s_nulo ;
      return ;
    }
  }
  numLargo = acum ;
  if (acum < 0x10000L) {
    simb = s_numero ;
    num = (word_t)acum ;
  }
  else
    simb = s_numeroLargo ;
}

/* - numHex analiza y obtiene el valor del siguiente numero hexadecimal -- */

void numHex ( void ) {            /* analizar siguiente numero hexadecimal */
  dword_t acum ;
  if (('0' <= car) && (car <= '9'))
    acum = (car - '0') ;
  else
    acum = 10 + (car - 'A') ;
  obtenCar() ;
  while ((('0' <= car) && (car <= '9')) || (('A' <= car) && (car <= 'F'))) {
    if (acum >= 0x10000000L) {
      simb = s_nulo ;
      return ;
    }
    if (('0' <= car) && (car <= '9'))
      acum = (acum << 4) + (car - '0') ;
    else
      acum = (acum << 4) + 10 + (car - 'A') ;
    obtenCar() ;
  }
  numLargo = acum ;
  if (acum < 0x10000L) {
    simb = s_numero ;
    num = (word_t)acum ;
  }
  else
    simb = s_numeroLargo ;
}

/* ----- identificador analiza y obtiene el siguiente identificador ------ */

void identificador ( void ) {          /* analizar siguiente identificador */
  int pos0, i, j ;
  char cmdEnMayusculas [tamComando] ;

  pos0 = pos -1 ;
  cmdEnMayusculas[pos0] = car ;
  while ((('A' <= car) && (car <= 'Z')) ||
         (('0' <= car) && (car <= '9')) || (car == '_') || (car == '-')) {
    obtenCar() ;
    cmdEnMayusculas[pos-1] = car ;
  }
  for ( i = 0 ; i < numCmds ; i++ ) {
    if ((pos - pos0 -1 == simbCmd[i].largo) &&
        (igualesHasta(simbCmd[i].str,
                      &cmdEnMayusculas[pos0],
                      simbCmd[i].largo))) {
      simb = simbCmd[i].simb ;
      j = 0 ;
      for ( i = pos0 ; i < pos-1 ; i++ )
        str[j++] = mayuscula(comando[inCmd][i]) ;
      str[j] = (char)0 ;
      return ;
    }
  }
  if (car == '.') {
    obtenCar() ;
    while ((('A' <= car) && (car <= 'Z')) ||
           (('0' <= car) && (car <= '9')) || (car == '_'))
      obtenCar() ;
  }
  j = 0 ;
  for ( i = pos0 ; i < pos-1 ; i++ )
    str[j++] = mayuscula(comando[inCmd][i]) ;
  str[j] = (char)0 ;
  simb = s_ident ;
//escribirStr(" ident = \"") ; escribirStr(str) ; escribirStr("\"\n") ;
}

/* ---------- obtenSimb analiza y obtiene el siguiente simbolo ----------- */

void obtenSimb ( void ) {                  /* obtener el siguiente símbolo */
  saltarBlancos() ;
  if (('A' <= car) && (car <= 'Z')) identificador() ;
  else if (('0' <= car) && (car <= '9')) numDec() ;
  else {
    switch (car) {
    case '>'     : simb = s_mayor ; break ;
    case '<'     : simb = s_menor ; break ;
    case '|'     : simb = s_pipe ; break ;
    case '&'     : simb = s_ampersand ; break ;
    case (char)0 : simb = s_fin ; break ;
    default      : simb = s_nulo ;
    }
  }
//printStr("\n simbolo = ") ; printDec(simb, 1) ; printLn() ;
}

/* ----------- obtenStr analiza y obtiene el siguiente string ------------ */

void obtenStr ( void ) {                    /* obtiene el siguiente string */
  int i = 0 ;
  saltarBlancos() ;
  do {
    str[i++] = car ;
    obtenCar() ;
  } while ((car != ' ') && (car != (char)0)) ;
  str[i] = (char)0 ;
  /* printStr("\n str = ") ; printStr(str) ; printLn() ; */
}

