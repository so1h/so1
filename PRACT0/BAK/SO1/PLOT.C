/* ----------------------------------------------------------------------- */
/*                                 plot.c                                  */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\tipos.h>        /* bool_t, TRUE, FALSE, word_t, pindx_t */
//                                                            /* pointer_t */
#include <so1pub.h\const.h>                                 /* maxProcesos */
#include <so1pub.h\def_proc.h>                                 /* estado_t */
#include <so1pub.h\msdos.h>
#include <so1pub.h\ccb.h>
#include <so1.h\procesos.h>                                  
#include <so1.h\gm.h>

/*
#include <so1.h\rs232.h>
#include <so1.h\rs232pol.h>
#include <so1.h\rs232int.h>
#include <so1.h\lpt.h>
#include <so1.h\lptpol.h>
*/

#include <so1.h\plot.h>

#define tamBufPlot 0x1000                           /* paragrafos => 64 KB */

/* vamos a sacar partido del tamanio: x mod 64KB = x (hay desbordamiento)  */

bool_t plotProcesos = FALSE ;

static int lpt ;
static int com ;
static int df = -1 ;

static word_t segmento = 0x0000 ;
static pointer_t ptrBufPlot ;

static word_t inBufPlot = 0 ;
static word_t outBufPlot = 0 ;
static word_t numBufPlot = 0 ;

typedef void ( * plotCar_t ) ( char car ) ;

plotCar_t plotCar ;

void plotCarNula ( char car ) ;
void plotCarLPT ( char car ) ;
void plotCarCOM ( char car ) ;
void plotCarCOMP ( char car ) ;
void plotCarFILE ( char car ) ;
void plotCarBuf ( char car ) ;
void plotCarE9 ( char car ) ;

int far tareaPlot ( void far * arg ) ;

int far tareaPrueba ( ) {
/*  printCarWin(win_so, '.') ; */
  return(1) ;
}

static bool_t vaciarBufPlot = FALSE ;              /* de momento no lo uso */

//static dword_t contTicsPlot = 0 ;

static estado_t estadoAnt [ maxProcesos ] ;

#pragma warn -aus

bool_t initPlot ( byte_t modo, char * filePlot ) {

  pindx_t pindx ;
  bool_t res ;
  word_t accion ;
  word_t error ;

  res = TRUE ;
  plotCar = plotCarNula ;

  for ( pindx = 0 ; pindx < maxProcesos ; pindx++ )
    estadoAnt[pindx] = libre ;

  switch (modo) {
  case 0x00 : ;
  case 0x01 : ;
  case 0x02 : ;
  case 0x03 : plotCar = plotCarLPT ;                        /* por polling */
              lpt = modo ;
              break ;
  case 0x04 : ;
  case 0x05 : ;
  case 0x06 : ;
  case 0x07 : plotCar = plotCarCOMP ;
              com = modo-4 ;
              break ;
  case 0x08 : asm in al,0E9h
              if (_AL == 0xE9) plotCar = plotCarE9 ;
              else res = FALSE ;
              break ;
  case 0x10 : if (filePlot != (char *)NULL) {             /* hay WindowsNT */
                plotCar = plotCarBuf ;
                if (segmento == 0x0000) {
                  segmento = k_buscarBloque(tamBufPlot) ;
                  ptrBufPlot = MK_FP(segmento, 0x0000) ;
                }
                inBufPlot = 0 ;
                outBufPlot = 0 ;
                numBufPlot = 0 ;
                if (filePlot[0] == (char)0) df = 1 ;    /* salida estandar */
                else                                   /* 0x4000 -> commit */
                accion = 0x0012 ;
                  df = extendedOpenDOS((pointer_t)filePlot, 0x2021, 0, &accion, &error) ;
               /* df = extendedOpenDOS((pointer_t)filePlot, 0x6021, 0, 0x0012) ; */
                encolarCcb((callBack_t)MK_FP(_CS, FP_OFF(tareaPlot)), ccbAlEpilogo) ;
                vaciarBufPlot = FALSE ;
/*                contTicsPlot = contTics ; */
              }
              else res = FALSE ;
              break ;
  case 0x24 : ;
  case 0x35 : ;
  case 0x46 : ;
  case 0x27 : plotCar = plotCarCOM ;                 /* por interrupciones */
              com = 0x0F & (modo-4) ;
              break ;
  default   : res = FALSE ;
  }
/* encolarTarea(tareaPrueba) ; */
  return(res) ;
}

#pragma warn +aus

#define tamBufEsc 512

#pragma warn -par

int far tareaPlot ( void far * arg ) {                 /* escribe en disco */

  char bufer [tamBufEsc] ;
  word_t num ;
  int j ;

/*
  printStrWin(win_so, "\n") ;
  printDecWin(win_so, numBufPlot, 1) ;
*/
  asm cli

//if (contTics <= contTicsPlot) return(1) ;    /* seccion critica (dwords) */
//contTicsPlot = contTics ; 

  num = numBufPlot ;
  while (num > 0) {
    for ( j = 0 ; j < tamBufEsc ; j++ )
      if (num > 0) {                                         /* consumidor */
        bufer[j] = ptrBufPlot[outBufPlot] ;
        outBufPlot++ ;                                        /* ojo: 64KB */
        num-- ;
      }
      else break ;
    writeDOS(df, (char *)&bufer, j) ;
    commitFileDOS(df) ;                                 /* extendedOpenDOS */

    asm cli
    numBufPlot = numBufPlot - j ;               /* seccion critica (dword) */
  }
  return(1) ;                                               /* fin parcial */
}

#pragma warn +par

void finPlot ( void ) {

  char bufer [tamBufEsc] ;
  int j ;

  if ((df > 0) && (segmento != 0x0000)) {
    while (numBufPlot > 0) {
      for ( j = 0 ; j < tamBufEsc ; j++ )
        if (numBufPlot > 0) {
          bufer[j] = ptrBufPlot[outBufPlot] ;
          outBufPlot++ ;                                      /* ojo: 64KB */
          numBufPlot-- ;
        }
        else break ;
      writeDOS(df, (char *)&bufer, j) ;
      commitFileDOS(df) ;
    }
    closeDOS(df) ;
    k_devolverBloque(segmento, tamBufPlot) ;
  }
}

/* --------------------------------- plot -------------------------------- */

#pragma option -w-par            /* omitir warning parameter is never used */

void plotCarNula ( char car ) {
  /* nada */
}

void plotCarLPT ( char car ) {
/*  printCarLPTP(lpt, car) ;   ******************************/
}

void plotCarCOM ( char car ) {                       /* por interrupciones */
/*  printCarRS232I(com, car) ; ******************************/
}

void plotCarCOMP ( char car ) {                             /* por polling */
/*  printCarRS232P(com, car) ; ******************************/
}

#pragma option -wpar           /* permitir warning parameter is never used */

void plotCarBuf ( char car ) {
  ptrBufPlot[inBufPlot] = car ;                               /* productor */
  inBufPlot = inBufPlot + 1  ;
  numBufPlot++ ;
}

void plotCarFILE ( char car ) {
  writeDOS(df, (char *) &car, 1) ;
}

void plotCarE9 ( char car ) {                         /* para bochs y qemu */
    asm 
	{
	    mov al,car ;
        out 0E9h,al ;
    }
}

bool_t cambioDeEstado ( void ) {
  pindx_t pindx ;
  pindx_t pindx1 ;
  for ( pindx = 0 ; pindx < maxProcesos ; pindx++ )
    if (estadoAnt[pindx] != descProceso[pindx].estado) {
      for ( pindx1 = pindx ; pindx1 < maxProcesos ; pindx1++ )
        estadoAnt[pindx1] = descProceso[pindx1].estado ;      /* actualizamos */
      return(TRUE) ;
    }
  return(FALSE) ;
}

/* --------------------------------- plot -------------------------------- */
/*                                                                         */
/* void plot ( char tipo, word_t ax, word_t cont )                         */
/*                                                                         */
/* Entrada: tipo ax  cont                                                  */
/*          ---- --- ----                                                  */
/*          t    0   cont     (int. timer)                                 */
/*          m    0   cont     (int. raton)                                 */
/*          k    car cont     (int. teclado con posible caracter ascii)    */
/*          1    0   cont     (int. com1)                                  */ /* nuevo 25-07-2012 */
/*          2    0   cont     (int. com2)                                  */ /* nuevo 25-07-2012 */
/*          3    0   cont     (int. com3)                                  */ /* nuevo 25-07-2012 */
/*          4    0   cont     (int. com4)                                  */ /* nuevo 25-07-2012 */
/*          <    car cont     (int. recepcion caracter ascii)              */ /* nuevo 25-07-2012 */
/*          " car cont     (int. envio caracter ascii)                  */ /* nuevo 25-07-2012 */
/*          s    AX  cont     (llamada al sistema con valor AX)            */
/*          i    0   cont     (inicio plot del sistema)                    */
/*          z    0   cont     (fin plot del sistema)                       */
/*          e    0   cont     (anotar estado de los procesos del sistema)  */
/*                                                                         */
/* ----------------------------------------------------------------------- */
/*                                                                         */
/* Envio de informacion mediante el procedimiento plot:                    */
/*                                                                         */
/*  T cc            (int. timer)                                           */
/*  M cc            (int. raton)                                           */
/*  K cc            (int. teclado)                                         */
/*  C cc c          (int. teclado con caracter c)                          */
/*  1 cc            (int. com1)                                            */ /* nuevo 25-07-2012 */
/*  2 cc            (int. com2)                                            */ /* nuevo 25-07-2012 */
/*  3 cc            (int. com3)                                            */ /* nuevo 25-07-2012 */
/*  4 cc            (int. com4)                                            */ /* nuevo 25-07-2012 */
/*  < cc c          (int. recepcion caracter c)                            */ /* nuevo 25-07-2012 */
/*  > cc c          (int. envio caracter c)                                */ /* nuevo 25-07-2012 */
/*  S cc x          (llamada al sistema con valor x de ax)                 */
/*                                                                         */
/*  E cc xxxx       (cambio de estado)                                     */
/*  N cc            (sin cambio de estado)                                 */
/*  I cc xxxx       (inicio indicando el estado inicial)                   */
/*  Z cc            (fin)                                                  */
/*                                                                         */
/*  0 12 3456       (cc es el valor del contador 0 del Timer)              */
/*                                                                         */
/* ----------------------------------------------------------------------- */

void plot ( char tipo, word_t ax, word_t cont ) {

  int i ;

  if (plotProcesos) {

/*
    printStrWin(win_so, "\n") ;
    printCarWin(win_so, tipo) ;
    printDecWin(win_so, cont, 5) ;
    printStrWin(win_so, "  ") ;
*/

    switch (tipo) {
    case 't' : plotCar('T') ;               /* t: interrupcion del timer   */
               plotCar((char)(cont >> 8)) ;
               plotCar((char)cont) ;
               break ;
    case 'm' : plotCar('M') ;               /* m: interrupcion del raton   */
               plotCar((char)(cont >> 8)) ;
               plotCar((char)cont) ;
               break ;
    case 'k' : if (ax == 0) {               /* k: interrupcion del teclado */
                 plotCar('K') ;
                 plotCar((char)(cont >> 8)) ;
                 plotCar((char)cont) ;
               }
               else {
                 plotCar('C') ;
                 plotCar((char)(cont >> 8)) ;
                 plotCar((char)cont) ;
                 plotCar((char)ax) ;            /* caracter ascii tecleado */
               }
               break ;

    case '1' : ;
    case '2' : ;
    case '3' : ;
    case '4' : plotCar('K') ;
               plotCar((char)(cont >> 8)) ;
               plotCar((char)cont) ;
               break ;
    case '<' : ;
    case '>' : break ;

    case 's' : plotCar('S') ;               /* s: llamada al sistema       */
               plotCar((char)(cont >> 8)) ;
               plotCar((char)cont) ;
               plotCar(
                 ((ax >> 4) & 0x00F0) |                /* AH (codOp) */
                 (ax & 0x000F)                         /* AL (subFn) */
               ) ;
               break ;
    case 'z' : plotCar('Z') ;               /* z: indica fin plot          */
               plotCar((char)(cont >> 8)) ;
               plotCar((char)cont) ;
               vaciarBufPlot = TRUE ;
               break ;
    case 'i' :                              /* i: indica inicio plot       */
               for ( i = 0 ; i < maxProcesos ; i++ )
                 estadoAnt[i] = libre ;
    case 'e' : if (cambioDeEstado()) {      /* e: estado de los procesos   */
                 /* printCarWin(win_so, tipo) ; */
                 if (tipo == 'i')
                   plotCar('I') ;
                 else
                   plotCar('E') ;
                 plotCar((char)(cont >> 8)) ;
                 plotCar((char)cont) ;
                 for ( i = 0 ; i < maxProcesos ; i = i+4 ) {
                   plotCar(
                     (descProceso[i  ].estado << 6) |
                     (descProceso[i+1].estado << 4) |
                     (descProceso[i+2].estado << 2) |
                      descProceso[i+3].estado
                   ) ;
                 }
               }
               else {
                 plotCar('N') ;
                 plotCar((char)(cont >> 8)) ;
                 plotCar((char)cont) ;
               }
               break ;
    default : ;
    }
  }
}

