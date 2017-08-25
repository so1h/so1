/* ----------------------------------------------------------------------- */
/*                                consola.c                                */
/* ----------------------------------------------------------------------- */
/*  version del driver de la consola (teclado+pantalla)  [con multiscreen] */
/* ----------------------------------------------------------------------- */

/*                                version 2                                */
/*                         si admite multiscreen                           */
/*          debe enlazarse con el codigo del sistema operativo             */
/*     en la constante maxConsolas se establece el numero de consolas      */
/*       admite gran numero de consolas ya que la memoria necesaria        */
/*         para las consolas se toma de un nuevo segmento de datos         */

#include <so1pub.h\def_tecl.h>
#include <so1pub.h\def_proc.h>                            /* fedCaracteres */
#include <so1pub.h\bios_0.h>                /* leerTeclaBIOS, printCarBIOS */
#include <so1pub.h\biosdata.h>                              /* ptrBiosArea */
#include <so1pub.h\strings.h>                                 /* copiarStr */
#include <so1pub.h\carsctrl.h>                                   /* CR, LF */
#include <so1pub.h\printgen.h>
#include <so1pub.h\ptrc2c.h>                                   /* inicPC2c */
#include <so1pub.h\pantalla.h>                               /* pantalla_t */
#include <so1pub.h\ll_s_so1.h>                        /* encolarCcbRecurso */
#include <so1pub.h\def_timer.h>                              /* argCbTimer */
#include <so1pub.h\pic.h>                                   /* IRQ_TECLADO */
#include <so1.h\ajustes.h>                   /* bloquearProcesoActual, ... */
#include <so1.h\procesos.h>                            /* indProcesoActual */
#include <so1.h\recursos.h>                         /* crearRec, crearFich */
#include <so1.h\interrup.h>                                       /* VIOrg */
#include <so1.h\memvideo.h>                      /* ptrPant, redimensionar */
#include <so1.h\plot.h>                                            /* plot */
#include <so1.h\gm.h>                                    /* k_buscarBloque */

rindx_t rec_consola ;

#define nVIntConsola 0x09     /* numero de vector utilizado por el teclado */

void far isr_consola ( void ) ;                                 /* forward */

typedef struct {
  pindx_t    propietario ;            /* proceso propietario de la consola */
  pantalla_t pantalla ;
  teclado_t  teclado ;
  byte_t     F ;                         /* fila de la posicion del cursor */
  byte_t     C ;                      /* columna de la posicion del cursor */
} descConsola_t ;

byte_t maxConsolas ;

descConsola_t far * descConsola ;

word_t consolaDeSuperficie = 0 ;

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
    case FF  : borrarPantalla(pantalla, numFilas) ;
               descConsola[con].F = 0 ;
               descConsola[con].C = 0 ;
               break ;
    case CR  : descConsola[con].C = 0 ;
               break ;
    case LF  : if (++descConsola[con].F == numFilas) {
                 scrollPantalla(pantalla, numFilas) ;
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
          scrollPantalla(pantalla, numFilas) ;
          descConsola[con].F = numFilas-1 ;
        }
      }
    }
  }
  return(0) ;
}

byte_t congen ;

int pCon ( char car ) {
  return(printCarConsola(congen, car)) ;
}

int printLnConsola ( byte_t con ) {
  congen = con ;
  return(printGenLn(pCon)) ;
}

int printStrConsola ( byte_t con, char * str ) {
  congen = con ;
  return(printGenStr(str, pCon)) ;
}

int printStrHastaConsola ( byte_t con, char * str, word_t n, bool_t lleno ) {
  congen = con ;
  return(printGenStrHasta(str, n, lleno, pCon)) ;
}

int printDecConsola ( byte_t con, word_t num, word_t l ) {
  congen = con ;
  return(printGenDec(num, l, pCon)) ;
}

int printIntConsola ( byte_t con, int num, word_t l ) {
  congen = con ;
  return(printGenInt(num, l, pCon)) ;
}

int printLIntConsola ( byte_t con, long int num, word_t l ) {
  congen = con ;
  return(printGenLInt(num, l, pCon)) ;
}

int printLDecConsola ( byte_t con, dword_t num, word_t l ) {
  congen = con ;
  return(printGenLDec(num, l, pCon)) ;
}

int printHexConsola ( byte_t con, word_t num, word_t l ) {
  congen = con ;
  return(printGenHex(num, l, pCon)) ;
}

int printLHexConsola ( byte_t con, dword_t num, word_t l ) {
  congen = con ;
  return(printGenLHex(num, l, pCon)) ;
}

int printPtrConsola ( byte_t con, pointer_t ptr ) {
  congen = con ;
  return(printGenPtr(ptr, pCon)) ;
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

#pragma warn -par

static int far openConsola ( int dfs, modoAp_t modo ) {
  return(0) ;
}

static int far releaseConsola ( int dfs ) {
  return(0) ;
}

static int far readConsola ( int dfs, pointer_t dir, word_t nbytes ) {
  int i = 0 ;
  char car ;
  int df = tramaProceso->BX ;
  word_t con = descFichero[dfs].menor ;
  teclado_t far * teclado = (teclado_t far *)&descConsola[con].teclado ;
  modoAp_t modoAp = descProceso[indProcesoActual].tfa[df].modoAp ;
  word_t nbytes0 = nbytes ;
  if (nbytes <= teclado->ncar) {
    while (nbytes > 0) {
      car = sacar(teclado) ;
      if ((car == CR) && (modoAp & O_TEXT)) car = LF ;
      dir[i++] = car ;
      nbytes-- ;
    }
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
  return(-1) ;
}

static int far aio_readConsola ( int dfs, pointer_t dir, word_t nbytes ) {
  int i = 0 ;
  char car ;
  int df = tramaProceso->BX ;
  word_t con = descFichero[dfs].menor ;
  teclado_t far * teclado = (teclado_t far *)&descConsola[con].teclado ;
  modoAp_t modoAp = descProceso[indProcesoActual].tfa[df].modoAp ;
  word_t nbARetornar0, nbARetornar ;
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
  return(nbARetornar0) ;
}

static int far writeConsola ( int dfs, pointer_t dir, word_t nbytes ) {
  char car ;
  int i ;
  word_t con = descFichero[dfs].menor ;
  for ( i = 0 ; i < nbytes ; i++ ) {
    car = dir[i] ;
    printCarConsola(con, car) ;
  }
  if (con == consolaDeSuperficie)
    goToXYHw(cursorF, cursorC) ;
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

int far cbForTimer ( void far * arg ) {                       /* call back */

  byte_t F0 ;
  byte_t C0 ;
  byte_t congen0 ;
  byte_t visibilidadAnt ;

  asm push ds
  establecerDS_SO1() ;

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

#define maxCbCN 0

static descCcb_t descCcbCN = { 0, 0, 0, maxCbCN, NULL } ;

void inicConsola ( byte_t maxCons ) {

  descRecurso_t dR ;
  pindx_t pindx ;
  dfs_t dfs ;
  word_t con ;
  int res ;
  char nomFich [ 12 ] ;

  maxConsolas = maxCons + 1 ;

  descConsola = (descConsola_t far *)
    pointer(
      k_buscarBloque(((maxConsolas*sizeof(descConsola_t))+15)/16),
      0x0000
    ) ;
/*
  printStrBIOS("\n descConsola = ") ;
  printPtrBIOS((pointer_t)descConsola) ;
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
    nbytesProceso[pindx] = 0 ;
    dirProceso[pindx] = (pointer_t)0x00000000 ;
  }

  dR.tipo = rDCaracteres ;
  copiarStr("CON", dR.nombre) ;
  dR.ccb = (ccb_t)&descCcbCN ;
  dR.pindx = indProcesoActual ;
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

  rec_consola = crearRec(&dR) ;

  copiarStr("CON0", nomFich) ;
  for ( con = 0 ; con < maxConsolas ; con++ ) {
    nomFich[3] = '0' + con ;
    crearFich(nomFich, rec_consola, con, fedCaracteres) ;
  }

  res = encolarCcbRecurso(
          (callBack_t)pointer(_CS, (word_t)cbForTimer),
          pointer(_DS, (word_t)&"TIMER")
  ) ;

  switch (res) {
  case  0 : break ;
  case -1 : printStrBIOS("\n CON: no existe el recurso TIMER ") ; break ;
  case -2 : printStrBIOS("\n CON: ccb de TIMER = ccbNulo ") ; break ;
  case -3 : printStrBIOS("\n CON: cbForTimer = cbNulo ") ; break ;
  case -4 : printStrBIOS("\n CON: ccb del TIMER lleno ") ; break ;
  default : printStrBIOS("\n CON: error desconocido en el TIMER ") ; break ;
  }

}

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

void far isr_consola ( void ) {

  word_t w ;
  char car ;
  static byte_t scanCode ;
  static byte_t scanCodeAnt ;
  static byte_t AltPulsada ;
  word_t nbytes ;
  pointer_t dir ;
  int df ;
  pindx_t pindx ;
  modoAp_t modoAp ;
  bool_t extendido ;
  teclado_t far * teclado ;
  byte_t con ;
  byte_t nuevaConsola = 0xFF ;
  word_t numFilasAnt = numFilas ;
  byte_t cursorFAux ;
  pantalla_t far * pantallaAux ;

  scanCodeAnt = scanCode ;

  asm in al,60h
  asm mov scanCode,al
/*
  printCarVideo('<') ;
  printHexVideo(scanCodeAnt, 2) ;
  printHexVideo(scanCode, 2) ;
  printCarVideo('>') ;
*/
  VIOrg[nVIntConsola]() ;       /* llamamos a la antigua rti (pushf + int) */

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

  w = leerTeclaListaBDA() ;

/* printHexBIOS(w, 4) ; printCarBIOS(' ') ; */
/* printHexVideo(w, 4) ; printCarVideo(' ') ; */

  if (w) {
    car = (char)w ;
    if (car == (char)0xE0)
      car = (char)0 ;
    extendido = (car == (char)0) ;

    teclado = (teclado_t far *)&descConsola[consolaDeSuperficie].teclado ;
/*
printStrVideo("\n numElem = ") ;
printDecVideo(teclado->bloqueados.numElem, 1) ;
printDecVideo(teclado->bloqueados.primero, 6) ;
printDecVideo(teclado->bloqueados.cabecera, 6) ;
printDecVideo(teclado->bloqueados.e[teclado->bloqueados.primero].sig, 6) ;
*/
    if (teclado->bloqueados.numElem > 0) {
      pindx = teclado->bloqueados.primero ;
      df = descProceso[pindx].trama->BX ;
      modoAp = descProceso[pindx].tfa[df].modoAp ;
      plot('k', car, contadorTimer0()) ;
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
        descProceso[pindx].trama->AX =
          descProceso[pindx].trama->CX ;                        /* nbytes0 */
        descProceso[pindx].tfa[df].pos +=
          descProceso[pindx].trama->CX ;
        descProceso[pindx].estado = preparado ;
        encolarPC2c(pindx, (ptrC2c_t)&c2cPFR[PUrgentes]) ;
        activarAlEpilogo1 = TRUE ;
        if (indProcesoActual > -1) {                        /* nivel 1 o 2 */
          descProceso[indProcesoActual].trama =
            (trama_t far *)pointer(SS_Proceso, SP_Proceso) ;
          descProceso[indProcesoActual].estado = preparado ;
          encolarPC2c(indProcesoActual, (ptrC2c_t)&c2cPFR[PPreparados]) ;
          indProcesoActual = -1 ;
          activarAlEpilogo1 = TRUE ;
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
}

