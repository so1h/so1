/* ----------------------------------------------------------------------- */
/*                               procesos.c                                */
/* ----------------------------------------------------------------------- */
/*                          Gestion de procesos                            */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>  /* word_t, dword_t, bool_t, NULL, FALSE, TRUE */
                                                               /* seg, off */
//                                            /* pointer_t, pointer, MK_FP */
#include <so1pub.h\const.h>     /* maxProcesos, dfsMax, maxRecursos, dfMax */
#include <so1pub.h\c2c.h>                          /* c2c_t, dobleEnlace_t */
#include <so1pub.h\ccb.h>                                 /* ccb_t, cbNulo */
#include <so1pub.h\def_trm.h>                                   /* trama_t */
#include <so1pub.h\def_proc.h>                           /* pid_t, pindx_t */ 
//                                        /* descFichero_t, descRecurso_t, */
//                                 /* numColasPFR, e2PFR_t, pid_t, pindx_t */
//             /* PPreparados, POrdenados, PDormidos, DPLibres, DPOcupados */
//                            /* libre, preparado, ejecutandose, bloqueado */ 
//                                    /* cabecera_t, STDIN, STDERR, flibre */
//                                                           /* rec_zombie */
#include <so1pub.h\ptrc2c.h>      /* ptrC2c_t, encolarPC2c, desencolarPC2c */
//                                     /* eliminarPC2c, inicPC2c, estaPC2c */
#include <so1pub.h\scanner.h>                                /* tamComando */
#include <so1pub.h\fcntl.h>                /* modoAp_t, O_RDONLY, O_WRONLY */
#include <so1pub.h\puertos.h>                            /* contadorTimer0 */
#include <so1pub.h\strings.h>                   /* copiarStr, igualesHasta */
#include <so1pub.h\copia.h>                           /* copia, copiaLarga */
#include <so1pub.h\startbss.h>                                 /* startBSS */
#include <so1pub.h\finbss.h>                                     /* finBSS */
#include <so1pub.h\caracter.h>                                      /* dig */
#include <so1pub.h\bios_0.h>                           /* printStrBIOS ... */

#include <so1.h\procesos.h>                            /* descProcesoExt_t */

#include <so1.h\blockpr.h>       /* enHalt, hayTic, SS_Proceso, SP_Proceso */
//                               /* SS_Tarea, SP_Tarea, nivelActivacionSO1 */
#include <so1.h\ajustes.h>    /* ptrMagicByteSO1, ptrMagicByteUsr, modoSO1 */
//                                /* modoSO1_Bin, modoSO1_Com, modoSO1_Exe */
//                                                       /* CS_SO1, DS_SO1 */
#include <so1.h\gm.h>                                  /* k_devolverBloque */
#include <so1.h\plot.h>                                            /* plot */
#include <so1.h\dbgword.h>                                    /* debugWord */ /***************/

descProcesoExt_t descProceso [ maxProcesos ] ;        /* tabla de procesos */

descFichero_t descFichero [ dfsMax ] ;                /* tabla de ficheros */
                                                   /* abiertos del sistema */
descRecurso_t descRecurso [ maxRecursos ] ;

c2c_t c2cPFR [ numColasPFR ] ;                           /* indices cPFR_t */

e2PFR_t e2PFR ;

#define maxCbAlEpilogo 9

descCcb_t descCcbAlEpilogo = { 0, 0, 0, maxCbAlEpilogo, (void far *)NULL,
  { cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo} } ;

ccb_t ccbAlEpilogo ;

word_t numPids = 0 ;          /* numero de pids asignados hasta el momento */

int indProcesoActual = 0 ;              /* indice del proceso en ejecucion */

int indProcesoDeSuperficie = 0 ;

dword_t contRodajas ;                               /* contador de rodajas */

int contTicsRodaja ;                     /* contador de tics de una rodaja */

word_t contadorTimer00 ;  /* valor de contadorTimer0() al pasar a ej. p.a. */

trama_t far * tramaProceso ;

trama_t far * tramaTarea ;

void resetPids ( void ) {
  numPids = 1 ;                        /* primer pid de un proceso usuario */
}

pid_t nuevoPid ( void ) {
  return(numPids++) ;
}

pindx_t indice ( pid_t pid ) {
  pindx_t i ;
  for ( i = 0 ; i < maxProcesos ; i++) {
    if ((descProceso[i].estado != libre) &&
        (descProceso[i].pid == pid))
      return(i) ;
  }
  return(-1) ;
}

int contOcioso = - 1 ;

pindx_t far sigProceso ( void ) {              /* planificador (scheduler) */

  static pindx_t pindx ;                   /* printStrWin(win_so, "\nS") ; */

  contOcioso = -1 ;
  while (c2cPFR[PPreparados].numElem == 0) {
    if (indProcesoActual != -1)
      plot('e', 0, contadorTimer0()) ;
    indProcesoActual = -1 ;                  /* printStrWin(win_so, "h") ; */
    enHalt = TRUE ;
    asm { 
	  sti ;
      hlt ;                                     /* detenemos el procesador */
      cli ;
	}
  }

  pindx = desencolarPC2c((ptrC2c_t)&c2cPFR[PPreparados]) ;
/*
if (debugWord == 0x0001) {
  printStrBIOS("\n sigProceso(): ") ;
  printStrBIOS("\n indProcesoActual = ") ;
  printIntBIOS(indProcesoActual, 1) ;
  printStrBIOS(" pindx = ") ;
  printIntBIOS(pindx, 1) ;
  while(TRUE) ;
}
*/
  return(pindx) ;

}

int far activarProceso ( pindx_t pindx ) {
/*
if (debugWord == 0x0001) {
  printStrBIOS("\n activarProceso(): ") ;
  printStrBIOS("\n indProcesoActual = ") ;
  printIntBIOS(indProcesoActual, 1) ;
  printStrBIOS(" pindx = ") ;
  printIntBIOS(pindx, 1) ;
  while(TRUE) ;
}
*/
/*
  printStrWin(win_so, "\nA(") ;
  printDecWin(win_so, pindx, 1) ;
  printStrWin(win_so, ") numPPrep = ") ;
  printDecWin(win_so, numPPrep, 1) ;
  printStrWin(win_so, "\n") ;
*/

  if ((pindx < 0) || (descProceso[pindx].pid < 0)) {
/*
    printStrWin(win_so, "\n activarProceso: no existe el proceso ") ;
    printDecWin(win_so, descProceso[pindx].pid, 1) ;
    printStrWin(win_so, " con indice ") ;
    printDecWin(win_so, pindx, 1) ;
    printStrWin(win_so, "\n") ;
*/
    return(-1) ;
  }

  if (descProceso[pindx].estado != preparado) {
/*
    printStrWin(win_so, "\n el proceso ") ;
    printDecWin(win_so, descProceso[pindx].pid, 1) ;
    printStrWin(win_so, " con indice ") ;
    printDecWin(win_so, pindx, 1) ;
    printStrWin(win_so, " no esta preparado\n") ;
*/
    return(-1) ;
  }

  descProceso[pindx].estado = ejecutandose ;
  indProcesoActual = pindx ;

  plot('e', 0, contadorTimer0()) ;

  contRodajas++ ;                                                      /*T0*/

  if (hayTic)                 /* se llamo a activarProceso desde rti_timer */
    contTicsRodaja = 0 ;
  else
    contTicsRodaja = -1 ;   /* ya que un tic puede estar a punto de llegar */
  hayTic = FALSE ;

  SS_Proceso = seg((pointer_t)(descProceso[indProcesoActual].trama)) ;
  SP_Proceso = off((pointer_t)(descProceso[indProcesoActual].trama)) ;

  /* SS_Proceso y SP_Proceso son variables globales. Si fueran variables   */
  /* locales habria que tener cuidado en el orden de las 3 instrucciones   */
  /* ya que se modifica la pila y el orden de las instrucciones seria      */
  /* critico                                                               */

  enHalt = FALSE ;

  nivelActivacionSO1 = 0 ;                 /* esta ejecutandose un proceso */

  asm {
	mov ss,SS_Proceso ;                /* establecemos la pila del proceso */
    mov sp,SP_Proceso ;

    pop ds ;              /* establecemos el segmento de datos del proceso */
    pop es ;

    popa ;                        /* restauramos los registros del proceso */

    iret ;                                   /* restauramos IP, CS y Flags */
  }
  
  return(0) ;   /* nunca se retorna por aqui (puesto para evitar warnings) */ 

}

void salvarProceso ( void ) {                    /* indProcesoActual != -1 */
  descProceso[indProcesoActual].trama =
    (trama_t far *)pointer(SS_Proceso, SP_Proceso) ;
}

void salvarTareaInterrumpida ( void ) {          /* indProcesoActual != -1 */
  descProceso[indProcesoActual].trama =
    (trama_t far *)pointer(SS_Tarea, SP_Tarea) ;
}

void ponerNumeroHex ( word_t num, char * * str ) {              /* num > 0 */
  word_t d = 0x1000 ;
  if (num == 0) *(*str)++ = '0' ;
  else {
    while (num/d == 0) d = (d >> 4) ;
    while (d > 0) {
      *(*str)++ = dig[num/d] ;
      num = num % d ;
      d = (d >> 4) ;
    }
  }
}

void ponerNumero ( word_t num, char * * str ) {                 /* num > 0 */
  word_t d = 10000 ;
  if (num == 0) *(*str)++ = '0' ;
  else {
    while (num/d == 0) d = d/10 ;
    while (d > 0) {
      *(*str)++ = '0' + (num/d) ;
      num = num % d ;
      d = d/10 ;
    }
  }
}

static int ponerArgs ( pindx_t pindx, word_t DSProc, word_t offDATADS, word_t far * offArgv ) {
  int i = 0 ;
  int argc = 0 ;
  char car ;
  bool_t estadoBlanco = TRUE ;
  pointer_t ptrCmdLine ;
  char * far * ptrArgv ;
  ptrCmdLine = pointer(DSProc, offDATADS + 1) ;
  *offArgv = offDATADS + 1 + tamComando ;      /* offset respecto DS pindx */
  ptrArgv = (char * far *)pointer(DSProc, offDATADS + 1 + tamComando) ;
  copiaLarga((pointer_t)&descProceso[pindx].comando, ptrCmdLine, tamComando) ;
  while ((car = ptrCmdLine[i]) != (char)0) {
    switch (estadoBlanco) {
    case TRUE  : if (car != ' ') {
                   if (argc >= 20) return(-1) ;           /* 20 == maxArgs */
                   estadoBlanco = FALSE ;
                   ptrArgv[argc] = (char *)off((pointer_t)&ptrCmdLine[i]) ;
                   argc++ ;
                 }
    case FALSE : if (car == ' ') {
                   estadoBlanco = TRUE ;
                   ptrCmdLine[i] = (char)0 ;
                 }
    }
    i++ ;
  }
  return(argc) ;
}

void registrarEnPOrdenados ( pindx_t pindx ) {
  dobleEnlace_t far * e ;
  word_t segmento ;
  int j, ant ;
  segmento = descProceso[pindx].CSProc ;
  e = c2cPFR[POrdenados].e ;
  j = c2cPFR[POrdenados].primero ;
  while ((j != c2cPFR[POrdenados].cabecera) && (descProceso[j].CSProc < segmento))
    j = e[j].sig ;
  ant = e[j].ant ;
  e[pindx].ant = ant ;
  e[pindx].sig = j ;
  e[ant].sig = pindx ;
  e[j].ant = pindx ;
  c2cPFR[POrdenados].numElem++ ;
}

pindx_t crearProceso (       word_t     segmento,
                             word_t     tam,
                             dword_t    tamFich,
                       const char far * programa,
                       const char far * comando,
                             pindx_t    pindx ) {

  cabecera_t far * cabecera ;
  pindx_t i, j ;
  word_t despl ;
  word_t offDATADS ; /* offset de la DATA del nuevo proceso respecto al DS */
  word_t SS_NuevoProceso ;                              /* DS_NuevoProceso */
  word_t SP_NuevoProceso ;
  word_t SR ;
  word_t tamCodigo ;
  word_t tamPila ;
  int argc ;
  word_t offArgv ;
  word_t far * ptrPila ;
  char carCmd ;
  int dfs ;
  modoAp_t modoAp ;

//return(-101) ;                                             /* depuracion */
           /* createProcess (pindx = -1) o exec (pindx = indProcesoActual) */

  if ((pindx < 0) && (c2cPFR[DPOcupados].numElem == maxProcesos)) return(-1) ;
  cabecera = (cabecera_t far *)pointer(segmento, 0x0000) ;
  if (!igualesHasta((char far *)cabecera->magicbyte,
                    (char far *)ptrMagicByteUsr, 3)) {          /* AJUSTES */
    if (!igualesHasta((char far *)cabecera->magicbyte,
                      (char far *)ptrMagicByteSO1, 3))
      return(-1) ;                                  /* cabecera incorrecta */
    cabecera = (cabecera_t far *)pointer(segmento, desplCab()) ;
  }
  if ((cabecera->magicbyte[ 5] != ptrMagicByteUsr[ 5]) || /* ver AJUSTES.H */
      (cabecera->magicbyte[ 8] != ptrMagicByteUsr[ 8]) ||
      (cabecera->magicbyte[11] != ptrMagicByteUsr[11]))
    return(-1) ;                                    /* cabecera incorrecta */
	
//return(-102) ;                                             /* depuracion */  

  despl = (16*cabecera->segDatosSR) + cabecera->startData ;
  if (despl & 0x0001) despl++ ;                     /* alineamos a palabra */
  SS_NuevoProceso = segmento + (despl >> 4) ;                  /* SS=DS=ES */
  SP_NuevoProceso = cabecera->SP0 ;
  offDATADS = despl & 0x000F ;                      /* 0 <= offDATADS < 16 */

  tamCodigo = (despl >> 4) ;                              /* en paragrafos */
  if (cabecera->SP0 <= 0xFFF0)
    tamPila = (cabecera->SP0+15)/16 ;
  else
    tamPila = 0x0FFF ;

  if ((tamCodigo + tamPila) != tam)
    return(-1) ;

  if (pindx >= 0) i = pindx ;                      /* exec ==> mismo pindx */
  else {
    i = desencolarPC2c((ptrC2c_t)&c2cPFR[DPLibres]) ;
    encolarPC2c(i, (ptrC2c_t)&c2cPFR[DPOcupados]) ;
  }

  descProceso[i].CSProc = segmento ;
  descProceso[i].tamCodigo = (16*cabecera->segDatosSR) + cabecera->startData ;
  descProceso[i].tamFichero = tamFich ;
//descProceso[i].desplBSS = tamFich - 16*(SS_NuevoProceso-segmento) ;
  descProceso[i].desplBSS = cabecera->finData ;               /* mas facil */

  descProceso[i].tam = tam ;

  copiarStr(programa, descProceso[i].programa) ;

  if (pindx == -1) {                                      /* createProcess */
    descProceso[i].pid = nuevoPid() ;
    descProceso[i].noStatus = FALSE ;          /* puede morir directamente */
    descProceso[i].ppindx = indProcesoActual ;
    descProceso[i].hpindx = -1 ;
    inicPC2c(&descProceso[i].c2cHijos, &e2PFR.e2Hijos, maxProcesos + i, TRUE) ;
    encolarPC2c(i, (ptrC2c_t)&descProceso[indProcesoActual].c2cHijos) ;
  /* descProceso[i].tCPU = 0 ; */         /* tiempo de CPU en (tics/2**16) */
  }
  else
    eliminarPC2c(i, (ptrC2c_t)&c2cPFR[POrdenados]) ;         /* 12/07/2015 */

//return(-103) ;                                             /* depuracion */

  descProceso[i].estado = preparado ;
  
//return(-(1035+i)) ;                                        /* depuracion */
  
  registrarEnPOrdenados(i) ;

  /* transmisión de la linea de comando al proceso suprimiendo cosas */

//return(-104) ;                                             /* depuracion */  

  for ( j = 0 ; (j < tamComando) &&
                ((carCmd = comando[j]) != (char)0) &&
                 (carCmd != '&') &&                           /* background */
                 (carCmd != '>') && (carCmd != '<') &&     /* redirecciones */
                 (carCmd != '|')                                   /* pipes */
        ; j++ )
    descProceso[i].comando[j] = carCmd ;
  descProceso[i].comando[j] = (char)0 ;
  descProceso[i].comando[j+1] = carCmd ; /* truco para saber si &, >, < o | */

  argc = ponerArgs(i, SS_NuevoProceso, offDATADS, &offArgv) ;

  ptrPila = (word_t far *)pointer(SS_NuevoProceso, SP_NuevoProceso) ;

//return(-105) ;                                             /* depuracion */  

  *(--ptrPila) = offArgv ;                         /* apilo parametro argv */
  *(--ptrPila) = argc ;                            /* apilo parametro argc */
  *(--ptrPila) = cabecera->desplFinish ; /* apilo dir. de retorno a finish */

  SP_NuevoProceso = off((pointer_t)ptrPila) - sizeof(trama_t) ;

          /* inicializacion de los registros en la pila del proceso creado */

  descProceso[i].trama =

    (trama_t far *) MK_FP(SS_NuevoProceso, SP_NuevoProceso) ;

  descProceso[i].trama->DS = SS_NuevoProceso ;
  descProceso[i].trama->ES = SS_NuevoProceso ;
  descProceso[i].trama->DI = 0x0000 ;
  descProceso[i].trama->SI = 0x0000 ;
  descProceso[i].trama->BP = ((cabecera->SP0) - 6) ;       /* IP, CS, Flags */
  descProceso[i].trama->SP = ((cabecera->SP0) - 6) ;       /* IP, CS, Flags */
  descProceso[i].trama->BX = 0x0000 ;
  descProceso[i].trama->DX = 0x0000 ;
  descProceso[i].trama->CX = 0x0000 ;
  descProceso[i].trama->AX = 0x0000 ;
  descProceso[i].trama->IP = cabecera->desplMain ;
  descProceso[i].trama->CS = segmento ;
  
//return(-106) ;                                              /* depuracion */ 
  
  asm {
    pushf ;
    pop ax ;
    mov SR,ax ;                                               /* SR = Flags */
  }
  descProceso[i].trama->Flags =
    (SR & 0xF000) | 0x0202 ;                   /* interrupciones permitidas */

/*  descProceso[i].trama->Flags = 0x7302 ; */                      /* traza */

  descProceso[i].desplPila = cabecera->finBSS ;

  if (pindx == -1) {                                       /* createProcess */
    descProceso[i].uid = descProceso[indProcesoActual].uid ;
    descProceso[i].gid = descProceso[indProcesoActual].gid ;
    descProceso[i].nfa = descProceso[indProcesoActual].nfa ;  /* hereda tfa */
    for ( j = 0 ; j < dfMax ; j++ ) {
      descProceso[i].tfa[j].dfs = descProceso[indProcesoActual].tfa[j].dfs ;
      descProceso[i].tfa[j].pos = descProceso[indProcesoActual].tfa[j].pos ;
      descProceso[i].tfa[j].modoAp = descProceso[indProcesoActual].tfa[j].modoAp ;
    }

    for ( j = 0 ; j < dfMax ; j++ ) {
      dfs = descProceso[i].tfa[j].dfs ;
      if (dfs >= 0) {
        modoAp = descProceso[i].tfa[j].modoAp ;
        if ((modoAp & 0x0007) == O_RDONLY) descFichero[dfs].contAp_L++ ;
        else if ((modoAp & 0x0007) == O_WRONLY) descFichero[dfs].contAp_E++ ;
      }
    }
  }
  else {                                                           /* exec */
    /* es innecesario todo lo anterior ya que i = pindx = indProcesoActual */
  }
  return(i) ;

}

char strSo1 [4][12] = { "SO1.BIN", "SO1.COM", "SO1.EXE", "SO1.SLX" } ;
char comandoSo1a [tamComando] = "boot de disquete" ;
char comandoSo1b [tamComando] = "SO1 (desde MSDOS)" ;
//char comandoSo1c [tamComando] = "SO1 (desde SYSLINUX)" ;

void inicProcesos ( void ) {

  pindx_t i ;
  cabecera_t far * cabecera = (cabecera_t far *)pointer(CS_SO1, desplCab()) ;

/*  rec_hijo = crearRec("HIJO", rOtro, sinInt) ;  */

  ccbAlEpilogo = (ccb_t)pointer(DS_SO1, (word_t)&descCcbAlEpilogo) ;

  /* inicializamos colas: */

  inicPC2c(&c2cPFR[DPLibres],    &e2PFR.e2DescProceso.Libres,   maxProcesos,     TRUE) ;
  inicPC2c(&c2cPFR[DPOcupados],  &e2PFR.e2DescProceso.Ocupados, maxProcesos + 1, TRUE) ;
  inicPC2c(&c2cPFR[PPreparados], &e2PFR.e2Preparados,           maxProcesos,     FALSE) ;
  inicPC2c(&c2cPFR[PUrgentes],   &e2PFR.e2Urgentes,             maxProcesos,     FALSE) ;
  inicPC2c(&c2cPFR[POrdenados],  &e2PFR.e2POrdenados,           maxProcesos,     FALSE) ;
  inicPC2c(&c2cPFR[PDormidos],   &e2PFR.e2PDormidos,            maxProcesos,     FALSE) ;

  for ( i = maxProcesos-1 ; i > 0 ; i-- ) {
    descProceso[i].estado = libre ;
    descProceso[i].pid = -1 ;
    descProceso[i].uid = -1 ;
    descProceso[i].gid = -1 ;
    descProceso[i].noStatus = TRUE ;           /* puede morir directamente */
    inicPC2c(&descProceso[i].c2cHijos, &e2PFR.e2Hijos, maxProcesos + i, TRUE) ;
    apilarPC2c(i, (ptrC2c_t)&c2cPFR[DPLibres]) ;             /* apilamos i */
    descProceso[i].trama = (trama_t far *)NULL ;
    descProceso[i].CSProc = 0x0000 ;
    descProceso[i].tamCodigo = 0x0000 ;
    descProceso[i].desplBSS = 0x0000 ;
    descProceso[i].desplPila = 0x0000 ;
    descProceso[i].tam = 0 ;
    descProceso[i].tamFichero = 0 ;
    descProceso[i].programa[0] = (char)0 ;
    /* descProceso[i].tCPU = 0 ; */       /* tiempo de CPU en (tics/2**16) */
  }

  descProceso[0].estado = ejecutandose ;
  descProceso[0].pid = nuevoPid() ;          /* el proceso 0 es la consola */
  descProceso[0].noStatus = TRUE ;             /* puede morir directamente */
  descProceso[0].ppindx = -1 ;                           /* no tiene padre */
  descProceso[0].hpindx = -1 ;
  inicPC2c(&descProceso[0].c2cHijos, &e2PFR.e2Hijos, maxProcesos + 0, TRUE) ;
  apilarPC2c(0, (ptrC2c_t)&c2cPFR[DPOcupados]) ;             /* apilamos 0 */
  apilarPC2c(0, (ptrC2c_t)&c2cPFR[POrdenados]) ;             /* apilamos 0 */
  descProceso[0].trama = (trama_t far *)pointer(DS_SO1, cabecera->SP0-sizeof(trama_t)) ;
  descProceso[0].CSProc = CS_SO1 ;
  descProceso[0].tamCodigo = (16*cabecera->segDatosSR) + cabecera->startData ;
  descProceso[0].desplBSS = off((pointer_t)startBSS) ;
//  printStrBIOS(" descProceso[0].desplBSS = ") ;
//  printHexBIOS(descProceso[0].desplBSS, 4) ;
  descProceso[0].desplPila = off((pointer_t)finBSS) ;
//  printStrBIOS(" descProceso[0].desplPila = ") ;
//  printHexBIOS(descProceso[0].desplPila, 4) ;
  descProceso[0].tamFichero =

    16 * (dword_t)(DS_SO1 - CS_SO1) + off((pointer_t)startBSS) ;
//  printStrBIOS(" descProceso[0].tamFichero = ") ;
//  printLDecBIOS(descProceso[0].tamFichero, 8) ;

  /* descProceso[0].tCPU = 0 ; */              /* expresado en tics/(2**16) */

  /* descProceso[0].tam                        se inicializa en inicMemoria */
  /* descProceso[0].win                        se inicializa en inicMemoria */
  /* descProceso[0].teclado                    se inicializa en inicMemoria */

  descProceso[0].nfa = 0 ;
  for ( i = 0 ; i < dfMax ; i++ ) {                /* inicializacion de tfa */
    descProceso[0].tfa[i].dfs = -1 ;
    descProceso[0].tfa[i].pos = 0 ;
    descProceso[0].tfa[i].modoAp = O_RDONLY ;
  }

#if (FALSE)
  descProceso[0].nfa = 3 ;  /* entrada (0), salida (1) y error (2) estandar */
  for ( i = STDIN ; i <= STDERR ; i++ ) {          /* inicializacion de tfa */
    descProceso[0].tfa[i].dfs = ( i ? 1 : 0) ;  /* 0 = TECLADO, 1 = VENTANA */
    descProceso[0].tfa[i].pos = 0 ;
    descProceso[0].tfa[i].modoAp = ( i ? O_WRONLY : O_RDONLY ) ;
  }                      /* posteriormente se redirigen a dfs = 2 = CONSOLA */
  for ( i = STDERR + 1 ; i < dfMax ; i++ )
    descProceso[0].tfa[i].dfs = -1 ;
#endif

  for ( i = 0 ; i < dfsMax ; i++ )        /* inicializacion de descFichero */
    descFichero[i].tipo = flibre ;

  switch (modoSO1()) {
  case modoSO1_Bin:                                      /* so1.bin (boot) */
    copiarStr(comandoSo1a, descProceso[0].comando) ;
    break ;
  case modoSO1_Com:
  case modoSO1_Exe:
    copiarStr(comandoSo1b, descProceso[0].comando) ;
    break ;
//case modoSO1_Bs :                             /* so1.bin (boot) SYSLINUX */
//  copiarStr(comandoSo1c, descProceso[0].comando) ;
//  break ;
  default : ;
  }
  copiarStr(strSo1[modoSO1()-1], descProceso[0].programa) ;

  indProcesoActual = 0 ;
  indProcesoDeSuperficie = 0 ;

  nivelActivacionSO1 = 0 ;

}

bool_t devolverParticion ( pindx_t pindx ) {
  if (!estaPC2c(pindx, (ptrC2c_t)&c2cPFR[POrdenados])) return(FALSE) ;
  eliminarPC2c(pindx, (ptrC2c_t)&c2cPFR[POrdenados]) ;
  return(k_devolverBloque(
           descProceso[pindx].CSProc,
           descProceso[pindx].tam)
  ) ;
}

int terminarProcIndx ( pindx_t pindx ) {          /* termina proceso pindx */
  modoAp_t modoAp ;
  int j, dfs ;

  if (pindx == 0) return(-1) ;                          /* proceso inicial */
  if ((pindx < 0) || (pindx >= maxProcesos)) return(-2) ;         /* error */
  if (descProceso[pindx].estado == libre) return(-3) ;
  if (descProceso[pindx].estado == preparado)
    eliminarPC2c(pindx, (ptrC2c_t)&c2cPFR[PPreparados]) ;

  for ( j = 0 ; j < dfMax ; j++ ) {            /* cerrar ficheros abiertos */
    dfs = descProceso[pindx].tfa[j].dfs ;
    if (dfs > 0) {
      modoAp = descProceso[pindx].tfa[j].modoAp ;
      if ((modoAp & 0x0007) == O_RDONLY) descFichero[dfs].contAp_L-- ;
      else if ((modoAp & 0x0007) == O_WRONLY) descFichero[dfs].contAp_E-- ;
    }
  }

  devolverParticion(pindx) ;

         /* quedaria pendiente eliminar a todos los procesos hijos zombies */
         /* poniendolos como hijos de un proceso init que ejecuta de forma */
         /* continuada waitpid hasta que waitpid lo bloquea.               */

  return(0) ;
}

int eliminarProcIndx ( pindx_t pindx ) {          /* elimina proceso pindx */
  descProceso[pindx].estado = libre ;
  descProceso[pindx].pid = -1 ;
  eliminarPC2c(pindx, (ptrC2c_t)&c2cPFR[DPOcupados]) ;
  apilarPC2c(pindx, (ptrC2c_t)&c2cPFR[DPLibres]) ;
  return(0) ;
}

int matarProcIndx ( pindx_t pindx ) {                /* mata proceso pindx */
  int err = 0 ;
  rindx_t rindx ;
  if (descProceso[pindx].estado == libre) return(-1) ;         /* ya libre */
  if (descProceso[pindx].estado == bloqueado) {
    rindx = descProceso[pindx].esperandoPor ;
    switch (rindx) {
/*  case rec_hijo           : return(-2) ; */
    case rec_zombie         : eliminarProcIndx(pindx) ;
                              return(0) ;
/*  case rec_desinstalacion : return(-2) ; */
    default : ;
      if ((0 <= rindx) && (rindx < maxRecursos)) {
        descRecurso[rindx].eliminar(pindx) ;
      }
    }
  }
  err = terminarProcIndx(pindx) ;
  if (err == 0)
    eliminarProcIndx(pindx) ;

  return(err) ;
}





