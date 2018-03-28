/* ----------------------------------------------------------------------- */
/*                                ajustusr.c                               */
/* ----------------------------------------------------------------------- */
/*    Ajuste de los registros de segmento y salto al programa principal    */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\scanner.h>                                /* tamComando */
#include <so1pub.h\ll_s_so1.h>                          /* terminarProceso */
#include <so1pub.h\ajustusr.h>                            /* startUserCode */


#define SPInicial 0x7FFE         /* puntero de pila "a priori" del proceso */
                                                     /* podria redefinirse */
#define maxArgs 20

#pragma option -w-use     /* warning: 'cmdLine' is declared but never used */

static char cmdLine [ tamComando ] = "" ;     /* no cambiar (crearProceso) */

char * argv [ maxArgs ] = { "" } ;            /* no cambiar (crearProceso) */

/* primerDato, cmdLine y argv deben estar inicializados (DATA) (no BSS)    */

/* main: funcion externa a este modulo a la que se le cede el control      */

/* void main ( void ) ; */
/* void main ( int argc, char * argv [ ] ) ; */

void main ( ) ;                                                 /* forward */

static void finish ( void ) ;                   /* main debe retornar aqui */

void startBin ( void ) {    /* Inicializacion de los registros de segmento */
    startUserCode
    asm {
		mov bx,OFFSET main ;                              /* jmp near main */
        push bx ;                                              /* apilamos */
        mov dx,OFFSET finish ;    /* para que figure finish en la cabecera */
        mov dx,OFFSET __sighandler ;    /* y para que figure __sighandler  */
	}
}                                                                   /* ret */

void finish ( void ) {                    /* por defecto main retorna aqui */

    manejador_t far * ptrManejadorAtExit ;
  
    asm push ax ;                     /* AX = resultado retornado por main */

    ptrManejadorAtExit = (manejador_t far *)MK_FP(_DS, valor_SPInicial-2) ;
  
    if (((word_t)(*ptrManejadorAtExit)) != 0x0000)       /* se ha cambiado */
    { 
       asm pop ax ;      /* sacamos de la pila el valor retornado por main */
       (*ptrManejadorAtExit)() ;   	  /* se llama a la funcion establecida */
    }
    else asm pop ax ;     /* restauramos en ax el valor retornado por main */
  
//  exit(0) ;                        
//  exit(_AX) ; /* codigo de terminacion = valor de retorno de main/atexit */
    asm {                            /* ponemos el codigo de exit en linea */
		mov bx,ax ;          
		mov ax,0004h ;       /* peligro si cambia ese codigo en LL_S_EXC.C */
		int nVIntSO1 ;
	}
}

/* En cuanto a la llamada a main se hace en ensamblador para evitar que el */
/* compilador de error si ponemos main() para llamar a main(argc, argv)    */
/* sin indicar los dos argumentos que tiene declarados la funci�n main.    */
/* El truco es que dichos argumentos argc y (char * *)&argv los va a       */
/* colocar el sistema operativo en el momento de la creaci�n del proceso   */
/* es decir en la funci�n preEjecutar del fichero EJECUTAR.C.              */

#if   (__TURBOC__ == 0x0200)                         /* compilador TCC 2.0 */
/* #error __TURBOC__ == 0x0200 */
#elif (__TURBOC__ == 0x0401)                         /* compilador TCC 3.0 */
/* #error __TURBOC__ == 0x0401 */
#elif (__TURBOC__ == 0x0520)                         /* compilador BCC 5.2 */
/* #error __TURBOC__ == 0x0520 */
#else
/* #error __TURBOC__ == 0x???? */
#endif

#if (__TURBOC__ == 0x0401) || (__TURBOC__ == 0x0520)
void far _setargv__ ( void ) {      /* el compilador requiere este s�mbolo */
                                            /* cuando main usa argc y argv */
}
#endif