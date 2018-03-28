/* ----------------------------------------------------------------------- */
/*                                consola.c                                */
/* ----------------------------------------------------------------------- */
/*   la version mas sencilla del driver de la consola (teclado+pantalla)   */
/* ----------------------------------------------------------------------- */

/*                                version 0                                */
/*                         no admite multiscreen                           */
/*          debe enlazarse con el codigo del sistema operativo             */

#include <so1pub.h\def_tecl.h>
#include <so1pub.h\def_proc.h>                            /* fedCaracteres */
#include <so1pub.h\bios_0.h>                /* leerTeclaBIOS, printCarBIOS */
#include <so1pub.h\strings.h>                                 /* copiarStr */
#include <so1pub.h\pic.h>                                   /* IRQ_TECLADO */
#include <so1.h\ajustes.h>                   /* bloquearProcesoActual, ... */
#include <so1.h\procesos.h>                            /* indProcesoActual */
#include <so1.h\recursos.h>                         /* crearRec, crearFich */
#include <so1.h\interrup.h>                                       /* VIOrg */
#include <so1.h\plot.h>                                            /* plot */

rindx_t rec_consola ;

#define nVIntConsola 0x09     /* numero de vector utilizado por el teclado */

void far isr_consola ( void ) ;                                 /* forward */

teclado_t teclado ;

word_t nbytesProceso [ maxProcesos ] ;        /* tabla de bytes pendientes */

pointer_t dirProceso [ maxProcesos ] ;     /* tabla de direcciones destino */

void meter ( char car ) {
  int in = teclado.in ;
  if (teclado.ncar < tamBTeclado) {
    teclado.bufer[in++] = car ;
    if (in >= tamBTeclado) in = 0 ;
    teclado.in = in ;
    teclado.ncar++ ;
  }
}

char sacar ( void ) {                    /* precondicion: teclado.ncar > 0 */
  char car ;
  int out = teclado.out ;
  car = teclado.bufer[out++] ;
  if (out >= tamBTeclado) out = 0 ;
  teclado.out = out ;
  teclado.ncar-- ;
  return(car) ;
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
  modoAp_t modoAp = descProceso[indProcesoActual].tfa[df].modoAp ;
  word_t nbytes0 = nbytes ;
  if (nbytes <= teclado.ncar) {
    while (nbytes > 0) {
      car = sacar() ;
      if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
      dir[i++] = car ;
      nbytes-- ;
    }
    return(nbytes0) ;
  }
  else {
    while (teclado.ncar > 0) {
      car = sacar() ;
      if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
      dir[i++] = car ;
      nbytes-- ;
    }
    nbytesProceso[indProcesoActual] = nbytes ;
    dirProceso[indProcesoActual] = (pointer_t)&dir[i] ;
    encolarPC2c(indProcesoActual, &teclado.bloqueados) ;
    bloquearProcesoActual(rec_consola) ;          /* no se retorna de aqui */
  }
  return(-1) ;
}

static int far aio_readConsola ( int dfs, pointer_t dir, word_t nbytes ) {
  int i = 0 ;
  char car ;
  int df = tramaProceso->BX ;
  modoAp_t modoAp = descProceso[indProcesoActual].tfa[df].modoAp ;
  word_t nbytes0 = nbytes ;
  if (nbytes <= teclado.ncar) {
    while (nbytes > 0) {
      car = sacar() ;
      if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
      dir[i++] = car ;
      nbytes-- ;
    }
    return(nbytes0-nbytes) ;
  }
  return(0) ;
}

static int far writeConsola ( int dfs, pointer_t dir, word_t nbytes ) {
  byte_t pag = 0 ;
  char car ;
  int i ;
  for ( i = 0 ; i < nbytes ; i++ ) {
    car = dir[i] ;
    switch (car) {
    case '\f' : goToXYPag(0, 0, pag) ;
                clrScrPagBDA(pag) ;                     /* clrScrBIOS () ; */
                setCursorBIOS(12, 15 ) ;
                break ;
    default : printCarBIOS(car) ;
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

  teclado.in = 0 ;
  teclado.out = 0 ;
  teclado.ncar = 0 ;
  inicPC2c(&teclado.bloqueados, &teclado.e2Bloqueados, maxProcesos + 0, TRUE) ;

  teclado.inRaw = 0 ;
  teclado.outRaw = 0 ;
  teclado.nRaw = 0 ;
  inicPC2c(&teclado.bloqueadosRaw, &teclado.e2Bloqueados, maxProcesos + 1, TRUE) ;

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

  setCursorBIOS(12, 15 ) ;
  goToXYPag(0, 0, 0) ;
  clrScrPagBDA(0) ;                    /* mejor asi para no llamar al BIOS */
  establecerPaginaActiva(0) ;
  setCursorBIOS(12, 15 ) ;
}

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
    car = (char)w ;
    if (car == (char)0xE0)
      car = (char)0 ;
    extendido = (car == (char)0) ;
    if (teclado.bloqueados.numElem > 0) {
      pindx = teclado.bloqueados.primero ;
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
          meter(car) ;
      }
      nbytesProceso[pindx] = nbytes ;
      dirProceso[pindx] = dir ;
      if (nbytes == 0) {
        desencolarPC2c(&teclado.bloqueados) ;
        descProceso[pindx].trama->AX =
          descProceso[pindx].trama->CX ;                /* nbytes0 */
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
      meter(car) ;                                                  /* car */
      if (extendido) {
        scanCode = (byte_t)(w >> 8) ;
        car = (char)scanCode ;                                 /* scanCode */
        meter(car) ;
      }
    }
  }
}

