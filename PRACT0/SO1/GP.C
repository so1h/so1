/* ----------------------------------------------------------------------- */
/*                                   gp.c                                  */
/* ----------------------------------------------------------------------- */
/*                           Gestor de Procesos                            */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\bios_0.h>
#include <so1pub.h\msdos.h>
#include <so1pub.h\strings.h>
#include <so1pub.h\ll_s_so1.h>
#include <so1pub.h\escribir.h>
#include <so1pub.h\memory.h>                          /* memcpy, memcpy_fd */
#include <so1.h\ajustes.h>
#include <so1.h\recursos.h>
#include <so1.h\procesos.h>
#include <so1.h\gp.h>

rindx_t rec_gp ;

pid_t k_fork ( void ) {

  /* indProcesoActual */

  return(0) ;
}

#pragma option -w-par            /* omitir warning parameter is never used */

pid_t k_exec ( const char far * nombre,
               const char far * comando ) {
  return(0) ;
}

pid_t k_waitpid ( pid_t pid, int far * statloc ) {
  return(0) ;
}

void k_exit ( int status ) {
}

#pragma option -wpar           /* permitir warning parameter is never used */

pid_t k_getpid ( void ) {
  return(0) ;
}

pindx_t k_getpindx ( void ) {
  return(0) ;
}

#pragma option -w-par            /* omitir warning parameter is never used */

int k_killpid ( int pid ) {
  return(0) ;
}

static int far openGP ( int dfs, modoAp_t modo ) {
  return(0) ;
}

static int far releaseGP ( int dfs ) {
  return(0) ;
}

#pragma option -wpar           /* permitir warning parameter is never used */

typedef struct {
  char far * nombre ;
  char far * comando ;
} regParametros_t ;

#pragma option -w-par            /* omitir warning parameter is never used */

int far readGP ( int dfs, pointer_t dir, word_t nbytes ) {
  regParametros_t far * regParam = (regParametros_t far *)dir ;
  pid_t   far * pid     = (pid_t   far *)dir ;
  pindx_t far * pindx   = (pindx_t far *)dir ;
  word_t  far * ptrWord = (word_t  far *)dir ;
  switch (nbytes) {
    case sizeof(regParametros_t) :
      *pid = k_exec(regParam->nombre, regParam->comando) ; return(nbytes) ;
    case 2 :
      switch (*ptrWord) { 
        case 0x0000 : *pid   = k_fork() ;     return(nbytes) ;
        case 0x0001 : *pid   = k_getpid() ;   return(nbytes) ;
        case 0x0002 : *pindx = k_getpindx() ; return(nbytes) ;
      }
  }
  return(-1) ;
}

int far aio_readGP ( int dfs, pointer_t dir, word_t nbytes ) {
  pid_t far * pid = (pid_t far *)dir ;
  if (nbytes == 2) {
    *pid = k_waitpid(*pid, (int far *)dir) ;
    return(nbytes) ;
  }
  return(-1) ;
}

int far writeGP ( int dfs, pointer_t dir, word_t nbytes ) { /* exit, kill */
  if (nbytes == 2)
    k_exit(*((int far *)dir)) ;
  return(-1) ;
}

int far aio_writeGP ( int dfs, pointer_t dir, word_t nbytes ) {
  pid_t far * pid = (pid_t far *)dir ;
  if (nbytes == 2) {
    *pid = k_kill(*pid) ;
    return(nbytes) ;
  }
  return(-1) ;
}

int far lseekGP ( int dfs, long int pos, word_t whence ) {
  return(0) ;
}

int far fcntlGP ( int dfs, word_t cmd, word_t arg ) {
  return(0) ;
}

int far ioctlGP ( int dfs, word_t request, word_t arg ) {
  return(0) ;
}

#pragma option -wpar           /* permitir warning parameter is never used */

void servidorGP ( void ) {
  escribirStr("\n servidorGP activo ") ;
  exit(0) ;
}

#define maxCbGP 0

descCcb_t descCcbGP = { 0, 0, 0, maxCbGP, NULL } ;

pid_t inicGP ( void ) {

  descRecurso_t dR ;
  pindx_t pindx ;
  word_t segmento, SSProc, SPProc ;

  dR.tipo = rGP ;
  copiarStr("GP", dR.nombre) ;
  dR.ccb = (ccb_t)&descCcbGP ;
  dR.pindx = indProcesoActual ;
  dR.numVI = 0 ;

  dR.open      = (open_t)pointer(_CS, (word_t)openGP) ;
  dR.release   = (release_t)pointer(_CS, (word_t)releaseGP) ;
  dR.read      = (read_t)pointer(_CS, (word_t)readGP) ;
  dR.aio_read  = (aio_read_t)pointer(_CS, (word_t)aio_readGP) ;
  dR.write     = (write_t)pointer(_CS, (word_t)writeGP) ;
  dR.aio_write = (aio_write_t)pointer(_CS, (word_t)aio_writeGP) ;
  dR.lseek     = (lseek_t)pointer(_CS, (word_t)lseekGP) ;
  dR.fcntl     = (fcntl_t)pointer(_CS, (word_t)fcntlGP) ;
  dR.ioctl     = (ioctl_t)pointer(_CS, (word_t)ioctlGP) ;

  rec_gp = crearRec(&dR) ;

  crearFich("GP", rec_gp, 0, fedCaracteres) ;

#if (FALSE)
  if (c2cPFR[DPLibres].numElem == 0) return(-1) ;
  if (ll_tamBloqueMax() < descProceso[0].tam) return(-1) ;

  segmento = ll_buscarBloque(descProceso[0].tam) ;   /* llamada al sistema */

  if (segmento == 0x0000) return(-1) ;

  pindx = desencolarPC2c((ptrC2c_t)&c2cPFR[DPLibres]) ;
  encolarPC2c(pindx, (ptrC2c_t)&c2cPFR[DPOcupados]) ;
  memcpy(&descProceso[pindx], &descProceso[0], sizeof(descProceso_t)) ;
//copiaLarga(
//  pointer(descProceso[0].CSProc, 0x0000),
//  pointer(descProceso[pindx].CSProc, 0x0000),
//  sizeof(descProceso_t)
//) ;
  memcpy(
    pointer(descProceso[pindx].CSProc, 0x0000),
    pointer(descProceso[0].CSProc, 0x0000),
    sizeof(descProceso_t)    /* ?????????????????????? */
  ) ;
  descProceso[pindx].pid = nuevoPid() ;
  descProceso[pindx].pid = preparado ;           /* realmente en ejecucion */
  descProceso[pindx].ppindx = indProcesoActual ;      /* esta en ejecucion */
  descProceso[pindx].hpindx = -1 ;
    inicPC2c(&descProceso[pindx].c2cHijos, &e2PFR.e2Hijos, maxProcesos + pindx, TRUE) ;
  descProceso[pindx].CSProc = segmento ;
  SSProc = seg((pointer_t)descProceso[pindx].trama)
             + (segmento - descProceso[pindx].CSProc) ;
  SPProc = _SP ;
  tramaProceso = (trama_t far *)pointer(SSProc, SPProc + sizeof(trama_t)) ;
  descProceso[pindx].trama = tramaProceso ;

  registrarEnPOrdenados(pindx) ;

  descProceso[pindx].trama->DS = SSProc ;
  descProceso[pindx].trama->ES = SSProc ;
  descProceso[pindx].trama->DI = 0x0000 ;
  descProceso[pindx].trama->SI = 0x0000 ;
  descProceso[pindx].trama->BP = (SPProc - 6) ;                /* IP, CS, Flags */
  descProceso[pindx].trama->SP = (SPProc - 6) ;                /* IP, CS, Flags */
  descProceso[pindx].trama->BX = 0x0000 ;
  descProceso[pindx].trama->DX = 0x0000 ;
  descProceso[pindx].trama->CX = 0x0000 ;
  descProceso[pindx].trama->AX = 0x0000 ;
  descProceso[pindx].trama->IP = (word_t)servidorGP ;
  descProceso[pindx].trama->CS = segmento ;
  descProceso[pindx].trama->Flags = 0x7202 ;       /* interrupciones permitidas */

  encolarPC2c(pindx, &c2cPFR[PPreparados]) ;
#endif

  return(descProceso[pindx].pid) ;

}

