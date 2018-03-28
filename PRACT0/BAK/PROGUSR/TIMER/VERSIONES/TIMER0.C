/* ----------------------------------------------------------------------- */
/*                                 timer.c                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\puertos.h>
#include <so1pub.h\strings.h>
#include <so1pub.h\pic.h>
#include <so1pub.h\ccb.h>                                    /* atenderCcb */
#include <so1.h\ajustes.h>
#include <so1.h\bios.h>
#include <so1.h\interrup.h>
#include <so1.h\procesos.h>
#include <so1.h\recursos.h>
#include <so1.h\raton.h>
#include <so1.h\sonido.h>
#include <so1.h\textwin.h>
#include <so1.h\timer.h>
#include <so1.h\plot.h>

rindx_t rec_timer ;

#define nVIntTimer   0x08       /* numero de vector utilizado por el timer */

void far isr_timer ( void ) ;                                   /* forward */

argCbTimer_t argCbTimer ;

static dobleEnlace_t eTimer [ maxProcesos + 1 ] ;

static c2c_t colaTimer ;

static int ticsPorLeer [ maxProcesos ] ;                  /* tics por leer */

static pointer_t dirDestino [ maxProcesos ] ;  /* dir. donde dejar sig tic */

word_t ticsPorRodaja = 18 ;

word_t ticsVerReloj  = 4 ;

word_t contTicsReloj = 0 ;                   /* 0, 1, 2, ... ticsVerReloj */

char ocioso [4] = { '|', '/', '-', '\\' } ;

/* ----------------------------------------------------------------------- */
/*            Rutina de servicio de la interrupcion del timer              */
/* ----------------------------------------------------------------------- */

void procesarTicReloj_verReloj ( void ) ;

#if (FALSE)

void procesarTicReloj_verReloj ( void ) {

  static ptrWindow_t winActivaAux ;
  static word_t cursorFAnt ;
  static word_t cursorCAnt ;
  static pindx_t pindx ;
  static int i ;

  contTicsReloj = 0 ;
  if (++contOcioso == 4) contOcioso = 0 ;

  winActivaAux = winActiva ;
  winActiva = ptrWinNIL ;

  cursorFAnt = win_so->cursorF ;
  cursorCAnt = win_so->cursorC ;

  win_so->cursorF = win_so->esqSupIzF ;
  win_so->cursorC = win_so->esqInfDeC-6 ;
  printLDecWin(win_so, contRodajas, 7) ;

  win_so->cursorF = win_so->esqSupIzF+1 ;
  win_so->cursorC = win_so->esqInfDeC-6 ;
  printStrWin(win_so, "  ") ;
  if (indProcesoActual == -1)
    printCarWin(win_so, ocioso[contOcioso]) ;
  else {
    /* printDecWin(win_so, descProceso[indProcesoActual].pid, 3) ; */
    printHexWin(win_so, indProcesoActual, 1) ;
  }
  /* printDecWin(win_so, descProceso[indProcesoDeSuperficie].pid, 2) ; */
  printCarWin(win_so, ' ') ;
  printHexWin(win_so, indProcesoDeSuperficie, 1) ;

  /* printDecWin(win_so, c2cPFR[PPreparados].numElem, 2) ; */    /* num procesos preparados */
  printCarWin(win_so, ' ') ;
  printHexWin(win_so, c2cPFR[PPreparados].numElem, 1) ;

  win_so->cursorF = win_so->esqSupIzF+2 ;
  win_so->cursorC =
    win_so->esqInfDeC-3-2*((c2cPFR[PPreparados].numElem <= 10) ? c2cPFR[PPreparados].numElem-1 : 10) ;
  /* printStrWin(win_so, "\n  ") ; */
  printStrWin(win_so, "  ") ;
  pindx = c2cPFR[PPreparados].primero ;
  for ( i = 0 ; (i < c2cPFR[PPreparados].numElem) && (i < 10) ; i++ ) {
    /* printDecWin(win_so, pindx, 2) ; */
    printCarWin(win_so, ' ') ;
    printHexWin(win_so, ((word_t)pindx) % 16, 1) ;
    pindx = c2cPFR[PPreparados].e[pindx].sig ;
  }
/*
  printDecWin(win_so, winList->num, 1) ;
  printDecWin(win_so, winList->sig->num, 2) ;
  printDecWin(win_so, winList->sig->sig->num, 2) ;
  printDecWin(win_so, winList->sig->sig->sig->num, 2) ;
*/

  win_so->cursorF = cursorFAnt ;
  win_so->cursorC = cursorCAnt ;

  winActiva = winActivaAux ;

}

#endif

#pragma warn -par

static int far openTimer ( int dfs, modoAp_t modo ) {
  return(0) ;
}

static int far releaseTimer ( int dfs ) {
  return(0) ;
}

static int far readTimer ( int dfs, pointer_t dir, word_t nbytes ) {

  /* El proceso se bloquea a la espera de que transcurran nbytes tics      */
  /* Si (dir != NULL) para cada siguiente tic i                            */
  /*   se guarda en dir[i] el bytes (contTics % 256)                       */

  ticsPorLeer[indProcesoActual] = nbytes ;
  if (seg(dir) == descProceso[indProcesoActual].CSProc)
    dirDestino[indProcesoActual] = dir ;
  else if (dir != NULL)
    dirDestino[indProcesoActual] = dir ;
  else
    dirDestino[indProcesoActual] = dir ;          /* solo espera sin copia */
  encolarPC2c(indProcesoActual, (ptrC2c_t)&colaTimer) ;
  bloquearProcesoActual(rec_timer) ;                         /* no retorna */
  return(-1) ;
}

#define tamBuf 12

static int far aio_readTimer ( int dfs, pointer_t dir, word_t nbytes ) {

  /* El proceso lee los bytes que solicita del "fichero" virtual:          */
  /*                                                                       */
  /*   TTTTTTTTRRRRRRRRCCCCXXXX                                            */
  /*                                                                       */
  /* donde TTTTTTTT son los 8 bytes de contTics                            */
  /* donde RRRRRRRR son los 8 bytes de contRodajas                         */
  /* donde CCCC     son los 8 bytes de contTicsRodaja                      */
  /* donde XXXX     son los 8 bytes de ticsPorRodaja                       */

  int i ;
  char buf [ tamBuf ] ;
  int df = tramaProceso->BX ;
  word_t pos = (word_t)descProceso[indProcesoActual].tfa[df].pos ;
  word_t posNueva ;
  if (pos >= tamBuf) nbytes = 0 ;
  else {
    posNueva = pos + nbytes ;
    *((dword_t far *)&(buf[0])) = contTics ;                   /* contTics */
    if (posNueva > 4) {
      *((dword_t far *)&(buf[4])) = contRodajas ;           /* contRodajas */
      if (posNueva > 8) {
        *((word_t far *)&(buf[8])) = contTicsRodaja ;    /* contTicsRodaja */
        if (posNueva > 10) {
          *((word_t far *)&(buf[10])) = ticsPorRodaja ;   /* ticsPorRodaja */
          if (posNueva > tamBuf) nbytes = tamBuf - pos ;
        }
      }
    }
    for ( i = pos ; i < posNueva ; i++ )                 /* primeros bytes */
      dir[i] = buf[i] ;
  }
  return(nbytes) ;
}

static int far writeTimer ( int dfs, pointer_t dir, word_t nbytes ) {
  return(0) ;
}

static int far aio_writeTimer ( int dfs, pointer_t dir, word_t nbytes ) {
  return(0) ;
}

static int far lseekTimer ( int dfs, long int pos, word_t whence ) {
  return(0) ;
}

static int far fcntlTimer ( int dfs, word_t cmd, word_t arg ) {
  return(0) ;
}

static int far ioctlTimer ( int dfs, word_t request, word_t arg ) {
  return(0) ;
}

#pragma warn +par

argCbTimer_t argCbTimer ;

#define maxCbTM 9

static descCcb_t descCcbTM = { 0, 0, 0, maxCbTM, NULL,
  cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo
} ;

void inicTimer ( void ) {

  descRecurso_t dR ;
  int i ;

  dR.tipo = rDCaracteres ;
  copiarStr("TIMER", dR.nombre) ;
  dR.ccb = (ccb_t)&descCcbTM ;
  dR.ccb->arg = pointer(_DS, (word_t)&argCbTimer) ;
  dR.pindx = indProcesoActual ;
  dR.numVI = 1 ;
  dR.nVInt[0] = nVIntTimer ;
  dR.irq[0] = IRQ_TIMER ;
  dR.isr[0] = (isr_t)pointer(_CS, (word_t)isr_timer) ;

  dR.open      = (open_t)pointer(_CS, (word_t)openTimer) ;
  dR.release   = (release_t)pointer(_CS, (word_t)releaseTimer) ;
  dR.read      = (read_t)pointer(_CS, (word_t)readTimer) ;
  dR.aio_read  = (aio_read_t)pointer(_CS, (word_t)aio_readTimer) ;
  dR.write     = (write_t)pointer(_CS, (word_t)writeTimer) ;
  dR.aio_write = (aio_write_t)pointer(_CS, (word_t)aio_writeTimer) ;
  dR.lseek     = (lseek_t)pointer(_CS, (word_t)lseekTimer) ;
  dR.fcntl     = (fcntl_t)pointer(_CS, (word_t)fcntlTimer) ;
  dR.ioctl     = (ioctl_t)pointer(_CS, (word_t)ioctlTimer) ;

  rec_timer = crearRec(&dR) ;

  crearFich("TIMER", rec_timer, 0, fedCaracteres) ;

  inicPC2c(&colaTimer, &eTimer, maxProcesos + 0, FALSE) ;

  for ( i = 0 ; i < maxProcesos ; i ++ ) ticsPorLeer[i] = 0 ;

}

void procesarTicColaTimer ( void ) {
  int i ;
  pindx_t pindx ;
  for ( i = 0 ; i < colaTimer.numElem ; i++ ) {
    pindx = desencolarPC2c(&colaTimer) ;
    if (dirDestino[pindx] != NULL)
      *(dirDestino[pindx])++ = (char)(contTics % 256) ;
    if (--(ticsPorLeer[pindx]) <= 0) {
      descProceso[pindx].estado = preparado ;
      encolarPC2c(pindx, (ptrC2c_t)&c2cPFR[PPreparados]) ;
    }
    else
      encolarPC2c(pindx, (ptrC2c_t)&colaTimer) ;
  }
}

void far isr_timer ( ) {

  plot('t', 0, contadorTimer0()) ;  /* mejor antes de llamar a la rti bios */

                       /* simulamos la interrupcion con el vector guardado */
                            /* necesario para el timeout de leerSectorBIOS */
                                                   /* int 13h con ah = 02h */

  VIOrg[nVIntTimer]() ;      /* (pushf + int) da la int. por servida (pic) */

  contTics++ ;

  argCbTimer.contTics = contTics ;
  argCbTimer.ticsPorRodaja = ticsPorRodaja ;
  argCbTimer.contTicsRodaja = contTicsRodaja ;

  atenderCcb((ccb_t)&descCcbTM) ;

  procesarTicColaTimer() ;

/*
  procesarTicReloj_sonido() ;

  procesarTicReloj_raton() ;

  if (++contTicsReloj == ticsVerReloj) procesarTicReloj_verReloj() ;
*/

  if (indProcesoActual != -1) {           /* se esta ejecutando un proceso */
    if (++contTicsRodaja == ticsPorRodaja) {
      if ((c2cPFR[PPreparados].numElem > 0) &&
          (nivelActivacionSO1 == 1)) {                       /* 17-10-2010 */
        descProceso[indProcesoActual].trama =
          (trama_t far *) pointer(SS_Proceso, SP_Proceso) ;
        descProceso[indProcesoActual].estado = preparado ;
        encolarPC2c(indProcesoActual, (ptrC2c_t)&c2cPFR[PPreparados]) ;
        hayTic = TRUE ;
        buscarNuevoProcesoActual() ;                      /* hayTic = TRUE */
      }
      else {
        contRodajas++ ;
        contTicsRodaja = 0 ;
      }
    }
    plot('e', 0, contadorTimer0()) ;            /* no hay cambio de estado */
  }
  else {                                         /* indProcesoActual == -1 */
    if (c2cPFR[PPreparados].numElem > 0)    /* va a haber cambio de estado */
      descProceso[c2cPFR[PPreparados].primero].estado = ejecutandose ;     /* truco */
    plot('e', 0, contadorTimer0()) ;
    if (c2cPFR[PPreparados].numElem > 0)
      descProceso[c2cPFR[PPreparados].primero].estado = preparado ;
  }
}

/*
void esperaActiva ( word_t numTics ) {
  word_t contTics0 ;
  bool_t iguales ;
  word_t i ;
  for ( i = 0 ; i <= numTics ; i++ ) {
    asm cli
    contTics0 = contTicsReloj ;
    asm sti
    do {
      asm cli
      iguales = (contTicsReloj == contTics0) ;
      asm sti
    } while (iguales) ;
  }
}
*/
