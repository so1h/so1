/* ----------------------------------------------------------------------- */
/*                               llamadas.c                                */
/* ----------------------------------------------------------------------- */
/*                  manejadores de las llamadas al sistema                 */
/* ----------------------------------------------------------------------- */

#include <so1pub.h\strings.h>
#include <so1pub.h\tipos.h>
#include <so1pub.h\pic.h>
#include <so1.h\ajustes.h>
#include <so1.h\procesos.h>
#include <so1.h\blockpr.h>
#include <so1.h\bios.h>                                    /* printStrBIOS */
#include <so1.h\plot.h>
#include <so1.h\llamadas.h>

#include <so1.h\matrix.h>                                 /* NUM_MATRICULA */

/* ----------------------------------------------------------------------- */
/*        manejadores de las diferentes llamadas al sistema de SO1         */
/* ----------------------------------------------------------------------- */

extern void so1_manejador_00 ( void ) ;        /* 0: createProcess 1: fork */
//                                           /* 2: exec 3: waitpid 4: exit */
//                                     /* 5: getpid 6: getpindx 7: getppid */
//                   /* 8: getuid 9: setuid a: getgid b: setgid c: killpid */

extern void so1_manejador_01 ( void ) ;                /* 0: open 1: close */
//                                      /* 2: leer 3: leerListo 4: leerRaw */
//                                   /* 5: leerRawListo 6: escribir 7: eof */

extern void so1_manejador_02 ( void ) ;                 /* 0: obtenInfoSO1 */
//                                      /* 1: crearRecurso 2: crearFichero */
//                          /* 3: esperarDesinstalacion 4: destruirRecurso */
//                           /* 5: encolarCcbRecurso 6: eliminarCcbRecurso */

extern void so1_manejador_03 ( void ) ;                      /* 0: retardo */
//                                       /* 1: obtenInfoPS 2: obtenInfoMEM */
//                                     /* 3: obtenInfoFAB 4: obtenInfoINFO */
//                                  /* 5: getdisk 6: findFirst 7: findNext */

extern void so1_manejador_04 ( void ) ;                 /* 0: activarTraza */
//                                                  /* 1: analizarProcesos */

extern void so1_manejador_05 ( void ) ;      /* 0 sendrec 1:send 2:receive */
//                                                     /* 3: notify 4:echo */

extern void so1_manejador_06 ( void ) ;                       /* 0: system */

extern void so1_manejador_07 ( void ) ;            /* 0: kill 1: sigaction */
//                                                   /* 2: alarm  3: pause */

void so1_manejador_Nulo ( void )
{
}

manejador_t manejador [ ] =                   /*   tabla de manejadores    */
{                                             /*                           */  
    so1_manejador_00,                         /* 0x00 gestion de procesos  */
    so1_manejador_01,                         /* 0x01 manejo de ficheros   */
    so1_manejador_02,                         /* 0x02 soporte para drivers */
    so1_manejador_03,                         /* 0x03 obtenInfo            */
    so1_manejador_04,                         /* 0x04 activarTraza         */
    so1_manejador_Nulo,                       /* 0x05                      */
//  so1_manejador_Nulo,                       /* 0x06 system               */
    so1_manejador_06,                         /* 0x06 system               */
    so1_manejador_Nulo,                       /* 0x07 señales              */
//  so1_manejador_07,                         /* 0x07 señales              */
    so1_manejador_Nulo,                       /* 0x08                      */
    so1_manejador_Nulo,                       /* 0x09                      */
    so1_manejador_Nulo,                       /* 0x0A                      */
    so1_manejador_Nulo,                       /* 0x0B                      */
    so1_manejador_Nulo,                       /* 0x0C                      */
    so1_manejador_Nulo,                       /* 0x0D                      */
    so1_manejador_Nulo,                       /* 0x0E                      */
    so1_manejador_Nulo                        /* 0x0F                      */
} ;

#define maxLlamadas sizeof(manejador)/sizeof(manejador_t)

byte_t codOp ;

word_t dirDestino ;

/* Punto de entrada del S.O. */

void far isr_SO1 ( void )
{

#ifdef NUM_MATRICULA
    macroEmboscada() ;
#endif

    if (nivelActivacionSO1 > 1)
    {
        printStrBIOS("\n llamada al sistema desde el nivel ") ;
        printDecBIOS(nivelActivacionSO1-1, 1) ;
        printStrBIOS(" (0: usuario, 1: tarea) pindx = ") ;
        printIntBIOS(indProcesoActual, 1) ;
        printStrBIOS(" AX = ") ;
        printHexBIOS(tramaProceso->AX, 4) ;
        /* while (TRUE) ; */
        asm cli ;
        asm hlt ;                                        /* paramos la CPU */
    }

//       /* los parametros de entrada de la llamada al sistema estan en la */
//                                                     /* trama de la pila */

    tramaProceso = (trama_t far *)MK_FP(SS_Proceso, SP_Proceso) ;

    plot('s', tramaProceso->AX, contadorTimer0()) ;

#if (FALSE)                                             /* para depuracion */
    printStrWin(win_so, "\n pindx = ") ;
    printIntWin(win_so, indProcesoActual, 1) ;
    printStrWin(win_so, " AX = ") ;
    printHexWin(win_so, tramaProceso->AX, 4) ;
#endif

    codOp = tramaProceso->AH ;                                       /* ah */

    if (codOp < maxLlamadas) manejador[codOp]() ;

    plot('e', 0, contadorTimer0()) ;

    /* los parametros de salida de la llamada al sistema estan en la trama */

}