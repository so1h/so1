/* ----------------------------------------------------------------------- */
/*                                 conrat.c                                */
/* ----------------------------------------------------------------------- */
/* version del recurso consola (teclado+pantalla+raton)  [con multiscreen] */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\comundrv.h>                               /* dirDescSO1 */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>

#include <so1pub.h\carsctrl.h>                               /* CR, LF, FF */
#include <so1pub.h\caracter.h>                           /* dig, mayuscula */
#include <so1pub.h\strings.h>                                   /* iguales */
#include <so1pub.h\scanner.h>
#include <so1pub.h\biosdata.h>                              /* ptrBiosArea */
#include <so1pub.h\bios_0.h>
#include <so1pub.h\bios_crt.h>
#include <so1pub.h\copia.h>                                       /* copia */
#include <so1pub.h\pantalla.h>                    /* maxFilas, maxColumnas */
#include <so1pub.h\crthw.h>
#include <so1pub.h\memvideo.h>                             /* printCarBIOS */
#include <so1pub.h\def_tecl.h>                                /* teclado_t */
#include <so1pub.h\pic.h>
#include <so1pub.h\def_timer.h>                            /* argCbTimer_t */
#include <so1pub.h\printgen.h>

#include <so1pub.h\msdos.h>
#include <so1pub.h\bios_rat.h>
#include <so1.h\plot.h>
#include <so1.h\interrup.h>

/*
#include <so1pub.h\puertos.h>
#include <so1.h\so1.h>
#include <so1.h\bios.h>
#include <so1.h\dokill.h>
*/

info_t info ;                                                /* depuracion *//**************/

static rindx_t rec_consola ;

static rindx_t rec_raton ;

/* ----------------------------------------------------------------------- */
/*                   seccion de implementacion del driver                  */
/* ----------------------------------------------------------------------- */

#define nVIntConsola 0x09     /* numero de vector utilizado por el teclado */

#define nVIntRaton 0x74          /* obtenido experimentalmente MSD + DEBUG */

/* ------------------------------- consola ------------------------------- */

typedef struct {
  pantalla_t pantalla ;
  teclado_t  teclado ;
  byte_t     F ;                         /* fila de la posicion del cursor */
  byte_t     C ;                      /* columna de la posicion del cursor */
} descConsola_t ;

descConsola_t far * descConsola ;

#define numConsolasPorDefecto 6                       /* valor por defecto */

byte_t maxConsolas ;

word_t consolaDeSuperficie ;

word_t nbytesProceso [ maxProcesos ] ;        /* tabla de bytes pendientes */

pointer_t dirProceso [ maxProcesos ] ;     /* tabla de direcciones destino */

void meter ( char car, teclado_t far * teclado ) {
  int in = teclado->in ;
  if (teclado->ncar < tamBTeclado) {
    teclado->bufer[in++] = car ;
    if (in >= tamBTeclado) in = 0 ;
    teclado->in = in ;
    teclado->ncar++ ;
  }
}

char sacar ( teclado_t far * teclado  ) {   /* se supone: teclado.ncar > 0 */
  char car ;
  int out = teclado->out ;
  car = teclado->bufer[out++] ;
  if (out >= tamBTeclado) out = 0 ;
  teclado->out = out ;
  teclado->ncar-- ;
  return(car) ;
}

int printCarConsola ( byte_t con, char car ) {
  if (con == consolaDeSuperficie)
    printCarVideo(car) ;
  else {
    pantalla_t far * pantalla = (pantalla_t far *)&descConsola[con].pantalla ;
    byte_t F = descConsola[con].F ;
    byte_t C = descConsola[con].C ;
    switch (car) {
    case FF  : borrarCPantalla(pantalla, numFilas) ;
               /* borrarPantalla(pantalla, numFilas) ; */
               descConsola[con].F = 0 ;
               descConsola[con].C = 0 ;
               break ;
    case CR  : descConsola[con].C = 0 ;
               break ;
    case LF  : if (++descConsola[con].F == numFilas) {
                 scrollCPantalla(pantalla, numFilas) ;
                 descConsola[con].F = numFilas-1 ;
               }
               break ;
    case BS  : if (C > 0) {
                 C-- ;
                 pantalla->t[F][C].car = ' ' ;
                 descConsola[con].C = C ;
               }
               else printCarBIOS(BEL) ;
               break ;
    case HT  : car = ' ' ;
    case BEL : printCarBIOS(car) ;
               break ;
    default  :
      pantalla->t[F][C].car = car ;
      if (++descConsola[con].C == 80) {
        descConsola[con].C = 0 ;
        if (++descConsola[con].F == numFilas) {
          scrollCPantalla(pantalla, numFilas) ;
          descConsola[con].F = numFilas-1 ;
        }
      }
    }
  }
  return(0) ;
}

void goToXYConsola ( byte_t con, byte_t F, byte_t C ) {
  if (F >= numFilas) return ;
  if (C >= 80) return ;
  if (con == consolaDeSuperficie) {
/*    goToXYHw(F, C) ; */
    cursorF = F ;
    cursorC = C ;
  }
  else {
    descConsola[con].F = F ;
    descConsola[con].C = C ;
  }
}

void establecerConsolaDeSuperficie ( void ) {
  pantalla_t far * pantalla =
    (pantalla_t far *)&descConsola[consolaDeSuperficie].pantalla ;
  copiarCPantalla(pantalla, ptrPant, numFilas) ;
  /* copiarPantalla(pantalla, ptrPant, numFilas) ; */
  goToXYHw(
    descConsola[consolaDeSuperficie].F,
    descConsola[consolaDeSuperficie].C
  ) ;
  cursorF = descConsola[consolaDeSuperficie].F ;
  cursorC = descConsola[consolaDeSuperficie].C ;
}

void salvarConsolaDeSuperficie ( void ) {
  pantalla_t far * pantalla =
    (pantalla_t far *)&descConsola[consolaDeSuperficie].pantalla ;
  copiarCPantalla(ptrPant, pantalla, numFilas) ;
  /* copiarPantalla(ptrPant, pantalla, numFilas) ; */
  descConsola[consolaDeSuperficie].F = cursorF ;
  descConsola[consolaDeSuperficie].C = cursorC ;
}

/* -------------------------------- raton -------------------------------- */

c2c_t bloqueadosRaton ;

dobleEnlace_t e2BloqueadosRaton ;

word_t nbytesProceso [ maxProcesos ] ;        /* tabla de bytes pendientes */

pointer_t dirProceso [ maxProcesos ] ;     /* tabla de direcciones destino */

/* ----------------------------------------------------------------------- */
/*          variables globales que controlan el estado del raton           */
/* ----------------------------------------------------------------------- */

static int inB = 0 ;                       /* indice de entrada al bufer B */
static byte_t B[3] = { 0x00, 0x00, 0x00 } ;

/* no cambiar el orden en las siguientes lineas (ver leerEstadoRaton)      */

static estadoRaton_t er = {
  /* S0            */ 0x00,                           /* byte estado 8042  */
  /* B0            */ 0x00,                           /* Byte 0 = 00SS1MRL */
  /* B1            */ 0x00,                           /* Byte 1 = X        */
  /* B2            */ 0x00,                           /* Byte 2 = Y        */
  /* W0            */ 0x0000,
  /* intX          */ 0,
  /* intY          */ 0,
  /* X             */ 0,
  /* Y             */ 0,
  /* F             */ 0,
  /* C             */ 0,
  /* botonIz       */ (int)FALSE,
  /* botonDe       */ (int)FALSE,
  /* botonMe       */ (int)FALSE,
  /* botonIzAnt    */ (int)FALSE,
  /* botonDeAnt    */ (int)FALSE,
  /* botonMeAnt    */ (int)FALSE,
  /* visible       */ FALSE,
  /* forzarLectura */ FALSE
} ;

static int XAux = 0 ;
static int YAux = 0 ;
static int FAntCursor ;
static int CAntCursor ;

static byte_t atrAux ;

int cursorRatonActivo = TRUE ;

static int moviendo = FALSE ;
static int redimensionando = FALSE ;

int primerClick = TRUE ;

int contTics2Click = 0 ;

static bool_t huboInt = TRUE ;     /* hubo int. del raton sin leer todavia */

static int primeraInt = TRUE ;  /* la siguiente interrupcion es la primera */

static void inicEstadoRaton ( void ) {

  er.S0 = 0x00 ;
  er.B0 = 0x00 ;
  er.B1 = 0x00 ;
  er.B2 = 0x00 ;
  er.W0 = 0x0000 ;
  er.X = maxX/2 ;
/* er.Y = maxY/2 ; */
  er.Y = (ptrBiosArea->VIDEO_lastrow + 1)/2 ;
  er.F = er.Y >> 3 ;                                            /* F = Y/8 */
  er.C = er.X >> 3 ;                                            /* C = X/8 */
  er.botonIz = FALSE ;
  er.botonDe = FALSE ;
  er.botonMe = FALSE ;

  primeraInt = TRUE ;
  cursorRatonActivo = TRUE ;
  primerClick = TRUE ;
  contTics2Click = 0 ;

}

bool_t leerEstadoRaton ( estadoRaton_t far * estado ) {

  copia((pointer_t)&er, (pointer_t)estado, sizeof(estadoRaton_t)) ;
  if (huboInt) {
    huboInt = FALSE ;
    return(TRUE) ;
  }
  else
    return(FALSE) ;
}

/* ------------------------------- consola ------------------------------- */

#pragma warn -par

static int far openConsola ( int dfs, modoAp_t modo ) {
  return(0) ;
}

static int far releaseConsola ( int dfs ) {
  return(0) ;
}

static int far readConsola ( int dfs, pointer_t dir, word_t nbytes ) {

  word_t DS_Consola = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
  pindx_t indProcesoActual ;
  teclado_t far * teclado ;
  modoAp_t modoAp ;
  word_t nbytes0 ;
  int df ;
  int i = 0 ;
  char car ;
  word_t con ;

  asm push ds
  asm mov ds,DS_Consola

  indProcesoActual = *ptrIndProcesoActual ;
  df = (*ptrTramaProceso)->BX ;
  con = ptrDescFichero[dfs].menor ;
  teclado = (teclado_t far *)&descConsola[con].teclado ;
  modoAp = ptrDescProceso[indProcesoActual].tfa[df].modoAp ;

  nbytes0 = nbytes ;
  if (nbytes <= teclado->ncar) {
    while (nbytes > 0) {
      car = sacar(teclado) ;
      if ((car == CR) && (modoAp & O_TEXT)) car = LF ;
      dir[i++] = car ;
      nbytes-- ;
    }
    asm pop ds                    /* 2013-07-09: faltaba esto (sino CRASH) */
    return(nbytes0) ;
  }
  else {
    while (teclado->ncar > 0) {
      car = sacar(teclado) ;
      if ((car == CR) && (modoAp & O_TEXT)) car = LF ;
      dir[i++] = car ;
      nbytes-- ;
    }
    nbytesProceso[indProcesoActual] = nbytes ;
    dirProceso[indProcesoActual] = (pointer_t)&dir[i] ;
    encolarPC2c(indProcesoActual, &(teclado->bloqueados)) ;
    bloquearProcesoActual(rec_consola) ;          /* no se retorna de aqui */
  }

  asm pop ds
  return(-1) ;
}

static int far aio_readConsola ( int dfs, pointer_t dir, word_t nbytes ) {
  word_t DS_Consola = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
  pindx_t indProcesoActual ;
  teclado_t far * teclado ;
  modoAp_t modoAp ;
  word_t nbARetornar0, nbARetornar ;
  word_t pos ;
  int df ;
  int i = 0 ;
  char car ;
  word_t con ;

  asm push ds
  asm mov ds,DS_Consola

  indProcesoActual = *ptrIndProcesoActual ;
  df = (*ptrTramaProceso)->BX ;
  con = ptrDescFichero[dfs].menor ;
  teclado = (teclado_t far *)&descConsola[con].teclado ;
  modoAp = ptrDescProceso[indProcesoActual].tfa[df].modoAp ;

  if (nbytes <= teclado->ncar)
    nbARetornar = nbytes ;
  else
    nbARetornar = teclado->ncar ;
  nbARetornar0 = nbARetornar ;
  while (nbARetornar > 0) {
    car = sacar(teclado) ;
    if ((car == CR) && (modoAp & O_TEXT)) car = LF ;
    dir[i++] = car ;
    nbARetornar-- ;
  }

  asm pop ds
  return(nbARetornar0) ;
}

static int far writeConsola ( int dfs, pointer_t dir, word_t nbytes ) {
  word_t DS_Consola = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
  int i ;
  char car ;
  word_t con ;

  asm push ds
  asm mov ds,DS_Consola

  con = ptrDescFichero[dfs].menor ;
  for ( i = 0 ; i < nbytes ; i++ ) {
    car = dir[i] ;
    printCarConsola(con, car) ;
  }
  if (con == consolaDeSuperficie)
    goToXYHw(cursorF, cursorC) ;

  asm pop ds
  return(nbytes) ;
}

static int far aio_writeConsola ( int dfs, pointer_t dir, word_t nbytes ) {
  return(writeConsola(dfs, dir, nbytes)) ;
}

static long int far lseekConsola ( int dfs, long int pos, word_t whence ) {

/* posisicionar el cursor */

  return((long int)-1) ;
}

static int far fcntlConsola ( int dfs, word_t cmd, word_t arg ) {
  return(0) ;
}

static int far ioctlConsola ( int dfs, word_t request, word_t arg ) {
  return(0) ;
}

static int far eliminarConsola ( pindx_t pindx ) {
  return(0) ;
}

#pragma warn +par

/* ------------------------------- consola ------------------------------- */

#pragma warn -par

static int far openRaton ( int dfs, modoAp_t modo ) {
  return(0) ;
}

static int far releaseRaton ( int dfs ) {
  return(0) ;
}

static int far readRaton ( int dfs, pointer_t dir, word_t nbytes ) {

  word_t DS_Raton = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
  word_t nbARetornar ;
  pindx_t indProcesoActual ;

  asm push ds
  asm mov ds,DS_Raton

  indProcesoActual = *ptrIndProcesoActual ;
  if (nbytes <= sizeof(estadoRaton_t))
    nbARetornar = nbytes ;
  else
    nbARetornar = sizeof(estadoRaton_t) ;
  copia((pointer_t)&er, dir, nbARetornar) ;

  nbytesProceso[indProcesoActual] = nbARetornar ; /* nbytes esperando leer */
  dirProceso[indProcesoActual] = dir ;
  encolarPC2c(indProcesoActual, &bloqueadosRaton) ;
  bloquearProcesoActual(rec_raton) ;

  asm pop ds
  return(-1) ;

}

static int far aio_readRaton ( int dfs, pointer_t dir, word_t nbytes ) {

  word_t DS_Raton = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
  word_t nbARetornar ;

  asm push ds
  asm mov ds,DS_Raton

  if (nbytes <= sizeof(estadoRaton_t))
    nbARetornar = nbytes ;
  else
    nbARetornar = sizeof(estadoRaton_t) ;
  copia((pointer_t)&er, dir, nbARetornar) ;

  asm pop ds
  return(nbARetornar) ;

}

static int far writeRaton ( int dfs, pointer_t dir, word_t nbytes ) {
  return(0) ;
}

static int far aio_writeRaton ( int dfs, pointer_t dir, word_t nbytes ) {
  return(0) ;
}

static long int far lseekRaton ( int dfs, long int pos, word_t whence ) {
  return((long int)-1) ;
}

static int far fcntlRaton ( int dfs, word_t cmd, word_t arg ) {
  return(0) ;
}

static int far ioctlRaton ( int dfs, word_t request, word_t arg ) {
  return(0) ;
}

static int far eliminarRaton ( pindx_t pindx ) {
  teclado_t far * teclado ;
  int i ;
  for ( i = 0 ; i < maxConsolas ; i++ ) {
    teclado = (teclado_t far *)&descConsola[consolaDeSuperficie].teclado ;
    if estaPC2c(pindx, &teclado->bloqueados) {
      eliminarPC2c(pindx, &teclado->bloqueados) ;
      break ;
    }
  }
  return(0) ;
}

#pragma warn +par

/* ----------------------------------------------------------------------- */
/*            rutina de servicio de la interrupcion del teclado            */
/* ----------------------------------------------------------------------- */

#define CSAlt     (byte_t)0x38

#define AltBSlash (byte_t)0x29

#define AltF1     (byte_t)0x3B
#define AltF2     (byte_t)0x3C
#define AltF3     (byte_t)0x3D
#define AltF4     (byte_t)0x3E
#define AltF5     (byte_t)0x3F
#define AltF6     (byte_t)0x40
#define AltF7     (byte_t)0x41
#define AltF8     (byte_t)0x42

#define AltF9     (byte_t)0x43
#define AltF10    (byte_t)0x44

#define AltF11    (byte_t)0x57
#define AltF12    (byte_t)0x58

/*
#define AltF1     (byte_t)0x68
#define AltF2     (byte_t)0x69
#define AltF3     (byte_t)0x6a
#define AltF4     (byte_t)0x6b
#define AltF5     (byte_t)0x6c
#define AltF6     (byte_t)0x6d
#define AltF7     (byte_t)0x6e
#define AltF8     (byte_t)0x6f

#define AltF9     (byte_t)0x70
#define AltF10    (byte_t)0x71
*/

static byte_t scanCode = 0x00 ;
static byte_t scanCodeAnt = 0x00 ;
static byte_t AltPulsada = FALSE ;

void far isr_consola ( void ) {

  word_t DS_Consola = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
  word_t w ;
  char car ;
  word_t nbytes ;
  pointer_t dir ;
  int df ;
  pindx_t indProcesoActual ;
  pindx_t pindx ;
  modoAp_t modoAp ;
  bool_t extendido ;
  teclado_t far * teclado ;
  byte_t con ;
  byte_t nuevaConsola ;
  word_t numFilasAnt ;
  byte_t cursorFAux ;
  pantalla_t far * pantallaAux ;
  byte_t F, C ;

  asm push ds
  asm mov ds,DS_Consola

  scanCodeAnt = scanCode ;

  asm in al,60h
  asm mov scanCode,al
/*
  printCarBIOS('<') ;
  printHexBIOS(scanCodeAnt, 2) ;
  printHexBIOS(scanCode, 2) ;
  printCarBIOS('>') ;
*/
  ptrVIOrg[nVIntConsola]() ;    /* llamamos a la antigua rti (pushf + int) */

  nuevaConsola = 0xFF ;
  numFilasAnt = numFilas ;

  if (scanCodeAnt == CSAlt) AltPulsada = TRUE ;
  else if (scanCodeAnt == (CSAlt | 0x80)) AltPulsada = FALSE ;

  if (AltPulsada) {

    switch (scanCode) {

    case AltBSlash : nuevaConsola = 0 ; break ;

    case AltF1     : ;
    case AltF2     : ;
    case AltF3     : ;
    case AltF4     : ;
    case AltF5     : ;
    case AltF6     : ;
    case AltF7     : ;
    case AltF8     : ;
    case AltF9     : ;
    case AltF10    : con = scanCode - AltF1 + 1 ;
                     if (con < maxConsolas)
                     nuevaConsola = con ;
                     break ;
    default        : ;
    }

    if (nuevaConsola != 0xFF) {
      if (nuevaConsola != consolaDeSuperficie) {
        salvarConsolaDeSuperficie() ;
        consolaDeSuperficie = nuevaConsola ;
        establecerConsolaDeSuperficie() ;
      }
    }
    else if (redimensionable) {
      switch (scanCode) {
      case 0x1B : ;            /* Alt+ */
      case 0x35 : ;            /* Alt- */
        if (scanCode == 0x1B)      /* Alt+ */
          switch(numFilasAnt) {
          case 25 : numFilas = 28 ; break ;
          case 28 : numFilas = 50 ; break ;
          case 50 : numFilas = 25 ; break ;
          default : break ;
          }
        else                       /* Alt- */
          switch(numFilasAnt) {
          case 25 : numFilas = 50 ; break ;
          case 28 : numFilas = 25 ; break ;
          case 50 : numFilas = 28 ; break ;
          default : break ;
          }
        if (numFilas != numFilasAnt) {
          if (cursorF >= numFilas) {
            copiarCPantalla((pantalla_t far *)&ptrPant->t[cursorF-numFilas+1][0], ptrPant, numFilas) ;
/*          copiarPantalla((pantalla_t far *)&ptrPant->t[cursorF-numFilas+1][0], ptrPant, numFilas) ; */
            cursorF = numFilas-1 ;
          }
          salvarConsolaDeSuperficie() ;
          redimensionar(numFilas, 80) ;
          numFilas = ptrBiosArea->VIDEO_lastrow + 1 ;
          establecerConsolaDeSuperficie() ;
          for ( con = 0 ; con < maxConsolas ; con ++ ) {
            if (con != consolaDeSuperficie) {
              cursorFAux = descConsola[con].F ;
              if (cursorFAux >= numFilas) {
                pantallaAux = (pantalla_t far *)&descConsola[con].pantalla ;
                copiarCPantalla((pantalla_t far *)&pantallaAux->t[cursorFAux-numFilas+1][0], pantallaAux, numFilas) ;
/*              copiarPantalla((pantalla_t far *)&pantallaAux->t[cursorFAux-numFilas+1][0], pantallaAux, numFilas) ; */
                descConsola[con].F = numFilas-1 ;
              }
            }
          }
        }
        break ;
      default : ;
      }
    }
  }

  if (scanCode & 0x80) {
    asm pop ds
    return ;
  }

  w = leerTeclaListaBDA() ;

/* printHexBIOS(w, 4) ; printCarBIOS(' ') ; */

  if (w) {
    car = (char)w ;
    if (car == (char)0xE0)
      car = (char)0 ;
    extendido = (car == (char)0) ;

    teclado = (teclado_t far *)&descConsola[consolaDeSuperficie].teclado ;

    if (teclado->bloqueados.numElem > 0) {
      pindx = teclado->bloqueados.primero ;
      df = ptrDescProceso[pindx].trama->BX ;
      modoAp = ptrDescProceso[pindx].tfa[df].modoAp ;
/*    plot('k', car, contadorTimer0()) ; */
      nbytes = nbytesProceso[pindx] ;
      dir = dirProceso[pindx] ;
      nbytes-- ;
      if ((car == CR) && (modoAp & O_TEXT)) car = LF ;
      dir[0] = car ;                                                /* car */
      dir++ ;
      if (extendido) {
        scanCode = (byte_t)(w >> 8) ;
        car = (char)scanCode ;
        if (nbytes > 0) {
          nbytes-- ;
          dir[1] = car ;                                       /* scanCode */
          dir++ ;
        }
        else
          meter(car, teclado) ;
      }
      nbytesProceso[pindx] = nbytes ;
      dirProceso[pindx] = dir ;
      if (nbytes == 0) {
        desencolarPC2c(&teclado->bloqueados) ;
        ptrDescProceso[pindx].trama->AX =
          ptrDescProceso[pindx].trama->CX ;                     /* nbytes0 */
        ptrDescProceso[pindx].tfa[df].pos +=
          ptrDescProceso[pindx].trama->CX ;
        ptrDescProceso[pindx].estado = preparado ;
        encolarPC2c(pindx, (ptrC2c_t)&ptrC2cPFR[PUrgentes]) ;
        *ptrActivarAlEpilogo = TRUE ;
        indProcesoActual = *ptrIndProcesoActual ;
        if (indProcesoActual > -1) {                        /* nivel 1 o 2 */
          ptrDescProceso[indProcesoActual].trama =
            (trama_t far *)pointer(*ptrSS_Proceso, *ptrSP_Proceso) ;
          ptrDescProceso[indProcesoActual].estado = preparado ;
          encolarPC2c(indProcesoActual, (ptrC2c_t)&ptrC2cPFR[PPreparados]) ;
          *ptrIndProcesoActual = -1 ;
        }
      }
    }
    else {
      meter(car, teclado) ;                                         /* car */
      if (extendido) {
        scanCode = (byte_t)(w >> 8) ;
        car = (char)scanCode ;                                 /* scanCode */
        meter(car, teclado) ;
      }
    }
  }
/*
if (*info.ptrDebugWord > 0) {
  printStrBIOS("\n debugWord = ") ;
  printHexBIOS(*info.ptrDebugWord, 4) ;
  printStrBIOS("\n isr_consola pto 2: ") ;
  printStrBIOS("\n indProcesoActual = ") ;
  printIntBIOS(indProcesoActual, 1) ;
  printStrBIOS(" pindx = ") ;
  printIntBIOS(pindx, 1) ;
  *info.ptrDebugWord = *info.ptrDebugWord - 1 ;
  if (*info.ptrDebugWord == 0) while(TRUE) ;
}
*/
  asm pop ds
}

/* ----------------------------------------------------------------------- */
/*             rutina de servicio de la interrupcion del raton             */
/* ----------------------------------------------------------------------- */

void procesarRaton ( word_t W0, int X, int Y ) ;                /* forward */

void procesarColaBloqueadosRaton ( void ) ;                     /* forward */

void tratarRaton ( void ) ;                                     /* forward */

void actualizarCursor ( void ) ;                                /* forward */

void far isr_raton ( void ) {

  word_t DS_Raton = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;

  asm push ds
  asm mov ds,DS_Raton

/*  plot('m', 0, contadorTimer0()) ; */
  tratarRaton() ;
/*  plot('e', 0, contadorTimer0()) ; */
  eoi_pic2() ;

  asm pop ds
}

void far isr_raton_dosbox ( void ) {

  word_t DS_Raton = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;

  asm push ds
  asm mov ds,DS_Raton

  ptrVIOrg[nVIntRaton]() ;      /* llamamos a la antigua rti (pushf + int) */
/*  plot('m', 0, contadorTimer0()) ; */
  procesarRaton((word_t)er.B0, er.X, er.Y) ;
/*  plot('e', 0, contadorTimer0()) ; */
/* eoi_pic2() ; */

  asm pop ds
}

static word_t rH_CS ;
static word_t rH_IP ;
static word_t rH_AX ;
static word_t rH_BX ;
static word_t rH_CX ;
static word_t rH_DX ;
static word_t rH_SI ;
static word_t rH_DI ;

void far isr_raton_BIOS ( void ) {

  word_t DS_Raton = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;

  asm push ds
  asm mov ds,DS_Raton

/*
printStrBIOS("\n isr_raton_bios") ;
printStrBIOS("\n isr_raton_bios") ;
*/

/*  plot('m', 0, contadorTimer0()) ; */
  procesarRaton(rH_BX, rH_CX, rH_DX)  ;
/*  plot('e', 0, contadorTimer0()) ; */
/* eoi_pic2() ; */

  asm pop ds
}

rti_t rti_nVIntRaton ;                               /* ver so1\interrup.c */
word_t segment_rti_nVIntRaton ;
word_t offset_rti_nVIntRaton ;

word_t far * ptrWordAux ;                          /* para depurar la pila */

void far rti_raton_BIOS ( void ) {
  asm sub sp,10                                 /* Flags, CS, IP (Proceso) */
                                                 /* segment_rti_nVIntRaton */
                                                  /* offset_rti_nVIntRaton */
  asm push ds                                              /* DS (Proceso) */

  asm mov ds,word ptr cs:[segDatos]          /* segmento de datos DS_Raton */

  asm push ax
  asm push bp

  asm mov bp,sp
  asm mov ax,rH_CS
  asm mov ss:[bp+12],ax
  asm mov ax,rH_IP
  asm mov ss:[bp+10],ax
  asm mov ax,segment_rti_nVIntRaton
  asm mov ss:[bp+8],ax
  asm mov ax,offset_rti_nVIntRaton
  asm mov ss:[bp+6],ax
/*
printStrBIOS("\n isr_raton_BIOS") ;
  ptrWordAux = (word_t far  *)pointer(_SS,_SP) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[0], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[1], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[2], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[3], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[4], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[5], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[6], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[7], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[8], 4) ;
printStrBIOS("\n ") ;
*/
  asm pop bp
  asm pop ax
  asm pop ds
                                        /* retorno lejano a rti_nVIntRaton */
}

void far ratonHandler ( void ) {

  /* se llama a esta funcion tras la interrupcion y mediante una llamada   */
  /* a procedimiento lejano (CALL FAR). Por ese motivo la pila contiene lo */
  /* siguiente: Flags, CS, IP del proceso y direccion de retorno lejana.   */

  asm push ds                    /* segmento de datos del driver del raton */
  asm mov ds,word ptr cs:[segDatos]          /* segmento de datos DS_Raton */
/*
printStrBIOS("\n ratonHandler principio") ;
  ptrWordAux = (word_t far  *)pointer(_SS,_SP) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[0], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[1], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[2], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[3], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[4], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[5], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[6], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[7], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[8], 4) ;
printStrBIOS("\n ") ;
*/
  asm mov rH_AX,ax                         /* evento ocurrido, ver techelp */
  asm mov rH_BX,bx                                              /* botones */
  asm mov rH_CX,cx                                                    /* X */
  asm mov rH_DX,dx                                                    /* Y */
  /* asm mov rH_SI,si */                                           /* incX */
  /* asm mov rH_DI,di */                                           /* incY */

  asm push ax
  asm push bp
  asm mov bp,sp
  asm mov ax,ss:[bp+12]
  asm mov rH_CS,ax                                       /* CS del proceso */
    asm mov ax,ss:[bp+10]                                  /* IP del proceso */
  asm mov rH_IP,ax

  asm mov ss:[bp+12],cs                                              /* CS */
  asm mov ax,OFFSET rti_raton_BIOS                       /* rti_raton_BIOS */
  asm mov ss:[bp+10],ax
  asm pop bp
  asm pop ax
/*
printStrBIOS("\n ratonHandler fin") ;
  ptrWordAux = (word_t far  *)pointer(_SS,_SP) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[0], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[1], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[2], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[3], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[4], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[5], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[6], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[7], 4) ;
printStrBIOS("\n ") ;
printHexBIOS(ptrWordAux[8], 4) ;
printStrBIOS("\n ") ;
*/
  asm pop ds                                     /* se retorna del handler */
}                            /* de el a isr_raton_bios y de ahi al proceso */

#define TECLADO_ESTADO  0x64
#define TECLADO_COMANDO 0x64
#define TECLADO_DATO    0x60

#define RATON_ESTADO    TECLADO_ESTADO
#define RATON_COMANDO   TECLADO_COMANDO
#define RATON_DATO      TECLADO_DATO

void tratarRaton ( void ) {

  static byte_t byteEstado ;
  static byte_t byteDato ;
  static int incX, incY ;

  asm in al,64h
  asm mov byteEstado,al
  if ((byteEstado & 0x21) == 0x21) {      /* hay byte procedente del raton */
    er.S0 = byteEstado ;
    asm in al,60h
    asm mov byteDato,al

    if ((inB != 0) || (((byteDato^0x08) & 0xC8) == 0)) {

      B[inB] = byteDato ;
      inB = (inB + 1) % 3 ;

      if (inB == 0) {

        er.B0 = B[0] ;
        er.B1 = B[1] ;
        if (er.B0 & 0x10) {
          asm mov al, byte ptr er.B1
          asm mov ah,0ffh
          asm mov incX,ax
          er.incX = incX ;
        }
        else er.incX = er.B1 ;
        XAux = er.X + er.incX ;
        if ((0 <= XAux) && (XAux < maxX)) er.X = XAux ;      /* 8*80 = 640 */

        er.B2 = B[2] ;
        if (er.B0 & 0x20) {
          asm mov al, byte ptr er.B2
          asm mov ah,0ffh
          asm mov incY,ax
          er.incY = incY ;
        }
        else er.incY = er.B2 ;
        er.incY = -er.incY ;                                        /* ojo */
        YAux = er.Y + er.incY ;
        if ((0 <= YAux) && (YAux < maxY)) er.Y = YAux ;      /* 8*25 = 200 */

        procesarRaton((word_t)er.B0, er.X, er.Y) ;

      }
    }
  }
}

void procesarRaton ( word_t parW0, int parX, int parY ) {

//  static int posWF ;
//  static int posWC ;
//  static int FReal ;
//  static int CReal ;

  er.W0 = parW0 ;
  er.X = parX ;
  er.Y = parY ;

  er.F = er.Y >> 3 ;                                            /* F = Y/8 */
  er.C = er.X >> 3 ;                                            /* C = X/8 */

  er.B0 = (byte_t)(er.W0 & 0x00FF) ;

  huboInt = TRUE ;               /* obtenidos nuevos valores para el raton */

  er.botonIzAnt = er.botonIz ;
  er.botonDeAnt = er.botonDe ;
  er.botonMeAnt = er.botonMe ;

  er.botonIz = (er.W0 & 0x0001) ;
  er.botonDe = ((er.W0 & 0x0002) >> 1) ;
  er.botonMe = ((er.W0 & 0x0004) >> 2) ;

  procesarColaBloqueadosRaton() ;

  if (cursorRatonActivo) {
    if (er.botonIz) {                        /* boton izquierdo presionado */
#if (FALSE)
      if (moviendo || redimensionando) {
        if (moviendo) {                                        /* moviendo */
          posWF = win->esqSupIzF ;
          posWC = win->esqSupIzC ;
          moverWin(win, er.F-FAntCursor, er.C-CAntCursor,
                        er.F-FAntCursor, er.C-CAntCursor) ;
          FReal = FAntCursor + (win->esqSupIzF - posWF) ;        /* F Real */
          CReal = CAntCursor + (win->esqSupIzC - posWC) ;        /* C Real */
        }
        else {                                          /* redimensionando */
          posWF = win->esqInfDeF ;
          posWC = win->esqInfDeC ;
          moverWin(win, 0, 0, er.F-FAntCursor, er.C-CAntCursor) ;
          FReal = FAntCursor + (win->esqInfDeF - posWF) ;        /* F Real */
          CReal = CAntCursor + (win->esqInfDeC - posWC) ;        /* C Real */
        }
        if ((FReal != er.F) || (CReal != er.C)) {
          er.Y = (FReal << 3) + 3 ;                            /* ajuste Y */
          er.X = (CReal << 3) + 3 ;                            /* ajuste X */
          if (tipoRaton == msdos)
            situarCursorRatonBIOS(er.X, er.Y) ;
        }
        er.F = FReal ;
        er.C = CReal ;
        actualizarCursor() ;
      }
      else if ((nwin = winVisible[er.F][er.C]) != 0) {
        if (!er.botonIzAnt) {          /* primera pulsacion del boton izdo */
          win = winNum[nwin] ;
          pindx = win->pindx ;

          if (er.F == win->esqSupIzF-1) {                /* marco superior */
            if (er.C == win->esqInfDeC-1) botonCerrarVentana() ;    /* [X] */
            else if (er.C == win->esqInfDeC-4) maxMinWin(win, FALSE) ; /* [f] */
            else if ((er.F == win->esqSupIzF-1) &&
                     (win->esqSupIzC <= er.C) &&
                     (er.C <= win->esqSupIzC+2+2*(win->pindx!=0)))
              maxMinWin(win, TRUE) ;                                /* max */
            else
              tratarClickMarco() ;
          }
          else if ((er.F == win->esqInfDeF+1) &&
                   (er.C == win->esqInfDeC+1)) {          /* redimensionar */
            focalizarWin(win) ;
            redimensionando = TRUE ;
            actualizarCursor() ;
          }
          else if ((er.F == win->esqInfDeF+1) ||
                   (er.C == win->esqSupIzC-1) ||
                   (er.C == win->esqInfDeC+1))               /* otro marco */
            tratarClickMarco() ;
          else focalizarWin(win) ;                       /* punto interior */
        }
        else actualizarCursor() ;
      }
#endif
    }
    else {                                /* boton izquierdo no presionado */
      if (moviendo)
        moviendo = FALSE ;
      else if (redimensionando)
        redimensionando = FALSE ;
      actualizarCursor() ;
    }
  }
}

void procesarColaBloqueadosRaton ( void ) {

  pindx_t indProcesoActual ;
  pindx_t pindx ;

  if (bloqueadosRaton.numElem > 0) {
    pindx = bloqueadosRaton.primero ;
    indProcesoActual = *ptrIndProcesoActual ;
    if (indProcesoActual > -1) {                            /* nivel 1 o 2 */
      ptrDescProceso[indProcesoActual].trama =
        (trama_t far *)pointer(*ptrSS_Proceso, *ptrSP_Proceso) ;
      ptrDescProceso[indProcesoActual].estado = preparado ;
      encolarPC2c(indProcesoActual, (ptrC2c_t)&(ptrC2cPFR[PPreparados])) ;
      *ptrIndProcesoActual = -1 ;
      *ptrActivarAlEpilogo = TRUE ;
    }

    do {

      copia((pointer_t)&er, dirProceso[pindx], nbytesProceso[pindx]) ;

#if (FALSE)

      switch (descProceso[pindx].trama->DX) {
      case 0 : if (descProceso[pindx].win != descProceso[indProcesoDeSuperficie].win) break ;
      case 1 : if (winVisible[estado->F][estado->C] != descProceso[pindx].win->num) break ;
      case 2 :

        switch (descProceso[pindx].esperandoPor) {
        case rec_tecladoRaton :
          eliminarPC2c(pindx, &descProceso[pindx].teclado->bloqueados) ;
          descProceso[pindx].trama->AL = (char)0 ;
          break ;
        case rec_tecladoRawRaton :
          eliminarPC2c(pindx, &descProceso[pindx].teclado->bloqueadosRaw) ;
          descProceso[pindx].trama->AX = 0x0000 ;
          descProceso[pindx].trama->BX = 0x0000 ;           /* modificamos BX */
          break ;
        }
#endif
        ptrDescProceso[pindx].estado = preparado ;
        encolarPC2c(pindx, (ptrC2c_t)&ptrC2cPFR[PUrgentes]) ;
#if (FALSE)

      }

#endif

      desencolarPC2c(&bloqueadosRaton) ;
      pindx = bloqueadosRaton.primero ;
    }
    while (bloqueadosRaton.numElem > 0) ;
  }
}

#define ptrPant ((pantalla_t far *)pointer(0xB800, 0x0000))

#define rotarDerecha(x, n)                                                   \
  asm mov al,x ;                                                             \
  asm ror al,n ;                           /* rotacion a la derecha n bits */\
  asm mov x,al ;

void actualizarCursor ( void ) {
  if (cursorRatonActivo) {               /* actualizar el cursor del raton */
    if (primeraInt) primeraInt = FALSE ;
    else if ((er.F != FAntCursor) || (er.C != CAntCursor)) {

      atrAux = ptrPant->t[FAntCursor][CAntCursor].atr ;
      /* atrAux = (atrAux << 4) | (atrAux >> 4) ; */
      if (atrAux != atrNormal) {
        rotarDerecha(atrAux, 4) ;
        ptrPant->t[FAntCursor][CAntCursor].atr = atrAux ;
      }
      atrAux = ptrPant->t[er.F][er.C].atr ;
      /* atrAux = (atrAux << 4) | (atrAux >> 4) ; */
      rotarDerecha(atrAux, 4) ;
      ptrPant->t[er.F][er.C].atr = atrAux ;
      FAntCursor = er.F ;
      CAntCursor = er.C ;

    }
  }
}

/* ----------------------------------------------------------------------- */
/*                     handler para el raton (int 15h)                     */
/* ----------------------------------------------------------------------- */

void far handlerRatonNulo ( void ) {
}

void far handlerRaton ( dword_t y, word_t x, word_t s ) {

  static bool_t priVez = TRUE ;   /* Lo necesito para ignorar los incrementos .. */
  static int incX = 0, incY = 0 ; /* .. X,Y iniciales que son basura */
  int X1 ;
  int Y1 ;
  int numFilas ;

  asm cli
  asm mov ds,word ptr cs:[segDatos]
/*  establecerDS_SO1() ; */
  if (priVez)
    priVez = FALSE ;              /* Ignoro la primera lectura por ser basura */
  else {
    incX = x ; incY = (y >> 16) ; /* Tienen sentido respecto a MOVING/SIZING y */
    er.B0 = s ; /* (s & BITS_IGN) | (M.stat & ~BITS_IGN) ; */
  } /* Respeto los que habia */
  /* Considero el bit de signo .. */
  if (er.B0 & 0x20) incY |= 0xFF00 ;  /* ..haciendo una extension.. */
  if (er.B0 & 0x10) incX |= 0xFF00 ;  /* ..del bit de signo */
  X1 = (er.X + incX) ;
  Y1 = (er.Y - incY) ;

  if (X1 < 0) er.X = 0 ;
  else if (X1 >= maxX) er.X = maxX-1 ;
  else er.X = X1 ;
/*
  if (Y1 < 0) er.Y = 0 ;
  else if (Y1 >= maxY) er.Y = maxY-1 ;
  else er.Y = Y1 ;
*/
  if (Y1 < 0) er.Y = 0 ;
  else {
    numFilas = ptrBiosArea->VIDEO_lastrow + 1 ;
    if (Y1 >= numFilas) er.Y = numFilas-1 ;
    else er.Y = Y1 ;
  }
}

/* ----------------------------------------------------------------------- */
/*                     call back para el recurso TIMER                     */
/* ----------------------------------------------------------------------- */

static byte_t congen ;

static int pCon ( char car ) {
  printCarConsola(congen, car) ;
  return(1) ;
}

static int printLnConsola ( byte_t con ) {
  congen = con ;
  return(printGenLn(pCon)) ;
}

static int printStrConsola ( byte_t con, const char far * str ) {
  congen = con ;
  return(printGenStr(str, pCon)) ;
}

static int printStrHastaConsola ( byte_t con, const char far * str, word_t n, bool_t lleno ) {
  congen = con ;
  return(printGenStrHasta(str, n, lleno, pCon)) ;
}

static int printDecConsola ( byte_t con, word_t num, word_t l ) {
  congen = con ;
  return(printGenDec(num, l, pCon)) ;
}

static int printIntConsola ( byte_t con, int num, word_t l ) {
  congen = con ;
  return(printGenInt(num, l, pCon)) ;
}

static int printLIntConsola ( byte_t con, long int num, word_t l ) {
  congen = con ;
  return(printGenLInt(num, l, pCon)) ;
}

static int printLDecConsola ( byte_t con, dword_t num, word_t l ) {
  congen = con ;
  return(printGenLDec(num, l, pCon)) ;
}

static int printHexConsola ( byte_t con, word_t num, word_t l ) {
  congen = con ;
  return(printGenHex(num, l, pCon)) ;
}

static int printLHexConsola ( byte_t con, dword_t num, word_t l ) {
  congen = con ;
  return(printGenLHex(num, l, pCon)) ;
}

static int printPtrConsola ( byte_t con, pointer_t ptr ) {
  congen = con ;
  return(printGenPtr(ptr, pCon)) ;
}

static int far cbForTimer ( void far * arg ) {                /* call back */
  word_t DS_Consola = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
  byte_t F0 ;
  byte_t C0 ;
  byte_t congen0 ;
  byte_t visibilidadAnt ;
  trama_t far * tramaProceso ;

  asm push ds
  asm mov ds,DS_Consola  /* peligro SS (Sistema Operativo) != DS (Consola) */
                         /* punteros a pila (parametros/variables locales) */
/*
  tramaProceso = (trama_t far *)pointer(*ptrSS_Proceso, *ptrSP_Proceso) ;
*/
  F0 = cursorF ;
  C0 = cursorC ;
  congen0 = congen ;
  visibilidadAnt = setCursorVisibilidad(0) ;
  if (consolaDeSuperficie != 0) {
    goToXYConsola(consolaDeSuperficie, 0, 75) ;
    printStrConsola(consolaDeSuperficie, "     ") ;
    goToXYConsola(consolaDeSuperficie, 1, 75) ;
    printStrConsola(consolaDeSuperficie, "CON") ;
    printIntConsola(consolaDeSuperficie, consolaDeSuperficie, 1) ;
  }
  else {
    goToXYConsola(0, 0, 64) ;
    printStrConsola(0, "                ") ;
    goToXYConsola(0, 1, 64) ;
    printLDecConsola(0, ((argCbTimer_t far *)arg)->contTics, 11) ;
    printIntConsola(0, ((argCbTimer_t far *)arg)->contTicsRodaja, 4) ;
  }
  congen = congen0 ;
  cursorF = F0 ;
  cursorC = C0 ;
  goToXYHw(cursorF, cursorC) ;
  setCursorVisibilidad(visibilidadAnt) ;

  asm pop ds
  return(1) ;
}

/* ----------------------------------------------------------------------- */
/*              seccion de programa de usuario de la consola               */
/* ----------------------------------------------------------------------- */

descProceso_t descProceso [ maxProcesos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

/* ----------------------------------------------------------------------- */
/*                seccion de programa de usuario del raton                 */
/* ----------------------------------------------------------------------- */

void outKBD ( byte_t puerto,   /* envia un comando al dispositivo auxiliar */
              byte_t valor ) {

  asm mov dl,puerto
  asm mov dh,valor
  asm push dx
  asm xor dx,dx
  asm mov es,dx
outKBD_0:
  asm mov dl,byte ptr es:[046Ch]                             /* BIOS_timer */
outKBD_1:
  asm in al,64h                                         /* keyboard status */
  asm test al,00000010b                    /* =2 check if we can send data */
  asm jmp outKBD_2                           /* jump if write buffer empty */
  asm cmp dl,byte ptr es:[046Ch]                             /* BIOS_timer */
  asm je outKBD_1
  asm xor dh,1
  asm jnz outKBD_0
outKBD_2:
  asm pop dx
  asm mov al,dh
  asm mov dh,0
  asm out dx,al
}

void flushKBD ( byte_t puerto,         /* Send command to auxiliary device */
                byte_t valor ) {

  outKBD(puerto, valor) ;
  asm mov ah,0
flushKBD_0:
  asm xor dx,dx
  asm mov es,dx
flushKBD_1:
  asm mov dl,byte ptr es:[046Ch]                             /* BIOS_timer */
flushKBD_2:
  asm in al,64h                                         /* keyboard status */
  asm test al,00000001b                          /* =1 if read buffer full */
  asm jz flushKBD_3
  asm in al,60h
  asm mov ah,al
  asm jmp flushKBD_0
flushKBD_3:
  asm cmp dl,byte ptr es:[046Ch]                             /* BIOS_timer */
  asm je flushKBD_2
  asm xor dh,1
  asm jnz flushKBD_1
}

void setRate ( byte_t rate ) {

  outKBD(0x64, 0x0D4) ;                          /* enable mouse functions */
  flushKBD(0x60, 0x0F3) ;                        /* set rate...            */
                                  /* should be 0FAh (ACK) or 0FEh (resend) */
  outKBD(0x64, 0x0D4) ;                          /* enable mouse functions */
  flushKBD(0x60, rate) ;                         /* ...value               */
                                  /* should be 0FAh (ACK) or 0FEh (resend) */
}

void far PS2dummy ( void ) {
}

bool_t checkPS2 ( printCar_t pCar ) {

  asm int 11h                                        /* get equipment list */
  asm and al,00000100b                                  /* 00000100 HW_PS2 */
  asm jz noPS2                               /* jump if PS/2 not indicated */

  asm mov bh,3
  asm mov ax,0C205h                    /* inicializar raton, bh = datasize */
  asm int 15h
  asm jc noPS2
  asm test ah,ah
  asm jnz noPS2

  asm mov bh,3
  asm mov ax,0C203h             /* establecer la resoluci¢n del raton (bh) */
  asm int 15h
  asm jc noPS2
  asm test ah,ah
  asm jnz noPS2

  asm push cs
  asm pop es
  asm mov bx,OFFSET PS2dummy
  asm mov ax,0C207h                    /* establecer mouse handler (ES:BX) */
  asm int 15h
  asm jc noPS2
  asm test ah,ah
  asm jnz noPS2

  asm xor bx,bx
  asm mov es,bx
  asm mov ax,0C207h                        /* borrar mouse handler (ES:BX) */
  asm int 15h
                            /* select IntelliMouse Z wheel + 3 button mode */
  asm pushf
  asm sti
  pCar('.') ;
  setRate(200) ;                            /* 200->100->80 rate does this */
  pCar('.') ;
  setRate(100) ;
  pCar('.') ;
  setRate( 80) ;
                                                     /* check if sucessful */
  outKBD(0x64, 0x0D4) ;                          /* enable mouse functions */
  flushKBD(0x60, 0x0F2) ;                        /* get ID en ah           */
                                                 /* ID=3 -> 3 button+wheel */
  asm popf

  return(TRUE) ;                                            /* =PS/2 mouse */
noPS2:
  return(FALSE) ;
}

rti_t rtiRatonOrg ;                      /* rti antes de instalar el raton */

void disablePS2 ( void ) {

  asm pushf
  asm cli
  ptrTVI[nVIntRaton] = rtiRatonOrg ;  /* falta actualizar la mascara de interrupcion del pic ?? */
  asm popf
  /* restablecerInt(nVIntRaton) ; */

  asm xor bx,bx
  asm mov ax,0C200h                                       /* set mouse off */
  asm int 15h

  asm mov es,bx
  asm mov ax,0C207h                      /* borrar mouse handler (ES:BX=0) */
  asm int 15h

}

void enablePS2 ( handler_t handler ) {

  static word_t handlerWord ;

  handlerWord = (word_t)handler ;

  asm mov bx,0
  asm mov ax,0C200h                                       /* set mouse off */
  asm int 15h

  asm mov bh,3
  asm mov ax,0C205h                    /* inicializar raton, bh = datasize */
  asm int 15h

  asm mov bh,3
  asm mov ax,0C203h             /* establecer la resoluci¢n del raton (bh) */
  asm int 15h

#if (FALSE)
  asm mov bx,0
  asm mov es,bx
  asm mov ax,0C207h                      /* borrar mouse handler (ES:BX=0) */
  asm int 15h
#endif

  asm push cs
  asm pop es
  asm mov bx,handlerWord
  asm mov ax,0C207h
  asm int 15h               /* Establece mouse handler ES:BX=handler_Raton */

  asm mov bh,1
  asm mov ax,0C200h                                        /* set mouse on */
  asm int 15h

}

typedef enum { ninguno, msdos, ps2 } tipoRaton_t ;

void inicRaton ( tipoRaton_t * tipoRaton, isr_t * isrRaton ) {

  word_t numBotones ;

  info_t info ;

  bool_t hayNT ;
  bool_t hayDBox ;

  obtenInfoINFO((info_t far *)&info) ;

  hayNT = FALSE ;
  switch (info.modoSO1) {
  case modoSO1_Com :                                            /* so1.com */
  case modoSO1_Exe :                                            /* so1.exe */
    hayNT = (bool_t)(hayWindowsNT()) ;
  default : ;
  } ;

  hayDBox  = (bool_t)(igualesHasta(ptrFechaBios, "01/01/92", 8)) ;

  if (hayNT) escribirStr("\n\n hayNT ") ;
  if (hayDBox) escribirStr("\n\n hayDBox ") ;

  *tipoRaton = ninguno ;

  inicEstadoRaton() ;
  if (hayNT) {
    resetRatonBIOS(&numBotones) ;
    if (hayRatonBIOS()) {                           /* raton msdos int 33h */
      *tipoRaton = msdos ;                               /* WinXP o DOSBox */
      *isrRaton = isr_raton_BIOS ;
      installMouseEventHandler(           /* hay que poner _CS por el caso */
        (handler_t)pointer(_CS, (word_t)ratonHandler)           /* SO1.COM */
      ) ;
    }
    else escribirStr("\n\n no hay raton BIOS ") ;
  }
  if (*tipoRaton == ninguno) {
    escribirStr("\n\n comprobando si hay raton PS/2 ") ;
    if (checkPS2(escribirCar)) {
      escribirStr(" Ok") ;
      *tipoRaton = ps2 ;                                      /* raton PS2 */
      if (hayDBox) {                               /* dosbox (qemu, bochs) */
        enablePS2((handler_t)handlerRaton) ;
        *isrRaton = isr_raton_dosbox ;
      } else {                                              /* qemu, bochs */
        enablePS2((handler_t)handlerRatonNulo) ;
        *isrRaton = isr_raton ;
      }
    }
    else {
      escribirStr("\r no se ha encontrado ningun raton \n") ;
      exit(-1) ;
    }
  }

  inicPC2c(&bloqueadosRaton, &e2BloqueadosRaton, maxProcesos + 0, FALSE) ;

}

char strTipoRaton [ ] [ 8 ] = {
  "ninguno", "msdos", "ps2"
} ;

#define maxCbRaton 9

static descCcb_t descCcbRaton = { 0, 0, 0, maxCbRaton,
  cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo
} ;

tipoRaton_t tipoRaton ;

static int integrarRaton ( void ) {

  int dfRaton ;
  dfs_t dfs ;
  descRecurso_t dR ;
  int res ;
  pindx_t pindx ;
  char nomFich [ 12 ] ;
  isr_t isrRaton ;
  rti_t rtiRecRatonOrg ;

  rtiRatonOrg = ptrTVI[nVIntRaton] ;       /* antes de instalar el handler */

  for ( pindx = 0 ; pindx < maxProcesos ; pindx++ ) {
    nbytesProceso[pindx] = 0 ;                    /* nbytes esperando leer */
    dirProceso[pindx] = (pointer_t)0x00000000 ;
  }

/* de momento esto solo funciona si hay msdos <--------
  dfRaton = open("RATON", O_RDONLY) ;
  if (dfRaton >= 0) {
    escribirStr("\n\n error: RATON ya instalado \n") ;
    close(dfRaton) ;
    exit(-1) ;
  }
*/

  inicRaton(&tipoRaton, &isrRaton) ;               /* tipoRaton != ninguno */

  obtenInfoSO1(dirDescSO1) ;                 /* obtenemos los datos de SO1 */
  *((word_t far *)pointer(_CS, (word_t)segDatos)) = _DS ;     /* guardo DS */

  dR.tipo = rDCaracteres ;
  copiarStr("RATON", dR.nombre) ;
  dR.ccb = (ccb_t)&descCcbRaton ;
  dR.ccb->arg = NULL ;
  dR.pindx = getpindx() ;
  dR.numVI = 1 ;
  dR.nVInt[0] = nVIntRaton ;
  dR.irq[0] = IRQ_RATON ;
  dR.isr[0] = (isr_t)pointer(_CS, (word_t)isrRaton) ;

  dR.open      = (open_t)pointer(_CS, (word_t)openRaton) ;
  dR.release   = (release_t)pointer(_CS, (word_t)releaseRaton) ;
  dR.read      = (read_t)pointer(_CS, (word_t)readRaton) ;
  dR.aio_read  = (aio_read_t)pointer(_CS, (word_t)aio_readRaton) ;
  dR.write     = (write_t)pointer(_CS, (word_t)writeRaton) ;
  dR.aio_write = (aio_write_t)pointer(_CS, (word_t)aio_writeRaton) ;
  dR.lseek     = (lseek_t)pointer(_CS, (word_t)lseekRaton) ;
  dR.fcntl     = (fcntl_t)pointer(_CS, (word_t)fcntlRaton) ;
  dR.ioctl     = (ioctl_t)pointer(_CS, (word_t)ioctlRaton) ;

  dR.eliminar  = (eliminar_t)pointer(_CS, (word_t)eliminarRaton) ;

  asm pushf
  asm cli

  rtiRecRatonOrg = ptrTVI[nVIntRaton] ;         /* tras instalar el handler */
  rec_raton = crearRecurso(&dR) ;
  if (tipoRaton == msdos) {
    rti_nVIntRaton = ptrTVI[nVIntRaton] ;          /* ver so1\interrup.c */
    segment_rti_nVIntRaton = seg((pointer_t)rti_nVIntRaton) ;
    offset_rti_nVIntRaton = off((pointer_t)rti_nVIntRaton) ;
    ptrTVI[nVIntRaton] = rtiRecRatonOrg ; /* restablecer el v.i. nVIntRaton */
/*
    escribirStr("\n rti_nVIntRaton = ") ;
    escribirPtr((pointer_t)rti_nVIntRaton) ;
    escribirStr("\n segment_rti_nVIntRaton = ") ;
    escribirHex(segment_rti_nVIntRaton, 4) ;
    escribirStr("\n offset_rti_nVIntRaton = ") ;
    escribirHex(offset_rti_nVIntRaton, 4) ;
*/
  }
  asm popf

  if (rec_raton >= 0) {

    copiarStr("RATON", nomFich) ;

    dfs = crearFichero(nomFich, rec_raton, 0, fedCaracteres) ;

    if (dfs < 0) {
      switch(dfs) {
      case -1 : escribirStr(" tipo de fichero erroneo") ; break ;
      case -2 : escribirStr(" numero de recurso erroneo") ; break ;
      case -3 : escribirStr(" RATON nombre de fichero ya existente") ; break ;
      case -4 : escribirStr(" no hay descriptores de fichero libres") ; break ;
      default : escribirStr(" no se ha podido crear el fichero RATON") ;
      }
      destruirRecurso("RATON") ;
    }
/*
    res = encolarCcbRecurso(
            (callBack_t)pointer(_CS, (word_t)cbForTimer),
            pointer(_DS, (word_t)&"TIMER")
    ) ;

    if (res != 0) {
      escribirStr("\n\n no se ha podido encolar el callback en el TIMER \n") ;
      return(res) ;
    }
*/
    escribirStr("\n\n recurso RATON instalado (fichero: RATON) \n") ;

    escribirStr("\n tipo de raton = ") ;
    escribirStr(strTipoRaton[tipoRaton]) ;
    escribirStr("\n") ;

  }
  else switch(rec_raton) {
  case -1 : escribirStr("\n\n tipo de recurso erroneo \n") ; break ;
  case -2 : escribirStr("\n\n demasiados vectores de interrupcion \n") ; break ;
  case -3 : escribirStr("\n\n RATON nombre de recurso ya existente \n") ; break ;
  case -4 : escribirStr("\n\n no hay descriptores de recurso libres \n") ; break ;
  case -5 : escribirStr("\n\n numero de vector de interrupcion ya usado \n") ; break ;
  default : escribirStr("\n\n no se ha podido crear el recurso RATON \n") ;
  }
  return(-1) ;
}

static int desintegrarRaton ( void ) {
  int res ;
/*
  res = eliminarCcbRecurso(
          (callBack_t)pointer(_CS, (word_t)cbForTimer),
          pointer(_DS, (word_t)&"TIMER")
  ) ;
  if (res != 0)
    escribirStr("\n\n no pudo eliminarse el callback del TIMER \n") ;
*/
  switch (tipoRaton) {
    case msdos : uninstallMouseEventHandler(ratonHandler) ; break ;
    case ps2   : disablePS2() ;
  }
  return(res) ;
}

/* ----------------------------------------------------------------------- */
/*               seccion de programa de usuario consola+raton              */
/* ----------------------------------------------------------------------- */

static void formato ( void ) {
  escribirStr("\n\n formato: CONRAT [ [ -i ] [ num ] | -u | -h ] \n") ;
  exit(-1) ;
}

static void help ( void ) {
  escribirLn() ;
  escribirLn() ;
  escribirStr(" formato : CONRAT [ [ -i ] [ num ] | -u | -h ] \n\n") ;
  escribirStr(" instala/desinstala el driver consola+raton    \n\n") ;
  escribirStr(" opciones: (por defecto -i y num = 6)          \n\n") ;
  escribirStr("      -i : instala el driver (usar &)          \n") ;
  escribirStr("     num : numero de consolas                  \n") ;
  escribirStr("      -u : desintala el driver                 \n") ;
  escribirStr("      -h : muestra este help                   \n") ;
  exit(0) ;
}

#define maxCbConsola 0

static descCcb_t descCcbConsola = { 0, 0, 0, maxCbConsola, NULL } ;

static int integrarConsola ( byte_t numConsolas ) {

  dfs_t dfs ;
  descRecurso_t dR ;
  int res ;
  byte_t con ;
  pindx_t pindx ;
  char nomFich [ 12 ] ;

  obtenInfoINFO((info_t far *)&info) ;                       /* depuracion *//**************/

  inicCrtHw() ;
  inicBiosCrt() ;

  maxConsolas = numConsolas + 1 ;

  descConsola = (descConsola_t far *)
    pointer(
      ll_buscarBloque(((maxConsolas*sizeof(descConsola_t))+15)/16),  /* GM */
      0x0000
    ) ;
/*
  printStrBIOS("\n descConsola = ") ;
  printPtrBIOS((pointer_t)descConsola) ;
  printStrBIOS("\n numFilas = ") ;
  printIntBIOS(numFilas, 1) ;
  printStrBIOS(" numColumnas = ") ;
  printIntBIOS(numColumnas, 1) ;
*/
  for ( con = 0 ; con < maxConsolas ; con++ ) {
    descConsola[con].teclado.in = 0 ;
    descConsola[con].teclado.out = 0 ;
    descConsola[con].teclado.ncar = 0 ;
    inicPC2c(&descConsola[con].teclado.bloqueados,
             &descConsola[con].teclado.e2Bloqueados, maxProcesos + 0, TRUE) ;
    descConsola[con].teclado.inRaw = 0 ;
    descConsola[con].teclado.outRaw = 0 ;
    descConsola[con].teclado.nRaw = 0 ;
    inicPC2c(&descConsola[con].teclado.bloqueadosRaw,
             &descConsola[con].teclado.e2Bloqueados, maxProcesos + 1, TRUE) ;
  }

  consolaDeSuperficie = 0 ;

  for ( con = 0 ; con < maxConsolas ; con++ ) {
    borrarPantalla((pantalla_t far *)&descConsola[con].pantalla, numFilas) ;
    descConsola[con].F = 0 ;
    descConsola[con].C = 0 ;
  }

  for ( pindx = 0 ; pindx < maxProcesos ; pindx++ ) {
    nbytesProceso[pindx] = 0 ;                    /* nbytes esperando leer */
    dirProceso[pindx] = (pointer_t)0x00000000 ;
  }

  dR.tipo = rDCaracteres ;
  copiarStr("CONSOLA", dR.nombre) ;
  dR.ccb = (ccb_t)&descCcbConsola ;
  dR.ccb->arg = NULL ;
  dR.pindx = getpindx() ;
  dR.numVI = 1 ;
  dR.nVInt[0] = nVIntConsola ;
  dR.irq[0] = IRQ_TECLADO ;
  dR.isr[0] = (isr_t)pointer(_CS, (word_t)isr_consola) ;

  dR.open      = (open_t)pointer(_CS, (word_t)openConsola) ;
  dR.release   = (release_t)pointer(_CS, (word_t)releaseConsola) ;
  dR.read      = (read_t)pointer(_CS, (word_t)readConsola) ;
  dR.aio_read  = (aio_read_t)pointer(_CS, (word_t)aio_readConsola) ;
  dR.write     = (write_t)pointer(_CS, (word_t)writeConsola) ;
  dR.aio_write = (aio_write_t)pointer(_CS, (word_t)aio_writeConsola) ;
  dR.lseek     = (lseek_t)pointer(_CS, (word_t)lseekConsola) ;
  dR.fcntl     = (fcntl_t)pointer(_CS, (word_t)fcntlConsola) ;
  dR.ioctl     = (ioctl_t)pointer(_CS, (word_t)ioctlConsola) ;

  dR.eliminar  = (eliminar_t)pointer(_CS, (word_t)eliminarConsola) ;

  rec_consola = crearRecurso(&dR) ;

  if (rec_consola >= 0) {

    copiarStr("CON ", nomFich) ;
    for ( con = 0 ; con < maxConsolas ; con++ ) {
      nomFich[3] = '0' + con ;
      dfs = crearFichero(nomFich, rec_consola, con, fedCaracteres) ;
      if (dfs < 0) {
        switch(dfs) {
        case -1 : escribirStr(" tipo de fichero erroneo") ; break ;
        case -2 : escribirStr(" numero de recurso erroneo") ; break ;
        case -3 : escribirStr(" CON") ;
                  escribirDec(con, 1) ;
                  escribirStr(" nombre de fichero ya existente") ; break ;
        case -4 : escribirStr(" no hay descriptores de fichero libres") ; break ;
        default : escribirStr(" no se ha podido crear el fichero CON") ;
                  escribirDec(con, 1) ;
        }
        destruirRecurso("CONSOLA") ;
      }
    }

    res = encolarCcbRecurso(
            (callBack_t)pointer(_CS, (word_t)cbForTimer),
            pointer(_DS, (word_t)&"TIMER")
    ) ;

    if (res != 0) {
      escribirStr("\n\n no se ha podido encolar el callback en el TIMER \n") ;
      return(res) ;
    }
    escribirStr("\n\n recurso CONSOLA instalado (ficheros: CON0 ...) \n") ;
    return(0) ;
  }
  switch(rec_consola) {
  case -1 : escribirStr("\n\n tipo de recurso erroneo \n") ; break ;
  case -2 : escribirStr("\n\n demasiados vectores de interrupcion \n") ; break ;
  case -3 : escribirStr("\n\n CONSOLA nombre de recurso ya existente \n") ; break ;
  case -4 : escribirStr("\n\n no hay descriptores de recurso libres \n") ; break ;
  case -5 : escribirStr("\n\n numero de vector de interrupcion ya usado \n") ; break ;
  default : escribirStr("\n\n no se ha podido crear el recurso CONSOLA \n") ;
  }
  return(-1) ;
}

static int desintegrarConsola ( void ) {
  int res ;
  res = eliminarCcbRecurso(
          (callBack_t)pointer(_CS, (word_t)cbForTimer),
          pointer(_DS, (word_t)&"TIMER")
  ) ;
  if (res != 0)
    escribirStr("\n\n no pudo eliminarse el callback del TIMER \n") ;
  else if (ll_devolverBloque(seg((pointer_t)descConsola),            /* GM */
                             ((maxConsolas*sizeof(descConsola_t))+15)/16))
    return(-1) ;
/*  finBiosCrt() ; */
  return(res) ;
}

static int comprobarAmpersand ( void ) {
  int i ;
  obtenInfoPS((descProceso_t far *)&descProceso, (e2PFR_t far *)&e2PFR, (c2c_t far *)&c2cPFR) ;

  for ( i = 0 ; i < tamComando ; i++ )
    comando[0][i] = descProceso[getpindx()].comando[i] ;

  inicScanner() ;
  while (car != (char)0) obtenCar() ;
  if (comando[0][pos] != '&') { /* truco: crearProceso deja ahi &, <, >, | */
    escribirStr("\n\n este driver debe ejecutarse con & \n") ;
    return(-1) ;
  }
  return(0) ;
}

static int instalarConRat ( byte_t numConsolas ) {
  int res, resRat ;
  res = comprobarAmpersand() ;
  if (res != 0) return(res) ;
  obtenInfoSO1(dirDescSO1) ;                 /* obtenemos los datos de SO1 */
  *((word_t far *)pointer(_CS, (word_t)segDatos)) = _DS ;     /* guardo DS */
  res = integrarConsola(numConsolas) ;
  if (res != 0) return(res) ;
  resRat = integrarRaton() ;
  esperarDesinstalacion() ;                          /* bloquea el proceso */
  if (resRat == 0) desintegrarRaton() ;
  res = desintegrarConsola() ;
  return(res) ;
}

void main ( int argc, char * argv [ ] ) {
  int res ;
  if (argc > 3) formato() ;
  else if (argc == 1) exit(instalarConRat(numConsolasPorDefecto)) ;
  else if (argc == 2) {
    if (iguales(argv[1], "-h") || iguales(argv[1], "-H")) help() ;
    else if (iguales(argv[1], "-i") || iguales(argv[1], "-I"))
      exit(instalarConRat(numConsolasPorDefecto)) ;
    else if (iguales(argv[1], "-u") || iguales(argv[1], "-U")) {
      res = destruirRecurso("RATON") ;
      switch (res) {
      case  0 : escribirStr("\n\n recurso RATON desinstalado \n") ; break ;
      case -1 : escribirStr("\n\n recurso RATON no existe \n") ; break ;
      case -2 : escribirStr("\n\n recurso RATON en uso \n") ; break ;
      case -3 : escribirStr("\n\n fichero RATON no puede destruirse \n") ; break ;
      default : escribirStr("\n\n RATON no ha podido desinstalarse \n") ;
      }
      res = destruirRecurso("CONSOLA") ;
      switch (res) {
      case  0 : escribirStr("\n\n recurso CONSOLA desinstalado \n") ; break ;
      case -1 : escribirStr("\n\n recurso CONSOLA no existe \n") ; break ;
      case -2 : escribirStr("\n\n recurso CONSOLA en uso \n") ; break ;
      case -3 : escribirStr("\n\n fichero CONSOLA no puede destruirse \n") ; break ;
      default : escribirStr("\n\n CONSOLA no ha podido desinstalarse \n") ;
      }
      exit(res) ;
    }
  }
  if ((argc == 2) ||
      ((argc == 3) &&
       (iguales(argv[1], "-i") || iguales(argv[1], "-I"))
      )
     ) {
    copiarStr(argv[argc-1], comando[0]) ;
    inicScanner() ;
    obtenSimb() ;
    if (simb == s_numero) {
      if (num == 0) {
        escribirStr("\n\n numConsolas debe ser > 0 \n") ;
        exit(-1) ;
      }
      else exit(instalarConRat(num)) ;
    }
  }
  formato() ;
}


