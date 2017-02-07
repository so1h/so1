/* ----------------------------------------------------------------------- */
/*                                retardo.c                                */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\biosdata.h>
#include <so1pub.h\strings.h>
#include <so1.h\ajustes.h>                                     /* segDatos */
#include <so1.h\procesos.h>
#include <so1.h\recursos.h>
#include <so1.h\retardo.h>

rindx_t rec_retardo ;

static bool_t retardando [ maxProcesos ] ;        /* procesos retardandose */

static dword_t nVueltasRetardo ;

#define cuerpoVuelta {                                                       \
  for (j = 0 ; j < 10 ; j++) {                                               \
    asm mov dx,0x1234 ;                                                      \
    asm mov ax,0x5678 ;                                                      \
    asm mov bx,0xFFFF ;                                                      \
    asm div bx ;                                                             \
  }                                                                          \
  nVueltas++ ;                                                               \
}

static void retardar ( dword_t nVueltas ) {
  dword_t n = nVueltas ;
  dword_t i ;
  word_t j ;
  for ( i = 0 ; i < n ; i++ ) cuerpoVuelta ;
}

static int far retardarProceso ( void ) {
  word_t DS_Retardo = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
  int res ;
  asm push ds
  asm mov ds,DS_Retardo                      /* establecemos DS del driver */
  if (!retardando[indProcesoActual])
    res = -1 ;
  else {
    retardar(nVueltasRetardo) ;
    res = 0 ;
  }
  asm pop ds                                 /* restauramos DS del proceso */
  return(res) ;
}

#pragma warn -par

static int far openRetardo ( int dfs, modoAp_t modo ) {
  if (retardando[indProcesoActual]) return(-1) ;
  retardando[indProcesoActual] = TRUE ;
  return(0) ;
}

static int far releaseRetardo ( int dfs ) {
  if (!retardando[indProcesoActual]) return(-1) ;
  retardando[indProcesoActual] = FALSE ;
  return(0) ;
}

static int far readRetardo ( int dfs, pointer_t dir, word_t nbytes ) {
  dword_t far * ptrDWord = (dword_t far *)dir ;
  if (!retardando[indProcesoActual]) return(-1) ;    /* fichero no abierto */
  if (nbytes != 4) return(-2) ;                       /* nbytes incorrecto */
  switch (dir[0]) {
    case 0x00 : ptrDWord[0] = nVueltasRetardo ; break ;
    case 0x01 : ptrDWord[0] = (dword_t far *)
                  pointer(_CS, (word_t)retardarProceso) ;
                break ;
    default : return(-3) ;               /* codigo de operacion incorrecto */
  }
  return(4) ;
}

static int far aio_readRetardo ( int dfs, pointer_t dir, word_t nbytes ) {
  return(readRetardo(dfs, dir, nbytes)) ;
}

static int far writeRetardo ( int dfs, pointer_t dir, word_t nbytes ) {
  dword_t far * ptrDWord = (dword_t far *)dir ;
  if (!retardando[indProcesoActual]) return(-1) ;    /* fichero no abierto */
  if (nbytes != 4) return(-2) ;
  nVueltasRetardo = ptrDWord[0] ;
  return(0) ;
}

static int far aio_writeRetardo ( int dfs, pointer_t dir, word_t nbytes ) {
  return(-1) ;
}

static int far lseekRetardo ( int dfs, long int pos, word_t whence ) {
  return(-1) ;
}

static int far fcntlRetardo ( int dfs, word_t cmd, word_t arg ) {
  return(-1) ;
}

static int far ioctlRetardo ( int dfs, word_t request, word_t arg ) {
  return(-1) ;
}

#pragma warn +par

#define maxCbRT 0

static descCcb_t descCcbRT = { 0, 0, 0, maxCbRT, NULL } ;

dword_t valorRetardo ( void ) { /* permite las interrupciones temporalmente */
  word_t j ;
  dword_t nVueltas ;
  byte_t contTics0 ;
  byte_t contTics1 ;
  asm pushf
  asm sti
  contTics0 = (byte_t)ptrBiosArea->BIOS_timer ;
  while ((byte_t)ptrBiosArea->BIOS_timer == contTics0) ;
  nVueltas = 0 ;
  contTics1 = (byte_t)ptrBiosArea->BIOS_timer ;
  while ((byte_t)ptrBiosArea->BIOS_timer == contTics1) cuerpoVuelta ;
  asm popf
  return(nVueltas) ;                            /* 1 Tics = 0,055 segundos */
}

dword_t inicRetardo ( void ) {

  descRecurso_t dR ;
  int i ;

  dR.tipo = rDCaracteres ;
  copiarStr("RETARDO", dR.nombre) ;
  dR.ccb = (ccb_t)&descCcbRT ;
  dR.ccb->arg = NULL ;
  dR.pindx = indProcesoActual ;
  dR.numVI = 0 ;

  dR.open      = (open_t)pointer(_CS, (word_t)openRetardo) ;
  dR.release   = (release_t)pointer(_CS, (word_t)releaseRetardo) ;
  dR.read      = (read_t)pointer(_CS, (word_t)readRetardo) ;
  dR.aio_read  = (aio_read_t)pointer(_CS, (word_t)aio_readRetardo) ;
  dR.write     = (write_t)pointer(_CS, (word_t)writeRetardo) ;
  dR.aio_write = (aio_write_t)pointer(_CS, (word_t)aio_writeRetardo) ;
  dR.lseek     = (lseek_t)pointer(_CS, (word_t)lseekRetardo) ;
  dR.fcntl     = (fcntl_t)pointer(_CS, (word_t)fcntlRetardo) ;
  dR.ioctl     = (ioctl_t)pointer(_CS, (word_t)ioctlRetardo) ;

  rec_retardo = crearRec(&dR) ;

  crearFich("RETARDO", rec_retardo, 0, fedCaracteres) ;

  nVueltasRetardo = valorRetardo() ;

  for ( i = 0 ; i < maxProcesos ; i ++ ) retardando[i] = FALSE ;

  return(nVueltasRetardo) ;

}

