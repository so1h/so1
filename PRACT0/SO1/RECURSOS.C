/* ----------------------------------------------------------------------- */
/*                                recursos.c                               */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\strings.h>                            /* strcpy, strcmp */
#include <so1pub.h\pic.h>                                        /* ptrTVI */
#include <so1pub.h\fcntl.h>                                   /* O_DENYALL */
#include <so1pub.h\ptrc2c.h>            /* ptrC2c_t, inicPC2c, encolarPC2c */ /* apilarPC2c, desapilarPC2c, eliminarPC2c */
#include <so1.h\recursos.h>
#include <so1.h\ajustes.h>
#include <so1.h\procesos.h>
#include <so1.h\bios.h>
#include <so1.h\interrup.h>

void inicRecursos ( void ) {

  int rindx, dfs, i ;

  /* inicializamos las pilas de recursos/descriptores libres/ocupados */

  inicPC2c(&c2cPFR[DFLibres],   &e2PFR.e2DescFichero.Libres,   dfsMax,          TRUE) ;
  inicPC2c(&c2cPFR[DFOcupados], &e2PFR.e2DescFichero.Ocupados, dfsMax + 1,      TRUE) ;
  inicPC2c(&c2cPFR[DRLibres],   &e2PFR.e2DescRecurso.Libres,   maxRecursos,     TRUE) ;
  inicPC2c(&c2cPFR[DROcupados], &e2PFR.e2DescRecurso.Ocupados, maxRecursos + 1, TRUE) ;

  /* inicializamos todos los descriptores de recursos */

  for ( rindx = maxRecursos-1 ; rindx >= 0 ; rindx-- ) {
    apilarPC2c(rindx, (ptrC2c_t)&c2cPFR[DRLibres]) ;
    descRecurso[rindx].nombre[0] = (char)0 ;
    descRecurso[rindx].tipo = rLibre ;
    descRecurso[rindx].ccb = (ccb_t)0x00000000 ;
    descRecurso[rindx].pindx = (pindx_t)(-1) ;
    inicPC2c(&descRecurso[rindx].c2cFichRec, &e2PFR.e2FichRec, dfsMax + rindx, TRUE) ;
    descRecurso[rindx].numVI = 0 ;
    for ( i = 0 ; i < 2 ; i++ ) {
      descRecurso[rindx].nVInt[i] = 0x00 ;
      descRecurso[rindx].irq[i] = 0x00 ;
      descRecurso[rindx].isr[i] = (isr_t)0x00000000 ;
    }
    descRecurso[rindx].open =      (open_t)0x00000000 ;
    descRecurso[rindx].release =   (release_t)0x00000000 ;
    descRecurso[rindx].read =      (read_t)0x00000000 ;
    descRecurso[rindx].aio_read =  (aio_read_t)0x00000000 ;
    descRecurso[rindx].write =     (write_t)0x00000000 ;
    descRecurso[rindx].aio_write = (aio_write_t)0x00000000 ;
    descRecurso[rindx].lseek =     (lseek_t)0x00000000 ;
    descRecurso[rindx].fcntl =     (fcntl_t)0x00000000 ;
    descRecurso[rindx].ioctl =     (ioctl_t)0x00000000 ;

    descRecurso[rindx].eliminar =  (eliminar_t)0x00000000 ;

  }

  /* inicializamos todos los descriptores de fichero del sistema */

  for ( dfs = 0 ; dfs < dfsMax ; dfs++ ) {
    descFichero[dfs].nombre[0] = (char)0 ;
    descFichero[dfs].tipo = flibre ;
    descFichero[dfs].rindx = -1 ;
    descFichero[dfs].menor = 0 ;
    descFichero[dfs].contAp_L = 0 ;
    descFichero[dfs].contAp_E = 0 ;
    descFichero[dfs].shareMode = O_DENYALL ;
  }

  /* inicializamos la pila de descriptores de fichero del sist. libres */

  for ( dfs = dfsMax-1 ; dfs >= 0 ; dfs-- )
    apilarPC2c(dfs, (ptrC2c_t)&c2cPFR[DFLibres]) ;

}

rindx_t crearRec ( descRecurso_t far * dR ) {

  rindx_t rindx ;
  byte_t nVInt ;
  byte_t irq ;
  isr_t isr ;
  int i ;

  if ((dR->tipo <= rLibre) || (dR->tipo > rOtro)) return(-1) ;

  if (dR->numVI > 2) return(-2) ;                      /* 2 VI como máximo */

  rindx = c2cPFR[DROcupados].primero ;                 /* ver si existe ya */
  while (rindx != c2cPFR[DROcupados].cabecera) {
    if (!strcmp(descRecurso[rindx].nombre, dR->nombre)) return(-3) ;
    rindx = c2cPFR[DROcupados].e[rindx].sig ;
  }

  if (c2cPFR[DRLibres].numElem == 0) return(-4) ;   /* no hay desc. libres */

  for ( i = 0 ; i < dR->numVI ; i++ )
    if (recVInt[dR->nVInt[i]] >= 0) return(-5) ;    /* nVInt ocupado */

  rindx = desapilarPC2c((ptrC2c_t)&c2cPFR[DRLibres]) ;
  apilarPC2c(rindx, (ptrC2c_t)&c2cPFR[DROcupados]) ;

  descRecurso[rindx].tipo = dR->tipo ;
  strcpy(descRecurso[rindx].nombre, dR->nombre) ;
  descRecurso[rindx].ccb = dR->ccb ;
  descRecurso[rindx].pindx = dR->pindx ;
  inicPC2c(&descRecurso[rindx].c2cFichRec, &e2PFR.e2FichRec, dfsMax + rindx, TRUE) ;
  descRecurso[rindx].numVI = dR->numVI ;

  for ( i = 0 ; i < dR->numVI ; i++ ) {
    nVInt = dR->nVInt[i] ;
    irq = dR->irq[i] ;
    isr = dR->isr[i] ;
    descRecurso[rindx].nVInt[i] = nVInt ;
    descRecurso[rindx].irq[i] = irq ;
    descRecurso[rindx].isr[i] = isr ;
    if (irq < 16)
      redirigirIntHardware(irq, isr) ;
    else
      redirigirInt(nVInt, isr) ;
    recVInt[nVInt] = rindx ;
  }

  descRecurso[rindx].open =      dR->open ;
  descRecurso[rindx].release =   dR->release ;
  descRecurso[rindx].read =      dR->read ;
  descRecurso[rindx].aio_read =  dR->aio_read ;
  descRecurso[rindx].write =     dR->write ;
  descRecurso[rindx].aio_write = dR->aio_write ;
  descRecurso[rindx].lseek =     dR->lseek ;
  descRecurso[rindx].fcntl =     dR->fcntl ;
  descRecurso[rindx].ioctl =     dR->ioctl ;

  descRecurso[rindx].eliminar =  dR->eliminar ;

  return(rindx) ;

}

dfs_t crearFich ( const char far * nombre,
                  rindx_t          rindx,
                  word_t           menor,
                  tipoFichero_t    tipo ) {

  dfs_t dfs ;

  if (tipo == flibre) return(-1) ;

  if ((rindx < 0) && (rindx < maxRecursos)) return(-2) ;

  dfs = c2cPFR[DFOcupados].primero ;                   /* ver si existe ya */
  while (dfs != c2cPFR[DFOcupados].cabecera) {
    if (!strcmp(descFichero[dfs].nombre, nombre)) return(-3) ;
    dfs = c2cPFR[DFOcupados].e[dfs].sig ;
  }

  if (c2cPFR[DFLibres].numElem == 0) return(-4) ;   /* no hay desc. libres */
  dfs = desapilarPC2c((ptrC2c_t)&c2cPFR[DFLibres]) ;
  apilarPC2c(dfs, (ptrC2c_t)&c2cPFR[DFOcupados]) ;
  apilarPC2c(dfs, (ptrC2c_t)&descRecurso[rindx].c2cFichRec) ;
  strcpy(descFichero[dfs].nombre, nombre) ;
  descFichero[dfs].rindx = rindx ;
  descFichero[dfs].menor = menor ;
  descFichero[dfs].tipo = tipo ;

  return(dfs) ;

}

int destruirFich ( dfs_t dfs ) {
  rindx_t rindx ;
  dfs_t i ;
  if ((dfs < 0) || (dfs >= dfsMax)) return(-1) ;
  rindx = descFichero[dfs].rindx ;
  i = descRecurso[rindx].c2cFichRec.primero ;
  while (i != descRecurso[rindx].c2cFichRec.cabecera) {
    if (i == dfs) break ;
    i = descRecurso[rindx].c2cFichRec.e[i].sig ;
  }
  if (i != dfs) return(-1) ;                                  /* no existe */
  if ((descFichero[dfs].contAp_L > 0) || (descFichero[dfs].contAp_E > 0))
    return(-2) ;                                                 /* en uso */
  eliminarPC2c(dfs, (ptrC2c_t)&descRecurso[rindx].c2cFichRec) ;
  eliminarPC2c(dfs, (ptrC2c_t)&c2cPFR[DFOcupados]) ;
  apilarPC2c(dfs, (ptrC2c_t)&c2cPFR[DFLibres]) ;
  descFichero[dfs].nombre[0] = (char)0 ;
  descFichero[dfs].tipo = flibre ;
  descFichero[dfs].rindx = -1 ;
  descFichero[dfs].menor = 0 ;
  descFichero[dfs].contAp_L = 0 ;
  descFichero[dfs].contAp_E = 0 ;
  return(0) ;
}

int destruirRec ( const char far * nombre ) {
  rindx_t r ;
  rindx_t rindx ;
  pindx_t pindx ;
  dfs_t dfs, dfsSig ;
  byte_t nVInt ;
  int j ;
  word_t segmento ;

  r = c2cPFR[DROcupados].primero ;                     /* ver si existe ya */
  while (r != c2cPFR[DROcupados].cabecera) {
    if (!strcmp(descRecurso[r].nombre, nombre)) {
      rindx = r ;                                    /* recurso encontrado */
      break ;
    }
    r = c2cPFR[DROcupados].e[r].sig ;
  }
  if (r == c2cPFR[DROcupados].cabecera) return(-1) ;          /* no existe */

  /* eliminar todos los callbacks puestos por el recurso en otros recursos */

  segmento = descProceso[descRecurso[rindx].pindx].CSProc ;
  r = c2cPFR[DROcupados].primero ;
  while (r != c2cPFR[DROcupados].cabecera) {
    if (descRecurso[r].ccb != ccbNulo)
    eliminarSegCcb(segmento, descRecurso[r].ccb) ;   /* eliminar callbacks */
    r = c2cPFR[DROcupados].e[r].sig ;
  }

  /* mirar si el recurso esta en uso (hay procesos con ficheros abiertos)  */

  dfs = descRecurso[rindx].c2cFichRec.primero ;
  while (dfs != descRecurso[rindx].c2cFichRec.cabecera) {
    dfsSig = descRecurso[rindx].c2cFichRec.e[dfs].sig ;
    if ((descFichero[dfs].contAp_L > 0) ||
        (descFichero[dfs].contAp_E > 0))
      return(-2) ;                                               /* en uso */
    else if (destruirFich(dfs) < 0)
      return(-3) ;                             /* no se puede destruir dfs */
    dfs = dfsSig ;
  }
  if (descRecurso[rindx].c2cFichRec.numElem > 0) return(-4) ;

  descRecurso[rindx].nombre[0] = (char)0 ;
  descRecurso[rindx].tipo = rLibre ;
  vaciarCcb(descRecurso[rindx].ccb) ;
  pindx = descRecurso[rindx].pindx ;
  descProceso[pindx].estado = preparado ;     /* desbloqueamos el driver */
  encolarPC2c(pindx, (ptrC2c_t)&c2cPFR[PPreparados]) ;

//  encolarPC2c(pindx, (ptrC2c_t)&c2cPFR[PUrgentes]) ;        /* prioridad */
//  activarAlEpilogo1 = TRUE ;

  descRecurso[rindx].pindx = (pindx_t)(-1) ;

  for ( j = 0 ; j < descRecurso[rindx].numVI ; j++ ) {
    nVInt = descRecurso[rindx].nVInt[j] ;
    recVInt[nVInt] = -1 ;
    restablecerInt(nVInt) ;
    descRecurso[rindx].nVInt[j] = 0 ;
    descRecurso[rindx].isr[j] = (isr_t)0x00000000 ;
  }

  descRecurso[rindx].numVI = 0 ;

  descRecurso[rindx].open =      (open_t     )0x00000000 ;
  descRecurso[rindx].release =   (release_t  )0x00000000 ;
  descRecurso[rindx].read =      (read_t     )0x00000000 ;
  descRecurso[rindx].aio_read =  (aio_read_t )0x00000000 ;
  descRecurso[rindx].write =     (write_t    )0x00000000 ;
  descRecurso[rindx].aio_write = (aio_write_t)0x00000000 ;
  descRecurso[rindx].lseek =     (lseek_t    )0x00000000 ;
  descRecurso[rindx].fcntl =     (fcntl_t    )0x00000000 ;
  descRecurso[rindx].ioctl =     (ioctl_t    )0x00000000 ;

  descRecurso[rindx].eliminar =  (ioctl_t    )0x00000000 ;

  eliminarPC2c(rindx, (ptrC2c_t)&c2cPFR[DROcupados]) ;
  apilarPC2c(rindx, (ptrC2c_t)&c2cPFR[DRLibres]) ;
  
  matarProcIndx(pindx) ;                   /* en otro caso quedaria zombie */	

  return(0) ;
}

