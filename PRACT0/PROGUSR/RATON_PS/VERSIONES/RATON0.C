/* ----------------------------------------------------------------------- */
/*                                 raton.c                                 */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\puertos.h>
#include <so1pub.h\copia.h>
#include <so1pub.h\strings.h>
#include <so1pub.h\tipos.h>
#include <so1pub.h\printgen.h>
#include <so1pub.h\escribir.h>
#include <so1pub.h\bios_crt.h>
#include <so1pub.h\pantalla.h>
#include <so1pub.h\pic.h>
#include <so1.h\ajustes.h>
#include <so1.h\interrup.h>
#include <so1.h\procesos.h>
#include <so1.h\recursos.h>
#include <so1.h\so1.h>
#include <so1.h\bios.h>
#include <so1.h\plot.h>
#include <so1.h\dokill.h>
#include <so1.h\raton.h>

#define ptrPant ((pantalla_t far *)pointer(0xB800, 0x0000))

rindx_t rec_RATON ;

#define nVIntRaton 0x74          /* obtenido experimentalmente MSD + DEBUG */

void far isr_raton ( void ) ;                                   /* forward */




typedef enum { ninguno, msdos, ps2 } tipoRaton_t ;              /* forward */

void procesarRaton ( word_t W0, int X, int Y ) ;                /* forward */

bool_t checkPS2 ( printCar_t pCar ) ;                           /* forward */

void enablePS2 ( handler_t handler ) ;                          /* forward */

void far handlerRatonNulo ( void ) ;

void far handlerRaton ( dword_t y, word_t x, word_t s ) ;

/*

void inicEstadoRaton ( void ) ;

extern tipoRaton_t tipoRaton ;

extern int cursorRatonActivo ;

extern int primerClick ;

extern int contTics2Click ;

void disablePS2 ( void ) ;

void far isr_raton ( void ) ;

void far isr_raton_dosbox ( void ) ;

bool_t leerEstadoRaton ( estadoRaton_t far * estado ) ;

bool_t activarCursorRaton ( bool_t Ok ) ;

void clickTimeout ( void ) ;

void procesarTicReloj_raton ( void ) ;

*/


#define TECLADO_ESTADO  0x64
#define TECLADO_COMANDO 0x64
#define TECLADO_DATO    0x60

#define RATON_ESTADO    0x64
#define RATON_COMANDO   0x64
#define RATON_DATO      0x60

#define tics2Click 4               /* timeout (en tics) para el doble clik */

tipoRaton_t tipoRaton = ninguno ;

/* ----------------------------------------------------------------------- */
/*          variables globales que controlan el estado del raton           */
/* ----------------------------------------------------------------------- */

static int inB = 0 ;                       /* indice de entrada al bufer B */
static byte_t B[3] = { 0x00, 0x00, 0x00 } ;

/* no cambiar el orden en las siguientes lineas (ver leerEstadoRaton)      */

static estadoRaton_t er = {
  /* S0            */ 0x00,                           /* byte estado 8042  */
  /* B0            */ 0x00,                           /* Byte 0 = 00SS1MRL */
  /* B1            */ 0x00,                           /* Byte 1 = X        */
  /* B2            */ 0x00,                           /* Byte 2 = Y        */
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

static int XAux = 0 ;
static int YAux = 0 ;
static int FAntCursor ;
static int CAntCursor ;

static byte_t atrAux ;

int cursorRatonActivo = TRUE ;

static int moviendo = FALSE ;
static int redimensionando = FALSE ;

int primerClick = TRUE ;

int contTics2Click = 0 ;

static bool_t huboInt = TRUE ;     /* hubo int. del raton sin leer todavia */

static int primeraInt = TRUE ;  /* la siguiente interrupcion es la primera */

void inicEstadoRaton ( void ) {

  er.S0 = 0x00 ;
  er.B0 = 0x00 ;
  er.B1 = 0x00 ;
  er.B2 = 0x00 ;
  er.W0 = 0x0000 ;
  er.X = maxX/2 ;
  er.Y = maxY/2 ;
  er.F = er.Y >> 3 ;                                            /* F = Y/8 */
  er.C = er.X >> 3 ;                                            /* C = X/8 */
  er.botonIz = FALSE ;
  er.botonDe = FALSE ;
  er.botonMe = FALSE ;

  primeraInt = TRUE ;
  cursorRatonActivo = TRUE ;
  primerClick = TRUE ;
  contTics2Click = 0 ;

}

bool_t leerEstadoRaton ( estadoRaton_t far * estado ) {

  copia((pointer_t)&er, (pointer_t)estado, sizeof(estadoRaton_t)) ;
  if (huboInt) {
    huboInt = FALSE ;
    return(TRUE) ;
  }
  else
    return(FALSE) ;
}

bool_t activarCursorRaton ( bool_t Ok ) {
  bool_t cRAAnt = cursorRatonActivo ;
  if (!primeraInt)
    if (Ok != cursorRatonActivo) {
      atrAux = ptrPant->t[FAntCursor][CAntCursor].atr ;
      ptrPant->t[FAntCursor][CAntCursor].atr =
        (atrAux << 4) | (atrAux >> 4) ;
      cursorRatonActivo = Ok ;
    }
  return(cRAAnt) ;
}

static byte_t byteEstado ;
static byte_t byteDato ;

static pindx_t pindx ;
static pindx_t ppindx ;
static int nwin ;

void tratarRaton ( void ) ;

void far isr_raton ( void ) {

  plot('m', 0, contadorTimer0()) ;

  tratarRaton() ;

  plot('e', 0, contadorTimer0()) ;

  eoi_pic2() ;

}

void far isr_raton_dosbox ( void ) {

  VIOrg[nVIntRaton]() ;         /* llamamos a la antigua rti (pushf + int) */

  plot('m', 0, contadorTimer0()) ;

/*
  tratarRaton() ;
*/
  procesarRaton((word_t)er.B0, er.X, er.Y) ;

  plot('e', 0, contadorTimer0()) ;

/* eoi_pic2() ; */

}

void tratarRaton ( void ) {

  static int incX, incY ;

  asm in al,64h
  asm mov byteEstado,al
  if ((byteEstado & 0x21) == 0x21) {      /* hay byte procedente del raton */
    er.S0 = byteEstado ;
    asm in al,60h
    asm mov byteDato,al

    if ((inB != 0) || (((byteDato^0x08) & 0xC8) == 0)) {

      B[inB] = byteDato ;
      inB = (inB + 1) % 3 ;

      if (inB == 0) {

        er.B0 = B[0] ;
        er.B1 = B[1] ;
        if (er.B0 & 0x10) {
          asm mov al, byte ptr er.B1
          asm mov ah,0ffh
          asm mov incX,ax
          er.incX = incX ;
        }
        else er.incX = er.B1 ;
        XAux = er.X + er.incX ;
        if ((0 <= XAux) && (XAux < maxX)) er.X = XAux ;      /* 8*80 = 640 */
        er.B2 = B[2] ;
        if (er.B0 & 0x20) {
          asm mov al, byte ptr er.B2
          asm mov ah,0ffh
          asm mov incY,ax
          er.incY = incY ;
        }
        else er.incY = er.B2 ;
        er.incY = -er.incY ;                                        /* ojo */
        YAux = er.Y + er.incY ;
        if ((0 <= YAux) && (YAux < maxY)) er.Y = YAux ;      /* 8*25 = 200 */

        procesarRaton((word_t)er.B0, er.X, er.Y) ;

      }
    }
  }
}

static int posWF ;
static int posWC ;
static int FReal ;
static int CReal ;

static estadoRaton_t far * estado ;
static pindx_t pindx ;

void cerrarPrograma ( void ) ;                                  /* forward */

void botonCerrarVentana ( void ) ;                              /* forward */

void actualizarCursor ( void ) ;                                /* forward */

void tratarClickMarco ( void ) ;                                /* forward */

void procesarColaBloqueadosRaton ( void ) ;                     /* forward */

#pragma warn -struct

void procesarRaton ( word_t parW0, int parX, int parY ) {
#if (FALSE)
  er.W0 = parW0 ;
  er.X = parX ;
  er.Y = parY ;

  er.F = er.Y >> 3 ;                                            /* F = Y/8 */
  er.C = er.X >> 3 ;                                            /* C = X/8 */

  er.B0 = (byte_t)(er.W0 & 0x00FF) ;

  huboInt = TRUE ;               /* obtenidos nuevos valores para el raton */

  er.botonIzAnt = er.botonIz ;
  er.botonDeAnt = er.botonDe ;
  er.botonMeAnt = er.botonMe ;

  er.botonIz = (er.W0 & 0x0001) ;
  er.botonDe = ((er.W0 & 0x0002) >> 1) ;
  er.botonMe = ((er.W0 & 0x0004) >> 2) ;

  procesarColaBloqueadosRaton() ;

  if (cursorRatonActivo) {
    if (er.botonIz) {                        /* boton izquierdo presionado */
      if (moviendo || redimensionando) {
        if (moviendo) {                                        /* moviendo */
          posWF = win->esqSupIzF ;
          posWC = win->esqSupIzC ;
          moverWin(win, er.F-FAntCursor, er.C-CAntCursor,
                        er.F-FAntCursor, er.C-CAntCursor) ;
          FReal = FAntCursor + (win->esqSupIzF - posWF) ;        /* F Real */
          CReal = CAntCursor + (win->esqSupIzC - posWC) ;        /* C Real */
        }
        else {                                          /* redimensionando */
          posWF = win->esqInfDeF ;
          posWC = win->esqInfDeC ;
          moverWin(win, 0, 0, er.F-FAntCursor, er.C-CAntCursor) ;
          FReal = FAntCursor + (win->esqInfDeF - posWF) ;        /* F Real */
          CReal = CAntCursor + (win->esqInfDeC - posWC) ;        /* C Real */
        }
        if ((FReal != er.F) || (CReal != er.C)) {
          er.Y = (FReal << 3) + 3 ;                            /* ajuste Y */
          er.X = (CReal << 3) + 3 ;                            /* ajuste X */
          if (tipoRaton == msdos)
            situarCursorRatonBIOS(er.X, er.Y) ;
        }
        er.F = FReal ;
        er.C = CReal ;
        actualizarCursor() ;
      }
      else if ((nwin = winVisible[er.F][er.C]) != 0) {
        if (!er.botonIzAnt) {          /* primera pulsacion del boton izdo */
          win = winNum[nwin] ;
          pindx = win->pindx ;

          if (er.F == win->esqSupIzF-1) {                /* marco superior */
            if (er.C == win->esqInfDeC-1) botonCerrarVentana() ;    /* [X] */
            else if (er.C == win->esqInfDeC-4) maxMinWin(win, FALSE) ; /* [f] */
            else if ((er.F == win->esqSupIzF-1) &&
                     (win->esqSupIzC <= er.C) &&
                     (er.C <= win->esqSupIzC+2+2*(win->pindx!=0)))
              maxMinWin(win, TRUE) ;                                /* max */
            else
              tratarClickMarco() ;
          }
          else if ((er.F == win->esqInfDeF+1) &&
                   (er.C == win->esqInfDeC+1)) {          /* redimensionar */
            focalizarWin(win) ;
            redimensionando = TRUE ;
            actualizarCursor() ;
          }
          else if ((er.F == win->esqInfDeF+1) ||
                   (er.C == win->esqSupIzC-1) ||
                   (er.C == win->esqInfDeC+1))               /* otro marco */
            tratarClickMarco() ;
          else focalizarWin(win) ;                       /* punto interior */
        }
        else actualizarCursor() ;
      }
    }
    else {                                /* boton izquierdo no presionado */
      if (moviendo)
        moviendo = FALSE ;
      else if (redimensionando)
        redimensionando = FALSE ;
      actualizarCursor() ;
    }
  }
#endif
}

void procesarColaBloqueadosRaton ( void ) {

#if (FALSE)

  if (bloqueadosRaton.numElem > 0) {
    pindx = bloqueadosRaton.primero ;
    if (indProcesoActual > -1) {                            /* nivel 1 o 2 */
      descProceso[indProcesoActual].trama =
        (trama_t far *)pointer(SS_Proceso, SP_Proceso) ;
      descProceso[indProcesoActual].estado = preparado ;
      encolarPC2c(indProcesoActual, (ptrC2c_t)&(c2cPFR[PPreparados])) ;
      indProcesoActual = -1 ;
      activarAlEpilogo1 = TRUE ;
    }

    do {
      estado = (estadoRaton_t far *) pointer(
                descProceso[pindx].trama->ES,
                descProceso[pindx].trama->BX             /* utilizamos BX */
               ) ;
      leerEstadoRaton(estado) ;

      switch (descProceso[pindx].trama->DX) {
      case 0 : if (descProceso[pindx].win != descProceso[indProcesoDeSuperficie].win) break ;
      case 1 : if (winVisible[estado->F][estado->C] != descProceso[pindx].win->num) break ;
      case 2 :

        switch (descProceso[pindx].esperandoPor) {
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
        descProceso[pindx].estado = preparado ;
        encolarPC2c(pindx, (ptrC2c_t)&c2cPFR[PUrgentes]) ;
      }
      desencolarPC2c(&bloqueadosRaton) ;
      pindx = bloqueadosRaton.primero ;
    }
    while (bloqueadosRaton.numElem > 0) ;
  }
#endif
}

#pragma warn +struct

void procesarTicReloj_raton ( void ) {
  if (contTics2Click > 0)
    if (--contTics2Click == 0) {
      primerClick = TRUE ;
      moviendo = TRUE ;
    }
}

void cerrarPrograma ( void ) {
  for ( pindx = 1 ; pindx < maxProcesos ; pindx++ )
     if (descProceso[pindx].estado != libre)
       matarProcIndx(pindx) ;
  nivelActivacionSO1 = 0 ;
  indProcesoActual = 0 ;
  asm mov sp,SPInicial
  asm sti
  tirarSistema() ;
}

void botonCerrarVentana ( void ) {
  if (pindx != 0) {
    if (pindx == indProcesoActual) eoi_pic2() ;                     /* EOI */
    doKill(pindx) ;
  }
  else {
    eoi_pic2() ;                                                    /* EOI */
    cerrarPrograma() ;
  }
}

#define rotarDerecha(x, n)                                                   \
  asm mov al,x ;                                                             \
  asm ror al,n ;                          /* rotacion a la derecha n bits */\
  asm mov x,al ;

void actualizarCursor ( void ) {
  if (cursorRatonActivo) {               /* actualizar el cursor del raton */
    if (primeraInt) primeraInt = FALSE ;
    else if ((er.F != FAntCursor) || (er.C != CAntCursor)) {
      atrAux = ptrPant->t[FAntCursor][CAntCursor].atr ;
      /* atrAux = (atrAux << 4) | (atrAux >> 4) ; */
      rotarDerecha(atrAux, 4) ;
      ptrPant->t[FAntCursor][CAntCursor].atr = atrAux ;
    } else return ;
    atrAux = ptrPant->t[er.F][er.C].atr ;
    /* atrAux = (atrAux << 4) | (atrAux >> 4) ; */
    rotarDerecha(atrAux, 4) ;
    ptrPant->t[er.F][er.C].atr = atrAux ;
    FAntCursor = er.F ;
    CAntCursor = er.C ;
  }
}

void tratarClickMarco ( void ) {
#if (FALSE)
  if (primerClick) {
    focalizarWin(win) ;
    primerClick = FALSE ;
    contTics2Click = tics2Click ;
    actualizarCursor() ;
  }
  else {
    if (contTics2Click > 0) {
      maxMinWin(win, TRUE) ;
      contTics2Click = 0 ;
    }
    primerClick = TRUE ;
  }
#endif
}

/* ----------------------------------------------------------------------- */
/*                   operaciones de ficheros del raton                     */
/* ----------------------------------------------------------------------- */

static char leer ( void ) {
  estadoRaton_t est ;
  encolarPC2c(indProcesoActual, &bloqueadosRaton) ;
  bloquearProcesoActual(rec_raton) ;
  leerEstadoRaton(&est) ;
  return((char)(est.W0)) ;
}

static char leerListo ( void ) {
  estadoRaton_t est ;
  leerEstadoRaton(&est) ;
  return((char)(est.W0)) ;
}

static dword_t leerRaw ( void ) {
  estadoRaton_t est ;
  encolarPC2c(indProcesoActual, &bloqueadosRaton) ;
  bloquearProcesoActual(rec_raton) ;
  leerEstadoRaton(&est) ;
  return((dword_t)&est) ;
}

static dword_t leerRawListo ( void ) {
  estadoRaton_t est ;
  leerEstadoRaton(&est) ;
  return((dword_t)&est) ;
}

#pragma warn -par

static int far openRaton ( int dfs, modoAp_t modo ) {
  return(0) ;
}

static int far releaseRaton ( int dfs ) {
  return(0) ;
}

static int far readRaton ( int dfs, pointer_t dir, word_t nbytes ) {
  return(0) ;
}

static int far writeRaton ( int dfs, pointer_t dir, word_t nbytes ) {
  return(0) ;
}

static int far lseekRaton ( int dfs, long int pos, word_t whence ) {
  return(0) ;
}

static int far fcntlRaton ( int dfs, word_t cmd, word_t arg ) {
  return(0) ;
}

static int far ioctlRaton ( int dfs, word_t request, word_t arg ) {
  return(0) ;
}

#pragma warn +par

/* ----------------------------------------------------------------------- */
/*                     handler para el raton (int 15h)                     */
/* ----------------------------------------------------------------------- */

void far handlerRatonNulo ( void ) {
}

void far handlerRaton ( dword_t y, word_t x, word_t s ) {

  static bool_t priVez = TRUE ;   /* Lo necesito para ignorar los incrementos .. */
  static int incX = 0, incY = 0 ; /* .. X,Y iniciales que son basura */
  int X1 ;
  int Y1 ;
  asm cli
  establecerDS_SO1() ;
  if (priVez)
    priVez = FALSE ;              /* Ignoro la primera lectura por ser basura */
  else {
    incX = x ; incY = (y >> 16) ; /* Tienen sentido respecto a MOVING/SIZING y */
    er.B0 = s ; /* (s & BITS_IGN) | (M.stat & ~BITS_IGN) ; */
  } /* Respeto los que habia */
  /* Considero el bit de signo .. */
  if (er.B0 & 0x20) incY |= 0xFF00 ;  /* ..haciendo una extension.. */
  if (er.B0 & 0x10) incX |= 0xFF00 ;  /* ..del bit de signo */
  X1 = (er.X + incX) ;
  Y1 = (er.Y - incY) ;
  if (X1 < 0) er.X = 0 ;
  else if (X1 >= 640) er.X = 639 ;
  else er.X = X1 ;
  if (Y1 < 0) er.Y = 0 ;
  else if (Y1 >= 199) er.Y = 199 ;
  else er.Y = Y1 ;

}

#define maxCbMS 9

static descCcb_t descCcbMS = { 0, 0, 0, maxCbMS,
  cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo, cbNulo
} ;

void inicRaton ( bool_t hayNT, bool_t hayDBox ) {

  descRecurso_t dR ;

  word_t numBotones ;

  isr_t isrRaton ;

  int numVI = 0 ;

  dfs_t dfs ;

  inicEstadoRaton() ;
  if (hayNT) {
    resetRatonBIOS(&numBotones) ;
    if (hayRatonBIOS()) {                           /* raton msdos int 33h */
      tipoRaton = msdos ;                                /* WinXP o DOSBox */
      installMouseEventHandler(        /* hay que poner CS_SO1 por el caso */
        (handler_t)pointer(CS_SO1, (word_t)ratonHandler)        /* SO1.COM */
      ) ;
    }
    else escribirStr("\n\n no hay raton BIOS ") ;
  }
  if (tipoRaton == ninguno) {
    escribirStr("\n\n Comprobando si hay raton PS/2 ") ;
    if (checkPS2(escribirCar)) {
      escribirStr(" Ok") ;
      tipoRaton = ps2 ;                                       /* raton PS2 */
      numVI = 1 ;
      if (hayDBox) {                               /* dosbox (qemu, bochs) */
        enablePS2((handler_t)handlerRaton) ;
        isrRaton = isr_raton_dosbox ;
      } else {                                              /* qemu, bochs */
        enablePS2((handler_t)handlerRatonNulo) ;
        isrRaton = isr_raton ;
      }
    }
    else escribirStr("\r No se ha encontrado ningun raton ") ;
  }

  inicPC2c(&bloqueadosRaton, &e2BloqueadosRaton, maxProcesos + 0, FALSE) ;

  dR.tipo = rDCaracteres ;
  copiarStr("RATON", dR.nombre) ;
  dR.ccb = (ccb_t)&descCcbMS ;
  dR.pindx = indProcesoActual ;
  dR.numVI = numVI ;
  if (numVI > 0) {
    dR.nVInt[0] = nVIntRaton ;
    dR.irq[0] = IRQ_RATON ;
    dR.isr[0] = (isr_t)pointer(_CS, (word_t)isrRaton) ;
  }

  dR.open    = (open_t)pointer(_CS, (word_t)openRaton) ;
  dR.release = (release_t)pointer(_CS, (word_t)releaseRaton) ;
  dR.read    = (read_t)pointer(_CS, (word_t)readRaton) ;
  dR.write   = (write_t)pointer(_CS, (word_t)writeRaton) ;
  dR.lseek   = (lseek_t)pointer(_CS, (word_t)lseekRaton) ;
  dR.fcntl   = (fcntl_t)pointer(_CS, (word_t)fcntlRaton) ;
  dR.ioctl   = (ioctl_t)pointer(_CS, (word_t)ioctlRaton) ;

  rec_RATON = crearRec(&dR) ;

  dfs = crearFich("RATON", rec_RATON, 0, fedCaracteres) ;

}

void outKBD ( byte_t puerto,           /* Send command to auxiliary device */
              byte_t valor ) {

  asm mov dl,puerto
  asm mov dh,valor
  asm push dx
  asm xor dx,dx
  asm mov es,dx
outKBD_0:
  asm mov dl,byte ptr es:[046Ch]                             /* BIOS_timer */
outKBD_1:
  asm in al,64h                                         /* keyboard status */
  asm test al,00000010b                    /* =2 check if we can send data */
  asm jmp outKBD_2                           /* jump if write buffer empty */
  asm cmp dl,byte ptr es:[046Ch]                             /* BIOS_timer */
  asm je outKBD_1
  asm xor dh,1
  asm jnz outKBD_0
outKBD_2:
  asm pop dx
  asm mov al,dh
  asm mov dh,0
  asm out dx,al
}

void flushKBD ( byte_t puerto,         /* Send command to auxiliary device */
                byte_t valor ) {

  outKBD(puerto, valor) ;
  asm mov ah,0
flushKBD_0:
  asm xor dx,dx
  asm mov es,dx
flushKBD_1:
  asm mov dl,byte ptr es:[046Ch]                             /* BIOS_timer */
flushKBD_2:
  asm in al,64h                                         /* keyboard status */
  asm test al,00000001b                          /* =1 if read buffer full */
  asm jz flushKBD_3
  asm in al,60h
  asm mov ah,al
  asm jmp flushKBD_0
flushKBD_3:
  asm cmp dl,byte ptr es:[046Ch]                             /* BIOS_timer */
  asm je flushKBD_2
  asm xor dh,1
  asm jnz flushKBD_1
}

void setRate ( byte_t rate ) {

  outKBD(0x64, 0x0D4) ;                          /* enable mouse functions */
  flushKBD(0x60, 0x0F3) ;                        /* set rate...            */
                                  /* should be 0FAh (ACK) or 0FEh (resend) */
  outKBD(0x64, 0x0D4) ;                          /* enable mouse functions */
  flushKBD(0x60, rate) ;                         /* ...value               */
                                  /* should be 0FAh (ACK) or 0FEh (resend) */
}

void far PS2dummy ( void ) {
}

bool_t checkPS2 ( printCar_t pCar ) {

  asm int 11h                                        /* get equipment list */
  asm and al,00000100b                                  /* 00000100 HW_PS2 */
  asm jz noPS2                               /* jump if PS/2 not indicated */

  asm mov bh,3
  asm mov ax,0C205h                    /* inicializar raton, bh = datasize */
  asm int 15h
  asm jc noPS2
  asm test ah,ah
  asm jnz noPS2

  asm mov bh,3
  asm mov ax,0C203h             /* establecer la resoluci¢n del raton (bh) */
  asm int 15h
  asm jc noPS2
  asm test ah,ah
  asm jnz noPS2

  asm push cs
  asm pop es
  asm mov bx,OFFSET PS2dummy
  asm mov ax,0C207h                    /* establecer mouse handler (ES:BX) */
  asm int 15h
  asm jc noPS2
  asm test ah,ah
  asm jnz noPS2

  asm xor bx,bx
  asm mov es,bx
  asm mov ax,0C207h                        /* borrar mouse handler (ES:BX) */
  asm int 15h
                            /* select IntelliMouse Z wheel + 3 button mode */
  asm pushf
  asm sti
  pCar('.') ;
  setRate(200) ;                            /* 200->100->80 rate does this */
  pCar('.') ;
  setRate(100) ;
  pCar('.') ;
  setRate( 80) ;
                                                     /* check if sucessful */
  outKBD(0x64, 0x0D4) ;                          /* enable mouse functions */
  flushKBD(0x60, 0x0F2) ;                        /* get ID en ah           */
                                                 /* ID=3 -> 3 button+wheel */
  asm popf

  return(TRUE) ;                                            /* =PS/2 mouse */
noPS2:
  return(FALSE) ;
}

void disablePS2 ( void ) {

  restablecerInt(nVIntRaton) ;

  asm xor bx,bx
  asm mov ax,0C200h                                       /* set mouse off */
  asm int 15h

  asm mov es,bx
  asm mov ax,0C207h                      /* borrar mouse handler (ES:BX=0) */
  asm int 15h

}

void enablePS2 ( handler_t handler ) {

  static word_t handlerWord ;

  handlerWord = (word_t)handler ;

  asm mov bx,0
  asm mov ax,0C200h                                       /* set mouse off */
  asm int 15h

  asm mov bh,3
  asm mov ax,0C205h                    /* inicializar raton, bh = datasize */
  asm int 15h

  asm mov bh,3
  asm mov ax,0C203h             /* establecer la resoluci¢n del raton (bh) */
  asm int 15h

#if (FALSE)
  asm mov bx,0
  asm mov es,bx
  asm mov ax,0C207h                      /* borrar mouse handler (ES:BX=0) */
  asm int 15h
#endif

  asm push cs
  asm pop es
  asm mov bx,handlerWord
  asm mov ax,0C207h
  asm int 15h               /* Establece mouse handler ES:BX=handler_Raton */

  asm mov bh,1
  asm mov ax,0C200h                                        /* set mouse on */
  asm int 15h

}

