/* ----------------------------------------------------------------------- */
/*                               procesos.c                                */
/* ----------------------------------------------------------------------- */
/*                          Gestion de procesos                            */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>  /* word_t, dword_t, bool_t, NULL, FALSE, TRUE */
//                                  /* pointer_t, pointer, MK_FP, seg, off */
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
#include <so1pub.h\strings.h>                           /* strcpy, strncmp */
#include <so1pub.h\memory.h>                          /* memcpy, memcpy_fd */
#include <so1pub.h\startbss.h>                                 /* startBSS */
#include <so1pub.h\finbss.h>                                     /* finBSS */
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

descProceso_t * ptrDPActual ;            /* &descProceso[indProcesoActual] */
                                                /* para acelerar el acceso */
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

pindx_t far sigProceso ( void )                /* planificador (scheduler) */
{
    static pindx_t pindx ;                 /* printStrWin(win_so, "\nS") ; */

    contOcioso = -1 ;
    while (c2cPFR[PPreparados].numElem == 0) {
        if (indProcesoActual != -1)
        plot('e', 0, contadorTimer0()) ;
        indProcesoActual = -1 ;              /* printStrWin(win_so, "h") ; */
        enHalt = TRUE ;
        asm { 
            sti ;
            hlt ;                               /* detenemos el procesador */
            cli ;
	    }
    }

    pindx = desencolarPC2c((ptrC2c_t)&c2cPFR[PPreparados]) ;
    ptrDPActual = (descProceso_t *)&descProceso[indProcesoActual] ;
	
#if (FALSE)
    if (debugWord == 0x0001) 
	{
        printStrBIOS("\n sigProceso(): ") ;
        printStrBIOS("\n indProcesoActual = ") ;
        printIntBIOS(indProcesoActual, 1) ;
        printStrBIOS(" pindx = ") ;
        printIntBIOS(pindx, 1) ;
        while(TRUE) ;
    }
#endif

    return(pindx) ;
}

int far activarProceso ( pindx_t pindx ) 
{	
	descProceso_t * ptrDPPindx = (descProceso_t *)&descProceso[pindx] ;
	
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

    if ((pindx < 0) || (pindx >= maxProcesos)) return(-1) ;

    ptrDPPindx = (descProceso_t *)&descProceso[pindx] ;  
  
    if (ptrDPPindx->pid < 0) {
/*
        printStrWin(win_so, "\n activarProceso: no existe el proceso ") ;
        printDecWin(win_so, descProceso[pindx].pid, 1) ;
        printStrWin(win_so, " con indice ") ;
        printDecWin(win_so, pindx, 1) ;
        printStrWin(win_so, "\n") ;
*/
        return(-1) ;
    }

    if (ptrDPPindx->estado != preparado) {
/*
        printStrWin(win_so, "\n el proceso ") ;
        printDecWin(win_so, descProceso[pindx].pid, 1) ;
        printStrWin(win_so, " con indice ") ;
        printDecWin(win_so, pindx, 1) ;
        printStrWin(win_so, " no esta preparado\n") ;
*/
        return(-1) ;
    }

    ptrDPPindx->estado = ejecutandose ;
	
    indProcesoActual = pindx ;
    
	ptrDPActual = (descProceso_t *)&descProceso[indProcesoActual] ;
	
    plot('e', 0, contadorTimer0()) ;

    contRodajas++ ;                                                    /*T0*/

    if (hayTic)               /* se llamo a activarProceso desde rti_timer */
        contTicsRodaja = 0 ;
    else
        contTicsRodaja = -1 ;      /* un tic puede estar a punto de llegar */
    hayTic = FALSE ;

    SS_Proceso = FP_SEG(ptrDPActual->trama) ;
    SP_Proceso = FP_OFF(ptrDPActual->trama) ;

    /* SS_Proceso y SP_Proceso son variables globales. Si fueran variables */
    /* locales habria que tener cuidado en el orden de las 3 instrucciones */
    /* ya que se modifica la pila y el orden de las instrucciones seria    */
    /* critico                                                             */

    enHalt = FALSE ;

    nivelActivacionSO1 = 0 ;               /* esta ejecutandose un proceso */

    asm {
	    mov ss,SS_Proceso ;            /* establecemos la pila del proceso */
        mov sp,SP_Proceso ;

        pop ds ;          /* establecemos el segmento de datos del proceso */
        pop es ;

        popa ;                    /* restauramos los registros del proceso */

        iret ;                               /* restauramos IP, CS y Flags */
    }
    return(_AX) ;     /* nunca se llega aqui (puesto para evitar warnings) */ 
}

#define salvarProceso( )                         /* indProcesoActual != -1 */\
                                                                             \
    ptrDPActual->trama = (trama_t far *)MK_FP(SS_Proceso, SP_Proceso) ;      \

#define salvarTareaInterrumpida( )               /* indProcesoActual != -1 */\
                                                                             \
    ptrDPActual->trama = (trama_t far *)MK_FP(SS_Tarea, SP_Tarea) ;          \

static int ponerArgs ( pindx_t pindx, word_t DSProc, word_t offDATADS, word_t far * offArgv ) 
{
    int i = 0 ;
    int argc = 0 ;
    char car ;
    bool_t estadoBlanco = TRUE ;
    pointer_t ptrCmdLine ;
    char * far * ptrArgv ;
   
   ptrCmdLine = MK_FP(DSProc, offDATADS + 1) ;
    *offArgv = offDATADS + 1 + tamComando ;    /* offset respecto DS pindx */
    ptrArgv = (char * far *)MK_FP(DSProc, offDATADS + 1 + tamComando) ;
    memcpy(ptrCmdLine, &descProceso[pindx].comando, tamComando) ;
    while ((car = ptrCmdLine[i]) != '\0') 
    {
        if (estadoBlanco) 
     	{
            if (car != ' ') 
			{
                if (argc >= 20) return(-1) ;              /* 20 == maxArgs */
                    estadoBlanco = FALSE ;
                    ptrArgv[argc] = (char *)FP_OFF((pointer_t)&ptrCmdLine[i]) ;
                    argc++ ;
            }
	    }
	    else 
     	{
            if (car == ' ') 
	     	{
                estadoBlanco = TRUE ;
                ptrCmdLine[i] = '\0' ;
            }
        }
        i++ ;
    }
    return(argc) ;
}

void registrarEnPOrdenados ( pindx_t pindx ) 
{
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
                             pindx_t    pindx ) 
{
    cabecera_t far * cabecera ;
    pindx_t i, j ;
    word_t despl ;
    word_t offDATADS ;  /* offset de la DATA del nuevo proceso respecto DS */
    word_t SS_NuevoProceso ;                            /* DS_NuevoProceso */
    word_t SP_NuevoProceso ;
    trama_t far * tramaNuevoProceso ;        
    word_t SR ;
    word_t tamCodigo ;
    word_t tamPila ;
    int argc ;
    word_t offArgv ;
    word_t far * ptrPila ;
    char carCmd ;
    int dfs ;
    modoAp_t modoAp ;
	descProceso_t * ptrDPi ;

//  return(-101) ;                                           /* depuracion */
//         /* createProcess (pindx = -1) o exec (pindx = indProcesoActual) */

    if ((pindx < 0) && (c2cPFR[DPOcupados].numElem == maxProcesos)) return(-1) ;
    cabecera = (cabecera_t far *)MK_FP(segmento, 0x0000) ;
    if (strncmp((char far *)cabecera->magicbyte,
                (char far *)ptrMagicByteUsr, 3))                /* AJUSTES */
	{
        if (strncmp((char far *)cabecera->magicbyte,
                    (char far *)ptrMagicByteSO1, 3))
            return(-1) ;                            /* cabecera incorrecta */
        cabecera = (cabecera_t far *)MK_FP(segmento, desplCab()) ;
    }
    if ((cabecera->magicbyte[ 5] != ptrMagicByteUsr[ 5]) ||   /* AJUSTES.H */
        (cabecera->magicbyte[ 8] != ptrMagicByteUsr[ 8]) ||
        (cabecera->magicbyte[11] != ptrMagicByteUsr[11]))
        return(-1) ;                                /* cabecera incorrecta */
	
//  return(-102) ;                                           /* depuracion */  

    despl = (16*cabecera->segDatosSR) + cabecera->startData ;
    if (despl & 0x0001) despl++ ;                   /* alineamos a palabra */
    SS_NuevoProceso = segmento + (despl >> 4) ;                /* SS=DS=ES */
    SP_NuevoProceso = cabecera->SP0 ;
    offDATADS = despl & 0x000F ;                    /* 0 <= offDATADS < 16 */

    tamCodigo = (despl >> 4) ;                            /* en paragrafos */
    if (cabecera->SP0 <= 0xFFF0)
        tamPila = (cabecera->SP0+15)/16 ;
    else
        tamPila = 0x0FFF ;

    if ((tamCodigo + tamPila) != tam)
        return(-1) ;

    if (pindx >= 0) i = pindx ;                    /* exec ==> mismo pindx */
    else {
        i = desencolarPC2c((ptrC2c_t)&c2cPFR[DPLibres]) ;
        encolarPC2c(i, (ptrC2c_t)&c2cPFR[DPOcupados]) ;
    }

    ptrDPi = (descProceso_t *)&descProceso[i] ;

    ptrDPi->CSProc = segmento ;
    ptrDPi->tamCodigo = (16*cabecera->segDatosSR) + cabecera->startData ;
    ptrDPi->tamFichero = tamFich ;
//  ptrDPi->desplBSS = tamFich - 16*(SS_NuevoProceso-segmento) ;
    ptrDPi->desplBSS = cabecera->finData ;                    /* mas facil */

    ptrDPi->tam = tam ;

    strcpy(ptrDPi->programa, programa) ;

    if (pindx == -1) {                                    /* createProcess */
        ptrDPi->pid = nuevoPid() ;
        ptrDPi->noStatus = FALSE ;             /* puede morir directamente */
        ptrDPi->ppindx = indProcesoActual ;
        ptrDPi->hpindx = -1 ;
        inicPC2c(&ptrDPi->c2cHijos, &e2PFR.e2Hijos, maxProcesos + i, TRUE) ;
        encolarPC2c(i, (ptrC2c_t)&ptrDPActual->c2cHijos) ;
        inicPC2c(&ptrDPi->c2cEmisores,   &e2PFR.e2Procesos, maxProcesos + i,     TRUE) ;
        inicPC2c(&ptrDPi->c2cReceptores, &e2PFR.e2Procesos, 2 * maxProcesos + i, TRUE) ;
//      /* ptrDPi->tCPU = 0 ; */          /* tiempo de CPU en (tics/2**16) */
    }
    else
        eliminarPC2c(i, (ptrC2c_t)&c2cPFR[POrdenados]) ;     /* 12/07/2015 */

//  return(-103) ;                                           /* depuracion */

    ptrDPi->estado = preparado ;
  
//  return(-(1035+i)) ;                                      /* depuracion */
  
    registrarEnPOrdenados(i) ;

    /* transmisión de la linea de comando al proceso suprimiendo cosas */

//  return(-104) ;                                           /* depuracion */  

    for ( j = 0 ; (j < tamComando) &&
                  ((carCmd = comando[j]) != '\0') &&
                   (carCmd != '&') &&                        /* background */
                   (carCmd != '>') && (carCmd != '<') &&  /* redirecciones */
                   (carCmd != '|')                                /* pipes */
          ; j++ )
        ptrDPi->comando[j] = carCmd ;
	
    ptrDPi->comando[j] = '\0' ;
    ptrDPi->comando[j + 1] = carCmd ;   /* truco para saber si &, >, < o | */

    argc = ponerArgs(i, SS_NuevoProceso, offDATADS, &offArgv) ;

    ptrPila = (word_t far *)MK_FP(SS_NuevoProceso, SP_NuevoProceso) ;

//  return(-105) ;                                           /* depuracion */  

    *(--ptrPila) = 0x0000 ;   /* apilo 0x0000: ver finish y (macro) atexit */
    *(--ptrPila) = offArgv ;                       /* apilo parametro argv */
    *(--ptrPila) = argc ;                          /* apilo parametro argc */
    *(--ptrPila) = cabecera->desplFinish ;  /* apilo dir. de ret. a finish */

    SP_NuevoProceso = FP_OFF(ptrPila) - sizeof(trama_t) ;

//        /* inicializacion de los registros en la pila del proceso creado */

    tramaNuevoProceso = (trama_t far *)MK_FP(SS_NuevoProceso, SP_NuevoProceso) ;

    ptrDPi->trama = tramaNuevoProceso ;

    tramaNuevoProceso->DS = SS_NuevoProceso ;
    tramaNuevoProceso->ES = SS_NuevoProceso ;
    tramaNuevoProceso->DI = 0x0000 ;
    tramaNuevoProceso->SI = 0x0000 ;
    tramaNuevoProceso->BP = ((cabecera->SP0) - 6) ;       /* IP, CS, Flags */
    tramaNuevoProceso->SP = ((cabecera->SP0) - 6) ;       /* IP, CS, Flags */
    tramaNuevoProceso->BX = 0x0000 ;
    tramaNuevoProceso->DX = 0x0000 ;
    tramaNuevoProceso->CX = 0x0000 ;
    tramaNuevoProceso->AX = 0x0000 ;
    tramaNuevoProceso->IP = cabecera->desplMain ;
    tramaNuevoProceso->CS = segmento ;
  
//  return(-106) ;                                           /* depuracion */ 
  
    asm {
        pushf ;
        pop ax ;
        mov SR,ax ;                                          /* SR = Flags */
    }
    tramaNuevoProceso->Flags =
        (SR & 0xF000) | 0x0202 ;              /* interrupciones permitidas */

/*  tramaNuevoProceso->Flags = 0x7302 ; */                        /* traza */

    ptrDPi->desplPila = cabecera->finBSS ;

    if (pindx == -1)                                      /* createProcess */
	{
        ptrDPi->uid = ptrDPActual->uid ;
        ptrDPi->gid = ptrDPActual->gid ;
        ptrDPi->nfa = ptrDPActual->nfa ;                     /* hereda tfa */
        for ( j = 0 ; j < dfMax ; j++ ) {
            ptrDPi->tfa[j].dfs    = ptrDPActual->tfa[j].dfs ;
            ptrDPi->tfa[j].pos    = ptrDPActual->tfa[j].pos ;
            ptrDPi->tfa[j].modoAp = ptrDPActual->tfa[j].modoAp ;
        }

        for ( j = 0 ; j < dfMax ; j++ ) 
		{
            dfs = descProceso[i].tfa[j].dfs ;
            if (dfs >= 0) 
			{
                modoAp = ptrDPActual->tfa[j].modoAp ;
                if ((modoAp & 0x0007) == O_RDONLY) descFichero[dfs].contAp_L++ ;
                else if ((modoAp & 0x0007) == O_WRONLY) descFichero[dfs].contAp_E++ ;
            }
        }
    }
    else 
	{                                                              /* exec */
//  /* es innecesario todo lo anterior ya que i = pindx = indProcesoActual */
    }
    return(i) ;
}

char strSo1 [4][12] = { "SO1.BIN", "SO1.COM", "SO1.EXE", "SO1.SLX" } ;
char comandoSo1a [tamComando] = "boot de disquete" ;
char comandoSo1b [tamComando] = "SO1 (desde MSDOS)" ;
//char comandoSo1c [tamComando] = "SO1 (desde SYSLINUX)" ;

void inicProcesos ( void ) 
{
    pindx_t i ;
    cabecera_t far * cabecera = (cabecera_t far *)MK_FP(CS_SO1, desplCab()) ;
	descProceso_t * ptrDPi ;
	descProceso_t * ptrDP0 ;

/*  rec_hijo = crearRec("HIJO", rOtro, sinInt) ;  */

    ccbAlEpilogo = (ccb_t)MK_FP(DS_SO1, (word_t)&descCcbAlEpilogo) ;

/*  inicializamos colas: */

    inicPC2c(&c2cPFR[DPLibres    ], &e2PFR.e2DescProceso, maxProcesos,         TRUE) ;
    inicPC2c(&c2cPFR[DPOcupados  ], &e2PFR.e2DescProceso, maxProcesos + 1,     TRUE) ;
    inicPC2c(&c2cPFR[PPreparados ], &e2PFR.e2Procesos,    3 * maxProcesos,     TRUE) ;
    inicPC2c(&c2cPFR[PUrgentes   ], &e2PFR.e2Procesos,    3 * maxProcesos + 1, TRUE) ;
    inicPC2c(&c2cPFR[PDormidos   ], &e2PFR.e2Procesos,    3 * maxProcesos + 2, TRUE) ;
    inicPC2c(&c2cPFR[PReceptAny  ], &e2PFR.e2Procesos,    3 * maxProcesos + 3, TRUE) ;
    inicPC2c(&c2cPFR[POrdenados  ], &e2PFR.e2POrdenados,  maxProcesos,         FALSE) ;

    for ( i = maxProcesos-1 ; i > 0 ; i-- ) 
	{
        ptrDPi = (descProceso_t *)&descProceso[i] ;

        ptrDPi->estado = libre ;
        ptrDPi->pid = -1 ;
        ptrDPi->uid = -1 ;
        ptrDPi->gid = -1 ;
        ptrDPi->noStatus = TRUE ;              /* puede morir directamente */
        inicPC2c(&ptrDPi->c2cHijos,      &e2PFR.e2Hijos,    maxProcesos + i,     TRUE) ;
        inicPC2c(&ptrDPi->c2cEmisores,   &e2PFR.e2Procesos, maxProcesos + i,     TRUE) ;
        inicPC2c(&ptrDPi->c2cReceptores, &e2PFR.e2Procesos, 2 * maxProcesos + i, TRUE) ;
		ptrDPi->notificaciones = 0x00000000L ; 
        apilarPC2c(i, (ptrC2c_t)&c2cPFR[DPLibres]) ;         /* apilamos i */
        ptrDPi->trama = (trama_t far *)NULL ;
        ptrDPi->CSProc = 0x0000 ;
        ptrDPi->tamCodigo = 0x0000 ;
        ptrDPi->desplBSS = 0x0000 ;
        ptrDPi->desplPila = 0x0000 ;
        ptrDPi->tam = 0 ;
        ptrDPi->tamFichero = 0 ;
        ptrDPi->programa[0] = '\0' ;
//      /* ptrDPi->tCPU = 0 ; */          /* tiempo de CPU en (tics/2**16) */
    }

    ptrDP0 = (descProceso_t *)&descProceso[0] ;
	
    ptrDP0->estado = ejecutandose ;
    ptrDP0->pid = nuevoPid() ;               /* el proceso 0 es la consola */
    ptrDP0->noStatus = TRUE ;                  /* puede morir directamente */
    ptrDP0->ppindx = -1 ;                                /* no tiene padre */
    ptrDP0->hpindx = -1 ;
    inicPC2c(&ptrDP0->c2cHijos, &e2PFR.e2Hijos, maxProcesos + 0, TRUE) ;
    inicPC2c(&ptrDP0->c2cEmisores,   &e2PFR.e2Procesos, maxProcesos + 0,     TRUE) ;
    inicPC2c(&ptrDP0->c2cReceptores, &e2PFR.e2Procesos, 2 * maxProcesos + 0, TRUE) ;
	ptrDP0->notificaciones = 0x00000000L ; 
    apilarPC2c(0, (ptrC2c_t)&c2cPFR[DPOcupados]) ;           /* apilamos 0 */
    apilarPC2c(0, (ptrC2c_t)&c2cPFR[POrdenados]) ;           /* apilamos 0 */
    ptrDP0->trama = (trama_t far *)MK_FP(DS_SO1, cabecera->SP0-sizeof(trama_t)) ;
    ptrDP0->CSProc = CS_SO1 ;
    ptrDP0->tamCodigo = (16*cabecera->segDatosSR) + cabecera->startData ;
    ptrDP0->desplBSS = FP_OFF(startBSS) ;
//  printStrBIOS(" descProceso[0].desplBSS = ") ;
//  printHexBIOS(descProceso[0].desplBSS, 4) ;
    ptrDP0->desplPila = FP_OFF(finBSS) ;
//  printStrBIOS(" descProceso[0].desplPila = ") ;
//  printHexBIOS(descProceso[0].desplPila, 4) ;
    ptrDP0->tamFichero =

    16 * (dword_t)(DS_SO1 - CS_SO1) + FP_OFF((pointer_t)startBSS) ;
//  printStrBIOS(" descProceso[0].tamFichero = ") ;
//  printLDecBIOS(descProceso[0].tamFichero, 8) ;

//  /* descProceso[0].tCPU = 0 ; */           /* expresado en tics/(2**16) */

//  /* descProceso[0].tam                     se inicializa en inicMemoria */
//  /* descProceso[0].win                     se inicializa en inicMemoria */
//  /* descProceso[0].teclado                 se inicializa en inicMemoria */

    ptrDP0->nfa = 0 ;
    for ( i = 0 ; i < dfMax ; i++ ) {             /* inicializacion de tfa */
        ptrDP0->tfa[i].dfs = -1 ;
        ptrDP0->tfa[i].pos = 0 ;
        ptrDP0->tfa[i].modoAp = O_RDONLY ;
    }

    for ( i = 0 ; i < dfsMax ; i++ )      /* inicializacion de descFichero */
        descFichero[i].tipo = flibre ;

    switch (modoSO1()) 
	{
        case modoSO1_Bin:                                /* so1.bin (boot) */
                      strcpy(ptrDP0->comando, comandoSo1a) ;
                      break ;
        case modoSO1_Com:
        case modoSO1_Exe:
                      strcpy(ptrDP0->comando, comandoSo1b) ;
                      break ;
//      case modoSO1_Bs :                       /* so1.bin (boot) SYSLINUX */
//                    strcpy(ptrDP0->comando, comandoSo1c) ;
//                    break ;
        default : ;
    }
    strcpy(ptrDP0->programa, strSo1[modoSO1()-1]) ;

    indProcesoActual = 0 ;
    
	ptrDPActual = (descProceso_t *)&descProceso[indProcesoActual] ;
	
	indProcesoDeSuperficie = 0 ;

    nivelActivacionSO1 = 0 ;
}

/* hayThreads nos dice si hay threads del proceso pindx */ 

bool_t hayThreads ( pindx_t pindx ) 
{
  	word_t CSProc = descProceso[pindx].CSProc ;
    pindx_t otroPindx = c2cPFR[DPOcupados].primero ;
    int i ;
  	for ( i = 0 ; i < c2cPFR[DPOcupados].numElem ; i++ ) 
    	if ((otroPindx != pindx) &&
            (descProceso[otroPindx].CSProc == CSProc))
            return(TRUE) ;               /* no puede devolverse la memoria */
    return(FALSE) ;	
}

#define noEsThread( pindx ) \
    estaPC2c(pindx, (ptrC2c_t)&c2cPFR[POrdenados]) 

#define esThread( pindx ) \
    (!estaPC2c(pindx, (ptrC2c_t)&c2cPFR[POrdenados])) 

void devolverParticion ( pindx_t pindx ) 
{
	if (esThread(pindx)) return ;
    eliminarPC2c(pindx, (ptrC2c_t)&c2cPFR[POrdenados]) ;
    k_devolverBloque(
	    descProceso[pindx].CSProc,
        descProceso[pindx].tam
    ) ;		
}

int terminarProcIndx ( pindx_t pindx )            /* termina proceso pindx */
{
    modoAp_t modoAp ;
    int j, dfs ;
    bool_t hayThrds ; 
    bool_t noEsThrd ; 
  
    if (pindx == 0) return(-1) ;                        /* proceso inicial */
    if ((pindx < 0) || (pindx >= maxProcesos)) return(-2) ;       /* error */
    if (descProceso[pindx].estado == libre) return(-3) ;

    hayThrds = hayThreads(pindx) ;
    noEsThrd = noEsThread(pindx) ;  
    if (noEsThrd && hayThrds) return(-4) ;
  
    if (descProceso[pindx].estado == preparado)
        eliminarPC2c(pindx, (ptrC2c_t)&c2cPFR[PPreparados]) ;

    for ( j = 0 ; j < dfMax ; j++ )            /* cerrar ficheros abiertos */
	{
        dfs = descProceso[pindx].tfa[j].dfs ;
        if (dfs > 0) 
		{
            modoAp = descProceso[pindx].tfa[j].modoAp ;
            if ((modoAp & 0x0007) == O_RDONLY) descFichero[dfs].contAp_L-- ;
            else if ((modoAp & 0x0007) == O_WRONLY) descFichero[dfs].contAp_E-- ;
        }
    }

    if (noEsThrd) devolverParticion(pindx) ;

//       /* quedaria pendiente eliminar a todos los procesos hijos zombies */
//       /* poniendolos como hijos de un proceso init que ejecuta de forma */
//       /* continuada waitpid hasta que waitpid lo bloquea.               */

    return(0) ;
}

int eliminarProcIndx ( pindx_t pindx ) {          /* elimina proceso pindx */
    descProceso[pindx].estado = libre ;
    descProceso[pindx].pid = -1 ;
    eliminarPC2c(pindx, (ptrC2c_t)&c2cPFR[DPOcupados]) ;
    apilarPC2c(pindx, (ptrC2c_t)&c2cPFR[DPLibres]) ;
    return(0) ;
}

int matarProcIndx ( pindx_t pindx )                  /* mata proceso pindx */
{
    int err = 0 ;
    rindx_t rindx ;
    if (descProceso[pindx].estado == libre) return(-1) ;       /* ya libre */
    if (descProceso[pindx].estado == bloqueado) 
	{
        rindx = descProceso[pindx].esperandoPor ;
        switch (rindx) 
		{
/*          case rec_hijo           : return(-2) ; */
            case rec_zombie         : eliminarProcIndx(pindx) ;
                                      return(0) ;
/*          case rec_desinstalacion : return(-2) ; */
            default : ;
                if ((0 <= rindx) && (rindx < maxRecursos)) 
                    descRecurso[rindx].eliminar(pindx) ;
        }
    }
    err = terminarProcIndx(pindx) ;
    if (err == 0)
        eliminarProcIndx(pindx) ;
    return(err) ;
}