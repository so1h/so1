/* ----------------------------------------------------------------------- */
/*                                 reloj.c                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\caracter.h>
#include <so1pub.h\puertos.h>
#include <so1pub.h\strings.h>
#include <so1pub.h\cmos.h>
#include <so1.h\procesos.h>
#include <so1.h\recursos.h>

rindx_t rec_reloj ;

void leerHoras ( word_t * horas ) {
  *horas = leerCmos(CMOS_RTC_HOURS) ;                               /* BCD */
}

void leerMinutos ( word_t * minutos ) {
  *minutos = leerCmos(CMOS_RTC_MINUTES) ;                           /* BCD */
}

void leerSegundos ( word_t * segundos ) {
  *segundos = leerCmos(CMOS_RTC_SECONDS) ;                          /* BCD */
}

void leerHora ( word_t * horas, word_t * minutos, word_t * segundos ) {
  leerHoras(horas) ;
  leerMinutos(minutos) ;
  leerSegundos(segundos) ;
}

#pragma warn -par

static int far openReloj ( int dfs, modoAp_t modo ) {
  return(0) ;
}

static int far releaseReloj ( int dfs ) {
  return(0) ;
}

#define tamBuf 8

static int far readReloj ( int dfs, pointer_t dir, word_t nbytes ) {

  /* El proceso lee los bytes que solicita del fichero virtual: HH:MM:SS   */

  int i ;
  word_t w [ 3 ] ;
  char buf [ tamBuf ] ;
  int df = tramaProceso->BX ;
  dword_t pos = (word_t)descProceso[indProcesoActual].tfa[df].pos ;
  if (pos >= tamBuf)
    nbytes = 0 ;
  else {
    leerHora((word_t far *)&w[0], (word_t far *)&w[1], (word_t far *)&w[2]) ;
    for ( i = 0 ; i < tamBuf ; i++ ) {
      switch (i % 3) {
      case 0 : car = '0' + (w[i/3] >> 4) ; break ;
      case 1 : car = '0' + (w[i/3] & 0x000F) ; break ;
      case 2 : car = ':' ;
      }
      buf[i] = car ;
    }
    if (pos + nbytes > tamBuf) nbytes = tamBuf - pos ;
    for ( i = 0 ; i < nbytes ; i++ )
      dir[i] = buf[pos + i] ;
  }
  return(nbytes) ;
}

static int far aio_readReloj ( int dfs, pointer_t dir, word_t nbytes ) {
  return(readReloj(dfs, dir, nbytes)) ;
}

static int far writeReloj ( int dfs, pointer_t dir, word_t nbytes ) {
  return(0) ;
}

static int far aio_writeReloj ( int dfs, pointer_t dir, word_t nbytes ) {
  return(0) ;
}

static int far lseekReloj ( int dfs, long int pos, word_t whence ) {
  long int posNueva ;
  long int res = (long int)-1 ;
  int df = tramaProceso->BX ;
  switch (whence) {
    case SEEK_SET : if (pos <= (long int)8)
                      res = pos ;
                    break ;
    case SEEK_CUR : posNueva = descProceso[indProcesoActual].tfa[df].pos + pos ;
    case SEEK_END : if (whence == SEEK_END) posNueva = (long int)8 + pos ;
                    if (posNueva <= (long int)0)
                      res = 0 ;
                    else if (posNueva >= (long int)8)
                      res = (long int)8 ;
                    else
                      res = posNueva ;
                    break ;
    default : ;
  }
  return(res) ;
}

static int far fcntlReloj ( int dfs, word_t cmd, word_t arg ) {
  return(0) ;
}

static int far ioctlReloj ( int dfs, word_t request, word_t arg ) {
  return(0) ;
}

#pragma warn +par

#define maxCbRL 0

static descCcb_t descCcbRL = { 0, 0, 0, maxCbRL, NULL } ;

void inicReloj ( void ) {

  descRecurso_t dR ;

  dR.tipo = rDCaracteres ;
  copiarStr("RELOJ", dR.nombre) ;
  dR.ccb = (ccb_t)&descCcbRL ;
  dR.ccb->arg = NULL ;
  dR.pindx = indProcesoActual ;
  dR.numVI = 0 ;

  dR.open      = (open_t)pointer(_CS, (word_t)openReloj) ;
  dR.release   = (release_t)pointer(_CS, (word_t)releaseReloj) ;
  dR.read      = (read_t)pointer(_CS, (word_t)readReloj) ;
  dR.aio_read  = (aio_read_t)pointer(_CS, (word_t)aio_readReloj) ;
  dR.write     = (write_t)pointer(_CS, (word_t)writeReloj) ;
  dR.aio_write = (aio_write_t)pointer(_CS, (word_t)aio_writeReloj) ;
  dR.lseek     = (lseek_t)pointer(_CS, (word_t)lseekReloj) ;
  dR.fcntl     = (fcntl_t)pointer(_CS, (word_t)fcntlReloj) ;
  dR.ioctl     = (ioctl_t)pointer(_CS, (word_t)ioctlReloj) ;

  rec_reloj = crearRec(&dR) ;

  crearFich("RELOJ", rec_reloj, 0, fedCaracteres) ;

}

