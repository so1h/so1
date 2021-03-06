/* ----------------------------------------------------------------------- */
/*                                 timer.c                                 */
/* ----------------------------------------------------------------------- */
/*             implementacion del recurso TIMER (temporizador)             */
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
#include <so1pub.h\puertos.h>
#include <so1pub.h\pic.h>
#include <so1pub.h\def_timer.h>
/* #include <so1pub.h\bios_0.h> */

static rindx_t rec_timer ;

/* ----------------------------------------------------------------------- */
/*                   seccion de implementacion del driver                  */
/* ----------------------------------------------------------------------- */

#define nVIntTimer   0x08       /* numero de vector utilizado por el timer */

static void far isr_timer ( void ) ;                            /* forward */

static dobleEnlace_t eTimer [ maxProcesos + 1 ] ;

static c2c_t colaTimer ;

static int ticsPorLeer [ maxProcesos ] ;                  /* tics por leer */

static pointer_t dirDestino [ maxProcesos ] ;  /* dir. donde dejar sig tic */

#define ticsPorRodajaMax 180              /* maximo valor de ticsPorRodaja */

#define ticsPorRodajaPorDefecto 18                    /* valor por defecto */

static word_t ticsPorRodaja ;

static word_t ticsVerReloj  = 4 ;

static word_t contTicsReloj = 0 ;             /* 0, 1, 2, ... ticsVerReloj */

static char ocioso [4] = { '|', '/', '-', '\\' } ;

static dword_t contTics = 0 ;

/* ----------------------------------------------------------------------- */

#pragma warn -par

static int far openTimer ( int dfs, modoAp_t modo ) {
  return(0) ;
}

static int far releaseTimer ( int dfs ) {
  return(0) ;
}

static int far readTimer ( int dfs, pointer_t dir, word_t nbytes ) {

  word_t DS_Timer = valor_DS ;
  pindx_t indProcesoActual ;

  asm push ds
  asm mov ds,DS_Timer

  indProcesoActual = *ptrIndProcesoActual ;

  /* El proceso se bloquea a la espera de que transcurran nbytes tics      */
  /* Si (dir != NULL) para cada siguiente tic i                            */
  /*   se guarda en dir[i] el bytes (contTics % 256)                       */

  ticsPorLeer[indProcesoActual] = nbytes ;
  if (seg(dir) == ptrDescProceso[indProcesoActual].CSProc)
    dirDestino[indProcesoActual] = dir ;
  else if (dir != NULL)
    dirDestino[indProcesoActual] = dir ;
  else
    dirDestino[indProcesoActual] = dir ;          /* solo espera sin copia */
  encolarPC2c(indProcesoActual, (ptrC2c_t)&colaTimer) ;
  bloquearProcesoActual(rec_timer) ;                         /* no retorna */
}

#define tamBuf 12

static char buf [ tamBuf ] ;               /* no poner como variable local */

static int far aio_readTimer ( int dfs, pointer_t dir, word_t nbytes ) {

  /* El proceso lee los bytes que solicita del "fichero" virtual:          */
  /*                                                                       */
  /*   TTTTTTTTRRRRRRRRCCCCXXXX (digitos hexadecimales)                    */
  /*                                                                       */
  /* donde TTTTTTTT son los 4 bytes de contTics                            */
  /* donde RRRRRRRR son los 4 bytes de contRodajas                         */
  /* donde CCCC     son los 2 bytes de contTicsRodaja                      */
  /* donde XXXX     son los 2 bytes de ticsPorRodaja                       */

  word_t DS_Timer = valor_DS ;
  int i, j ;
  int df ;
  word_t pos ;
  word_t posNueva ;

  asm push ds
  asm mov ds,DS_Timer

  df = (*ptrTramaProceso)->BX ;
  pos = (word_t)ptrDescProceso[*ptrIndProcesoActual].tfa[df].pos ;

  if (pos >= tamBuf) nbytes = 0 ;
  else {
    *((dword_t far *)&(buf[ 0])) = contTics ;                  /* contTics */
    *((dword_t far *)&(buf[ 4])) = *ptrContRodajas ;        /* contRodajas */
    *((int     far *)&(buf[ 8])) = *ptrContTicsRodaja ;  /* contTicsRodaja */
    *((word_t  far *)&(buf[10])) = ticsPorRodaja ;        /* ticsPorRodaja */
    posNueva = pos + nbytes ;
    if (posNueva > tamBuf) {
      posNueva = tamBuf ;
      nbytes = posNueva - pos ;
    }
    j = 0 ;
    for ( i = pos ; i < posNueva ; i++ )                 /* primeros bytes */
      dir[j++] = buf[i] ;
  }

  asm pop ds
  return(nbytes) ;
}

static int far writeTimer ( int dfs, pointer_t dir, word_t nbytes ) {
  return(0) ;
}

static int far aio_writeTimer ( int dfs, pointer_t dir, word_t nbytes ) {

  /* El proceso escribe los bytes en el "fichero" virtual:                 */
  /*                                                                       */
  /*   TTTTTTTTRRRRRRRRCCCCXXXX (digitos hexadecimales)                    */
  /*                                                                       */
  /* donde TTTTTTTT son los 4 bytes de contTics                            */
  /* donde RRRRRRRR son los 4 bytes de contRodajas                         */
  /* donde CCCC     son los 2 bytes de contTicsRodaja                      */
  /* donde XXXX     son los 2 bytes de ticsPorRodaja                       */

  word_t DS_Timer = valor_DS ;
  int i, j ;
  int df ;
  word_t pos ;
  word_t posNueva ;

  dword_t contTicsAux ;
  dword_t contRodajasAux ;
  int contTicsRodajaAux ;
  word_t ticsPorRodajaAux ;

  asm push ds
  asm mov ds,DS_Timer

  df = (*ptrTramaProceso)->BX ;
  pos = (word_t)ptrDescProceso[*ptrIndProcesoActual].tfa[df].pos ;

  if (pos >= tamBuf) nbytes = 0 ;
  else {
    *((dword_t far *)&(buf[ 0])) = contTics ;                  /* contTics */
    *((dword_t far *)&(buf[ 4])) = *ptrContRodajas ;        /* contRodajas */
    *((int     far *)&(buf[ 8])) = *ptrContTicsRodaja ;  /* contTicsRodaja */
    *((word_t  far *)&(buf[10])) = ticsPorRodaja ;        /* ticsPorRodaja */
    posNueva = pos + nbytes ;
    if (posNueva > tamBuf) {
      posNueva = tamBuf ;
      nbytes = posNueva - pos ;
    }
    j = 0 ;
    for ( i = pos ; i < posNueva ; i++ )
      buf[i] = dir[j++];

    contTicsAux       = *((dword_t far *)&(buf[ 0])) ;         /* contTics */
    contRodajasAux    = *((dword_t far *)&(buf[ 4])) ;      /* contRodajas */
    contTicsRodajaAux = *((int     far *)&(buf[ 8])) ;   /* contTicsRodaja */
    ticsPorRodajaAux  = *((word_t  far *)&(buf[10])) ;    /* ticsPorRodaja */

    if ((-1 <= contTicsRodajaAux) &&               /* comprobar correccion */
        (0 < ticsPorRodajaAux) &&
        (ticsPorRodajaAux <= ticsPorRodajaMax)) {
      contTics           = contTicsAux ;           /* poner nuevos valores */
      *ptrContRodajas    = contRodajasAux ;
      if (contTicsRodajaAux < ticsPorRodajaAux)
        *ptrContTicsRodaja = contTicsRodajaAux ;
      else
        *ptrContTicsRodaja = -1 ;
      ticsPorRodaja      = ticsPorRodajaAux ;
    }
    else nbytes = -1 ;
  }
  asm pop ds
  return(nbytes) ;
}

static long far lseekTimer ( int dfs, long pos, word_t whence ) {

  word_t DS_Timer = valor_DS ;

  int df ;
  long res = -1L ;
  long posNueva ;

  asm push ds
  asm mov ds,DS_Timer

  switch (whence) {
    case SEEK_SET : if ((0 <= pos) && (pos <= (long)tamBuf))
                      res = pos ;
                    break ;
    case SEEK_CUR : df = (*ptrTramaProceso)->BX ;
                    posNueva = ptrDescProceso[*ptrIndProcesoActual].tfa[df].pos + pos ;
    case SEEK_END : if (whence == SEEK_END) posNueva = (long int)tamBuf + pos ;
                    if (posNueva < (long int)0)
                      res = 0 ;
                    else if (posNueva > (long int)tamBuf)
                      res = (long int)tamBuf ;
                    else
                      res = posNueva ;
                    break ;
    default : ;
  }

  asm pop ds
  return(res) ;
}

static int far fcntlTimer ( int dfs, word_t cmd, word_t arg ) {
  return(0) ;
}

static int far ioctlTimer ( int dfs, word_t request, word_t arg ) {
  return(0) ;
}

#pragma warn +par

/* ----------------------------------------------------------------------- */
/*                  rutina de servicio de la interrupcion                  */
/* ----------------------------------------------------------------------- */

static void procesarTicColaTimer ( void ) {
  int i ;
  pindx_t pindx ;
  for ( i = 0 ; i < colaTimer.numElem ; i++ ) {
    pindx = desencolarPC2c(&colaTimer) ;
    if (dirDestino[pindx] != NULL)
      *(dirDestino[pindx])++ = (char)(contTics % 256) ;
    if (--(ticsPorLeer[pindx]) <= 0) {
      ptrDescProceso[pindx].estado = preparado ;
      encolarPC2c(pindx, (ptrC2c_t)&ptrC2cPFR[PPreparados]) ;
    }
    else
      encolarPC2c(pindx, (ptrC2c_t)&colaTimer) ;
  }
}

#define maxCbTM 9

static descCcb_t descCcbTM = { 0, 0, 0, maxCbTM, NULL,
  cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo
} ;

static argCbTimer_t argCbTimer ;

static void far isr_timer ( ) {

  word_t DS_Timer = valor_DS ;
  pindx_t indProcesoActual ;

  asm push ds
  asm mov ds,DS_Timer

  indProcesoActual = *ptrIndProcesoActual ;

  /* plot('t', 0, contadorTimer0()) ; */ /* mejor antes de llamar a la rti bios */

                       /* simulamos la interrupcion con el vector guardado */
                            /* necesario para el timeout de leerSectorBIOS */
                                                   /* int 13h con ah = 02h */

  ptrVIOrg[nVIntTimer]() ;   /* (pushf + int) da la int. por servida (pic) */

/*  printCarBIOS('.') ; */
/*  printStrBIOS("\n contTics = ") ; */
/*  printLDecBIOS(contTics, 5) ; */

  contTics++ ;

  argCbTimer.contTics = contTics ;
  argCbTimer.contRodajas = *ptrContRodajas ;
  argCbTimer.contTicsRodaja = *ptrContTicsRodaja ;
  argCbTimer.ticsPorRodaja = ticsPorRodaja ;

  atenderCcb((ccb_t)&descCcbTM) ;

  procesarTicColaTimer() ;

  if (indProcesoActual != -1) {           /* se esta ejecutando un proceso */
    if (++(*ptrContTicsRodaja) == ticsPorRodaja) {
      if ((ptrC2cPFR[PPreparados].numElem > 0) &&
          (*ptrNivelActivacionSO1 == 1)) {                   /* 17-10-2010 */
        ptrDescProceso[indProcesoActual].trama =
          (trama_t far *)pointer(*ptrSS_Proceso, *ptrSP_Proceso) ;
        ptrDescProceso[indProcesoActual].estado = preparado ;
        encolarPC2c(indProcesoActual, (ptrC2c_t)&ptrC2cPFR[PPreparados]) ;
        *ptrHayTic = TRUE ;
        buscarNuevoProcesoActual() ;                      /* hayTic = TRUE */
      }
      else {
        (*ptrContRodajas)++ ;
        *ptrContTicsRodaja = 0 ;
      }
    }
/*    plot('e', 0, contadorTimer0()) ; */           /* no hay cambio de estado */
  }
  else {                                         /* indProcesoActual == -1 */
    if (ptrC2cPFR[PPreparados].numElem > 0)    /* va a haber cambio de estado */
      ptrDescProceso[ptrC2cPFR[PPreparados].primero].estado = ejecutandose ;     /* truco */
/*    plot('e', 0, contadorTimer0()) ; */
    if (ptrC2cPFR[PPreparados].numElem > 0)
      ptrDescProceso[ptrC2cPFR[PPreparados].primero].estado = preparado ;
  }

  asm pop ds
}

/* ----------------------------------------------------------------------- */
/*                      seccion de programa de usuario                     */
/* ----------------------------------------------------------------------- */

static void formato ( void ) {
  escribirStr("\n\n formato: TIMER [ -i [ tpr ] | -u | -s | -c tpr | -h | tpr ] \n") ;
  exit(-1) ;
}

static void help ( void ) {
  escribirLn() ;
  escribirLn() ;
  escribirStr(" formato : TIMER [ -i [ tpr ] | -u | -s | -c tpr | -h | tpr ] \n\n") ;
  escribirStr(" instala/desinstala el driver del timer  \n\n") ;
  escribirStr(" opciones: (por defecto -i y tpr = 18)   \n\n") ;
  escribirStr("     tpr : establece los tics por rodaja \n") ;
  escribirStr("      -i : instala el driver (usar &)    \n") ;
  escribirStr("      -u : desintala el driver           \n") ;
  escribirStr("      -s : muestra el estado del timer   \n") ;
  escribirStr("      -c : cambia los tics por rodaja    \n") ;
  escribirStr("      -h : muestra este help             \n") ;
  exit(0) ;
}

static descProceso_t descProceso [ maxProcesos ] ;

static e2PFR_t e2PFR ;

static c2c_t c2cPFR [ numColasPFR ] ;

static int instalarTimer ( word_t ticsPR ) {

  int i ;
  dfs_t dfs ;
  descRecurso_t dR ;

  if (ticsPR > ticsPorRodajaMax) {
    escribirStr(" ticsPorRodaja demasiado grande ") ;
    return(-1) ;
  }

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
    copiarStr("TIMER", dR.nombre) ;
    dR.ccb = (ccb_t)&descCcbTM ;
    dR.ccb->arg = pointer(_DS, (word_t)&argCbTimer) ;
    dR.pindx = getpindx() ;
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

    rec_timer = crearRecurso(&dR) ;

    if (rec_timer >= 0) {

      dfs = crearFichero("TIMER", rec_timer, 0, fedCaracteres) ;

      if (dfs >= 0) {
        inicPC2c(&colaTimer, &eTimer, maxProcesos + 0, FALSE) ;
        for ( i = 0 ; i < maxProcesos ; i ++ ) ticsPorLeer[i] = 0 ;
        ticsPorRodaja = ticsPR ;
        escribirStr("\n\n recurso TIMER instalado (fichero: TIMER) ") ;
        escribirStr("\n\n ticsPorRodaja = ") ;
        escribirDec(ticsPorRodaja, 1) ;
        escribirStr("\n") ;
        esperarDesinstalacion(0) ;                   /* bloquea el proceso */
        return(0) ;
      }
      else switch(dfs) {
      case -1 : escribirStr(" tipo de fichero erroneo") ; break ;
      case -2 : escribirStr(" numero de recurso erroneo") ; break ;
      case -3 : escribirStr(" TIMER nombre de fichero ya existente") ; break ;
      case -4 : escribirStr(" no hay descriptores de fichero libres") ; break ;
      default : escribirStr(" no se ha podido crear el fichero TIMER") ;
      }
      destruirRecurso("TIMER") ;
    }
    else switch(rec_timer) {
    case -1 : escribirStr(" tipo de recurso erroneo") ; break ;
    case -2 : escribirStr(" demasiados vectores de interrupcion") ; break ;
    case -3 : escribirStr(" TIMER nombre de recurso ya existente") ; break ;
    case -4 : escribirStr(" no hay descriptores de recurso libres") ; break ;
    case -5 : escribirStr(" numero de vector de interrupcion ya usado") ; break ;
    default : escribirStr(" no se ha podido crear el recurso TIMER") ;
    }
    return(-1) ;
  }
}

void main ( int argc, char * argv [ ] ) {
  int res ;
  int dfTimer ;
  dword_t contTics ;
  dword_t contRodajas ;
  int contTicsRodaja ;
  word_t ticsPorRodaja ;
  if (argc > 3) formato() ;
  else if (argc == 1) exit(instalarTimer(ticsPorRodajaPorDefecto)) ;
  else if (argc == 2) {
    if (iguales(argv[1], "-h") || iguales(argv[1], "-H")) help() ;
    else if (iguales(argv[1], "-i") || iguales(argv[1], "-I"))
      exit(instalarTimer(ticsPorRodajaPorDefecto)) ;
    else if (iguales(argv[1], "-s") || iguales(argv[1], "-S")) {
      dfTimer = open("TIMER", O_RDONLY) ;
      if (dfTimer < 0) {
        escribirStr(" recurso TIMER todavia no instalado") ;
        exit(-1) ;
      }
      else {
        aio_read(dfTimer, (pointer_t)&contTics, 4) ;
        aio_read(dfTimer, (pointer_t)&contRodajas, 4) ;
        aio_read(dfTimer, (pointer_t)&contTicsRodaja, 2) ;
        aio_read(dfTimer, (pointer_t)&ticsPorRodaja, 2) ;
        close(dfTimer) ;
        escribirStr("\n\n contTics = ") ;
        escribirLDec(contTics, 1) ;
        escribirStr(" contRodajas = ") ;
        escribirLDec(contRodajas, 1) ;
        escribirStr(" contTicsRodaja = ") ;
        escribirInt(contTicsRodaja, 1) ;
        escribirStr(" ticsPorRodaja = ") ;
        escribirDec(ticsPorRodaja, 1) ;
        escribirStr("\n") ;
        exit(0) ;
      }
    }
    else if (iguales(argv[1], "-u") || iguales(argv[1], "-U")) {
      res = destruirRecurso("TIMER") ;
      switch (res) {
      case  0 : escribirStr(" recurso TIMER desinstalado") ; break ;
      case -1 : escribirStr(" recurso TIMER no existe") ; break ;
      case -2 : escribirStr(" recurso TIMER en uso") ; break ;
      case -3 : escribirStr(" fichero TIMER no puede destruirse") ; break ;
      default : escribirStr(" TIMER no ha podido desinstalarse") ;
      }
      exit(res) ;
    }
  }
  if ((argc == 2) ||
      ((argc == 3) &&
       ((iguales(argv[1], "-i") || iguales(argv[1], "-I")) ||
        (iguales(argv[1], "-c") || iguales(argv[1], "-C"))
       )
      )
     ) {
    copiarStr(argv[argc-1], comando[0]) ;
    inicScanner() ;
    obtenSimb() ;
    if (simb == s_numero) {
      if (num == 0) {
        escribirStr(" ticsPorRodaja debe ser > 0") ;
        exit(-1) ;
      }
      else if (iguales(argv[1], "-c") || iguales(argv[1], "-C")) {
        dfTimer = open("TIMER", O_WRONLY) ;
        if (dfTimer < 0) {
          escribirStr(" recurso TIMER todavia no instalado") ;
          exit(-1) ;
        }
        else {
          if ((res = lseek(dfTimer, 10, SEEK_SET)) == (long int)10)
            res = aio_write(dfTimer, (pointer_t)&num, sizeof(num)) ;
          close(dfTimer) ;
          if (res < 0) exit(-1) ;
          else exit(0) ;
        }
      }
      else exit(instalarTimer(num)) ;
    }
  }
  else if (iguales(argv[1], "-c") || iguales(argv[1], "-C")) {
    copiarStr(argv[argc-1], comando[0]) ;
    inicScanner() ;
    obtenSimb() ;
    if (simb == s_numero) {
      if (num == 0) {
        escribirStr(" ticsPorRodaja debe ser > 0") ;
        exit(-1) ;
      }
      else if (iguales(argv[1], "-c") || iguales(argv[1], "-C")) {
        dfTimer = open("TIMER", O_WRONLY) ;
        if (dfTimer < 0) {
          escribirStr(" recurso TIMER todavia no instalado") ;
          exit(-1) ;
        }
        else {
          if ((res = lseek(dfTimer, 10, SEEK_SET)) == (long int)10)
            res = aio_write(dfTimer, (pointer_t)&num, sizeof(num)) ;
          close(dfTimer) ;
          if (res < 0) exit(-1) ;
          else exit(0) ;
        }
      }
      else exit(instalarTimer(num)) ;
    }
  }
  formato() ;
}

