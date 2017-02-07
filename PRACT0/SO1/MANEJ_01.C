/* ----------------------------------------------------------------------- */
/*                               manej_01.c                                */
/* ----------------------------------------------------------------------- */
/*          manejador de las llamadas al sistema del grupo AH = 01         */
/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
/* AX = 0x0100 open                                                        */
/* AX = 0x0101 close                                                       */
/* AX = 0x0102 read                                                        */
/* AX = 0x0103 aio_read                                                    */
/* AX = 0x0104 write                                                       */
/* AX = 0x0105 aio_write                                                   */
/* AX = 0x0106 lseek                                                       */
/* AX = 0x0107 fcntl                                                       */
/* AX = 0x0108 ioctl                                                       */
/*                                                                         */
/* en general hay que calcular df, dfs comprobar errores y llamar a la     */
/* funcion correspondiente del driver:                                     */
/*                                                                         */
/*          read  --> descRecurso[descFichero[dfs].rindx].read             */
/*          write --> descRecurso[descFichero[dfs].rindx].write            */
/*                                                                         */
/* luego el driver que se las apañe.                                       */
/*                                                                         */
/* El driver, en principio sigue utilizando la pila del sistema operativo. */
/* Para drivers complicados puede haber un primer thread que sirve las     */
/* peticiones mientras el proceso de usuario se bloquea. Las peticiones    */
/* pueden ser encomendadas a uno o varios threads del driver.              */
/* El driver debe poder admitir que los procesos encolen tareas sencillas  */
/* (callbacks) para que se ejecuten desde la isr(s) asociada(s) al driver. */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\strings.h>
#include <so1pub.h\tipos.h>
#include <so1pub.h\fcntl.h>
#include <so1pub.h\def_proc.h>
#include <so1.h\procesos.h>
#include <so1.h\recursos.h>
#include <so1.h\minifs.h>                                     /* rec_sf */
#include <so1.h\bios.h>

int indiceTFAS ( const char far * nombre ) { /* se puede mejorar con la cola c2c de ficheros */
  int dfs ;
  dfs = c2cPFR[DFOcupados].primero ;                   /* ver si existe ya */
  while (dfs != c2cPFR[DFOcupados].cabecera) {
    if (iguales(descFichero[dfs].nombre, nombre)) return(dfs) ;
    dfs = c2cPFR[DFOcupados].e[dfs].sig ;
  }
  return(-1) ;
}

int indiceTFA ( const char far * nombre ) {
  int df, dfs ;
  for ( df = 0 ; df < dfMax ; df++ ) {
    if (((dfs = descProceso[indProcesoActual].tfa[df].dfs) != -1) &&
        (igualesHasta(descFichero[dfs].nombre, nombre, 8)))
      return(df) ;
  }
  return(-1) ;
}

int nuevaEntradaTFA ( void ) {
  int df ;
  for ( df = 0 ; df < dfMax ; df++ )
    if (descProceso[indProcesoActual].tfa[df].dfs < 0) {
      return(df) ;
    }
  return(-1) ;
}

/* static int contOpen = 0 ; */                 /* para depurar (comentar) */

void so1_manejador_01 ( void ) {                       /* ah = 1 ; int SO1 */

  int df, dfs, dfs1, res ;
  rindx_t rindx ;
  char far * nombre ;
  modoAp_t modoAp ;
  pointer_t dir ;
  word_t nbytes ;
  long int posNueva ;
  long int pos ;
  int whence ;
  word_t cmd ;                                                    /* fcntl */
  word_t arg ;

  switch (tramaProceso->AL) {

    case 0x00 :                                                  /* 0x0100 */
      modoAp = tramaProceso->DX ;                                  /* open */
      nombre = (char far *)pointer(tramaProceso->ES, tramaProceso->BX) ;
      df = -1 ;                                             /* por defecto */
      if ((modoAp & (O_TEXT | O_BINARY)) == (O_TEXT | O_BINARY)) {
        tramaProceso->AX = df ;                                      /* -1 */
        break ;
      }
      if (!(modoAp & (O_TEXT | O_BINARY)))           /* por defecto O_TEXT */
        modoAp = modoAp + O_TEXT ;
      df = indiceTFA(nombre) ;
      if (df >= 0) {                       /* fichero ya abierto (proceso) */
        dfs = descProceso[indProcesoActual].tfa[df].dfs ;
        rindx = descFichero[dfs].rindx ;
      }
      else {                               /* fichero no abierto (proceso) */
        dfs = indiceTFAS(nombre) ;
        if (dfs >= 0)                      /* fichero ya abierto (sistema) */
          rindx = descFichero[dfs].rindx ;
        else                               /* fichero no abierto (sistema) */
          rindx = rec_sf ;                          /* sistema de ficheros */
      }
      if ((descRecurso[rindx].open != (open_t)NULL) &&
          ((dfs1 = descRecurso[rindx].open(dfs, modoAp)) >= 0)) {
        if ((df = nuevaEntradaTFA()) >= 0) {
          if (dfs1 > 0) dfs = dfs1 ;
          descProceso[indProcesoActual].nfa++ ;
          descProceso[indProcesoActual].tfa[df].modoAp = modoAp ;
          descProceso[indProcesoActual].tfa[df].dfs = dfs ;
          descProceso[indProcesoActual].tfa[df].pos = 0 ;
          if ((modoAp & 0x0007) == O_RDONLY) descFichero[dfs].contAp_L++ ;
          else if ((modoAp & 0x0007) == O_WRONLY) descFichero[dfs].contAp_E++ ;
        }
        else descRecurso[rindx].release(dfs) ;
      }
      tramaProceso->AX = df ;
      break ;

    case 0x01 :                                                  /* 0x0101 */
      df = tramaProceso->BX ;                                     /* close */
      res = -1 ;
      if ((0 <= df) && (df < dfMax)) {
        dfs = descProceso[indProcesoActual].tfa[df].dfs ;
        if ((0 <= dfs) && (dfs < dfsMax)) {
          rindx = descFichero[dfs].rindx ;
          /* printStrBIOS("\n close dfs = ") ; printDecBIOS(dfs, 1) ; */
          descProceso[indProcesoActual].nfa-- ;
          modoAp = descProceso[indProcesoActual].tfa[df].modoAp ;
          if ((modoAp & 0x0007) == O_RDONLY) descFichero[dfs].contAp_L-- ;
          else if ((modoAp & 0x0007) == O_WRONLY) descFichero[dfs].contAp_E-- ;
          descProceso[indProcesoActual].tfa[df].dfs = -1 ;
          descRecurso[rindx].release(dfs) ;
          res = 0 ;
        }
      }
      tramaProceso->AX = res ;
      break ;

    case 0x02 :                                 /* read      */ /* 0x0102 */
    case 0x03 :                                 /* aio_read  */ /* 0x0103 */
    case 0x04 :                                 /* write     */ /* 0x0104 */
    case 0x05 :                                 /* aio_write */ /* 0x0105 */
      df = tramaProceso->BX ;
      res = -1 ;
      if ((0 <= df) && (df < dfMax)) {
        nbytes = tramaProceso->CX ;
        if (nbytes == 0) res = 0 ;
        else {
          dfs = descProceso[indProcesoActual].tfa[df].dfs ;
          if ((0 <= dfs) && (dfs < dfsMax)) {
            rindx = descFichero[dfs].rindx ;
            if ((0 <= rindx) && (rindx < maxRecursos))
              modoAp = descProceso[indProcesoActual].tfa[df].modoAp ;
              dir = pointer(tramaProceso->ES, tramaProceso->DX) ;
              switch (tramaProceso->AL) {
              case 0x02 :                                          /* read */
              case 0x03 :                                      /* aio_read */
                if ((modoAp & 0x0007) == O_RDONLY) {
                  if (tramaProceso->AL == 0x02)
                    res = descRecurso[rindx].read(dfs, dir, nbytes) ;
                  else
                    res = descRecurso[rindx].aio_read(dfs, dir, nbytes) ;
                }
                break ;
              case 0x04 :                                         /* write */
              case 0x05 :                                     /* aio_write */
                if ((modoAp & 0x0007) == O_WRONLY) {
                  if (tramaProceso->AL == 0x04)
                    res = descRecurso[rindx].write(dfs, dir, nbytes) ;
                  else
                    res = descRecurso[rindx].aio_write(dfs, dir, nbytes) ;
                }
                break ;
              }
              if (res > 0)
                descProceso[indProcesoActual].tfa[df].pos += res ;
          }
        }
      }
      tramaProceso->AX = res ;
      break ;

    case 0x06 :                                                 /* 0x0106 */
      df = tramaProceso->BX ;                                    /* lseek */
      posNueva = (long int)-1 ;
      if ((0 <= df) && (df < dfMax)) {
        dfs = descProceso[indProcesoActual].tfa[df].dfs ;
        if ((0 <= dfs) && (dfs < dfsMax)) {
          rindx = descFichero[dfs].rindx ;
          if ((0 <= rindx) && (rindx < maxRecursos)) {
            pos = (long int)pointer(tramaProceso->ES, tramaProceso->DX) ;
            whence = tramaProceso->CX ;
            if ((SEEK_SET <= whence) && (whence <= SEEK_END)) {
              posNueva = descRecurso[rindx].lseek(dfs, pos, whence) ;
              if (posNueva >= (long int)0)
                descProceso[indProcesoActual].tfa[df].pos = posNueva ;
            }
          }
        }
      }
      tramaProceso->AX = (word_t)posNueva ;
      tramaProceso->BX = (word_t)(posNueva >> 16) ;
      break ;

    case 0x07 :                                                 /* 0x0107 */
      df = tramaProceso->BX ;                                    /* fcntl */
      cmd = tramaProceso->CX ;
      arg = tramaProceso->DX ;
      res = -1 ;
      if ((0 <= df) || (df < dfMax)) {
        modoAp = descProceso[indProcesoActual].tfa[df].modoAp ;
        res = 0 ;

        if (cmd == F_GETFD) res = modoAp ;
        else if (cmd == F_SETFD) {
          modoAp = modoAp & ~(O_APPEND | O_NONBLOCK) ;
          modoAp = modoAp | (arg & (O_APPEND | O_NONBLOCK)) ;
          descProceso[indProcesoActual].tfa[df].modoAp = modoAp ;
        }
/*
        switch (cmd) {
        case F_GETFD : res = modoAp ; break ;
        case F_SETFD : modoAp = modoAp & ~(O_APPEND | O_NONBLOCK) ;
                       modoAp = modoAp | (arg & (O_APPEND | O_NONBLOCK)) ;
                       descriptor[indProcesoActual].tfa[df].modoAp = modoAp ;
                       break ;
        default : ;
        }
*/
      }
      tramaProceso->AX = res ;
      break ;
    case 0x08 :                                                  /* 0x0108 */
      df = tramaProceso->BX ;                                    /* fcntl */
      cmd = tramaProceso->CX ;
      arg = tramaProceso->DX ;
      res = -1 ;
      if ((0 <= df) || (df < dfMax)) {
        dfs = descProceso[indProcesoActual].tfa[df].dfs ;
        if ((0 <= dfs) && (dfs < dfsMax)) {
          rindx = descFichero[dfs].rindx ;
          if ((0 <= rindx) && (rindx < maxRecursos)) {
            res = descRecurso[rindx].ioctl(dfs, cmd, arg) ;
          }
        }
      }
      tramaProceso->AX = res ;
      break ;
    default : ;
  }
}
