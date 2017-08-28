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

#include <so1pub.h\comundrv.h>       /* segDatos, ptrIndProcesoActual, ... */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\escribir.h>                 /* escribirStr, escribirDec */
#include <so1pub.h\carsctrl.h>                  /* CR, LF, FF, BS, HT, BEL */
#include <so1pub.h\caracter.h>                                /* mayuscula */
#include <so1pub.h\strings.h>                        /* copiarStr, iguales */
#include <so1pub.h\bios_0.h>            /* printCarBIOS, leerTeclaListaBDA */
#include <so1pub.h\bios_crt.h>      /* inicBiosCrt, ptrPant, numFilas, ... */
#include <so1pub.h\crthw.h>                         /* inicCrtHw, goToXYHw */
#include <so1pub.h\memvideo.h>          /* printCarVideo, cursorF, cursorC */
#include <so1pub.h\def_tecl.h>                                /* teclado_t */
#include <so1pub.h\pic.h>                                   /* IRQ_TECLADO */
#include <so1pub.h\def_timer.h>                            /* argCbTimer_t */
#include <so1pub.h\printgen.h>            /* printGenStr, printGenDec, ... */

#include <so1pub.h\debug.h>                               /* puntoDeParada */

#include <progusr\consola\consola.h>             /* codigos de exploracion */
#include <progusr\consola\tablasp.h>         /* tablaDeConversion, tablaSP */

#pragma option -w-use /* (comundrv.h) warning: 'x' declared but never used */

info_t info ;                                                /* depuracion */

rindx_t rec_consola ;

/* ----------------------------------------------------------------------- */
/*                   seccion de implementacion del driver                  */
/* ----------------------------------------------------------------------- */

#define nVIntConsola 0x09     /* numero de vector utilizado por el teclado */

typedef struct
{
    pantalla_t pantalla ;
    teclado_t  teclado ;
    byte_t     F ;                       /* fila de la posicion del cursor */
    byte_t     C ;                    /* columna de la posicion del cursor */
} descConsola_t ;

descConsola_t far * descConsola ;

#define numConsolasPorDefecto 6                       /* valor por defecto */

byte_t maxConsolas ;

word_t consolaDeSuperficie ;

word_t nbytesProceso [ maxProcesos ] ;        /* tabla de bytes pendientes */

pointer_t dirProceso [ maxProcesos ] ;     /* tabla de direcciones destino */

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
    {
        pantalla_t far * pantalla = (pantalla_t far *)&descConsola[con].pantalla ;
        byte_t F = descConsola[con].F ;
        byte_t C = descConsola[con].C ;
        switch (car)
        {
        case FF  :
            borrarCPantalla(pantalla, numFilas) ;
            /* borrarPantalla(pantalla, numFilas) ; */
            descConsola[con].F = 0 ;
            descConsola[con].C = 0 ;
            break ;
        case CR  :
            descConsola[con].C = 0 ;
            break ;
        case LF  :
            if (++descConsola[con].F == numFilas)
            {
                scrollCPantalla(pantalla, numFilas) ;
                descConsola[con].F = numFilas-1 ;
            }
            break ;
        case BS  :
            if (C > 0)
            {
                C-- ;
                pantalla->t[F][C].car = ' ' ;
                descConsola[con].C = C ;
            }
            else printCarBIOS(BEL) ;
            break ;
        case HT  :
            car = ' ' ;
        case BEL :
            printCarBIOS(car) ;
            break ;
        default  :
            pantalla->t[F][C].car = car ;
            if (++descConsola[con].C == 80)
            {
                descConsola[con].C = 0 ;
                if (++descConsola[con].F == numFilas)
                {
                    scrollCPantalla(pantalla, numFilas) ;
                    descConsola[con].F = numFilas-1 ;
                }
            }
        }
    }
    return(0) ;
}

int goToXYConsola ( byte_t con, byte_t F, byte_t C )
{
    if (F >= numFilas) return(-1) ;
    if (C >= 80) return(-1) ;
    if (con == consolaDeSuperficie)
    {
        /*    goToXYHw(F, C) ; */
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
    copiarCPantalla(pantalla, ptrPant, numFilas) ;
    /* copiarPantalla(pantalla, ptrPant, numFilas) ; */
    goToXYHw(
        descConsola[consolaDeSuperficie].F,
        descConsola[consolaDeSuperficie].C
    ) ;
    cursorF = descConsola[consolaDeSuperficie].F ;
    cursorC = descConsola[consolaDeSuperficie].C ;
}

void salvarConsolaDeSuperficie ( void )
{
    pantalla_t far * pantalla =
        (pantalla_t far *)&descConsola[consolaDeSuperficie].pantalla ;
    copiarCPantalla(ptrPant, pantalla, numFilas) ;
    /* copiarPantalla(ptrPant, pantalla, numFilas) ; */
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

    word_t DS_Consola = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
    pindx_t indProcesoActual ;
    teclado_t far * teclado ;
    modoAp_t modoAp ;
    word_t nbytes0 ;
    int df ;
    int i = 0 ;
    char car ;
    word_t con ;

    asm push ds
    asm mov ds,DS_Consola

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
//    car = sacar(teclado) ;
//    car = tablaSP[sacar(teclado)] ;
            car = tablaDeConversion[sacar(teclado)] ;
            if ((car == CR) && (modoAp & O_TEXT)) car = LF ;
            dir[i++] = car ;
            nbytes-- ;
        }
        asm pop ds                /* 2013-07-09: faltaba esto (sino CRASH) */
        return(nbytes0) ;
    }
    else
    {
        while (teclado->ncar > 0)
        {
//    car = sacar(teclado) ;
//    car = tablaSP[sacar(teclado)] ;
            car = tablaDeConversion[sacar(teclado)] ;
            if ((car == CR) && (modoAp & O_TEXT)) car = LF ;
            dir[i++] = car ;
            nbytes-- ;
        }
        nbytesProceso[indProcesoActual] = nbytes ;
        dirProceso[indProcesoActual] = (pointer_t)&dir[i] ;
        encolarPC2c(indProcesoActual, &(teclado->bloqueados)) ;
        bloquearProcesoActual(rec_consola) ;      /* no se retorna de aqui */
    }

    asm pop ds
    return(-1) ;
}

int far aio_readConsola ( int dfs, pointer_t dir, word_t nbytes )
{
    word_t DS_Consola = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
    pindx_t indProcesoActual ;
    teclado_t far * teclado ;
    modoAp_t modoAp ;
    word_t nbARetornar0, nbARetornar ;
    int df ;
    int i = 0 ;
    char car ;
    word_t con ;

    asm push ds
    asm mov ds,DS_Consola

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
        if ((car == CR) && (modoAp & O_TEXT)) car = LF ;
        dir[i++] = car ;
        nbARetornar-- ;
    }

    asm pop ds
    return(nbARetornar0) ;
}

int far writeConsola ( int dfs, pointer_t dir, word_t nbytes )
{
    word_t DS_Consola = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
    int i ;
    char car ;
    word_t con ;

    asm push ds
    asm mov ds,DS_Consola

    con = ptrDescFichero[dfs].menor ;
    for ( i = 0 ; i < nbytes ; i++ )
    {
        car = dir[i] ;
        printCarConsola(con, car) ;
    }
    if (con == consolaDeSuperficie)
        goToXYHw(cursorF, cursorC) ;

    asm pop ds
    return(nbytes) ;
}

int far aio_writeConsola ( int dfs, pointer_t dir, word_t nbytes )
{
    return(writeConsola(dfs, dir, nbytes)) ;
}

long int far lseekConsola ( int dfs, long int pos, word_t whence )
{

    /* posicionar el cursor */

    word_t DS_Consola = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
    word_t con ;
	long int posActual ;
	long int res ;

    asm push ds
    asm mov ds,DS_Consola

    con = ptrDescFichero[dfs].menor ;

	if (whence == SEEK_CUR) 
	{
		posActual = cursorF*80+cursorC ;
        pos = posActual + pos ;
	}		
	if (whence != SEEK_END) {                        /* SEEK_END, SEEK_CUR */
   	    if (goToXYConsola(con, pos/80, pos%80) == 0) 
		    res = pos ;
	    else 
		    res = -1 ;
	}
	else                                                       /* SEEK_END */
		res = posActual ;
		
    asm pop ds
    return(res) ;
}

int far fcntlConsola ( int dfs, word_t cmd, word_t arg )
{
    return(0) ;
}

int far ioctlConsola ( int dfs, word_t request, word_t arg )
{
#if (TRUE)
    word_t DS_Consola = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
    int res ;
    word_t nuevaConsola ;
	byte_t con ;
    byte_t cursorFAux ;
    pantalla_t far * pantallaAux ;
    int numFilasAnt ;
	
    asm push ds
    asm mov ds,DS_Consola

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
//  printStrBIOS("\n res = ") ;
//  printDecBIOS(res, 1) ;
    }
    else if (request == 0x0003) /* cambiar numero de lineas de la pantalla */
    {
		if (arg <= 0) res = numFilas ; /* devolvemos el numero de lineas actual */
		else if ((arg != 25) && (arg != 28) && (arg != 50)) res = -1 ;
		else if (arg == numFilasAnt) res = 0 ;
        else  
        {
	     	numFilasAnt = numFilas ;
            numFilas = arg ;		
//  printStrBIOS("\n numFilas = ") ;
//  printDecBIOS(numFilas, 1) ;
//  printStrBIOS(" numFilasAnt = ") ;
//  printDecBIOS(numFilasAnt, 1) ;
//  printStrBIOS(" cursorF = ") ;
//  printDecBIOS(cursorF, 1) ;
#if (TRUE)
            if (numFilasAnt >= numFilas)
            {
                copiarCPantalla((pantalla_t far *)&ptrPant->t[numFilasAnt-numFilas+1][0], ptrPant, numFilas) ;
//              copiarPantalla((pantalla_t far *)&ptrPant->t[cursorF-numFilas+1][0], ptrPant, numFilas) ;
//**/           borrarCPantalla((pantalla_t far *)&ptrPant->t[numFilas][0], numFilasAnt) ;
/**/            borrarCPantalla((pantalla_t far *)&ptrPant->t[numFilas][0], numFilasAnt-numFilas) ;
                cursorF = numFilas-1 ;
            }
#endif			
            salvarConsolaDeSuperficie() ;
            redimensionar(numFilas, 80) ;
            numFilas = ptrBiosArea->VIDEO_lastrow + 1 ;
            establecerConsolaDeSuperficie() ;
			res = numFilas ;
			if (numFilas == arg) { 
			
#if (FALSE)
            if (numFilasAnt >= numFilas)
            {
                copiarCPantalla((pantalla_t far *)&ptrPant->t[numFilasAnt-numFilas+1][0], ptrPant, numFilas) ;
//              copiarPantalla((pantalla_t far *)&ptrPant->t[cursorF-numFilas+1][0], ptrPant, numFilas) ;
//**/           borrarCPantalla((pantalla_t far *)&ptrPant->t[numFilas][0], numFilasAnt) ;
/**/            borrarCPantalla((pantalla_t far *)&ptrPant->t[numFilas][0], numFilasAnt-numFilas) ;
                cursorF = numFilas-1 ;
            }
#endif			
            for ( con = 0 ; con < maxConsolas ; con ++ )
            {
                if (con != consolaDeSuperficie)
                {
                    cursorFAux = descConsola[con].F ;
                    if (cursorFAux >= numFilas)
                    {
                        pantallaAux = (pantalla_t far *)&descConsola[con].pantalla ;
                        copiarCPantalla((pantalla_t far *)&pantallaAux->t[cursorFAux-numFilas+1][0], pantallaAux, numFilas) ;
//                      copiarPantalla((pantalla_t far *)&pantallaAux->t[cursorFAux-numFilas+1][0], pantallaAux, numFilas) ; */
//**/                   borrarCPantalla((pantalla_t far *)&pantallaAux->t[numFilas][0], numFilasAnt) ;
                        descConsola[con].F = numFilas-1 ;
                    }
                }
            }
			}
        }
		
    }

    asm pop ds
    return(res) ;
#endif
}

int far eliminarConsola ( pindx_t pindx )
{
    teclado_t far * teclado ;
    int i ;
    for ( i = 0 ; i < maxConsolas ; i++ )
    {
        teclado = (teclado_t far *)&descConsola[consolaDeSuperficie].teclado ;
        if estaPC2c(pindx, &teclado->bloqueados)
        {
            eliminarPC2c(pindx, &teclado->bloqueados) ;
            break ;
        }
    }
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

void far isr_consola ( void )
{

    word_t DS_Consola = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
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
    word_t numFilasAnt ;
    byte_t cursorFAux ;
    pantalla_t far * pantallaAux ;

    asm push ds
    asm mov ds,DS_Consola

    scanCodeAnt = scanCode ;
    asm in al,60h       /* ¿solo debe leerse una vez el registro de datos? */
    asm mov scanCode,al

    ptrVIOrg[nVIntConsola]() ;  /* llamamos a la antigua rti (pushf + int) */

    nuevaConsola = 0xFF ;
    numFilasAnt = numFilas ;

#if (FALSE)                               /* trazas por si hay que depurar */
    printCarBIOS('<') ;
    printHexBIOS(scanCodeAnt, 2) ;
    printHexBIOS(scanCode, 2) ;
    printCarBIOS('>') ;
#endif

    if (scanCodeAnt == CS_Alt) AltPulsada = TRUE ;
    else if (scanCodeAnt == (CS_Alt | 0x80)) AltPulsada = FALSE ;

    if (AltPulsada)
    {

        switch (scanCode)
        {

        case CS_ES_BSlash :
            nuevaConsola = 0 ;
            break ;

        case CS_F1     :
            ;
        case CS_F2     :
            ;
        case CS_F3     :
            ;
        case CS_F4     :
            ;
        case CS_F5     :
            ;
        case CS_F6     :
            ;
        case CS_F7     :
            ;
        case CS_F8     :
            ;
        case CS_F9     :
            ;
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
                    switch(numFilasAnt)
                    {
                    case 25 :
                        numFilas = 28 ;
                        break ;
                    case 28 :
                        numFilas = 50 ;
                        break ;
                    case 50 :
                        numFilas = 25 ;
                        break ;
                    default :
                        break ;
                    }
                else                                               /* Alt- */
                    switch(numFilasAnt)
                    {
                    case 25 :
                        numFilas = 50 ;
                        break ;
                    case 28 :
                        numFilas = 25 ;
                        break ;
                    case 50 :
                        numFilas = 28 ;
                        break ;
                    default :
                        break ;
                    }
                if (numFilas != numFilasAnt)
                {
                    if (cursorF >= numFilas)
                    {
                        copiarCPantalla((pantalla_t far *)&ptrPant->t[cursorF-numFilas+1][0], ptrPant, numFilas) ;
//                      copiarPantalla((pantalla_t far *)&ptrPant->t[cursorF-numFilas+1][0], ptrPant, numFilas) ;
//**/                   borrarCPantalla((pantalla_t far *)&ptrPant->t[numFilas][0], numFilasAnt) ;
                        cursorF = numFilas-1 ;
                    }
                    salvarConsolaDeSuperficie() ;
                    redimensionar(numFilas, 80) ;
                    numFilas = ptrBiosArea->VIDEO_lastrow + 1 ;
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
//                              copiarPantalla((pantalla_t far *)&pantallaAux->t[cursorFAux-numFilas+1][0], pantallaAux, numFilas) ; */
//**/                           borrarCPantalla((pantalla_t far *)&pantallaAux->t[numFilas][0], numFilasAnt) ;
                                descConsola[con].F = numFilas-1 ;
                            }
                        }
                    }
                }
                break ;
            default :
                ;
            }
        }
        w = leerTeclaListaBDA() ;                        /* /PP 2016-10-31 */
        asm pop ds ;                                     /* /PP 2016-10-31 */
        return ;                                         /* /PP 2016-10-31 */
    }

    if (scanCode & 0x80)
    {
        asm pop ds
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
            car = (char)0 ;
        extendido = (car == (char)0) ;

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
            if ((car == CR) && (modoAp & O_TEXT)) car = LF ;
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
                        (trama_t far *)pointer(*ptrSS_Proceso, *ptrSP_Proceso) ;
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
    asm pop ds
}

/* ----------------------------------------------------------------------- */
/*                     call back para el recurso TIMER                     */
/* ----------------------------------------------------------------------- */

byte_t congen ;

int pCon ( char car )
{
    printCarConsola(congen, car) ;
    return(1) ;
}

int printLnConsola ( byte_t con )
{
    congen = con ;
    return(printGenLn(pCon)) ;
}

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

int printDecConsola ( byte_t con, word_t num, word_t l )
{
    congen = con ;
    return(printGenDec(num, l, pCon)) ;
}

int printIntConsola ( byte_t con, int num, word_t l )
{
    congen = con ;
    return(printGenInt(num, l, pCon)) ;
}

int printLIntConsola ( byte_t con, long int num, word_t l )
{
    congen = con ;
    return(printGenLInt(num, l, pCon)) ;
}

int printLDecConsola ( byte_t con, dword_t num, word_t l )
{
    congen = con ;
    return(printGenLDec(num, l, pCon)) ;
}

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

int far cbForTimer ( void far * arg )                         /* call back */
{
    word_t DS_Consola = *((word_t far *)pointer(_CS, (word_t)segDatos)) ;
    byte_t F0 ;
    byte_t C0 ;
    byte_t congen0 ;
    byte_t visibilidadAnt ;
//trama_t far * tramaProceso ;

    asm push ds
    asm mov ds,DS_Consola  /* peligro SS (Sistema Operativo) != DS (Consola) */
    /* punteros a pila (parametros/variables locales) */
    /*
      tramaProceso = (trama_t far *)pointer(*ptrSS_Proceso, *ptrSP_Proceso) ;
    */
    F0 = cursorF ;
    C0 = cursorC ;
    congen0 = congen ;
    visibilidadAnt = setCursorVisibilidad(0) ;
    if (consolaDeSuperficie != 0)
    {
        goToXYConsola(consolaDeSuperficie, 0, 75) ;
        printStrConsola(consolaDeSuperficie, "     ") ;
        goToXYConsola(consolaDeSuperficie, 1, 75) ;
        printStrConsola(consolaDeSuperficie, "CON") ;
        printIntConsola(consolaDeSuperficie, consolaDeSuperficie, 1) ;
    }
    else
    {
        goToXYConsola(0, 0, 64) ;
        printStrConsola(0, "                ") ;
        goToXYConsola(0, 1, 64) ;
        printLDecConsola(0, ((argCbTimer_t far *)arg)->contTics, 11) ;
        printIntConsola(0, ((argCbTimer_t far *)arg)->contTicsRodaja, 4) ;
    }
    congen = congen0 ;
    cursorF = F0 ;
    cursorC = C0 ;
    goToXYHw(cursorF, cursorC) ;
    setCursorVisibilidad(visibilidadAnt) ;

    asm pop ds
    return(1) ;
}

/* ----------------------------------------------------------------------- */
/*                      seccion de programa de usuario                     */
/* ----------------------------------------------------------------------- */

void mostrarFormato ( void )
{
    escribirStr(
        "\n"
        "\n"
        " formato: CONSOLA [ [ -i | -q | -c | -l ] [ num ] | -N | -n | -u | -h ] \n"
    ) ;
}

void formato ( void )
{
    mostrarFormato() ;
    exit(-1) ;
}

void help ( void )
{
    mostrarFormato() ;
    escribirStr(
        "\n"
        " instala/desinstala el driver de la consola    \n"
        "\n"
        " opciones: (por defecto -i y num = 6)          \n"
        "\n"
        "      -i : instala el driver (usar &)          \n"
        "      -q : instala sin mensajes de salida (&)  \n"
        "      -c : cambia la consola actual a la num   \n"
        "      -l : establece en numero de lineas a num \n"
        "      -n : status = numero de consola actual   \n"
        "      -N : como -n y muestra un mensaje        \n"
        "     num : numero de consolas (o nueva c.)     \n"
        "      -u : desintala el driver                 \n"
        "      -h : muestra este help                   \n"
    ) ;
    exit(0) ;
}

descProceso_t descProceso [ maxProcesos ] ;

e2PFR_t e2PFR ;

c2c_t c2cPFR [ numColasPFR ] ;

#define maxCbConsola 0

descCcb_t descCcbConsola = { 0, 0, 0, maxCbConsola, NULL } ;

void establecerTablaDeConversion ( void )
{

    int anioBIOS ;

    tablaDeConversion = (char *)&tablaSP ;                           /* SP */

    anioBIOS = 10*(ptrFechaBios[6] - '0') + (ptrFechaBios[7] - '0') ;

    switch (anioBIOS)
    {
    case 99 : /* hayQemu   = TRUE ; */
        break ;                 /* "06/23/99" */
    case 11 : /* hayBochs  = TRUE ; */
        break ;                 /* "02/10/11" */            /* Bochs 2.6.7 */ 
    case 14 : /* hayBochs  = TRUE ; */
        break ;                 /* "12/26/14" */            /* Bochs 2.6.8 */ 
    case 17 : /* hayBochs  = TRUE ; */
        break ;                 /* "02/16/17" */            /* Bochs 2.6.9 */ 
    case 92 : /* hayDBox   = TRUE ;                          /* "01/01/92" */
        tablaDeConversion = (char *)&tablaNula ;                     /* US */
        break ;                 
    case 95 : /* hayNTVDM  = TRUE ; */
        break ;                 /* "07/03/95" */
		
    case 12 : /* hayFake86 = TRUE ; */
        break ;                 /* "05/02/12" */
//  case 99 : /* hayVDos   = TRUE ; */
//      break ;                 /* "01/01/99" */
    case 06 : /* hayMsdos  = TRUE ; */                           /* Takeda */
        tablaDeConversion = (char *)&tablaNula ;                     /* US */
    default  :
        tablaDeConversion = (char *)&tablaNula ;                     /* US */
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

	if (*((word_t far *)0xF000FFFB) == 0x3630)    /* anio fecha BIOS msdos */ /* Takeda */
        inicBiosCrt(24, 80, TRUE) ;           /* se fuerza redimensionable */
    else 
		inicBiosCrt(50, 80, FALSE) ;       /* no se fuerza redimensionable */

    establecerTablaDeConversion() ;                       /* teclado US/SP */

    maxConsolas = numConsolas + 1 ;

    descConsola = (descConsola_t far *)
                  pointer(
                      ll_buscarBloque(((maxConsolas*sizeof(descConsola_t))+15)/16),  /* GM */
                      0x0000
                  ) ;
/*
    printStrBIOS("\n descConsola = ") ;
    printPtrBIOS((pointer_t)descConsola) ;
    printStrBIOS("\n numFilas = ") ;
    printIntBIOS(numFilas, 1) ;
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
        borrarPantalla((pantalla_t far *)&descConsola[con].pantalla, numFilas) ;
        descConsola[con].F = 0 ;
        descConsola[con].C = 0 ;
    }

    for ( pindx = 0 ; pindx < maxProcesos ; pindx++ )
    {
        nbytesProceso[pindx] = 0 ;                /* nbytes esperando leer */
        dirProceso[pindx] = (pointer_t)0x00000000 ;
    }

    dR.tipo = rDCaracteres ;
    copiarStr("CONSOLA", dR.nombre) ;
    dR.ccb = (ccb_t)&descCcbConsola ;
    dR.ccb->arg = NULL ;
    dR.pindx = getpindx() ;
    dR.numVI = 1 ;
    dR.nVInt[0] = nVIntConsola ;
    dR.irq[0] = IRQ_TECLADO ;
    dR.isr[0] = (isr_t)pointer(_CS, (word_t)isr_consola) ;

    dR.open      = (open_t)pointer(_CS, (word_t)openConsola) ;
    dR.release   = (release_t)pointer(_CS, (word_t)releaseConsola) ;
    dR.read      = (read_t)pointer(_CS, (word_t)readConsola) ;
    dR.aio_read  = (aio_read_t)pointer(_CS, (word_t)aio_readConsola) ;
    dR.write     = (write_t)pointer(_CS, (word_t)writeConsola) ;
    dR.aio_write = (aio_write_t)pointer(_CS, (word_t)aio_writeConsola) ;
    dR.lseek     = (lseek_t)pointer(_CS, (word_t)lseekConsola) ;
    dR.fcntl     = (fcntl_t)pointer(_CS, (word_t)fcntlConsola) ;
    dR.ioctl     = (ioctl_t)pointer(_CS, (word_t)ioctlConsola) ;

    dR.eliminar  = (eliminar_t)pointer(_CS, (word_t)eliminarConsola) ;

    rec_consola = crearRecurso(&dR) ;

    if (rec_consola >= 0)
    {

        copiarStr("CON ", nomFich) ;
        for ( con = 0 ; con < maxConsolas ; con++ )
        {
            nomFich[3] = '0' + con ;
            dfs = crearFichero(nomFich, rec_consola, con, fedCaracteres) ;
            if (dfs < 0)
            {
                switch(dfs)
                {
                case -1 :
                    escribirStr(" tipo de fichero erroneo") ;
                    break ;
                case -2 :
                    escribirStr(" numero de recurso erroneo") ;
                    break ;
                case -3 :
                    escribirStr(" CON") ;
                    escribirDec(con, 1) ;
                    escribirStr(" nombre de fichero ya existente") ;
                    break ;
                case -4 :
                    escribirStr(" no hay descriptores de fichero libres") ;
                    break ;
                default :
                    escribirStr(" no se ha podido crear el fichero CON") ;
                    escribirDec(con, 1) ;
                }
                destruirRecurso("CONSOLA") ;
            }
        }

        if ((dfTimer = open("TIMER", O_RDONLY)) >= 0)
        {
            close(dfTimer) ;
            res = encolarCcbRecurso(
                      (callBack_t)pointer(_CS, (word_t)cbForTimer),
                      (char far *)pointer(_DS, (word_t)&"TIMER")
                  ) ;
            if (res != 0)
            {
                escribirStr("\n\n no se ha podido encolar el callback en el TIMER \n") ;
            }
        }
        else
        {
            escribirStr("\n\n no se ha podido encolar el callback en el TIMER \n") ;
        }

        if (conMensajes)
            escribirStr("\n\n recurso CONSOLA instalado (ficheros: CON0 ...) \n") ;
        return(0) ;
    }
    switch(rec_consola)
    {
    case -1 :
        escribirStr("\n\n tipo de recurso erroneo \n") ;
        break ;
    case -2 :
        escribirStr("\n\n demasiados vectores de interrupcion \n") ;
        break ;
    case -3 :
        escribirStr("\n\n CONSOLA nombre de recurso ya existente \n") ;
        break ;
    case -4 :
        escribirStr("\n\n no hay descriptores de recurso libres \n") ;
        break ;
    case -5 :
        escribirStr("\n\n numero de vector de interrupcion ya usado \n") ;
        break ;
    default :
        escribirStr("\n\n no ha podido crearse el recurso CONSOLA \n") ;
    }
    return(-1) ;
}

int desintegrarConsola ( void )
{
    int res ;
    res = eliminarCcbRecurso(
              (callBack_t)pointer(_CS, (word_t)cbForTimer),
              (char far *)pointer(_DS, (word_t)&"TIMER")
          ) ;
    if (res != 0)
        escribirStr("\n\n no pudo eliminarse el callback del TIMER \n") ;
    else if (ll_devolverBloque(seg((pointer_t)descConsola),          /* GM */
                               ((maxConsolas*sizeof(descConsola_t))+15)/16))
        return(-1) ;
    /*  finBiosCrt() ; */
    return(res) ;
}

int comprobarAmpersand ( void )
{
    int i ;
    obtenInfoPS((descProceso_t far *)&descProceso, (e2PFR_t far *)&e2PFR, (c2c_t far *)&c2cPFR) ;

    for ( i = 0 ; i < tamComando ; i++ )
        comando[0][i] = descProceso[getpindx()].comando[i] ;

    inicScanner() ;
    while (car != (char)0) obtenCar() ;
    if (comando[0][pos] != '&')   /* truco: crearProceso deja ahi &, <, >, | */
    {
        escribirStr("\n\n este driver debe ejecutarse con & \n") ;
        return(-1) ;
    }
    return(0) ;
}

int instalarConsola ( byte_t numConsolas, bool_t conMensajes )
{
    int res ;
    res = comprobarAmpersand() ;
    if (res != 0) return(res) ;
    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */
    *((word_t far *)pointer(_CS, (word_t)segDatos)) = _DS ;   /* guardo DS */
    res = integrarConsola(numConsolas, conMensajes) ;
    if (res != 0) return(res) ;
    esperarDesinstalacion() ;                        /* bloquea el proceso */
    res = desintegrarConsola() ;
    return(res) ;
}

void main ( int argc, char * argv [ ] )
{
    int res ;
    int dfCon ;
    if (argc > 3) formato() ;
    else if (argc == 1) exit(instalarConsola(numConsolasPorDefecto, TRUE)) ;
    else if (argc == 2)
    {
        if (argv[1][0] != '-') formato() ;
        switch (mayuscula(argv[1][1]))
        {
        case 'H' :
            help() ;
        case 'I' :
            exit(instalarConsola(numConsolasPorDefecto, TRUE)) ;
        case 'Q' :
            exit(instalarConsola(numConsolasPorDefecto, FALSE)) ;
        case 'N' :
            if ((dfCon = open("CON0", O_RDONLY)) < 0)
            {
                escribirStr(" no se puede abrir CON0") ;
                exit(-1) ;
            }
            else
            {
                res = ioctl(dfCon, 0x0002, 0x0000) ;
                if (argv[1][1] == 'N')
                {
                    if (res < 0) escribirStr(" error al consultar la consola actual ") ;
                    else
                    {
                        escribirStr(" la consola actual es CON") ;
                        escribirDec(res, 1) ;
                    }
                }
                exit(res) ;
            }
        case 'U' :
            res = destruirRecurso("CONSOLA") ;
            switch (res)
            {
            case  0 :
                escribirStr(" recurso CONSOLA desinstalado") ;
                break ;
            case -1 :
                escribirStr(" recurso CONSOLA no existe") ;
                break ;
            case -2 :
                escribirStr(" recurso CONSOLA en uso") ;
                break ;
            case -3 :
                escribirStr(" fichero CONSOLA no puede destruirse") ;
                break ;
            default :
                escribirStr(" CONSOLA no ha podido desinstalarse") ;
            }
            exit(res) ;
        default :
            ;
        }
    }
    if ((argc == 2) ||
            ((argc == 3) &&
             (iguales(argv[1], "-i") || iguales(argv[1], "-I") ||
              iguales(argv[1], "-q") || iguales(argv[1], "-Q"))
            )
       )
    {
        copiarStr(argv[argc-1], comando[0]) ;
        inicScanner() ;
        obtenSimb() ;
        if (simb == s_numero)
        {
            if (num == 0)
            {
                escribirStr("\n\n el numero de consolas debe ser > 0 \n") ;
                exit(-1) ;
            }
            else exit(instalarConsola(num, (mayuscula(argv[1][1]) == 'Q'))) ;
        }
    }
    else if (argc == 3)
        if (iguales(argv[1], "-c") || iguales(argv[1], "-C"))
        {
            copiarStr(argv[argc-1], comando[0]) ;
            inicScanner() ;
            obtenSimb() ;
            if (simb == s_numero)
            {
                if ((dfCon = open("CON0", O_RDONLY)) < 0)
                {
                    escribirStr(" no puede abrirse CON0") ;
                    exit(-1) ;
                }
                if (ioctl(dfCon, 0x0001, num) != 0)
                {
                    escribirStr(" numero de consola erroneo (") ;
                    escribirDec(num, 1) ;
                    escribirCar(')') ;
                    exit(-1) ;
                }
                exit(0) ;
            }
        }
        else if (iguales(argv[1], "-l") || iguales(argv[1], "-L"))
        {
            copiarStr(argv[argc-1], comando[0]) ;
            inicScanner() ;
            obtenSimb() ;
            if (simb == s_numero)
            {
                if ((num != 25) && (num != 28) && (num != 50)) {
					escribirStr(" el numero de lineas debe ser 25, 28 o 50") ;
                    exit(-1) ;
				}	
				if ((dfCon = open("CON0", O_RDONLY)) < 0)
                {
                    escribirStr(" no abrirse CON0") ;
                    exit(-1) ;
                }
                if (ioctl(dfCon, 0x0003, num) != 0)
                {
                    escribirStr(" numero de consola erroneo (") ;
                    escribirDec(num, 1) ;
                    escribirCar(')') ;
                    exit(-1) ;
                }
                exit(0) ;
            }
        }		
    formato() ;
}


