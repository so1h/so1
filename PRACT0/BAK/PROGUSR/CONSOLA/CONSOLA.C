/* ----------------------------------------------------------------------- */
/*                                consola.c                                */
/* ----------------------------------------------------------------------- */
/*    version del recurso consola (teclado+pantalla)  [con multiscreen]    */
/* ----------------------------------------------------------------------- */

/*                                version 3                                */
/*                           admite multiscreen                            */
/*                   se ejecuta como un proceso aparte                     */
/*     la constante maxConsolas establece el numero maximo de consolas     */
/*       admite gran numero de consolas ya que la memoria necesaria        */
/*         para las consolas se toma de un nuevo segmento de datos         */

#include <so1pub.h\ajustusr.h>          /* save_DS0, setraw_DS, restore_DS */
#include <so1pub.h\comundrv.h>  /* ptrIndProcesoActual, comprobarAmpersand */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                                      /* printf */
#include <so1pub.h\ctype.h>                                     /* toupper */
#include <so1pub.h\strings.h>                           /* strcpy, strcmpu */
#include <so1pub.h\bios_0.h>            /* printCarBIOS, leerTeclaListaBDA */
#include <so1pub.h\bios_crt.h>      /* inicBiosCrt, ptrPant, numFilas, ... */
#include <so1pub.h\crthw.h>                         /* inicCrtHw, goToXYHw */
#include <so1pub.h\memvideo.h>          /* printCarVideo, cursorF, cursorC */
#include <so1pub.h\def_tecl.h>                                /* teclado_t */
#include <so1pub.h\pic.h>                                   /* IRQ_TECLADO */
#include <so1pub.h\def_timer.h>                            /* argCbTimer_t */
#include <so1pub.h\printgen.h>            /* printGenStr, printGenDec, ... */

#include <so1pub.h\debug.h>                               /* puntoDeParada */

#include <progusr\consola\consola.h>          /* descConsola_t y scancodes */
#include <progusr\consola\tablasp.h>         /* tablaDeConversion, tablaSP */

#define REDUCIR_DRIVER TRUE     /* TRUE ==> reducimos la memoria utilizada */
//#define REDUCIR_DRIVER FALSE  

#pragma option -w-use /* (comundrv.h) warning: 'x' declared but never used */

info_t info ;                                                /* depuracion */

rindx_t rec_consola = 0 ;           /* inicializacion provisional --> DATA */ /* se necesita en readConsola */

/* ----------------------------------------------------------------------- */
/*                   seccion de implementacion del driver                  */
/* ----------------------------------------------------------------------- */

#define nVIntConsola 0x09     /* numero de vector utilizado por el teclado */

/* descConsola_t esta declarado en consola.h */

descConsola_t far * descConsola = NULL ;                           /* DATA */

#define numConsolasPorDefecto 6                       /* valor por defecto */

byte_t maxConsolas = 0 ;                                           /* DATA */

word_t consolaDeSuperficie = 0 ;                                   /* DATA */

word_t nbytesProceso [ maxProcesos ] = { 0 } ;         /* bytes pendientes */ /* DATA */

pointer_t dirProceso [ maxProcesos ] = { NULL } ;   /* direcciones destino */ /* DATA */

void meter ( char car, teclado_t far * teclado )
{
    int in = teclado->in ;
    if (teclado->ncar < tamBTeclado)
    {
        teclado->bufer[in++] = car ;
        if (in >= tamBTeclado) in = 0 ;
        teclado->in = in ;
        teclado->ncar++ ;
    }
}

char sacar ( teclado_t far * teclado  )     /* se supone: teclado.ncar > 0 */
{
    char car ;
    int out = teclado->out ;
    car = teclado->bufer[out++] ;
    if (out >= tamBTeclado) out = 0 ;
    teclado->out = out ;
    teclado->ncar-- ;
    return(car) ;
}

int printCarConsola ( byte_t con, char car )
{
    if (con == consolaDeSuperficie)
        printCarVideo(car) ;
    else
#if (0) 			
		printCarDisplay((display_t far *)&(descConsola[con]), car) ;
#else	
    {
	    byte_t numFilas = maxFilasAct ;
        pantalla_t far * pantalla = (pantalla_t far *)&descConsola[con].pantalla ;
        byte_t F = descConsola[con].F ;
        byte_t C = descConsola[con].C ;
        switch (car)
        {
        case '\f'  :
            borrarCPantalla(pantalla, numFilas) ;
//          borrarPantalla(pantalla, numFilas) ; 
            descConsola[con].F = 0 ;
            descConsola[con].C = 0 ;
            break ;
        case '\r'  :
            descConsola[con].C = 0 ;
            break ;
        case '\n'  :
            if (++descConsola[con].F == numFilas)
            {
                scrollCPantalla(pantalla, numFilas) ;
                descConsola[con].F = numFilas-1 ;
            }
            break ;
        case '\b'  :
            if (C > 0)
            {
                C-- ;
                pantalla->t[F][C].car = ' ' ;
                descConsola[con].C = C ;
            }
            else printf("\a") ;
            break ;
        case '\t'  :
            car = ' ' ;
        case '\a' :
            printf("\a") ;
            break ;
        default  :
#if (FALSE)
            if (C < 80)                      /* ptrBiosArea->VIDEO-width */
                pantalla->t[F][C].car = car ;
            descConsola[con].C++ ;
#else
            pantalla->t[F][C].car = car ;
            if (++descConsola[con].C == 80)  /* ptrBiosArea->VIDEO-width */
            {
                descConsola[con].C = 0 ;
                if (++descConsola[con].F == numFilas)
                {
                    scrollCPantalla(pantalla, numFilas) ;
                    descConsola[con].F = numFilas-1 ;
                }
            }
#endif
        }
    }
#endif		
    return(0) ;
}

int goToXYConsola ( byte_t con, byte_t F, byte_t C )
{
    if (F >= maxFilasAct) return(-1) ;
    if (C >= maxColumnasAct) return(-1) ;  
    if (con == consolaDeSuperficie)
    {
//      goToXYHw(F, C) ; 
        cursorF = F ;
        cursorC = C ;
    }
    else
    {
        descConsola[con].F = F ;
        descConsola[con].C = C ;
    }
    return(0) ;
}

void establecerConsolaDeSuperficie ( void )
{
    pantalla_t far * pantalla =
        (pantalla_t far *)&descConsola[consolaDeSuperficie].pantalla ;
    copiarCPantalla(pantalla, ptrPant, maxFilasAct) ;
//  copiarPantalla(pantalla, ptrPant, maxFilasAct) ; 
    cursorF = descConsola[consolaDeSuperficie].F ;
    cursorC = descConsola[consolaDeSuperficie].C ;
    goToXYHw(cursorF, cursorC) ;
}

void salvarConsolaDeSuperficie ( void )
{
    pantalla_t far * pantalla =
        (pantalla_t far *)&descConsola[consolaDeSuperficie].pantalla ;
    copiarCPantalla(ptrPant, pantalla, maxFilasAct) ;
//  copiarPantalla(ptrPant, pantalla, maxFilasAct) ;
    descConsola[consolaDeSuperficie].F = cursorF ;
    descConsola[consolaDeSuperficie].C = cursorC ;
}

/* ----------------------------------------------------------------------- */

#pragma warn -par

int far openConsola ( int dfs, modoAp_t modo )
{
    return(0) ;
}

int far releaseConsola ( int dfs )
{
    return(0) ;
}

int far readConsola ( int dfs, pointer_t dir, word_t nbytes )
{
    pindx_t indProcesoActual ;
    teclado_t far * teclado ;
    modoAp_t modoAp ;
    word_t nbytes0 ;
    int df ;
    int i = 0 ;
    char car ;
    word_t con ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    indProcesoActual = *ptrIndProcesoActual ;
    df = (*ptrTramaProceso)->BX ;
    con = ptrDescFichero[dfs].menor ;
    teclado = (teclado_t far *)&descConsola[con].teclado ;
    modoAp = ptrDescProceso[indProcesoActual].tfa[df].modoAp ;

    nbytes0 = nbytes ;
    if (nbytes <= teclado->ncar)
    {
        while (nbytes > 0)
        {
//          car = sacar(teclado) ;
//          car = tablaSP[sacar(teclado)] ;
            car = tablaDeConversion[sacar(teclado)] ;
            if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
            dir[i++] = car ;
            nbytes-- ;
        }
        restore_DS0() ;                           /* restaura el DS de SO1 */
        return(nbytes0) ;
    }
    else
    {
        while (teclado->ncar > 0)
        {
//          car = sacar(teclado) ;
//          car = tablaSP[sacar(teclado)] ;
            car = tablaDeConversion[sacar(teclado)] ;
            if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
            dir[i++] = car ;
            nbytes-- ;
        }
        nbytesProceso[indProcesoActual] = nbytes ;
        dirProceso[indProcesoActual] = (pointer_t)&dir[i] ;
        encolarPC2c(indProcesoActual, &(teclado->bloqueados)) ;
        bloquearProcesoActual(rec_consola) ;      /* no se retorna de aqui */
    }

    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(-1) ;
}

int far aio_readConsola ( int dfs, pointer_t dir, word_t nbytes )
{
    pindx_t indProcesoActual ;
    teclado_t far * teclado ;
    modoAp_t modoAp ;
    word_t nbARetornar0, nbARetornar ;
    int df ;
    int i = 0 ;
    char car ;
    word_t con ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    indProcesoActual = *ptrIndProcesoActual ;
    df = (*ptrTramaProceso)->BX ;
    con = ptrDescFichero[dfs].menor ;
    teclado = (teclado_t far *)&descConsola[con].teclado ;
    modoAp = ptrDescProceso[indProcesoActual].tfa[df].modoAp ;

    if (nbytes <= teclado->ncar)
        nbARetornar = nbytes ;
    else
        nbARetornar = teclado->ncar ;
    nbARetornar0 = nbARetornar ;
    while (nbARetornar > 0)
    {
        car = sacar(teclado) ;
        if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
        dir[i++] = car ;
        nbARetornar-- ;
    }

    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(nbARetornar0) ;
}

int far writeConsola ( int dfs, pointer_t dir, word_t nbytes )
{
    int i ;
    char car ;
    word_t con ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    con = ptrDescFichero[dfs].menor ;
    for ( i = 0 ; i < nbytes ; i++ )
    {
        car = dir[i] ;
        printCarConsola(con, car) ;
    }
    if (con == consolaDeSuperficie)
        if (cursorC < 80)                      /* ptrBiosArea->VIDEO-width */
            goToXYHw(cursorF, cursorC) ;

    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(nbytes) ;
}

int far aio_writeConsola ( int dfs, pointer_t dir, word_t nbytes )
{
    return(writeConsola(dfs, dir, nbytes)) ;
}

long int far lseekConsola ( int dfs, long int pos, word_t whence )
{
    /* posicionar el cursor */

    word_t con ;
    long int posActual ;
    long int res ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    con = ptrDescFichero[dfs].menor ;

    posActual = cursorF*80+cursorC ;

    if (whence == SEEK_CUR)                                    /* SEEK_CUR */
        pos = posActual + pos ;

    if (whence != SEEK_END) {                        /* SEEK_SET, SEEK_CUR */
        if (goToXYConsola(con, pos/80, pos%80) == 0)
            res = pos ;
        else
            res = -1 ;
    }
    else                                                       /* SEEK_END */
        res = posActual ;

    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(res) ;
}

int far fcntlConsola ( int dfs, word_t cmd, word_t arg )
{
    return(0) ;
}

int far ioctlConsola ( int dfs, word_t request, word_t arg )
{
    int res ;
    word_t nuevaConsola ;
    byte_t con ;
    byte_t cursorFAux ;
    pantalla_t far * pantallaAux ;
    
    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    res = 0 ;
    if (request == 0x0001)      /* cambiar la consola actual a la indicada */
    {
        nuevaConsola = arg ;
        if (nuevaConsola > maxConsolas) res = -1 ;
        else if (nuevaConsola != consolaDeSuperficie)
        {
            salvarConsolaDeSuperficie() ;
            consolaDeSuperficie = nuevaConsola ;
            establecerConsolaDeSuperficie() ;
        }
    }
    else if (request == 0x0002)               /* obtener la consola actual */
    {
        res = consolaDeSuperficie ;
//      printStrBIOS("\n res = ") ;
//      printDecBIOS(res, 1) ;
    }
    else if (request == 0x0003) /* cambiar numero de lineas de la pantalla */
    {
        int numFilasAnt = maxFilasAct ;
        if (arg <= 0) res = numFilasAnt ;       /* numero de lineas actual */
        else if ((arg != 25) && (arg != 28) && (arg != 50)) res = -1 ;
        else if (arg == numFilasAnt) res = 0 ;
        else
        {
			int numFilas = arg ;
            ptrBiosArea->VIDEO_lastrow = arg - 1 ;
            if (cursorF >= numFilas)
            {
//              copiarPantalla((pantalla_t far *)&ptrPant->t[maxF-numFilas+1][0], ptrPant, numFilas) ;
                copiarCPantalla((pantalla_t far *)&ptrPant->t[cursorF-numFilas+1][0], ptrPant, numFilas) ;
                borrarCPantalla((pantalla_t far *)&ptrPant->t[numFilas][0], numFilasAnt-numFilas) ;
                cursorF = numFilas-1 ;
            }
            salvarConsolaDeSuperficie() ;
            redimensionar(arg, 80) ;
            establecerConsolaDeSuperficie() ;
            res = maxFilasAct ;
            if (maxFilasAct == numFilas)
            {
                for ( con = 0 ; con < maxConsolas ; con ++ )
                {
                    if (con != consolaDeSuperficie)
                    {
                        cursorFAux = descConsola[con].F ;
                        if (cursorFAux >= numFilas)
                        {
                            pantallaAux = (pantalla_t far *)&descConsola[con].pantalla ;
//                          copiarCPantalla((pantalla_t far *)&pantallaAux->t[maxF-numFilas+1][0], pantallaAux, numFilas) ;
                            copiarCPantalla((pantalla_t far *)&pantallaAux->t[cursorFAux-numFilas+1][0], pantallaAux, numFilas) ;
                            borrarCPantalla((pantalla_t far *)&pantallaAux->t[numFilas][0], numFilasAnt-numFilas) ;
                            descConsola[con].F = numFilas-1 ;
                        }
                    }
                }
            }
        }
    }
    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(res) ;
}

int far eliminarConsola ( pindx_t pindx )
{
    teclado_t far * teclado ;
    int i ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    for ( i = 0 ; i < maxConsolas ; i++ )
    {
        teclado = (teclado_t far *)&descConsola[consolaDeSuperficie].teclado ;
        if estaPC2c(pindx, &teclado->bloqueados)
        {
            eliminarPC2c(pindx, &teclado->bloqueados) ;
            break ;
        }
    }

    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(0) ;
}

#pragma warn +par

/* ----------------------------------------------------------------------- */
/*                  rutina de servicio de la interrupcion                  */
/* ----------------------------------------------------------------------- */

/* se puede usar o no el BIOS (usa_BIOS) */

#define usa_BIOS FALSE                               /* valor normal FALSE */

byte_t keyStroke = 0x00 ;   /* extended ASCII keystroque (INT 16H Fn 0x00) */
byte_t scanCode = 0x00 ;
byte_t scanCodeAnt = 0x00 ;
byte_t AltPulsada = FALSE ;
byte_t CtrlPulsada = FALSE ;

void far isr_consola ( void )
{
    word_t w ;
    char car ;
    word_t nbytes ;
    pointer_t dir ;
    int df ;
    pindx_t indProcesoActual ;
    pindx_t pindx ;
    modoAp_t modoAp ;
    bool_t extendido ;
    teclado_t far * teclado ;
    byte_t con ;
    byte_t nuevaConsola ;
    word_t numFilas ;
    word_t numFilasAnt ;
    byte_t cursorFAux ;
    pantalla_t far * pantallaAux ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */

    scanCodeAnt = scanCode ;
    asm in al,60h       /* ¿solo debe leerse una vez el registro de datos? */
    asm mov scanCode,al

    ptrVIOrg[nVIntConsola]() ;  /* llamamos a la antigua rti (pushf + int) */

    nuevaConsola = 0xFF ;
    numFilasAnt = maxFilasAct ;

#if (TRUE)                               /* trazas por si hay que depurar */
    printCarBIOS('<') ;
    printHexBIOS(scanCodeAnt, 2) ;
    printHexBIOS(scanCode, 2) ;
    printCarBIOS('>') ;
#endif

    if (scanCodeAnt == CS_Alt) AltPulsada = TRUE ;
    else if (scanCodeAnt == (CS_Alt | 0x80)) AltPulsada = FALSE ;

    if (scanCodeAnt == CS_Ctrl) CtrlPulsada = TRUE ;
    else if (scanCodeAnt == (CS_Ctrl | 0x80)) CtrlPulsada = FALSE ;

    if ((CtrlPulsada) && (scanCode == 0x26))                     /* Ctrl+L */
        cambiarTeclaListaBDA((teclaListaBDA() & 0xFF00) | '\f') ;

    if (AltPulsada)
    {

        switch (scanCode)
        {

        case CS_ES_BSlash :
            nuevaConsola = 0 ;
            break ;

        case CS_F1     : ;
        case CS_F2     : ;
        case CS_F3     : ;
        case CS_F4     : ;
        case CS_F5     : ;
        case CS_F6     : ;
        case CS_F7     : ;
        case CS_F8     : ;
        case CS_F9     : ;
        case CS_F10    :
            con = scanCode - CS_F1 + 1 ;
            if (con < maxConsolas)
                nuevaConsola = con ;
            break ;
        default        :
            ;
        }

        if (nuevaConsola != 0xFF)
        {
            if (nuevaConsola != consolaDeSuperficie)
            {
                salvarConsolaDeSuperficie() ;
                consolaDeSuperficie = nuevaConsola ;
                establecerConsolaDeSuperficie() ;
            }
        }
        else if (redimensionable)
        {
            switch (scanCode)
            {
            case 0x1B : ;                                          /* Alt+ */
            case 0x35 : ;                                          /* Alt- */
                if (scanCode == 0x1B)                              /* Alt+ */
                {
                    if (numFilasAnt >= 50) numFilas = 25 ;
                    else if (numFilasAnt >= 28) numFilas = 50 ;
                    else numFilas = 28 ;
                }
                else                                               /* Alt- */
                {
                    if (numFilasAnt >= 50) numFilas = 28 ;
                    else if (numFilasAnt >= 28) numFilas = 25 ;
                    else numFilas = 50 ;
                }
////            if (numFilas != numFilasAnt)
////            {
                    if (cursorF >= numFilas)
                    {
                        copiarCPantalla((pantalla_t far *)&ptrPant->t[cursorF-numFilas+1][0], ptrPant, numFilas) ;
                        borrarCPantalla((pantalla_t far *)&ptrPant->t[numFilas][0], numFilasAnt-numFilas) ;
//                      copiarPantalla((pantalla_t far *)&ptrPant->t[cursorF-numFilas+1][0], ptrPant, numFilas) ;
                        cursorF = numFilas-1 ;
                    }
                    salvarConsolaDeSuperficie() ;
                    redimensionar(numFilas, 80) ;
#define MSDOS_Player 1
#ifdef MSDOS_Player
                    goToXYHw(24, 79) ;
                    goToXYHw(cursorF, cursorC) ;   /* necesario para MSDOS Player (Takeda) ??? */
#endif
                    if (numFilas == ptrBiosArea->VIDEO_lastrow + 1)
                    {
                        establecerConsolaDeSuperficie() ;
                        for ( con = 0 ; con < maxConsolas ; con ++ )
                        {
                            if (con != consolaDeSuperficie)
                            {
                                cursorFAux = descConsola[con].F ;
                                if (cursorFAux >= numFilas)
                                {
                                    pantallaAux = (pantalla_t far *)&descConsola[con].pantalla ;
                                    copiarCPantalla((pantalla_t far *)&pantallaAux->t[cursorFAux-numFilas+1][0], pantallaAux, numFilas) ;
                                    borrarCPantalla((pantalla_t far *)&pantallaAux->t[numFilas][0], numFilasAnt-numFilas) ;
                                    descConsola[con].F = numFilas-1 ;
                                }
                            }
                        }
                    }
////            }
                break ;
            default :
                ;
            }
        }
        w = leerTeclaListaBDA() ;                        /* /PP 2016-10-31 */
        restore_DS0() ;                           /* restaura el DS de SO1 */
        return ;                                         /* /PP 2016-10-31 */
    }

    if (scanCode & 0x80)
    {
        restore_DS0() ;                           /* restaura el DS de SO1 */
        return ;
    }

#if (!usa_BIOS)
    w = leerTeclaListaBDA() ;           /* para sacar la palabra del bufer */
//puntoDeParada() ;
#else
    w = teclaListaBIOS() ;
    if (w) leerTeclaBIOS() ;            /* para sacar la palabra del bufer */
    /* funciona mal en qemu/bochs ¿permite las interrupciones? */
//  puntoDeParada() ;
#endif

//  printHexBIOS(w, 4) ; printCarBIOS(' ') ;

    if (w)
    {
        car = (char)w ;
//      car = tablaSP[car] ;
        car = tablaDeConversion[car] ;

        if (car == (char)0xE0)
            car = '\0' ;
        extendido = (car == '\0') ;

        teclado = (teclado_t far *)&descConsola[consolaDeSuperficie].teclado ;

        if (teclado->bloqueados.numElem > 0)
        {
            pindx = teclado->bloqueados.primero ;
            df = ptrDescProceso[pindx].trama->BX ;
            modoAp = ptrDescProceso[pindx].tfa[df].modoAp ;
            /*    plot('k', car, contadorTimer0()) ; */
            nbytes = nbytesProceso[pindx] ;
            dir = dirProceso[pindx] ;
            nbytes-- ;
            if ((car == '\r') && (modoAp & O_TEXT)) car = '\n' ;
            dir[0] = car ;                                          /* car */
            dir++ ;
            if (extendido)
            {
                keyStroke = (byte_t)(w >> 8) ;
                car = (char)keyStroke ;
                if (nbytes > 0)
                {
                    nbytes-- ;
                    dir[1] = car ;                             /* scanCode */
                    dir++ ;
                }
                else
                    meter(car, teclado) ;
            }
            nbytesProceso[pindx] = nbytes ;
            dirProceso[pindx] = dir ;
            if (nbytes == 0)
            {
                desencolarPC2c(&teclado->bloqueados) ;
                ptrDescProceso[pindx].trama->AX =
                    ptrDescProceso[pindx].trama->CX ;           /* nbytes0 */
                ptrDescProceso[pindx].tfa[df].pos +=
                    ptrDescProceso[pindx].trama->CX ;
                ptrDescProceso[pindx].estado = preparado ;
                encolarPC2c(pindx, (ptrC2c_t)&ptrC2cPFR[PUrgentes]) ;
                *ptrActivarAlEpilogo = TRUE ;
                indProcesoActual = *ptrIndProcesoActual ;
                if (indProcesoActual > -1)                  /* nivel 1 o 2 */
                {
                    ptrDescProceso[indProcesoActual].trama =
                        MK_FP(*ptrSS_Proceso, *ptrSP_Proceso) ;
                    ptrDescProceso[indProcesoActual].estado = preparado ;
                    encolarPC2c(indProcesoActual, (ptrC2c_t)&ptrC2cPFR[PPreparados]) ;
                    *ptrIndProcesoActual = -1 ;
                }
            }
        }
        else
        {
            meter(car, teclado) ;                                   /* car */
            if (extendido)
            {
                keyStroke = (byte_t)(w >> 8) ;
                car = (char)keyStroke ;                       /* keyStroke */
                meter(car, teclado) ;
            }
        }
    }
    /*
    if (*info.ptrDebugWord > 0)
    {
        printStrBIOS("\n debugWord = ") ;
        printHexBIOS(*info.ptrDebugWord, 4) ;
        printStrBIOS("\n isr_consola pto 2: ") ;
        printStrBIOS("\n indProcesoActual = ") ;
        printIntBIOS(indProcesoActual, 1) ;
        printStrBIOS(" pindx = ") ;
        printIntBIOS(pindx, 1) ;
        *info.ptrDebugWord = *info.ptrDebugWord - 1 ;
        if (*info.ptrDebugWord == 0) while(TRUE) ;
    }
    */
    restore_DS0() ;                               /* restaura el DS de SO1 */
}

/* ----------------------------------------------------------------------- */
/*                     call back para el recurso TIMER                     */
/* ----------------------------------------------------------------------- */

/* necesitamos funciones para escribir desde el call back                  */

byte_t congen = 0 ;                                                /* DATA */

int pCon ( char car )
{
    printCarConsola(congen, car) ;
    return(1) ;
}
#if (REDUCIR_DRIVER) 
int printLnConsola ( byte_t con )
{
    congen = con ;
    return(printGenLn(pCon)) ;
}
#endif /* REDUCIR_DRIVER */
int printStrConsola ( byte_t con, const char far * str )
{
    congen = con ;
    return(printGenStr(str, pCon)) ;
}

int printStrHastaConsola ( byte_t con, const char far * str, word_t n, bool_t lleno )
{
    congen = con ;
    return(printGenStrHasta(str, n, lleno, pCon)) ;
}
#if (REDUCIR_DRIVER)
int printDecConsola ( byte_t con, word_t num, word_t l )
{
    congen = con ;
    return(printGenDec(num, l, pCon)) ;
}
#endif /* REDUCIR_DRIVER */
int printIntConsola ( byte_t con, int num, word_t l )
{
    congen = con ;
    return(printGenInt(num, l, pCon)) ;
}
#if (REDUCIR_DRIVER)
int printLIntConsola ( byte_t con, long int num, word_t l )
{
    congen = con ;
    return(printGenLInt(num, l, pCon)) ;
}
#endif /* REDUCIR_DRIVER */
int printLDecConsola ( byte_t con, dword_t num, word_t l )
{
    congen = con ;
    return(printGenLDec(num, l, pCon)) ;
}
#if (REDUCIR_DRIVER)
int printHexConsola ( byte_t con, word_t num, word_t l )
{
    congen = con ;
    return(printGenHex(num, l, pCon)) ;
}

int printLHexConsola ( byte_t con, dword_t num, word_t l )
{
    congen = con ;
    return(printGenLHex(num, l, pCon)) ;
}

int printPtrConsola ( byte_t con, pointer_t ptr )
{
    congen = con ;
    return(printGenPtr(ptr, pCon)) ;
}
#endif /* REDUCIR_DRIVER */
/* las siguientes cadenas de caracteres estan pensadas para ser utilizadas */
/* en cbForTimer. Son convenientes cuando se reduce el tamanio del driver  */
/* al instalarlo haciendo la llamada al sistema:                           */
/*                                                                         */
/*   esperarDesinstalacion(tamDATA, finCodeDriver, finishDriver, tamPila   */
/*                                                                         */
/* El problema es que el compilador pone las cadenas de caracteres (ver s@ */
/* en los ficheros de listado del ensamblado) al final, por lo que al      */
/* reducir el segmento de datos (DATA) se pierden. Esto se evita haciendo  */
/* la declaracion de esas variables aqui e inicializandolas.               */

//char strCbBlancos [ ] = "                " ;
char strCbBlancos [ ] = "" ;
char strCbCON [ ] = "CON" ;

int far cbForTimer ( void far * arg )                         /* call back */
{
    byte_t F0 ;
    byte_t C0 ;
    byte_t congen0 ;
    byte_t visibilidadAnt ;
//  trama_t far * tramaProceso ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
    setraw_DS() ;           /* establece el DS correspondiente al programa */
    /* punteros a pila (parametros/variables locales) */
    /*
      tramaProceso = MK_FP(*ptrSS_Proceso, *ptrSP_Proceso) ;
    */
    F0 = cursorF ;
    C0 = cursorC ;
    congen0 = congen ;
    visibilidadAnt = setCursorVisibilidad(0) ;
    if (consolaDeSuperficie != 0)
    {
        goToXYConsola(consolaDeSuperficie, 0, 75) ;
//      printStrConsola(consolaDeSuperficie, "     ") ;
        printStrHastaConsola(consolaDeSuperficie, strCbBlancos, 5, TRUE) ;
        goToXYConsola(consolaDeSuperficie, 1, 75) ;
//      printStrConsola(consolaDeSuperficie, "CON") ;
        printStrConsola(consolaDeSuperficie, strCbCON) ;
        printIntConsola(consolaDeSuperficie, consolaDeSuperficie, 1) ;
    }
    else
    {
        goToXYConsola(0, 0, 64) ;
//      printStrConsola(0, "                ") ;
//      printStrConsola(0, strCbBlancos) ;
        printStrHastaConsola(0, strCbBlancos, 16, TRUE) ;
        goToXYConsola(0, 1, 64) ;
        printLDecConsola(0, ((argCbTimer_t far *)arg)->contTics, 11) ;
        printIntConsola(0, ((argCbTimer_t far *)arg)->contTicsRodaja, 4) ;
    }
    congen = congen0 ;
    cursorF = F0 ;
    cursorC = C0 ;
    goToXYHw(cursorF, cursorC) ;
    setCursorVisibilidad(visibilidadAnt) ;

    restore_DS0() ;                               /* restaura el DS de SO1 */
    return(1) ;
}

int finishConsola ( void )
{
//  exit(0) ;              /* obligaria a meter la biblioteca ll_s_so1.lib */
    return(0) ;
}

void finCodeDriver ( void )   /* marca el fin del codigo propio del driver */
{
}

#define maxCbConsola 0

descCcb_t descCcbConsola = { 0, 0, 0, maxCbConsola, NULL } ;

/* ----------------------------------------------------------------------- */
/*                      seccion de programa de usuario                     */
/* ----------------------------------------------------------------------- */

void mostrarFormato ( void )
{
    printf(" formato: CONSOLA [ [ -i | -q | -c | -l ] [ num ] | -N | -n | -u | -k | -h ] ") ;
}

int formato ( void )
{
    printf("\n\n") ;
    mostrarFormato() ;
    printf("\n") ;
    return(-1) ;
}

int help ( void )
{
    printf("\n\n") ;
    mostrarFormato() ;
    printf(
        ""                                                               "\n"
        ""                                                               "\n"
        " instala/desinstala el driver de la consola"                    "\n"
        ""                                                               "\n"
        " opciones: (por defecto -i y num = 6)"                          "\n"
        ""                                                               "\n"
        "      -i  : instala el driver (usar &)"                         "\n"
        "      -q  : instala sin mensajes de salida (&)"                 "\n"
        "      -c  : cambia la consola actual a la num"                  "\n"
        "      -l  : establece en numero de lineas a num"                "\n"
        "      -n  : status = numero de consola actual"                  "\n"
        "      -nq : como -n pero sin mensaje por pantalla"              "\n"
        "      num : numero de consolas (o nueva c.)"                    "\n"
        "      -u  : desintala el driver"                                "\n"
        "      -k  : desintala el driver (matando)"                      "\n"
        "      -h  : muestra este help"                                  "\n"
    ) ;
    return(0) ;
}

void hacerTablaNula ( char * tablaDeConversion ) 
{
	int i ;
	for ( i = 0 ; i < 256 ; i++ ) 
		tablaDeConversion[i] = (char)i ;
}

void establecerTablaDeConversion ( void )
{
    int anioBIOS ;

    anioBIOS = 10*(ptrFechaBios[6] - '0') + (ptrFechaBios[7] - '0') ;

    switch (anioBIOS)
    {
    case 99 : /* hayQemu   */   /* "06/23/99" */
    case 11 : /* hayBochs  */   /* "02/10/11" */            /* Bochs 2.6.7 */
    case 14 : /* hayBochs  */   /* "12/26/14" */            /* Bochs 2.6.8 */
    case 17 : /* hayBochs  */   /* "02/16/17" */            /* Bochs 2.6.9 */
    case 95 : /* hayNTVDM  */   /* "07/03/95" */
    case 12 : /* hayFake86 */   /* "05/02/12" */
//  case 99 : /* hayVDos   */   /* "01/01/99" */ 
        break ;                 
    case  6 : /* hayMsdos  */                                    /* Takeda */
    case 92 : /* hayDBox   */   /* "01/01/92" */
    default  :
        hacerTablaNula(tablaDeConversion) ;                          /* US */
    }
}

int integrarConsola ( byte_t numConsolas, bool_t conMensajes )
{
    dfs_t dfs ;
    descRecurso_t dR ;
    int dfTimer ;
    int res ;
    byte_t con ;
    pindx_t pindx ;
    char nomFich [ 12 ] ;

//  obtenInfoINFO((info_t far *)&info) ;                     /* depuracion */

    inicCrtHw() ;

//  if (*((word_t far *)0xF000FFFB) == 0x3630)    /* anio fecha BIOS msdos */ /* Takeda */
    if (*((word_t far *)0xF000FFFB) == 0x3239)    /* anio fecha BIOS msdos */ /* Takeda */
//      inicBiosCrt(24, 80, TRUE) ;           /* se fuerza redimensionable */
    {
        inicBiosCrt(ptrBiosArea->VIDEO_lastrow + 1, 80, TRUE) ; /* se fuerza redimensionable */
    }
    else
        inicBiosCrt(50, 80, FALSE) ;       /* no se fuerza redimensionable */

    establecerTablaDeConversion() ;                       /* teclado US/SP */

    maxConsolas = numConsolas + 1 ;

#if (FALSE)
    descConsola = MK_FP(
                      ll_buscarBloque(((maxConsolas*sizeof(descConsola_t))+15)/16),  /* GM */
                      0x0000
                  ) ;
#else
	/* se supone que el proceso inicial ha reservado antes el bloque */
	descConsola = MK_FP(_CS-(((maxConsolas*sizeof(descConsola_t))+15)/16), 0x0000) ;
#endif
				  
/*
    printStrBIOS("\n descConsola = ") ;
    printPtrBIOS((pointer_t)descConsola) ;
    printStrBIOS("\n numFilas = ") ;
    printIntBIOS(maxFilasAct, 1) ;
    printStrBIOS(" numColumnas = ") ;
    printIntBIOS(numColumnas, 1) ;
*/
    for ( con = 0 ; con < maxConsolas ; con++ )
    {
        descConsola[con].teclado.in = 0 ;
        descConsola[con].teclado.out = 0 ;
        descConsola[con].teclado.ncar = 0 ;
        inicPC2c(&descConsola[con].teclado.bloqueados,
                 &descConsola[con].teclado.e2Bloqueados, maxProcesos + 0, TRUE) ;
        descConsola[con].teclado.inRaw = 0 ;
        descConsola[con].teclado.outRaw = 0 ;
        descConsola[con].teclado.nRaw = 0 ;
        inicPC2c(&descConsola[con].teclado.bloqueadosRaw,
                 &descConsola[con].teclado.e2Bloqueados, maxProcesos + 1, TRUE) ;
    }

    consolaDeSuperficie = 0 ;

    for ( con = 0 ; con < maxConsolas ; con++ )
    {
        borrarPantalla((pantalla_t far *)&descConsola[con].pantalla, maxFilas) ;
        descConsola[con].F = 0 ;
        descConsola[con].C = 0 ;
    }

    for ( pindx = 0 ; pindx < maxProcesos ; pindx++ )
    {
        nbytesProceso[pindx] = 0 ;                /* nbytes esperando leer */
        dirProceso[pindx] = NULL ;
    }

    dR.tipo = rDCaracteres ;
    strcpy(dR.nombre, "CONSOLA") ;
    dR.ccb = (ccb_t)&descCcbConsola ;
    dR.ccb->arg = NULL ;
    dR.pindx = getpindx() ;
    dR.numVI = 1 ;
    dR.nVInt[0] = nVIntConsola ;
    dR.irq[0] = IRQ_TECLADO ;
    dR.isr[0] = MK_FP(_CS, FP_OFF(isr_consola)) ;

    dR.open      = (open_t)     MK_FP(_CS, FP_OFF(openConsola)) ;
    dR.release   = (release_t)  MK_FP(_CS, FP_OFF(releaseConsola)) ;
    dR.read      = (read_t)     MK_FP(_CS, FP_OFF(readConsola)) ;
    dR.aio_read  = (aio_read_t) MK_FP(_CS, FP_OFF(aio_readConsola)) ;
    dR.write     = (write_t)    MK_FP(_CS, FP_OFF(writeConsola)) ;
    dR.aio_write = (aio_write_t)MK_FP(_CS, FP_OFF(aio_writeConsola)) ;
    dR.lseek     = (lseek_t)    MK_FP(_CS, FP_OFF(lseekConsola)) ;
    dR.fcntl     = (fcntl_t)    MK_FP(_CS, FP_OFF(fcntlConsola)) ;
    dR.ioctl     = (ioctl_t)    MK_FP(_CS, FP_OFF(ioctlConsola)) ;

    dR.eliminar  = (eliminar_t) MK_FP(_CS, FP_OFF(eliminarConsola)) ;

    rec_consola = crearRecurso(&dR) ;

    if (rec_consola >= 0)
    {
        strcpy(nomFich, "CON ") ;
        for ( con = 0 ; con < maxConsolas ; con++ )
        {
            nomFich[3] = '0' + con ;
            dfs = crearFichero(nomFich, rec_consola, con, fedCaracteres) ;
            if (dfs < 0)
            {
                switch(dfs)
                {
                case -1 : printf(" tipo de fichero erroneo") ; break ;
                case -2 : printf(" numero de recurso erroneo") ; break ;
                case -3 : printf(" CON%i nombre de fichero ya existente", con) ; break ;
                case -4 : printf(" no hay descriptores de fichero libres") ; break ;
                default : printf(" no ha podido crearse el fichero CON%i", con) ;
                }
                destruirRecurso("CONSOLA", TRUE) ;              /* matando */
            }
        }

        if ((dfTimer = open("TIMER", O_RDONLY)) >= 0)
        {
            close(dfTimer) ;
            res = encolarCcbRecurso(
                      (callBack_t)MK_FP(_CS, (word_t)cbForTimer),
                      (char far *)MK_FP(_DS, (word_t)&"TIMER")
                  ) ;
            if (res != 0)
                printf("\n\n no ha podido encolarse el callback en el TIMER \n") ;
        }
        else
            printf("\n\n no ha podido encolarse el callback en el TIMER \n") ;

        if (conMensajes)
            printf("\n\n recurso CONSOLA instalado (ficheros: CON0 ...) \n") ;
        return(0) ;
    }

    printf("\n\n") ;
    switch(rec_consola)
    {
    case -1 : printf(" tipo de recurso erroneo ") ; break ;
    case -2 : printf(" demasiados vectores de interrupcion ") ; break ;
    case -3 : printf(" CONSOLA nombre de recurso ya existente ") ; break ;
    case -4 : printf(" no hay descriptores de recurso libres ") ; break ;
    case -5 : printf(" numero de vector de interrupcion ya usado ") ; break ;
    default : printf(" no ha podido crearse el recurso CONSOLA ") ;
    }
    printf("\n") ;
    return(-1) ;
}

int desintegrarConsola ( void )
{
    int res ;
    res = eliminarCcbRecurso(
              (callBack_t)MK_FP(_CS, (word_t)cbForTimer),
              (char far *)MK_FP(_DS, (word_t)&"TIMER")
          ) ;
    if (res != 0)
        printf(" no pudo eliminarse el callback del TIMER ") ;
    else if (ll_devolverBloque(                                      /* GM */
                FP_SEG(descConsola),
                ((maxConsolas*sizeof(descConsola_t))+15)/16)
            )
        return(-1) ;
/*  finBiosCrt() ; */
    return(res) ;
}

int instalarConsola ( byte_t numConsolas, bool_t conMensajes )
{
    int res ;
    res = comprobarAmpersand() ;
    if (res != 0) return(res) ;
    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */
    res = integrarConsola(numConsolas, conMensajes) ;
    if (res != 0) return(res) ;
#if (!REDUCIR_DRIVER)
    esperarDesinstalacion(0) ;                       /* bloquea el proceso */
#else
    esperarDesinstalacion(                           /* bloquea el proceso */
//      FP_OFF(dirDescSO1) + sizeof(descSO1_t)
//          ...
//          + sizeof(descCcbConsola) + 0*sizeof(callBack_t),    /* tamDATA */
        FP_OFF(&descCcbConsola)
            + sizeof(descCcbConsola) + 0*sizeof(callBack_t),    /* tamDATA */
        FP_OFF(finCodeDriver),                            /* finCodeDriver */
        FP_OFF(finishConsola),                             /* finishDriver */
		0x0000                                                  /* tamPila */ 
    ) ;
/*  se retorna a finishConsola */		
#endif
/*  solo se llega aqui en el caso esperarDesinstalacion(0) */
    res = desintegrarConsola() ;
    return(res) ;
}

int main ( int argc, char * argv [ ] )
{
    int res ;
    int dfCon ;
    if (argc > 3) return(formato()) ;
    else if (argc == 1) return(instalarConsola(numConsolasPorDefecto, TRUE)) ;
    else if (argc == 2)
    {
        if (argv[1][0] != '-') return(formato()) ;
        switch (toupper(argv[1][1]))
        {
        case 'H' :
            return(help()) ;
        case 'I' :
            return(instalarConsola(numConsolasPorDefecto, TRUE)) ;
        case 'Q' :
            return(instalarConsola(numConsolasPorDefecto, FALSE)) ;
        case 'N' :
            if ((dfCon = open("CON0", O_RDONLY)) < 0)
            {
                printf(" no puede abrirse CON0") ;
                return(-1) ;
            }
            else
            {
                res = ioctl(dfCon, 0x0002, 0x0000) ;
                if (res < 0) printf(" error al consultar la consola actual ") ;
                else
                {
                    if (toupper(argv[1][2]) != 'Q') /* (-nq no muestra mensaje) */
                        printf(" la consola actual es CON%i", res) ;
                }
                return(res) ;
            }
        case 'U' :
        case 'K' :
            res = destruirRecurso("CONSOLA", tolower(argv[1][1]) == 'k') ; 
            switch (res)
            {
            case  0 : printf(" recurso CONSOLA desinstalado") ; break ;
            case -1 : printf(" recurso CONSOLA no existe") ; break ;
            case -2 : printf(" recurso CONSOLA en uso") ; break ;
            case -3 : printf(" fichero CONSOLA no puede destruirse") ; break ;
            default : printf(" CONSOLA no ha podido desinstalarse") ;
            }
            return(res) ;
        default :
            ;
        }
    }
    if ((argc == 2) ||
        ((argc == 3) &&
         (!strcmpu(argv[1], "-i") || !strcmpu(argv[1], "-q"))
        )
       )
    {
        strcpy(comando[0], argv[argc-1]) ;
        inicScanner() ;
        obtenSimb() ;
        if (simb == s_numero)
        {
            if (num == 0)
            {
                printf("\n\n el numero de consolas debe ser > 0 \n") ;
                return(-1) ;
            }
            else return(instalarConsola(num, (toupper(argv[1][1]) == 'Q'))) ;
        }
    }
    else if (argc == 3)
        if (!strcmpu(argv[1], "-c"))
        {
            strcpy(comando[0], argv[argc-1]) ;
            inicScanner() ;
            obtenSimb() ;
            if (simb == s_numero)
            {
                if ((dfCon = open("CON0", O_RDONLY)) < 0)
                {
                    printf(" no puede abrirse CON0") ;
                    return(-1) ;
                }
                if (ioctl(dfCon, 0x0001, num) != 0)
                {
                    printf(" numero de consola erroneo (%i)", num) ;
                    return(-1) ;
                }
                return(0) ;
            }
        }
        else if (!strcmpu(argv[1], "-l"))
        {
            strcpy(comando[0], argv[argc-1]) ;
            inicScanner() ;
            obtenSimb() ;
            if (simb == s_numero)
            {
                if ((num != 25) && (num != 28) && (num != 50)) {
                    printf(" el numero de lineas debe ser 25, 28 o 50") ;
                    return(-1) ;
                }
                if ((dfCon = open("CON0", O_RDONLY)) < 0)
                {
                    printf(" no pudo abrirse CON0") ;
                    return(-1) ;
                }
                if (ioctl(dfCon, 0x0003, num) == -1)
                {
                    printf(" numero de consola erroneo (%i)", num) ;
                    return(-1) ;
                }
                return(0) ;
            }
        }
    return(formato()) ;
}