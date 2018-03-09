/* ----------------------------------------------------------------------- */
/*                                 raton.c                                 */
/* ----------------------------------------------------------------------- */
/*                              recurso raton                              */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\ajustusr.h>          /* save_DS0, setraw_DS, restore_DS */
#include <so1pub.h\comundrv.h>  /* ptrIndProcesoActual, comprobarAmpersand */
#include <so1pub.h\ll_s_so1.h>    /* biblioteca de llamadas al sistema SO1 */
#include <so1pub.h\stdio.h>                                      /* printf */
#include <so1pub.h\escribir.h>                                 /* printCar */ 

#include <so1pub.h\tipos.h>          /* byte_t, word_t, pointer_t, rindx_t */ /* TRUE, FALSE */
#include <so1pub.h\c2c.h>                          /* c2c_t, dobleEnlace_t */
#include <so1pub.h\ctype.h>                                     /* toupper */
#include <so1pub.h\strings.h>                  /* strcpy, strcmpu, strncmp */
#include <so1pub.h\scanner.h>
#include <so1pub.h\biosdata.h>               /* ptrBiosArea, VIDEO_lastrow */
#include <so1pub.h\bios_0.h>
#include <so1pub.h\memory.h>                                     /* memcpy */
#include <so1pub.h\pantalla.h>                    /* maxFilas, maxColumnas */
#include <so1pub.h\memvideo.h>                             /* printCarBIOS */
#include <so1pub.h\def_tecl.h>                                /* teclado_t */
#include <so1pub.h\pic.h>                                        /* ptrTVI */
#include <so1pub.h\def_timer.h>                            /* argCbTimer_t */
#include <so1pub.h\printgen.h>
//#include <so1pub.h\msdos.h>                              /* hayWindowsNT */
#include <so1pub.h\def_rat.h>                       /* maxX, estadoRaton_t */
#include <so1pub.h\bios_rat.h>

#include <so1.h\interrup.h>

//#define REDUCIR_DRIVER TRUE     /* TRUE ==> reducimos la memoria utilizada */
#define REDUCIR_DRIVER FALSE  

#pragma option -w-use /* (comundrv.h) warning: 'x' declared but never used */

rindx_t rec_raton = 0 ;             /* inicializacion provisional --> DATA */ /* se necesita en readConsola */

/* ----------------------------------------------------------------------- */
/*                   seccion de implementacion del driver                  */
/* ----------------------------------------------------------------------- */

#define nVIntRaton 0x74          /* obtenido experimentalmente MSD + DEBUG */

c2c_t bloqueadosRaton = { 0, 0, 0, NULL } ;                                   /* DATA */

dobleEnlace_t e2BloqueadosRaton [ maxProcesos + 1 ] = { { 0, 0 } } ;          /* DATA */ 

word_t nbytesProceso [ maxProcesos ] = { 0 } ;   /* tabla bytes pendientes */ /* DATA */

pointer_t dirProceso [ maxProcesos ] = { NULL } ;   /* direcciones destino */ /* DATA */

/* ----------------------------------------------------------------------- */
/*          variables globales que controlan el estado del raton           */
/* ----------------------------------------------------------------------- */

int inB = 0 ;                              /* indice de entrada al bufer B */

byte_t B [ 3 ] = { 0x00, 0x00, 0x00 } ;

/* no cambiar el orden en las siguientes lineas (ver leerEstadoRaton)      */

estadoRaton_t er =
{
    /* S0            */ 0x00,                         /* byte estado 8042  */
    /* B0            */ 0x00,                         /* Byte 0 = 00SS1MRL */
    /* B1            */ 0x00,                         /* Byte 1 = X        */
    /* B2            */ 0x00,                         /* Byte 2 = Y        */
    /* W0            */ 0x0000,
    /* intX          */ 0,
    /* intY          */ 0,
    /* X             */ 0,
    /* Y             */ 0,
    /* F             */ 0,
    /* C             */ 0,
    /* botonIz       */ (int)FALSE,
    /* botonDe       */ (int)FALSE,
    /* botonMe       */ (int)FALSE,
    /* botonIzAnt    */ (int)FALSE,
    /* botonDeAnt    */ (int)FALSE,
    /* botonMeAnt    */ (int)FALSE,
    /* visible       */ FALSE,
    /* forzarLectura */ FALSE
} ;

int XAux = 0 ;
int YAux = 0 ;
int FAntCursor = 0 ;                                               /* DATA */
int CAntCursor = 0 ;                                               /* DATA */

byte_t atrAux = 0 ;                                                /* DATA */

int cursorRatonActivo = TRUE ;

int moviendo = FALSE ;
int redimensionando = FALSE ;

int primerClick = TRUE ;

int contTics2Click = 0 ;

bool_t huboInt = TRUE ;            /* hubo int. del raton sin leer todavia */

int primeraInt = TRUE ;         /* la siguiente interrupcion es la primera */

void inicEstadoRaton ( void )
{
    er.S0 = 0x00 ;
    er.B0 = 0x00 ;
    er.B1 = 0x00 ;
    er.B2 = 0x00 ;
    er.W0 = 0x0000 ;
    er.X = maxX/2 ;
    /* er.Y = maxY/2 ; */
    er.Y = (ptrBiosArea->VIDEO_lastrow + 1)/2 ;
    er.F = er.Y >> 3 ;                                          /* F = Y/8 */
    er.C = er.X >> 3 ;                                          /* C = X/8 */
    er.botonIz = FALSE ;
    er.botonDe = FALSE ;
    er.botonMe = FALSE ;

    primeraInt = TRUE ;
    cursorRatonActivo = TRUE ;
    primerClick = TRUE ;
    contTics2Click = 0 ;
}

bool_t leerEstadoRaton ( estadoRaton_t far * estado )
{
    memcpy(estado, &er, sizeof(estadoRaton_t)) ;
    if (huboInt)
    {
        huboInt = FALSE ;
        return(TRUE) ;
    }
    else
        return(FALSE) ;
}

/* ----------------------------------------------------------------------- */

#pragma warn -par

int far openRaton ( int dfs, modoAp_t modo )
{
    return(0) ;
}

int far releaseRaton ( int dfs )
{
    return(0) ;
}

int far readRaton ( int dfs, pointer_t dir, word_t nbytes )
{
    word_t nbARetornar ;
    pindx_t indProcesoActual ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
 	setraw_DS() ;           /* establece el DS correspondiente al programa */

    indProcesoActual = *ptrIndProcesoActual ;
    if (nbytes <= sizeof(estadoRaton_t))
        nbARetornar = nbytes ;
    else
        nbARetornar = sizeof(estadoRaton_t) ;
    memcpy(dir, (pointer_t)&er, nbARetornar) ;

    nbytesProceso[indProcesoActual] = nbARetornar ; /* nbytes esperando leer */
    dirProceso[indProcesoActual] = dir ;
    encolarPC2c(indProcesoActual, &bloqueadosRaton) ;
    bloquearProcesoActual(rec_raton) ;

	restore_DS0() ;                               /* restaura el DS de SO1 */
    return(-1) ;
}

int far aio_readRaton ( int dfs, pointer_t dir, word_t nbytes )
{
    word_t nbARetornar ;

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
 	setraw_DS() ;           /* establece el DS correspondiente al programa */

    if (nbytes <= sizeof(estadoRaton_t))
        nbARetornar = nbytes ;
    else
        nbARetornar = sizeof(estadoRaton_t) ;
    memcpy(dir, &er, nbARetornar) ;

	restore_DS0() ;                               /* restaura el DS de SO1 */
    return(nbARetornar) ;
}

int far writeRaton ( int dfs, pointer_t dir, word_t nbytes )
{
    return(0) ;
}

int far aio_writeRaton ( int dfs, pointer_t dir, word_t nbytes )
{
    return(0) ;
}

long int far lseekRaton ( int dfs, long int pos, word_t whence )
{
    return((long int)-1) ;
}

int far fcntlRaton ( int dfs, word_t cmd, word_t arg )
{
    return(0) ;
}

int far ioctlRaton ( int dfs, word_t request, word_t arg )
{
    return(0) ;
}

int far eliminarRaton ( pindx_t pindx )
{
    save_DS0() ;                            /* guarda el DS anterior (SO1) */
 	setraw_DS() ;           /* establece el DS correspondiente al programa */
	
    if (estaPC2c(pindx, &bloqueadosRaton))
        eliminarPC2c(pindx, &bloqueadosRaton) ;

	restore_DS0() ;                               /* restaura el DS de SO1 */
    return(0) ;
}

#pragma warn +par

/* ----------------------------------------------------------------------- */
/*                  rutina de servicio de la interrupcion                  */
/* ----------------------------------------------------------------------- */

void procesarRaton ( word_t W0, int X, int Y ) ;                /* forward */

void procesarColaBloqueadosRaton ( void ) ;                     /* forward */

void tratarRaton ( void ) ;                                     /* forward */

void actualizarCursor ( void ) ;                                /* forward */

void far isr_raton ( void )
{
    save_DS0() ;                            /* guarda el DS anterior (SO1) */
 	set_DS() ;              /* establece el DS correspondiente al programa */

    /*  plot('m', 0, contadorTimer0()) ; */
    tratarRaton() ;
    /*  plot('e', 0, contadorTimer0()) ; */
    eoi_pic2() ;

	restore_DS0() ;                               /* restaura el DS de SO1 */
}

void far isr_raton_dosbox ( void )
{
    save_DS0() ;                            /* guarda el DS anterior (SO1) */
 	set_DS() ;              /* establece el DS correspondiente al programa */

    ptrVIOrg[nVIntRaton]() ;    /* llamamos a la antigua rti (pushf + int) */
    /*  plot('m', 0, contadorTimer0()) ; */
    procesarRaton((word_t)er.B0, er.X, er.Y) ;
    /*  plot('e', 0, contadorTimer0()) ; */
    /* eoi_pic2() ; */

	restore_DS0() ;                               /* restaura el DS de SO1 */
}

word_t rH_CS = 0x000 ;                                             /* DATA */
word_t rH_IP = 0x000 ;                                             /* DATA */
word_t rH_AX = 0x000 ;                                             /* DATA */
word_t rH_BX = 0x000 ;                                             /* DATA */
word_t rH_CX = 0x000 ;                                             /* DATA */
word_t rH_DX = 0x000 ;                                             /* DATA */
word_t rH_SI = 0x000 ;                                             /* DATA */
word_t rH_DI = 0x000 ;                                             /* DATA */

void far isr_raton_BIOS ( void )
{
    save_DS0() ;                            /* guarda el DS anterior (SO1) */
 	set_DS() ;              /* establece el DS correspondiente al programa */

    /*
    printStrBIOS("\n isr_raton_bios") ;
    printStrBIOS("\n isr_raton_bios") ;
    */

    /*  plot('m', 0, contadorTimer0()) ; */
    procesarRaton(rH_BX, rH_CX, rH_DX)  ;
    /*  plot('e', 0, contadorTimer0()) ; */
    /* eoi_pic2() ; */

	restore_DS0() ;                               /* restaura el DS de SO1 */
}

rti_t rti_nVIntRaton = (rti_t)NULL ;                 /* ver so1\interrup.c */
word_t segment_rti_nVIntRaton = 0x0000 ;
word_t offset_rti_nVIntRaton = 0x0000 ;

word_t far * ptrWordAux = NULL ;                   /* para depurar la pila */

void far rti_raton_BIOS ( void )
{
    asm {		
		sub sp,10 ;                             /* Flags, CS, IP (Proceso) */
//                                               /* segment_rti_nVIntRaton */
//                                                /* offset_rti_nVIntRaton */
    }
    save_DS0() ;                            /* guarda el DS anterior (SO1) */
 	set_DS() ;              /* establece el DS correspondiente al programa */

    asm {				  
        push ax ;
        push bp ;

        mov bp,sp ;
        mov ax,rH_CS ;                                     /* CS (Proceso) */  
        mov ss:[bp+12],ax ;   
        mov ax,rH_IP ;
        mov ss:[bp+10],ax ;                                /* IP (Proceso) */
        mov ax,segment_rti_nVIntRaton ;
        mov ss:[bp+8],ax ;                       /* segment_rti_nVIntRaton */      
        mov ax,offset_rti_nVIntRaton ;
        mov ss:[bp+6],ax ;                        /* offset_rti_nVIntRaton */
	}
/*
        printStrBIOS("\n isr_raton_BIOS") ;
        ptrWordAux = (word_t far  *)MK_FP(_SS,_SP) ;
        printStrBIOS("\n ") ;
        printHexBIOS(ptrWordAux[0], 4) ;
        printStrBIOS("\n ") ;
        printHexBIOS(ptrWordAux[1], 4) ;
        printStrBIOS("\n ") ;
        printHexBIOS(ptrWordAux[2], 4) ;
        printStrBIOS("\n ") ;
        printHexBIOS(ptrWordAux[3], 4) ;
        printStrBIOS("\n ") ;
        printHexBIOS(ptrWordAux[4], 4) ;
        printStrBIOS("\n ") ;
        printHexBIOS(ptrWordAux[5], 4) ;
        printStrBIOS("\n ") ;
        printHexBIOS(ptrWordAux[6], 4) ;
        printStrBIOS("\n ") ;
        printHexBIOS(ptrWordAux[7], 4) ;
        printStrBIOS("\n ") ;
        printHexBIOS(ptrWordAux[8], 4) ;
        printStrBIOS("\n ") ;
*/
    asm {
        pop bp ;
        pop ax ;
        /* retorno lejano a rti_nVIntRaton */
	}
	restore_DS0() ;                               /* restaura el DS de SO1 */	
}

void far ratonHandler ( void )
{

    /* se llama a esta funcion tras la interrupcion y mediante un CALL FAR */
    /* llamada a procedimiento lejano. Por ese motivo la pila contiene lo  */
    /* siguiente: Flags, CS, IP del proceso y direccion de retorno lejana. */

    save_DS0() ;                            /* guarda el DS anterior (SO1) */
 	set_DS() ;              /* establece el DS correspondiente al programa */
    /*
    printStrBIOS("\n ratonHandler principio") ;
      ptrWordAux = (word_t far  *)MK_FP(_SS,_SP) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[0], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[1], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[2], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[3], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[4], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[5], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[6], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[7], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[8], 4) ;
    printStrBIOS("\n ") ;
    */
    asm mov rH_AX,ax                       /* evento ocurrido, ver techelp */
    asm mov rH_BX,bx                                            /* botones */
    asm mov rH_CX,cx                                                  /* X */
    asm mov rH_DX,dx                                                  /* Y */
    /* asm mov rH_SI,si */                                         /* incX */
    /* asm mov rH_DI,di */                                         /* incY */

    asm push ax
    asm push bp
    asm mov bp,sp
    asm mov ax,ss:[bp+12]
    asm mov rH_CS,ax                                     /* CS del proceso */
    asm mov ax,ss:[bp+10]                                /* IP del proceso */
    asm mov rH_IP,ax

    asm mov ss:[bp+12],cs                                            /* CS */
    asm mov ax,OFFSET rti_raton_BIOS                     /* rti_raton_BIOS */
    asm mov ss:[bp+10],ax
    asm pop bp
    asm pop ax
    /*
    printStrBIOS("\n ratonHandler fin") ;
      ptrWordAux = (word_t far  *)MK_FP(_SS,_SP) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[0], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[1], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[2], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[3], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[4], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[5], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[6], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[7], 4) ;
    printStrBIOS("\n ") ;
    printHexBIOS(ptrWordAux[8], 4) ;
    printStrBIOS("\n ") ;
    */
	restore_DS0() ;                               /* restaura el DS de SO1 */	
}                            /* de el a isr_raton_bios y de ahi al proceso */

#define TECLADO_ESTADO  0x64
#define TECLADO_COMANDO 0x64
#define TECLADO_DATO    0x60

#define RATON_ESTADO    TECLADO_ESTADO
#define RATON_COMANDO   TECLADO_COMANDO
#define RATON_DATO      TECLADO_DATO

void tratarRaton ( void )
{

    static byte_t byteEstado = 0 ;                                 /* DATA */
    static byte_t byteDato = 0 ;                                   /* DATA */
    static int incX = 0, incY = 0 ;                                /* DATA */

    asm in al,64h
    asm mov byteEstado,al
    if ((byteEstado & 0x21) == 0x21)      /* hay byte procedente del raton */
    {
        er.S0 = byteEstado ;
        asm in al,60h
        asm mov byteDato,al

        if ((inB != 0) || (((byteDato^0x08) & 0xC8) == 0))
        {
            B[inB] = byteDato ;
            inB = (inB + 1) % 3 ;

            if (inB == 0)
            {

                er.B0 = B[0] ;
                er.B1 = B[1] ;
                if (er.B0 & 0x10)
                {
                    asm mov al, byte ptr er.B1
                    asm mov ah,0ffh
                    asm mov incX,ax
                    er.incX = incX ;
                }
                else er.incX = er.B1 ;
                XAux = er.X + er.incX ;
                if ((0 <= XAux) && (XAux < maxX))            /* 8*80 = 640 */
                    er.X = XAux ;

                er.B2 = B[2] ;
                if (er.B0 & 0x20)
                {
                    asm mov al, byte ptr er.B2
                    asm mov ah,0ffh
                    asm mov incY,ax
                    er.incY = incY ;
                }
                else er.incY = er.B2 ;
                er.incY = -er.incY ;                                /* ojo */
                YAux = er.Y + er.incY ;
                if ((0 <= YAux) && (YAux < maxY))            /* 8*25 = 200 */
                    er.Y = YAux ;

                procesarRaton((word_t)er.B0, er.X, er.Y) ;

            }
        }
    }
}

void procesarRaton ( word_t parW0, int parX, int parY )
{

//  static int posWF ;
//  static int posWC ;
//  static int FReal ;
//  static int CReal ;

    er.W0 = parW0 ;
    er.X = parX ;
    er.Y = parY ;

    er.F = er.Y >> 3 ;                                          /* F = Y/8 */
    er.C = er.X >> 3 ;                                          /* C = X/8 */

    er.B0 = (byte_t)(er.W0 & 0x00FF) ;

    huboInt = TRUE ;             /* obtenidos nuevos valores para el raton */

    er.botonIzAnt = er.botonIz ;
    er.botonDeAnt = er.botonDe ;
    er.botonMeAnt = er.botonMe ;

    er.botonIz = (er.W0 & 0x0001) ;
    er.botonDe = ((er.W0 & 0x0002) >> 1) ;
    er.botonMe = ((er.W0 & 0x0004) >> 2) ;

    procesarColaBloqueadosRaton() ;

    if (cursorRatonActivo)
    {
        if (er.botonIz)                      /* boton izquierdo presionado */
        {
#if (FALSE)
            if (moviendo || redimensionando)
            {
                if (moviendo)                                  /* moviendo */
                {
                    posWF = win->esqSupIzF ;
                    posWC = win->esqSupIzC ;
                    moverWin(win, er.F-FAntCursor, er.C-CAntCursor,
                             er.F-FAntCursor, er.C-CAntCursor) ;
                    FReal = FAntCursor + (win->esqSupIzF - posWF) ; /* F Real */
                    CReal = CAntCursor + (win->esqSupIzC - posWC) ; /* C Real */
                }
                else                                    /* redimensionando */
                {
                    posWF = win->esqInfDeF ;
                    posWC = win->esqInfDeC ;
                    moverWin(win, 0, 0, er.F-FAntCursor, er.C-CAntCursor) ;
                    FReal = FAntCursor + (win->esqInfDeF - posWF) ; /* F Real */
                    CReal = CAntCursor + (win->esqInfDeC - posWC) ; /* C Real */
                }
                if ((FReal != er.F) || (CReal != er.C))
                {
                    er.Y = (FReal << 3) + 3 ;                     /* ajuste Y */
                    er.X = (CReal << 3) + 3 ;                     /* ajuste X */
                    if (tipoRaton == msdos)
                        situarCursorRatonBIOS(er.X, er.Y) ;
                }
                er.F = FReal ;
                er.C = CReal ;
                actualizarCursor() ;
            }
            else if ((nwin = winVisible[er.F][er.C]) != 0)
            {
                if (!er.botonIzAnt)    /* primera pulsacion del boton izdo */
                {
                    win = winNum[nwin] ;
                    pindx = win->pindx ;

                    if (er.F == win->esqSupIzF-1)        /* marco superior */
                    {
                        if (er.C == win->esqInfDeC-1) botonCerrarVentana() ;       /* [X] */
                        else if (er.C == win->esqInfDeC-4) maxMinWin(win, FALSE) ; /* [f] */
                        else if ((er.F == win->esqSupIzF-1) &&
                                 (win->esqSupIzC <= er.C) &&
                                 (er.C <= win->esqSupIzC+2+2*(win->pindx!=0)))
                            maxMinWin(win, TRUE) ;                                 /* max */
                        else
                            tratarClickMarco() ;
                    }
                    else if ((er.F == win->esqInfDeF+1) &&
                             (er.C == win->esqInfDeC+1))  /* redimensionar */
                    {
                        focalizarWin(win) ;
                        redimensionando = TRUE ;
                        actualizarCursor() ;
                    }
                    else if ((er.F == win->esqInfDeF+1) ||
                             (er.C == win->esqSupIzC-1) ||
                             (er.C == win->esqInfDeC+1))     /* otro marco */
                        tratarClickMarco() ;
                    else focalizarWin(win) ;             /* punto interior */
                }
                else actualizarCursor() ;
            }
#endif
        }
        else                              /* boton izquierdo no presionado */
        {
            if (moviendo)
                moviendo = FALSE ;
            else if (redimensionando)
                redimensionando = FALSE ;
            actualizarCursor() ;
        }
    }
}

void procesarColaBloqueadosRaton ( void )
{

    pindx_t indProcesoActual ;
    pindx_t pindx ;

    if (bloqueadosRaton.numElem > 0)
    {
        pindx = bloqueadosRaton.primero ;
        indProcesoActual = *ptrIndProcesoActual ;
        if (indProcesoActual > -1)                          /* nivel 1 o 2 */
        {
            ptrDescProceso[indProcesoActual].trama =
                (trama_t far *)MK_FP(*ptrSS_Proceso, *ptrSP_Proceso) ;
            ptrDescProceso[indProcesoActual].estado = preparado ;
            encolarPC2c(indProcesoActual, (ptrC2c_t)&(ptrC2cPFR[PPreparados])) ;
            *ptrIndProcesoActual = -1 ;
            *ptrActivarAlEpilogo = TRUE ;
        }

        do
        {
            memcpy(dirProceso[pindx], &er, nbytesProceso[pindx]) ;

#if (FALSE)

            switch (descProceso[pindx].trama->DX)
            {
            case 0 :
                if (descProceso[pindx].win != descProceso[indProcesoDeSuperficie].win) break ;
            case 1 :
                if (winVisible[estado->F][estado->C] != descProceso[pindx].win->num) break ;
            case 2 :

                switch (descProceso[pindx].esperandoPor)
                {
                case rec_tecladoRaton :
                    eliminarPC2c(pindx, &descProceso[pindx].teclado->bloqueados) ;
                    descProceso[pindx].trama->AL = (char)0 ;
                    break ;
                case rec_tecladoRawRaton :
                    eliminarPC2c(pindx, &descProceso[pindx].teclado->bloqueadosRaw) ;
                    descProceso[pindx].trama->AX = 0x0000 ;
                    descProceso[pindx].trama->BX = 0x0000 ;           /* modificamos BX */
                    break ;
                }
#endif
                ptrDescProceso[pindx].estado = preparado ;
                encolarPC2c(pindx, (ptrC2c_t)&ptrC2cPFR[PUrgentes]) ;
#if (FALSE)

            }

#endif

            desencolarPC2c(&bloqueadosRaton) ;
            pindx = bloqueadosRaton.primero ;
        }
        while (bloqueadosRaton.numElem > 0) ;
    }
}

#define ptrPant ((pantalla_t far *)MK_FP(0xB800, 0x0000))

#define rotarDerecha(x, n)                                                   \
  asm mov al,x ;                                                             \
  asm ror al,n ;                           /* rotacion a la derecha n bits */\
  asm mov x,al ;

void actualizarCursor ( void )
{
    if (cursorRatonActivo)               /* actualizar el cursor del raton */
    {
        if (primeraInt) primeraInt = FALSE ;
        else if ((er.F != FAntCursor) || (er.C != CAntCursor))
        {

            atrAux = ptrPant->t[FAntCursor][CAntCursor].atr ;
            /* atrAux = (atrAux << 4) | (atrAux >> 4) ; */
            if (atrAux != atrNormal)
            {
                rotarDerecha(atrAux, 4) ;
                ptrPant->t[FAntCursor][CAntCursor].atr = atrAux ;
            }
            atrAux = ptrPant->t[er.F][er.C].atr ;
            /* atrAux = (atrAux << 4) | (atrAux >> 4) ; */
            rotarDerecha(atrAux, 4) ;
            ptrPant->t[er.F][er.C].atr = atrAux ;
            FAntCursor = er.F ;
            CAntCursor = er.C ;

        }
    }
}

/* ----------------------------------------------------------------------- */
/*                     handler para el raton (int 15h)                     */
/* ----------------------------------------------------------------------- */

void far handlerRatonNulo ( void )
{
}

void far handlerRaton ( dword_t y, word_t x, word_t s )
{

    static bool_t priVez = TRUE ; /* Lo necesito para ignorar los incrementos .. */
    static int incX = 0, incY = 0 ; /* .. X,Y iniciales que son basura */
    int X1 ;
    int Y1 ;
    int numFilasAct ;
	int maxYAct ;

    asm cli
//  asm mov ds,word ptr cs:[segDatos]
      asm push ax ;
	  asm push bx ;
      asm mov ax,cs:[3] ;
	  asm mov bx,cs ;
      asm add ax,bx
	  asm mov ds,ax ;
	  asm pop bx
	  asm pop ax       

    /*  establecerDS_SO1() ; */
    if (priVez)
        priVez = FALSE ;       /* Ignoro la primera lectura por ser basura */
    else
    {
        incX = x ;
        incY = (word_t)(y >> 16) ;   /* Tienen sentido respecto a MOVING/SIZING y */
        er.B0 = s ; /* (s & BITS_IGN) | (M.stat & ~BITS_IGN) ; */
    } /* Respeto los que habia */
    /* Considero el bit de signo .. */
    if (er.B0 & 0x20) incY |= 0xFF00 ;  /* ..haciendo una extension.. */
    if (er.B0 & 0x10) incX |= 0xFF00 ;  /* ..del bit de signo */
    X1 = (er.X + incX) ;
    Y1 = (er.Y - incY) ;

    if (X1 < 0) er.X = 0 ;
    else if (X1 >= maxX) er.X = maxX-1 ;
    else er.X = X1 ;

    if (Y1 < 0) er.Y = 0 ;
    else
    {
        numFilasAct = ptrBiosArea->VIDEO_lastrow + 1 ;
		maxYAct = (numFilasAct << 3) ;
        if (Y1 >= maxYAct) er.Y = maxYAct-1 ;
        else er.Y = Y1 ;
    }

}

int finishRaton ( void )
{
    exit(0) ;
    return(0) ;
}

void finCodeDriver ( void )   /* marca el fin del codigo propio del driver */
{
}

#define maxCbRaton 9

descCcb_t descCcbRaton = { 0, 0, 0, maxCbRaton, NULL,
    { cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo }
} ;

/* ----------------------------------------------------------------------- */
/*                      seccion de programa de usuario                     */
/* ----------------------------------------------------------------------- */

void outKBD ( byte_t puerto,   /* envia un comando al dispositivo auxiliar */
              byte_t valor )
{
asm 
{ 
    mov dl,puerto ;
    mov dh,valor ;
    push dx ;
    xor dx,dx ;
    mov es,dx ;
outKBD_0: ;
    mov dl,byte ptr es:[046Ch] ;                             /* BIOS_timer */
outKBD_1: ;
    in al,64h ;                                         /* keyboard status */
    test al,00000010b ;                    /* =2 check if we can send data */
    jmp outKBD_2 ;                           /* jump if write buffer empty */
    cmp dl,byte ptr es:[046Ch] ;                             /* BIOS_timer */
    je outKBD_1 ;
    xor dh,1 ;
    jnz outKBD_0 ;
outKBD_2: ;
    pop dx ;
    mov al,dh ;
    mov dh,0 ;
    out dx,al ;
}
}

void flushKBD ( byte_t puerto,         /* Send command to auxiliary device */
                byte_t valor )
{
    outKBD(puerto, valor) ;
asm 
{
    mov ah,0 ;
flushKBD_0: ;
    xor dx,dx ;
    mov es,dx ;
flushKBD_1: ;
    mov dl,byte ptr es:[046Ch] ;                             /* BIOS_timer */
flushKBD_2: ;
    in al,64h ;                                         /* keyboard status */
    test al,00000001b ;                          /* =1 if read buffer full */
    jz flushKBD_3 ;
    in al,60h ;
    mov ah,al ;
    jmp flushKBD_0 ;
flushKBD_3: ;
    cmp dl,byte ptr es:[046Ch]                               /* BIOS_timer */
    je flushKBD_2
    xor dh,1
    jnz flushKBD_1
}	
}

void setRate ( byte_t rate )
{

    outKBD(0x64, 0x0D4) ;                        /* enable mouse functions */
    flushKBD(0x60, 0x0F3) ;                      /* set rate...            */
//                                /* should be 0FAh (ACK) or 0FEh (resend) */
    outKBD(0x64, 0x0D4) ;                        /* enable mouse functions */
    flushKBD(0x60, rate) ;                       /* ...value               */
//                                /* should be 0FAh (ACK) or 0FEh (resend) */
}

void far PS2dummy ( void )
{
}

bool_t checkPS2 ( printCar_t pCar )
{
asm
{
    int 11h ;                                        /* get equipment list */
    and al,00000100b ;                                  /* 00000100 HW_PS2 */
    jz noPS2 ;                               /* jump if PS/2 not indicated */

    mov bh,3 ;
    mov ax,0C205h ;                    /* inicializar raton, bh = datasize */
    int 15h ;
    jc noPS2 ;
    test ah,ah ;
    jnz noPS2 ;

    mov bh,3 ;
    mov ax,0C203h ;             /* establecer la resoluci¢n del raton (bh) */
    int 15h ;
    jc noPS2 ;
    test ah,ah ;
    jnz noPS2 ;

    push cs ;
    pop es ;
    mov bx,OFFSET PS2dummy ;
    mov ax,0C207h ;                    /* establecer mouse handler (ES:BX) */
    int 15h ;
    jc noPS2 ;
    test ah,ah ;
    jnz noPS2 ;

    xor bx,bx ;
    mov es,bx ;
    mov ax,0C207h ;                        /* borrar mouse handler (ES:BX) */
    int 15h ;
//                          /* select IntelliMouse Z wheel + 3 button mode */
    pushf ;
    sti ;
}
    pCar('.') ;
    setRate(200) ;                          /* 200->100->80 rate does this */
    pCar('.') ;
    setRate(100) ;
    pCar('.') ;
    setRate( 80) ;
//                                                   /* check if sucessful */
    outKBD(0x64, 0x0D4) ;                        /* enable mouse functions */
    flushKBD(0x60, 0x0F2) ;                      /* get ID en ah           */
//                                               /* ID=3 -> 3 button+wheel */
    asm popf

    return(TRUE) ;                                          /* =PS/2 mouse */
noPS2:
    return(FALSE) ;
}

rti_t rtiRatonOrg ;                      /* rti antes de instalar el raton */

void disablePS2 ( void )
{
    asm pushf ;
    asm cli ;
    ptrTVI[nVIntRaton] = rtiRatonOrg ;  /* falta actualizar la mascara de interrupcion del pic ?? */
    asm popf ;
//                                         /* restablecerInt(nVIntRaton) ; */
asm
{
    xor bx,bx ;
    mov ax,0C200h ;                                       /* set mouse off */
    int 15h ;

    mov es,bx ;
    mov ax,0C207h ;                      /* borrar mouse handler (ES:BX=0) */
    int 15h ;
}
}

void enablePS2 ( handler_t handler )
{
    static word_t handlerWord ;

    handlerWord = FP_OFF(handler) ;
asm 
{
    mov bx,0 ;
    mov ax,0C200h ;                                       /* set mouse off */
    int 15h ;

    mov bh,3 ;
    mov ax,0C205h ;                    /* inicializar raton, bh = datasize */
    int 15h ;

    mov bh,3 ;
    mov ax,0C203h ;             /* establecer la resolucion del raton (bh) */
    int 15h ;

#if (FALSE)
    mov bx,0 ;
    mov es,bx ;
    mov ax,0C207h ;                      /* borrar mouse handler (ES:BX=0) */
    int 15h ;
#endif

    push cs ;
    pop es ;
    mov bx,handlerWord ;           /* necesitamos handlerWordl static (DS) */
    mov ax,0C207h ;
    int 15h ;               /* Establece mouse handler ES:BX=handler_Raton */

    mov bh,1 ;
    mov ax,0C200h ;                                        /* set mouse on */
    int 15h ;
}
}

typedef enum { ninguno, msdos, ps2 } tipoRaton_t ;

void noPrintCar ( char car ) ;
void noPrintCar ( )                          /* no se utiliza el argumento */
{
}

void inicRaton ( tipoRaton_t * tipoRaton,
                 isr_t * isrRaton,
                 bool_t conMensajes)
{
    word_t numBotones ;

    info_t info ;

    bool_t hayNT ;
    bool_t hayDBox ;

    printCar_t pCar ;

    obtenInfoINFO((info_t far *)&info) ;

    hayNT = FALSE ;
    switch (info.modoSO1)
    {
    case modoSO1_Com :                                          /* so1.com */
    case modoSO1_Exe :                                          /* so1.exe */
        hayNT = (bool_t)(hayWindowsNT()) ;
    default :
        ;
    } ;

    hayDBox  = (bool_t)(!strncmp((char far *)ptrFechaBios, "01/01/92", 8)) ;

    if (conMensajes)
    {
        if (hayNT) printf("\n\n hayNT ") ;
        if (hayDBox) printf("\n\n hayDBox ") ;
    }

    *tipoRaton = ninguno ;

    inicEstadoRaton() ;
    if (hayNT)
    {
        resetRatonBIOS((word_t *)&numBotones) ;
        if (hayRatonBIOS())                         /* raton msdos int 33h */
        {
            *tipoRaton = msdos ;                         /* WinXP o DOSBox */
            *isrRaton = isr_raton_BIOS ;
            installMouseEventHandler(     /* hay que poner _CS por el caso */
                (handler_t)MK_FP(_CS, FP_OFF(ratonHandler))     /* SO1.COM */
            ) ;
        }
        else if (conMensajes)
            printf("\n\n no hay raton BIOS ") ;
    }
    if (*tipoRaton == ninguno)
    {
        if (conMensajes)
            printf("\n\n comprobando si hay raton PS/2 ") ;
        if (conMensajes)
            pCar = escribirCar ;
        else
            pCar = (printCar_t)noPrintCar ;
        if (checkPS2(pCar))
        {
            if (conMensajes)
                printf(" Ok") ;
            *tipoRaton = ps2 ;                                /* raton PS2 */
            if (hayDBox)                           /* dosbox (qemu, bochs) */
            {
                enablePS2((handler_t)handlerRaton) ;
                *isrRaton = isr_raton_dosbox ;
            }
            else                                            /* qemu, bochs */
            {
                enablePS2((handler_t)handlerRatonNulo) ;
                *isrRaton = isr_raton ;
            }
        }
        else
        {
            if (conMensajes)
                printf("\r no se ha encontrado ningun raton \n") ;
            exit(-1) ;
        }
    }

    inicPC2c(&bloqueadosRaton, &e2BloqueadosRaton, maxProcesos + 0, FALSE) ;
}

void mostrarFormato ( void )
{
    printf(" formato: RATON [ [ -i | -q ] [ num ] | -u | -h ] ") ;
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
        " instala/desinstala el driver del raton"                        "\n"
        ""                                                               "\n"
        " opciones: (por defecto -i)"                                    "\n"
        ""                                                               "\n"
        "      -i  : instala el driver (usar &)"                         "\n"
        "      -u  : desintala el driver"                                "\n"
        "      -h  : muestra este help"                                  "\n"
    ) ;
    return(0) ;
}

char strTipoRaton [ ] [ 8 ] =
{
    "ninguno", "msdos", "ps2"
} ;

tipoRaton_t tipoRaton ;

int integrarRaton ( bool_t conMensajes )
{
    int dfRaton ;
    dfs_t dfs ;
    descRecurso_t dR ;
    int res ;
    pindx_t pindx ;
    char nomFich [ 12 ] ;
    isr_t isrRaton ;
    rti_t rtiRecRatonOrg ;

    rtiRatonOrg = ptrTVI[nVIntRaton] ;     /* antes de instalar el handler */

    for ( pindx = 0 ; pindx < maxProcesos ; pindx++ )
    {
        nbytesProceso[pindx] = 0 ;                /* nbytes esperando leer */
        dirProceso[pindx] = NULL ;
    }

    /* de momento esto solo funciona si hay msdos <--------
       dfRaton = open("RATON", O_RDONLY) ;
       if (dfRaton >= 0)
       {
           escribirStr("\n\n error: RATON ya instalado \n") ;
           close(dfRaton) ;
           exit(-1) ;
       }
    */

    inicRaton((tipoRaton_t *)&tipoRaton,
              (isr_t *)&isrRaton,
              (bool_t)conMensajes) ;               /* tipoRaton != ninguno */

    dR.tipo = rDCaracteres ;
    strcpy(dR.nombre, "RATON") ;
    dR.ccb = (ccb_t)&descCcbRaton ;
    dR.ccb->arg = NULL ;
    dR.pindx = getpindx() ;
    dR.numVI = 1 ;
    dR.nVInt[0] = nVIntRaton ;
    dR.irq[0] = IRQ_RATON ;
    dR.isr[0] = (isr_t)MK_FP(_CS, FP_OFF(isrRaton)) ;

    dR.open      = (open_t)     MK_FP(_CS, FP_OFF(openRaton)) ;
    dR.release   = (release_t)  MK_FP(_CS, FP_OFF(releaseRaton)) ;
    dR.read      = (read_t)     MK_FP(_CS, FP_OFF(readRaton)) ;
    dR.aio_read  = (aio_read_t) MK_FP(_CS, FP_OFF(aio_readRaton)) ;
    dR.write     = (write_t)    MK_FP(_CS, FP_OFF(writeRaton)) ;
    dR.aio_write = (aio_write_t)MK_FP(_CS, FP_OFF(aio_writeRaton)) ;
    dR.lseek     = (lseek_t)    MK_FP(_CS, FP_OFF(lseekRaton)) ;
    dR.fcntl     = (fcntl_t)    MK_FP(_CS, FP_OFF(fcntlRaton)) ;
    dR.ioctl     = (ioctl_t)    MK_FP(_CS, FP_OFF(ioctlRaton)) ;

    dR.eliminar  = (eliminar_t) MK_FP(_CS, FP_OFF(eliminarRaton)) ;

    asm pushf
    asm cli

    rtiRecRatonOrg = ptrTVI[nVIntRaton] ;      /* tras instalar el handler */
    rec_raton = crearRecurso(&dR) ;
    if (tipoRaton == msdos)
    {
        rti_nVIntRaton = ptrTVI[nVIntRaton] ;        /* ver so1\interrup.c */
        segment_rti_nVIntRaton = FP_SEG((pointer_t)rti_nVIntRaton) ;
        offset_rti_nVIntRaton = FP_OFF((pointer_t)rti_nVIntRaton) ;
        ptrTVI[nVIntRaton] = rtiRecRatonOrg ; /* restablecer el v.i.nVIntRaton */
        /*
            escribirStr("\n rti_nVIntRaton = ") ;
            escribirPtr((pointer_t)rti_nVIntRaton) ;
            escribirStr("\n segment_rti_nVIntRaton = ") ;
            escribirHex(segment_rti_nVIntRaton, 4) ;
            escribirStr("\n offset_rti_nVIntRaton = ") ;
            escribirHex(offset_rti_nVIntRaton, 4) ;
        */
    }
	
    asm popf

    if (rec_raton >= 0)
    {
        strcpy(nomFich, "RATON") ;

        dfs = crearFichero(nomFich, rec_raton, 0, fedCaracteres) ;

        if (dfs < 0)
        {
            switch(dfs)
            {
            case -1 : printf(" tipo de fichero erroneo") ; break ;
            case -2 : printf(" numero de recurso erroneo") ; break ;
            case -3 : printf(" RATON nombre de fichero ya existente") ; break ;
            case -4 : printf(" no hay descriptores de fichero libres") ; break ;
            default : printf(" no ha podido crearse el fichero RATON") ;
            }
            destruirRecurso("RATON") ;
        }
        if (conMensajes)
            printf(
                ""                                                       "\n"
                ""                                                       "\n"
                " recurso RATON instalado (fichero: RATON)"              "\n"
                ""                                                       "\n"
                " tipo de raton = %s\n", 
				strTipoRaton[tipoRaton]
            ) ;
        return(0) ;
    }

    printf("\n\n") ;
    switch(rec_raton)
    {
    case -1 : printf(" tipo de recurso erroneo ") ; break ;
    case -2 : printf(" demasiados vectores de interrupcion ") ; break ;
    case -3 : printf(" RATON nombre de recurso ya existente ") ; break ;
    case -4 : printf(" no hay descriptores de recurso libres ") ; break ;
    case -5 : printf(" numero de vector de interrupcion ya usado ") ; break ;
    default : printf(" no ha podido crearse el recurso RATON ") ;
    }
    printf("\n") ;
    return(-1) ;
}

int desintegrarRaton ( void )
{
    int res ;
    res = 0 ;
    switch (tipoRaton)
    {
    case msdos :
        uninstallMouseEventHandler(ratonHandler) ;
        break ;
    case ps2   :
        disablePS2() ;
    }
    return(res) ;
}

int instalarRaton ( bool_t conMensajes )
{
    int res ;
    res = comprobarAmpersand() ;
    if (res != 0) return(res) ;
    obtenInfoSO1(dirDescSO1) ;               /* obtenemos los datos de SO1 */
    res = integrarRaton(conMensajes) ;
    if (res != 0) return(res) ;
#if (!REDUCIR_DRIVER)
    esperarDesinstalacion(0) ;                       /* bloquea el proceso */
#else
    esperarDesinstalacion(                           /* bloquea el proceso */
//      FP_OFF(dirDescSO1) + sizeof(descSO1_t)
//          ...
//          + sizeof(descCcbConsola) + 0*sizeof(callBack_t),    /* tamDATA */
        FP_OFF(&descCcbRaton)
            + sizeof(descCcbRaton) + 9*sizeof(callBack_t),      /* tamDATA */
        FP_OFF(finCodeDriver),                            /* finCodeDriver */
        FP_OFF(finishRaton)                                /* finishDriver */
    ) ;
#endif
    res = desintegrarRaton() ;
    return(res) ;
}

int main ( int argc, char * argv [ ] )
{
    int res ;
    if (argc > 3) return(formato()) ;
    else if (argc == 1) return(instalarRaton(TRUE)) ;
    else if (argc == 2)
    {
        if (!strcmpu(argv[1], "-h")) return(help()) ;
        else if (!strcmpu(argv[1], "-i")) return(instalarRaton(TRUE)) ;
        else if (!strcmpu(argv[1], "-q")) return(instalarRaton(FALSE)) ;
        else if (!strcmpu(argv[1], "-u"))
        {
            res = destruirRecurso("RATON") ;
            switch (res)
            {
            case  0 : printf(" recurso RATON desinstalado") ; break ;
            case -1 : printf(" recurso RATON no existe") ; break ;
            case -2 : printf(" recurso RATON en uso") ; break ;
            case -3 : printf(" fichero RATON no puede destruirse") ; break ;
            default : printf(" RATON no ha podido desinstalarse") ;
            }
            return(res) ;
        }
    }
    if ((argc == 2) ||
            ((argc == 3) &&
             (!strcmpu(argv[1], "-i") ||
              !strcmpu(argv[1], "-q"))
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
                printf("\n\n numConsolas debe ser > 0 \n") ;
                return(-1) ;
            }
            else return(instalarRaton(toupper(argv[1][1]) == 'Q')) ;
        }
    }
    return(formato()) ;
}

/***************************************************************************/

#if (FALSE)

#define tics2Click 4               /* timeout (en tics) para el doble clik */

bool_t activarCursorRaton ( bool_t Ok )
{
    bool_t cRAAnt = cursorRatonActivo ;
    if (!primeraInt)
        if (Ok != cursorRatonActivo)
        {
            atrAux = ptrPant->t[FAntCursor][CAntCursor].atr ;
            ptrPant->t[FAntCursor][CAntCursor].atr =
                (atrAux << 4) | (atrAux >> 4) ;
            cursorRatonActivo = Ok ;
        }
    return(cRAAnt) ;
}

pindx_t pindx ;
pindx_t ppindx ;
int nwin ;

estadoRaton_t far * estado ;

void cerrarPrograma ( void ) ;                                  /* forward */

void botonCerrarVentana ( void ) ;                              /* forward */

void actualizarCursor ( void ) ;                                /* forward */

void tratarClickMarco ( void ) ;                                /* forward */

#pragma warn -struct

#pragma warn +struct

void procesarTicReloj_raton ( void )
{
    if (contTics2Click > 0)
        if (--contTics2Click == 0)
        {
            primerClick = TRUE ;
            moviendo = TRUE ;
        }
}

void cerrarPrograma ( void )
{
    for ( pindx = 1 ; pindx < maxProcesos ; pindx++ )
        if (descProceso[pindx].estado != libre)
            matarProcIndx(pindx) ;
    nivelActivacionSO1 = 0 ;
    indProcesoActual = 0 ;
    asm mov sp,SPInicial
    asm sti
    tirarSistema() ;
}

void botonCerrarVentana ( void )
{
    if (pindx != 0)
    {
        if (pindx == indProcesoActual) eoi_pic2() ;                 /* EOI */
        doKill(pindx) ;
    }
    else
    {
        eoi_pic2() ;                                                /* EOI */
        cerrarPrograma() ;
    }
}

void tratarClickMarco ( void )
{
#if (FALSE)
    if (primerClick)
    {
        focalizarWin(win) ;
        primerClick = FALSE ;
        contTics2Click = tics2Click ;
        actualizarCursor() ;
    }
    else
    {
        if (contTics2Click > 0)
        {
            maxMinWin(win, TRUE) ;
            contTics2Click = 0 ;
        }
        primerClick = TRUE ;
    }
#endif
}

#endif
