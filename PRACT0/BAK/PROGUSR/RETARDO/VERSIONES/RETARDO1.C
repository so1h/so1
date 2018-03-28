/* ----------------------------------------------------------------------- */
/*                                retardo.c                                */
/* ----------------------------------------------------------------------- */
/*           implementacion del recurso RETARDO (retardo activo)           */
/* ----------------------------------------------------------------------- */

#define SPInicial 0x3FFE      /* Valor inicial puntero de pila del proceso */

#include <so1pub.h\ajustusr.h>                                 /* valor_DS */
#include <so1pub.h\comundrv.h>                 /* ptrIndProcesoActual, ... */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>
#include <so1pub.h\carsctrl.h>                                       /* FF */
#include <so1pub.h\caracter.h>                           /* dig, mayuscula */
#include <so1pub.h\strings.h>                                   /* iguales */
#include <so1pub.h\scanner.h>
#include <so1pub.h\biosdata.h>                              /* ptrBiosArea */

rindx_t rec_retardo ;

/* ----------------------------------------------------------------------- */
/*                   seccion de implementacion del driver                  */
/* ----------------------------------------------------------------------- */

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
  word_t DS_Retardo = valor_DS ;
  int res ;
  asm push ds
  asm mov ds,DS_Retardo                      /* establecemos DS del driver */
  if (!retardando[*ptrIndProcesoActual]) res = -1 ;
  else {
    retardar(nVueltasRetardo) ;
    res = 0 ;
  }
  asm pop ds                                 /* restauramos DS del proceso */
  return(res) ;
}

#pragma warn -par

static int far openRetardo ( int dfs, modoAp_t modo ) {
  word_t DS_Retardo = valor_DS ;
  pindx_t indProcesoActual ;
  int res = 0 ;

  asm push ds
  asm mov ds,DS_Retardo

  indProcesoActual = *ptrIndProcesoActual ;
  if (retardando[indProcesoActual]) res = -1 ;
  else retardando[indProcesoActual] = TRUE ;

  asm pop ds
  return(res) ;
}

static int far releaseRetardo ( int dfs ) {
  word_t DS_Retardo = valor_DS ;
  pindx_t indProcesoActual ;
  int res = 0 ;

  asm push ds
  asm mov ds,DS_Retardo

  indProcesoActual = *ptrIndProcesoActual ;
  if (!retardando[indProcesoActual]) res = -1 ;
  else retardando[indProcesoActual] = FALSE ;

  asm pop ds
  return(res) ;
}

static int far readRetardo ( int dfs, pointer_t dir, word_t nbytes ) {
  word_t DS_Retardo = valor_DS ;
  pindx_t indProcesoActual ;
  int res = 4 ;
  dword_t far * ptrDWord = (dword_t far *)dir ;

  asm push ds
  asm mov ds,DS_Retardo

  indProcesoActual = *ptrIndProcesoActual ;
  if (!retardando[indProcesoActual]) res = -1 ;      /* fichero no abierto */
  else if (nbytes != 4) res = -2 ;                    /* nbytes incorrecto */
  else
    switch (dir[0]) {
    case 0x00 : ptrDWord[0] = nVueltasRetardo ;
                break ;
    case 0x01 : ptrDWord[0] = (dword_t far *)
                  pointer(_CS, (word_t)retardarProceso) ;
                break ;
    default : res = -3 ;                 /* codigo de operacion incorrecto */
   }

  asm pop ds
  return(res) ;
}

static int far aio_readRetardo ( int dfs, pointer_t dir, word_t nbytes ) {
  return(readRetardo(dfs, dir, nbytes)) ;
}

static int far writeRetardo ( int dfs, pointer_t dir, word_t nbytes ) {
  word_t DS_Retardo = valor_DS ;
  pindx_t indProcesoActual ;
  int res = 4 ;
  dword_t far * ptrDWord = (dword_t far *)dir ;

  asm push ds
  asm mov ds,DS_Retardo

  indProcesoActual = *ptrIndProcesoActual ;
  if (!retardando[indProcesoActual]) res = -1 ;      /* fichero no abierto */
  else if (nbytes != 4) res = -2 ;
  else nVueltasRetardo = ptrDWord[0] ;

  asm pop ds
  return(res) ;
}

static int far aio_writeRetardo ( int dfs, pointer_t dir, word_t nbytes ) {
  return(-1) ;
}

static long int far lseekRetardo ( int dfs, long int pos, word_t whence ) {
  return((long int)-1) ;
}

static int far fcntlRetardo ( int dfs, word_t cmd, word_t arg ) {
  return(-1) ;
}

static int far ioctlRetardo ( int dfs, word_t request, word_t arg ) {
  return(-1) ;
}

#pragma warn +par

/* ----------------------------------------------------------------------- */
/*                      seccion de programa de usuario                     */
/* ----------------------------------------------------------------------- */

static void formato ( void ) {
  escribirStr(" formato: RETARDO [ -i | -u | -h ] ") ;
  exit(-1) ;
}

static void help ( void ) {
  escribirLn() ;
  escribirLn() ;
  escribirStr(" formato : RETARDO [ -i | -u | -h ]             \n\n") ;
  escribirStr(" instala/desinstala el driver de retardo activo \n\n") ;
  escribirStr(" opciones: (por defecto -i)                     \n\n") ;
  escribirStr("      -i : instala el driver (usar &)           \n") ;
  escribirStr("      -u : desintala el driver                  \n") ;
  escribirStr("      -h : muestra este help                    \n") ;
  exit(0) ;
}

descProceso_t descProceso [ maxProcesos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

#define maxCbRT 0

static descCcb_t descCcbRT = { 0, 0, 0, maxCbRT, NULL } ;

static dword_t valorRetardo ( void ) { /* permite las interrupciones temporalmente */
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

static int instalarRetardo ( void ) {

  int i ;
  int dfs ;
  descRecurso_t dR ;

  obtenInfoPS((descProceso_t far *)&descProceso, (e2PFR_t far *)&e2PFR, (c2c_t far *)&c2cPFR) ;

  for ( i = 0 ; i < tamComando ; i++ )
    comando[0][i] = descProceso[getpindx()].comando[i] ;

  inicScanner() ;
  while (car != (char)0) obtenCar() ;
  if (comando[0][pos] != '&') { /* truco: crearProceso deja ahi &, <, >, | */
    escribirStr(" este driver debe ejecutarse con & ") ;
    return(-1) ;
  }
  else {

    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */

    dR.tipo = rDCaracteres ;
    copiarStr("RETARDO", dR.nombre) ;
    dR.ccb = (ccb_t)&descCcbRT ;
    dR.ccb->arg = NULL ;
    dR.pindx = getpindx() ;
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

    rec_retardo = crearRecurso(&dR) ;

    nVueltasRetardo = valorRetardo() ;

    for ( i = 0 ; i < maxProcesos ; i ++ ) retardando[i] = FALSE ;

    if (rec_retardo >= 0) {

      dfs = crearFichero("RETARDO", rec_retardo, 0, fedCaracteres) ;

      if (dfs >= 0) {
        escribirStr("\n\n recurso RETARDO instalado (fichero: RETARDO) ") ;
        escribirStr("\n\n nVueltasRetardo = ") ;
        escribirLDec(nVueltasRetardo, 1) ;
        escribirStr("\n") ;
        esperarDesinstalacion(0) ;                   /* bloquea el proceso */
        return(0) ;
      }
      else switch(dfs) {
      case -1 : escribirStr(" tipo de fichero erroneo") ; break ;
      case -2 : escribirStr(" numero de recurso erroneo") ; break ;
      case -3 : escribirStr(" RETARDO nombre de fichero ya existente") ; break ;
      case -4 : escribirStr(" no hay descriptores de fichero libres") ; break ;
      default : escribirStr(" no se ha podido crear el fichero RETARDO") ;
      }
      destruirRecurso("RETARDO") ;
    }
    else switch(rec_retardo) {
    case -1 : escribirStr(" tipo de recurso erroneo") ; break ;
    case -2 : escribirStr(" demasiados vectores de interrupcion") ; break ;
    case -3 : escribirStr(" RETARDO nombre de recurso ya existente") ; break ;
    case -4 : escribirStr(" no hay descriptores de recurso libres") ; break ;
    case -5 : escribirStr(" numero de vector de interrupcion ya usado") ; break ;
    default : escribirStr(" no se ha podido crear el recurso RETARDO") ;
    }
    return(-1) ;
  }
}

void main ( int argc, char * argv [ ] ) {
  int res ;
  if (argc > 2) formato() ;
  else if (argc == 1) exit(instalarRetardo()) ;
  else if (iguales(argv[1], "-h") || iguales(argv[1], "-H")) help() ;
  else if (iguales(argv[1], "-i") || iguales(argv[1], "-I")) exit(instalarRetardo()) ;
  else if (iguales(argv[1], "-u") || iguales(argv[1], "-U")) {
    res = destruirRecurso("RETARDO") ;
    switch (res) {
    case  0 : escribirStr(" recurso RETARDO desinstalado") ; break ;
    case -1 : escribirStr(" recurso RETARDO no existe") ; break ;
    case -2 : escribirStr(" recurso RETARDO en uso") ; break ;
    case -3 : escribirStr(" fichero RETARDO no puede destruirse") ; break ;
    default : escribirStr(" RETARDO no ha podido desinstalarse") ;
    }
    exit(res) ;
  }
  else formato() ;
}
