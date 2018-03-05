/* ----------------------------------------------------------------------- */
/*                                consola.c                                */
/* ----------------------------------------------------------------------- */
/*  version del driver de la consola (teclado+pantalla)  [con multiscreen] */
/* ----------------------------------------------------------------------- */

/*                                version 1                                */
/*                         si admite multiscreen                           */
/*          debe enlazarse con el codigo del sistema operativo             */
/*     en la constante maxConsolas se establece el numero de consolas      */
/*   el numero de consolas esta muy limitado ya que la memoria necesaria   */
/*      para las consolas se toma del segmento de datos del programa       */

#include <so1pub.h\def_tecl.h>
#include <so1pub.h\def_pant.h>                               /* pantalla_t */
#include <so1pub.h\def_proc.h>                            /* fedCaracteres */
#include <so1pub.h\bios_0.h>                /* leerTeclaBIOS, printCarBIOS */
#include <so1pub.h\biosdata.h>                              /* ptrBiosArea */
#include <so1pub.h\strings.h>                                 /* copiarStr */
#include <so1pub.h\pic.h>                                   /* IRQ_TECLADO */
#include <so1.h\ajustes.h>                   /* bloquearProcesoActual, ... */
#include <so1.h\procesos.h>                            /* indProcesoActual */
#include <so1.h\recursos.h>                         /* crearRec, crearFich */
#include <so1.h\interrup.h>                                       /* VIOrg */
#include <so1.h\memvideo.h>                                     /* ptrPant */
#include <so1.h\plot.h>                                            /* plot */

rindx_t rec_consola ;

#define nVIntConsola 0x09     /* numero de vector utilizado por el teclado */

void far isr_consola ( void ) ;                                 /* forward */

typedef struct {
  pindx_t    propietario ;            /* proceso propietario de la consola */
  pantalla_t pantalla ;
  teclado_t  teclado ;
  byte_t     cursorF ;                   /* fila de la posicion del cursor */
  byte_t     cursorC ;                /* columna de la posicion del cursor */
} descConsola_t ;

#define maxConsolas 2 /* valores mas altos pueden dar problemas de memoria */

descConsola_t descConsola [ maxConsolas ];

word_t consolaDeSuperficie ;

word_t nbytesProceso [ maxProcesos ] ;        /* tabla de bytes pendientes */

pointer_t dirProceso [ maxProcesos ] ;     /* tabla de direcciones destino */

void meter ( char car, teclado_t * teclado ) {
  int in = teclado->in ;
  if (teclado->ncar < tamBTeclado) {
    teclado->bufer[in++] = car ;
    if (in >= tamBTeclado) in = 0 ;
    teclado->in = in ;
    teclado->ncar++ ;
  }
}

char sacar ( teclado_t * teclado  ) {    /* precondicion: teclado.ncar > 0 */
  char car ;
  int out = teclado->out ;
  car = teclado->bufer[out++] ;
  if (out >= tamBTeclado) out = 0 ;
  teclado->out = out ;
  teclado->ncar-- ;
  return(car) ;
}

void borrarPantalla ( pantalla_t * pantalla ) {
  byte_t F, C ;
  for ( F = 0 ; F < 25 ; F++ )
    for ( C = 0 ; C < 80 ; C++ ) {
      pantalla->t[F][C].car = ' ' ;
      pantalla->t[F][C].atr = atrNormal ;
    }
}

typedef struct {
  word_t w [24*80] ;
} * ptrStructScroll_t ;

void scrollPantalla ( pantalla_t * pantalla ) {
/*
  ptrStructScroll_t ptrStructScroll = (ptrStructScroll_t )&pantalla->t[1] ;
  *((ptrStructScroll_t)pantalla) = *ptrStructScroll ;
*/
  byte_t F, C ;
  for ( F = 0 ; F < 24 ; F++ ) {
    for ( C = 0 ; C < 80 ; C++ ) {
      pantalla->t[F][C].car = pantalla->t[F+1][C].car ;
      pantalla->t[F][C].atr = pantalla->t[F+1][C].atr ;
    }
  }
  for ( C = 0 ; C < 80 ; C++ ) {
    pantalla->t[24][C].car = ' ' ;
    pantalla->t[24][C].atr = atrNormal ;
  }
}

void establecerConsolaDeSuperficie ( void ) {
  byte_t F, C ;
  byte_t pag = 0 ;
  pantalla_t * pantalla = (pantalla_t *)&descConsola[consolaDeSuperficie].pantalla ;
  for ( F = 0 ; F < 25 ; F++ ) {
    for ( C = 0 ; C < 80 ; C++ ) {
      ptrPant->t[F][C].car = pantalla->t[F][C].car ;
      ptrPant->t[F][C].atr = pantalla->t[F][C].atr ;
    }
  }
  goToXYPag(
    descConsola[consolaDeSuperficie].cursorF,
    descConsola[consolaDeSuperficie].cursorC,
    pag
  ) ;
}

void salvarConsolaDeSuperficie ( void ) {
  byte_t F, C ;
  byte_t pag = 0 ;
  pantalla_t * pantalla = (pantalla_t *)&descConsola[consolaDeSuperficie].pantalla ;
  for ( F = 0 ; F < 25 ; F++ ) {
    for ( C = 0 ; C < 80 ; C++ ) {
      pantalla->t[F][C].car = ptrPant->t[F][C].car ;
      pantalla->t[F][C].atr = ptrPant->t[F][C].atr ;
    }
  }
  descConsola[consolaDeSuperficie].cursorF = ptrBiosArea->CURSOR_pos[pag][1] ;
  descConsola[consolaDeSuperficie].cursorC = ptrBiosArea->CURSOR_pos[pag][0] ;
}

#pragma warn -par

static int far openConsola ( int dfs, modoAp_t modo ) {
/*
  if iguales(descFixhero[dfs].nombre, "CON")
    if (c2cConsolas.numElen > 0)
      return(desencolarPC2c(&c2cConsolas))
    else
      return(-1) ;
*/
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
  teclado_t * teclado = &descConsola[con].teclado ;
  modoAp_t modoAp = descProceso[indProcesoActual].tfa[df].modoAp ;
  word_t nbytes0 = nbytes ;
  if (nbytes <= teclado->ncar) {
    while (nbytes > 0) {
      car = sacar(teclado) ;
      if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
      dir[i++] = car ;
      nbytes-- ;
    }
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
  return(-1) ;
}

static int far aio_readConsola ( int dfs, pointer_t dir, word_t nbytes ) {
  int i = 0 ;
  char car ;
  int df = tramaProceso->BX ;
  word_t con = descFichero[dfs].menor ;
  teclado_t * teclado = &descConsola[con].teclado ;
  modoAp_t modoAp = descProceso[indProcesoActual].tfa[df].modoAp ;
  word_t nbARetornar0, nbARetornar ;
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
  return(nbARetornar0) ;
}

static int far writeConsola ( int dfs, pointer_t dir, word_t nbytes ) {
  byte_t pag = 0 ;
  char car ;
  int i ;
  word_t con = descFichero[dfs].menor ;
  if (con == consolaDeSuperficie) {
    for ( i = 0 ; i < nbytes ; i++ ) {
      car = dir[i] ;
      switch (car) {
      case '\f' : goToXYPag(0, 0, pag) ;
                  clrScrPagBDA(pag) ;                     /* clrScrBIOS () ; */
                  setCursorBIOS(12, 15 ) ;
                  break ;
      default   : printCarBIOS(car) ;
      }
    }
  }
  else {
    pantalla_t * pantalla = &descConsola[con].pantalla ;
    byte_t cursorF = descConsola[con].cursorF ;
    byte_t cursorC = descConsola[con].cursorC ;
    for ( i = 0 ; i < nbytes ; i++ ) {
      car = dir[i] ;
      switch (car) {
      case '\f' : borrarPantalla(pantalla) ;
                  descConsola[con].cursorF = 0 ;
                  descConsola[con].cursorC = 0 ;
                  break ;
      case '\r' : descConsola[con].cursorC = 0 ;
                  break ;
      case '\n' : if (++descConsola[con].cursorF == 25) {
                    scrollPantalla(pantalla) ;
                    descConsola[con].cursorF = 24 ;
                  }
                  break ;
      case '\a' : printCarBIOS(dir[i]) ;
                  break ;
      default  :
        pantalla->t[cursorF][cursorC].car = dir[i] ;
        if (++descConsola[con].cursorC == 80) {
          descConsola[con].cursorC = 0 ;
          if (++descConsola[con].cursorF == 25) {
            scrollPantalla(pantalla) ;
            descConsola[con].cursorF = 24 ;
          }
        }
      }
    }
  }
  return(nbytes) ;
}

static int far aio_writeConsola ( int dfs, pointer_t dir, word_t nbytes ) {
  return(writeConsola(dfs, dir, nbytes)) ;
}

static int far lseekConsola ( int dfs, long int pos, word_t whence ) {
  return(0) ;
}

static int far fcntlConsola ( int dfs, word_t cmd, word_t arg ) {
  return(0) ;
}

static int far ioctlConsola ( int dfs, word_t request, word_t arg ) {
  return(0) ;
}

#pragma warn +par

#define maxCbCN 0

static descCcb_t descCcbCN = { 0, 0, 0, maxCbCN, NULL } ;

void inicConsola ( void ) {

  descRecurso_t dR ;
  pindx_t pindx ;
  dfs_t dfs ;
  word_t con ;

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
    borrarPantalla(&descConsola[con].pantalla) ;
    descConsola[con].cursorF = 0 ;
    descConsola[con].cursorC = 0 ;
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

  crearFich("CON", rec_consola, 0, fedCaracteres) ;
  crearFich("CON1", rec_consola, 1, fedCaracteres) ;

  setCursorBIOS(12, 15 ) ;
  goToXYPag(0, 0, 0) ;
  clrScrPagBDA(0) ;                    /* mejor asi para no llamar al BIOS */
  establecerPaginaActiva(0) ;
  setCursorBIOS(12, 15 ) ;
}

#define AltF1  (byte_t)0x68
#define AltF2  (byte_t)0x69
#define AltF3  (byte_t)0x6a
#define AltF4  (byte_t)0x6b
#define AltF5  (byte_t)0x6c
#define AltF6  (byte_t)0x6d
#define AltF7  (byte_t)0x6e
#define AltF8  (byte_t)0x6f

#define AltF9  (byte_t)0x70
#define AltF10 (byte_t)0x71

void far isr_consola ( void ) {

  word_t w ;
  char car ;
  byte_t scanCode ;
  word_t nbytes ;
  pointer_t dir ;
  int df ;
  pindx_t pindx ;
  modoAp_t modoAp ;
  bool_t extendido ;
  teclado_t * teclado ;
/*
  asm in al,60h
  asm mov scanCode,al
  printCarBIOS('<') ;
  printHexBIOS(scanCode, 2) ;
  printCarBIOS('>') ;
*/
  VIOrg[nVIntConsola]() ;       /* llamamos a la antigua rti (pushf + int) */

  w = leerTeclaListaBDA() ;

  /* printHexBIOS(w, 4) ; printCarBIOS(' ') ; */

  if (w) {

    scanCode = (byte_t)(w >> 8) ;
    if ((AltF1 <= scanCode) && (scanCode < (AltF1+maxConsolas))) {
      if (consolaDeSuperficie != (scanCode - AltF1)) {
        salvarConsolaDeSuperficie() ;
        consolaDeSuperficie = scanCode - AltF1 ;
        establecerConsolaDeSuperficie() ;
      }
      return ;
    }
    else if (scanCode == AltF10) {
      asm int 3 ;                                   /* AltF10 para depurar */
      return ;
    }

    car = (char)w ;
    if (car == (char)0xE0)
      car = (char)0 ;
    extendido = (car == (char)0) ;

    teclado = (teclado_t *)&descConsola[consolaDeSuperficie].teclado ;
    if (teclado->bloqueados.numElem > 0) {
      pindx = teclado->bloqueados.primero ;
      df = descProceso[pindx].trama->BX ;
      modoAp = descProceso[pindx].tfa[df].modoAp ;
      plot('k', car, contadorTimer0()) ;
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

