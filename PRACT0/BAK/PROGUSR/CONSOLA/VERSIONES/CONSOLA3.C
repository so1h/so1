/* ----------------------------------------------------------------------- */
/*                                consola.c                                */
/* ----------------------------------------------------------------------- */
/*    version del recurso consola (teclado+pantalla)  [con multiscreen]    */
/* ----------------------------------------------------------------------- */

/*                                version 3                                */
/*                           admite multiscreen                            */
/*                   se ejecuta como un proceso aparte                     */
/*     la constante maxConsolas establece el numero maximo de consolas     */
/*       admite gran numero de consolas ya que la memoria necesaria        */
/*         para las consolas se toma de un nuevo segmento de datos         */

#define SPInicial 0x3FFE      /* Valor inicial puntero de pila del proceso */

#include <so1pub.h\ajustusr.h>                                 /* valor_DS */
#include <so1pub.h\comundrv.h>                 /* ptrIndProcesoActual, ... */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>
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

info_t info ;                                                /* depuracion *//**************/

static rindx_t rec_consola ;

/* ----------------------------------------------------------------------- */
/*                   seccion de implementacion del driver                  */
/* ----------------------------------------------------------------------- */

#define nVIntConsola 0x09     /* numero de vector utilizado por el teclado */

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
    case '\f' : borrarPantalla(pantalla, numFilas) ;
                descConsola[con].F = 0 ;
                descConsola[con].C = 0 ;
                break ;
    case 'r'  : descConsola[con].C = 0 ;
                break ;
    case '\n' : if (++descConsola[con].F == numFilas) {
                  scrollPantalla(pantalla, numFilas) ;
                  descConsola[con].F = numFilas-1 ;
                }
                break ;
    case '\b' : if (C > 0) {
                  C-- ;
                  pantalla->t[F][C].car = ' ' ;
                  descConsola[con].C = C ;
                }
                else printCarBIOS(car) ;
                break ;
    case '\t' : car = ' ' ;
    case '\a' : printCarBIOS(car) ;
                break ;
    default   :
      pantalla->t[F][C].car = car ;
      if (++descConsola[con].C == 80) {
        descConsola[con].C = 0 ;
        if (++descConsola[con].F == numFilas) {
          scrollPantalla(pantalla, numFilas) ;
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
  byte_t F, C ;
  pantalla_t far * pantalla =
    (pantalla_t far *)&descConsola[consolaDeSuperficie].pantalla ;
  copiarPantalla(pantalla, ptrPant, numFilas) ;
  goToXYHw(
    descConsola[consolaDeSuperficie].F,
    descConsola[consolaDeSuperficie].C
  ) ;
  cursorF = descConsola[consolaDeSuperficie].F ;
  cursorC = descConsola[consolaDeSuperficie].C ;
}

void salvarConsolaDeSuperficie ( void ) {
  byte_t F, C ;
  pantalla_t far * pantalla =
    (pantalla_t far *)&descConsola[consolaDeSuperficie].pantalla ;
  copiarPantalla(ptrPant, pantalla, numFilas) ;
  descConsola[consolaDeSuperficie].F = cursorF ;
  descConsola[consolaDeSuperficie].C = cursorC ;
}

/* ----------------------------------------------------------------------- */

#pragma warn -par

static int far openConsola ( int dfs, modoAp_t modo ) {
  return(0) ;
}

static int far releaseConsola ( int dfs ) {
  return(0) ;
}

static int far readConsola ( int dfs, pointer_t dir, word_t nbytes ) {

  word_t DS_Consola = valor_DS ;
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
      if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
      dir[i++] = car ;
      nbytes-- ;
    }
    asm pop ds                    /* 2013-07-09: faltaba esto (sino CRASH) */
    return(nbytes0) ;
  }
  else {
    while (teclado->ncar > 0) {
      car = sacar(teclado) ;
      if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
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

  word_t DS_Consola = valor_DS ;
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
    if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
    dir[i++] = car ;
    nbARetornar-- ;
  }

  asm pop ds
  return(nbARetornar0) ;
}

static int far writeConsola ( int dfs, pointer_t dir, word_t nbytes ) {
	
  word_t DS_Consola = valor_DS ;
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

#pragma warn +par

/* ----------------------------------------------------------------------- */
/*                  rutina de servicio de la interrupcion                  */
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

  word_t DS_Consola = valor_DS ;
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
            copiarPantalla((pantalla_t far *)&ptrPant->t[cursorF-numFilas+1][0], ptrPant, numFilas) ;
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
                copiarPantalla((pantalla_t far *)&pantallaAux->t[cursorFAux-numFilas+1][0], pantallaAux, numFilas) ;
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
      if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
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

static int printStrConsola ( byte_t con, char * str ) {
  congen = con ;
  return(printGenStr(str, pCon)) ;
}

static int printStrHastaConsola ( byte_t con, char * str, word_t n, bool_t lleno ) {
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

  word_t DS_Consola = valor_DS ;
  byte_t F0 ;
  byte_t C0 ;
  byte_t congen0 ;
  byte_t visibilidadAnt ;
  trama_t far * tramaProceso ;

  asm push ds
  asm mov ds,DS_Consola

  tramaProceso = (trama_t far *)pointer(*ptrSS_Proceso, *ptrSP_Proceso) ;

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
/*                      seccion de programa de usuario                     */
/* ----------------------------------------------------------------------- */

static void formato ( void ) {
  escribirStr("\n\n formato: CONSOLA [ [ -i ] [ num ] | -u | -h ] \n") ;
  exit(-1) ;
}

static void help ( void ) {
  escribirLn() ;
  escribirLn() ;
  escribirStr(" formato : CONSOLA [ [ -i ] [ num ] | -u | -h ] \n\n") ;
  escribirStr(" instala/desinstala el driver de la consola     \n\n") ;
  escribirStr(" opciones: (por defecto -i y num = 6)           \n\n") ;
  escribirStr("      -i : instala el driver (usar &)           \n") ;
  escribirStr("     num : numero de consolas                   \n") ;
  escribirStr("      -u : desintala el driver                  \n") ;
  escribirStr("      -h : muestra este help                    \n") ;
  exit(0) ;
}

descProceso_t descProceso [ maxProcesos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

#define maxCbConsola 0

static descCcb_t descCcbConsola = { 0, 0, 0, maxCbConsola, NULL } ;

static int instalarConsola ( byte_t numConsolas ) {

  int i ;
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

  obtenInfoPS((descProceso_t far *)&descProceso, (e2PFR_t far *)&e2PFR, (c2c_t far *)&c2cPFR) ;

  for ( i = 0 ; i < tamComando ; i++ )
    comando[0][i] = descProceso[getpindx()].comando[i] ;

  inicScanner() ;
  while (car != (char)0) obtenCar() ;
  if (comando[0][pos] != '&') { /* truco: crearProceso deja ahi &, <, >, | */
    escribirStr("\n\n este driver debe ejecutarse con & \n") ;
    return(-1) ;
  }
  else {

    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */

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

      esperarDesinstalacion(0) ;                     /* bloquea el proceso */

      res = eliminarCcbRecurso(
              (callBack_t)pointer(_CS, (word_t)cbForTimer),
              pointer(_DS, (word_t)&"TIMER")
      ) ;
      if (res != 0)
        escribirStr("\n\n no pudo eliminarse el callback del TIMER \n") ;
      else if (ll_devolverBloque(seg((pointer_t)descConsola),        /* GM */
                 ((maxConsolas*sizeof(descConsola_t))+15)/16))
        return(-1) ;
/*    finBiosCrt() ; */
      return(res) ;
    }
    else switch(rec_consola) {
    case -1 : escribirStr("\n\n tipo de recurso erroneo \n") ; break ;
    case -2 : escribirStr("\n\n demasiados vectores de interrupcion \n") ; break ;
    case -3 : escribirStr("\n\n CONSOLA nombre de recurso ya existente \n") ; break ;
    case -4 : escribirStr("\n\n no hay descriptores de recurso libres \n") ; break ;
    case -5 : escribirStr("\n\n numero de vector de interrupcion ya usado \n") ; break ;
    default : escribirStr("\n\n no se ha podido crear el recurso CONSOLA \n") ;
    }
    return(-1) ;
  }
}

void main ( int argc, char * argv [ ] ) {
  int res ;
  if (argc > 3) formato() ;
  else if (argc == 1) exit(instalarConsola(numConsolasPorDefecto)) ;
  else if (argc == 2) {
    if (iguales(argv[1], "-h") || iguales(argv[1], "-H")) help() ;
    else if (iguales(argv[1], "-i") || iguales(argv[1], "-I"))
      exit(instalarConsola(numConsolasPorDefecto)) ;
    else if (iguales(argv[1], "-u") || iguales(argv[1], "-U")) {
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
      else exit(instalarConsola(num)) ;
    }
  }
  formato() ;
}

