/* ----------------------------------------------------------------------- */
/*                                 minisf.c                                */
/* ----------------------------------------------------------------------- */
/*                        sistema de ficheros minimo                       */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\def_proc.h>                            /* fedCaracteres */
#include <so1pub.h\bios_0.h>                /* leerTeclaBIOS, printCarBIOS */
#include <so1pub.h\strings.h>                                 /* copiarStr */
#include <so1pub.h\msdos.h>                         /* getDiskDOS, readDOS */
#include <so1pub.h\fcntl.h>                                     /* O_CREAT */
#include <so1.h\ajustes.h>                                       /* CS_SO1 */
#include <so1.h\procesos.h>                            /* indProcesoActual */
#include <so1.h\recursos.h>                                    /* crearRec */
#include <so1.h\interrup.h>                                       /* VIOrg */
#include <so1.h\plot.h>                                            /* plot */
#include <so1.h\sf.h>                                   /* formatearNombre */

rindx_t rec_sf ;

dfs_t dfs_sf ;

#pragma warn -par

static int far openSF ( int dfs, modoAp_t modo ) {

    char far * nombre = (char far *)pointer(tramaProceso->ES, tramaProceso->BX) ;
    int dfMSDOS ;                           /* descriptor de fichero MSDOS */
    int modoDOSExt = 0x2000 ;
    word_t accion = 0x0001 ;
    word_t error ;
    if (modo & O_WRONLY) {
      modoDOSExt = 0x2001 ;
      if (modo & O_CREAT) accion |= 0x0010 ;
      if (modo & O_TRUNC) accion = (accion & 0xFFF0) | 0x0002 ;
    }

    dfMSDOS = extendedOpenDOS((pointer_t)nombre, modoDOSExt, 0, &accion, &error) ; /* atr normal */
    if (dfMSDOS >= 0) {
      dfs = crearFich(nombre, rec_sf, dfMSDOS, fRegular) ;   /* menor = df */
      return(dfs) ;
    }
    return(-1) ;
}

static int far releaseSF ( int dfs ) {
    /* printStrBIOS("\n release dfs = ") ; printDecBIOS(dfs, 1) ; */
    closeDOS(descFichero[dfs].menor) ;       /* se cierra el fichero MSDOS */
    destruirFich(dfs) ;
    return(0) ;
}

static int far readSF ( int dfs, pointer_t dir, word_t nbytes ) {
    int n ;
    n = readDOS(descFichero[dfs].menor, (char far *)dir, nbytes) ;
    return(n) ;
}

static int far aio_readSF ( int dfs, pointer_t dir, word_t nbytes ) {
    return(0) ;
}

static int far writeSF ( int dfs, pointer_t dir, word_t nbytes ) {
    int n ;
    n = writeDOS(descFichero[dfs].menor, (char far *)dir, nbytes) ;
    return(n) ;
}

static int far aio_writeSF ( int dfs, pointer_t dir, word_t nbytes ) {
    return(0) ;
}

static long int far lseekSF ( int dfs, long int pos, word_t whence ) {
    long int posFich = (dword_t)pos ;
    lseekDOS(descFichero[dfs].menor, (dword_t *)&posFich, whence) ;
    return(posFich) ;
}

static int far fcntlSF ( int dfs, word_t cmd, word_t arg ) {
    return(0) ;
}

static int far ioctlSF ( int dfs, word_t request, word_t arg ) {
    return(0) ;
}

#pragma warn +par

#define maxCbSF 0

static descCcb_t descCcbSF = { 0, 0, 0, maxCbSF, NULL } ;

int inicMinisfMSDOS ( void ) {

    descRecurso_t dR ;

    dR.tipo = rSF ;
    copiarStr("SF", dR.nombre) ;
    dR.ccb = (ccb_t)&descCcbSF ;
    dR.pindx = indProcesoActual ;
    dR.numVI = 0 ;

    dR.open      = (open_t)pointer(_CS, (word_t)openSF) ;
    dR.release   = (release_t)pointer(_CS, (word_t)releaseSF) ;
    dR.read      = (read_t)pointer(_CS, (word_t)readSF) ;
    dR.aio_read  = (aio_read_t)pointer(_CS, (word_t)aio_readSF) ;
    dR.write     = (write_t)pointer(_CS, (word_t)writeSF) ;
    dR.aio_write = (aio_write_t)pointer(_CS, (word_t)aio_writeSF) ;
    dR.lseek     = (lseek_t)pointer(_CS, (word_t)lseekSF) ;
    dR.fcntl     = (fcntl_t)pointer(_CS, (word_t)fcntlSF) ;
    dR.ioctl     = (ioctl_t)pointer(_CS, (word_t)ioctlSF) ;

    rec_sf = crearRec(&dR) ;

    dfs_sf = crearFich("SF", rec_sf, 0, fedCaracteres) ;

    return(0) ;
}

int inicMinisfFAT ( void ) {
    return(0) ;
}

